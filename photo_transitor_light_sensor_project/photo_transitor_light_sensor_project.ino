  const int greenledpin = 9;
  const int redledpin = 10;
  const int blueledpin = 11;

  const int redsensorpin = A0;
  const int greensensorpin = A1;
  const int bluesensorpin = A2;

  int greenValue = 0;
  int redValue = 0;
  int blueValue = 0;

  int redsensorValue = 0;
  int greensensorValue = 0;
  int bluesensorValue = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(greenledpin , OUTPUT);
  pinMode(redledpin , OUTPUT);
  pinMode(blueledpin , OUTPUT);
  
}

void loop() {
  redsensorValue = analogRead (redsensorpin);
  delay(5);
  greensensorValue = analogRead (greensensorpin);
  delay(5);
  bluesensorValue = analogRead (bluesensorpin);
  delay(5);

  Serial.print("\t red:");
  Serial.print(redsensorValue);
  Serial.print("\t green:");
  Serial.print(greensensorValue);
  Serial.print("\t blue:");
  Serial.print(bluesensorValue);

  redValue = redsensorValue/4;
  greenValue = redsensorValue/4;
  blueValue = redsensorValue/4;

  Serial.print("\t red:");
  Serial.print(redValue);
  Serial.print("\t green:");
  Serial.print(greenValue);
  Serial.print("\t blue:");
  Serial.print(blueValue);

  analogWrite(greenledpin,greenValue );
  analogWrite(redledpin,redValue );
  analogWrite(blueledpin,blueValue );

  





  // put your main code here, to run repeatedly:

}
