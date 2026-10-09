#include <Servo.h>


Servo myServo;
int const potpin = A0 ;
int potval;
int angle;
void setup() {
  // put your setup code here, to run once:
  myServo.attach(9);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  potval = analogRead(potpin);
  Serial.print("potval");
  Serial.print(potval);

  angle = map(potval, 0, 1023, 0, 179);
  Serial.print("angle");
  Serial.print(angle);
  myServo.write(angle);
  delay(15);
}
