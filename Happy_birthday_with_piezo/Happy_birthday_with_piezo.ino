// Define the pins
int buzzerPin = 8;
int ledPins[] = {2, 3, 4, 5, 10}; 
int numLeds = 5;                 
int buttonPin = 7; // Our new button pin

// Frequencies for the notes of "Happy Birthday"
int melody[] = {
  262, 262, 294, 262, 349, 330,  
  262, 262, 294, 262, 392, 349,  
  262, 262, 523, 440, 349, 330, 294, 
  466, 466, 440, 349, 392, 349   
};

// Note durations: 4 = quarter note, 8 = eighth note, 2 = half note
int noteDurations[] = {
  8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 4, 2,
  8, 8, 4, 4, 4, 2
};

void setup() {
  // Set up the buzzer pin
  pinMode(buzzerPin, OUTPUT);
  
  // Set up all 5 LED pins as outputs
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // Set up the button pin using the Arduino's built-in pull-up resistor.
  // This means the pin reads "HIGH" when left alone, and "LOW" when pressed.
  pinMode(buttonPin, INPUT_PULLUP); 
}

void loop() {
  // --- THE WAITING ROOM ---
  // The Arduino will stay stuck in this tiny loop doing nothing 
  // until the button is pressed (which makes the pin read LOW).
  while (digitalRead(buttonPin) == HIGH) {
    delay(10); // Take a tiny breath, then check again
  }
  
  // --- THE SHOW ---
  // Once the button is pressed, it escapes the waiting room and plays the song!
  for (int thisNote = 0; thisNote < 25; thisNote++) {
    
    int noteDuration = 1200 / noteDurations[thisNote];
    int currentLed = ledPins[thisNote % numLeds]; 

    digitalWrite(currentLed, HIGH);
    tone(buzzerPin, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(noteDuration); 

    digitalWrite(currentLed, LOW);
    delay(pauseBetweenNotes - noteDuration);
    noTone(buzzerPin);
  }
  
  // We removed the 5-second delay here! 
  // Now, when the song finishes, the code just goes back to the top 
  // and waits for her to press the button again.
}
