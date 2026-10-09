/*
  Singing Stepper - Tone Tester
  Engr 346 DLA

  Hardware: Arduino Uno + ULN2003 driver board + 28BYJ-48 stepper motor
  Wiring:   IN1 -> D8, IN2 -> D9, IN3 -> D10, IN4 -> D11
            Driver "+" -> 5 V from the Power Supply Module
            Driver "-" -> GND (also connect this GND to the Arduino GND)

  Open the Serial Monitor at 115200 baud with line ending set to "Newline".
  Commands:
    <number>  play that step rate in steps/s (this is the expected pitch in Hz), e.g. 440
    w         wave drive  (one coil on at a time)
    f         full step   (two coils on at a time)
    h         half step   (alternates one coil and two coils)
    r         toggle ramp-up (start slow and speed up to the target rate)
    s         stop and turn all coils off
*/

// Coil patterns. Bit 0 = IN1 (D8), bit 1 = IN2 (D9), bit 2 = IN3 (D10), bit 3 = IN4 (D11)
const byte SEQ_WAVE[4] = {0b0001, 0b0010, 0b0100, 0b1000};
const byte SEQ_FULL[4] = {0b0011, 0b0110, 0b1100, 0b1001};
const byte SEQ_HALF[8] = {0b0001, 0b0011, 0b0010, 0b0110,
                          0b0100, 0b1100, 0b1000, 0b1001};

// Steps per output-shaft revolution (using the nominal 64:1 gearbox)
const float STEPS_PER_REV_FULL = 2048.0;
const float STEPS_PER_REV_HALF = 4096.0;

// Ramp settings (used only when ramp is ON)
const float RAMP_START_HZ = 100.0;   // step rate the ramp starts from
const float RAMP_ACCEL    = 2000.0;  // how fast the step rate rises, in (steps/s) per second

const byte* seq = SEQ_FULL;
byte seqLen = 4;
byte seqIndex = 0;
char modeName = 'f';

bool running = false;
bool rampOn = false;
float targetHz = 0;
float currentHz = 0;
unsigned long periodUs = 0;
unsigned long lastStepUs = 0;

char buf[16];
byte bufLen = 0;

// Write all four coil pins at once through the port register.
// This takes well under 1 microsecond, while four digitalWrite() calls take several microseconds.
void setCoils(byte pattern) {
  PORTB = (PORTB & 0xF0) | (pattern & 0x0F);
}

void doStep() {
  seqIndex = (seqIndex + 1) % seqLen;
  setCoils(seq[seqIndex]);
}

void stopMotor() {
  running = false;
  setCoils(0);  // all coils off so the motor does not heat up
  Serial.println(F("Stopped. Coils off."));
}

void setMode(char m) {
  if (m == 'w') { seq = SEQ_WAVE; seqLen = 4; }
  if (m == 'f') { seq = SEQ_FULL; seqLen = 4; }
  if (m == 'h') { seq = SEQ_HALF; seqLen = 8; }
  modeName = m;
  seqIndex = 0;
  if (running) setCoils(seq[seqIndex]);

  Serial.print(F("Mode: "));
  if (m == 'w') Serial.println(F("wave drive"));
  if (m == 'f') Serial.println(F("full step"));
  if (m == 'h') Serial.println(F("half step"));
}

void startTone(float hz) {
  targetHz = hz;
  if (rampOn && hz > RAMP_START_HZ) currentHz = RAMP_START_HZ;
  else currentHz = hz;

  periodUs = lround(1000000.0 / currentHz);
  setCoils(seq[seqIndex]);
  lastStepUs = micros();
  running = true;

  // Report what the Arduino is actually timing
  unsigned long targetPeriod = lround(1000000.0 / hz);
  float timedHz = 1000000.0 / targetPeriod;
  float stepsPerRev = (seqLen == 8) ? STEPS_PER_REV_HALF : STEPS_PER_REV_FULL;
  float rpm = hz * 60.0 / stepsPerRev;

  Serial.print(F("Mode "));        Serial.print(modeName);
  Serial.print(F(" | commanded ")); Serial.print(hz, 2);
  Serial.print(F(" steps/s | period ")); Serial.print(targetPeriod);
  Serial.print(F(" us | timed rate ")); Serial.print(timedHz, 2);
  Serial.print(F(" steps/s | output shaft ")); Serial.print(rpm, 2);
  Serial.print(F(" RPM | ramp "));  Serial.println(rampOn ? F("ON") : F("OFF"));
}

void handleCommand(char* cmd) {
  char c = cmd[0];
  if (c == 'w' || c == 'f' || c == 'h') {
    setMode(c);
  } else if (c == 'r') {
    rampOn = !rampOn;
    Serial.print(F("Ramp "));
    Serial.println(rampOn ? F("ON") : F("OFF"));
  } else if (c == 's') {
    stopMotor();
  } else {
    float hz = atof(cmd);
    if (hz >= 1 && hz <= 3000) startTone(hz);
    else Serial.println(F("Type a number from 1 to 3000, or w / f / h / r / s."));
  }
}

// Read Serial without blocking, so the motor keeps stepping while you type
void checkSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (bufLen > 0) {
        buf[bufLen] = '\0';
        handleCommand(buf);
        bufLen = 0;
      }
    } else if (bufLen < sizeof(buf) - 1) {
      buf[bufLen++] = c;
    }
  }
}

void setup() {
  DDRB |= 0x0F;  // D8-D11 as outputs
  setCoils(0);
  Serial.begin(115200);
  Serial.println(F("Singing Stepper tone tester"));
  Serial.println(F("Type a step rate (e.g. 440), or w / f / h / r / s"));
  setMode('f');
}

void loop() {
  checkSerial();
  if (!running) return;

  unsigned long now = micros();

  // If we fell far behind (for example after heavy Serial activity), resync instead of bursting steps
  if (now - lastStepUs > 4 * periodUs) lastStepUs = now;

  if (now - lastStepUs >= periodUs) {
    // Add the period to the last scheduled time (not to "now"),
    // so small delays do not pile up and pull the pitch flat.
    lastStepUs += periodUs;
    doStep();

    // Ramp: rate rises by (accel x time per step) = accel / currentHz each step
    if (currentHz < targetHz) {
      currentHz += RAMP_ACCEL / currentHz;
      if (currentHz > targetHz) currentHz = targetHz;
      periodUs = lround(1000000.0 / currentHz);
    }
  }
}
