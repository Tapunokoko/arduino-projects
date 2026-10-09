const int sensorPin = A0;
const float baselineTemp = 28.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // open port 
  for(int pinnumber =2 ; pinnumber <5 ; pinnumber++){
    pinMode (pinnumber, OUTPUT) ; 
    digitalWrite (pinnumber, LOW); 
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorval = analogRead(sensorPin);
  Serial.print("Sensor Value");
  Serial.print(sensorval);
  //convert the adc reading to voltage 
  float voltage = (sensorval/1024.0) * 5.0; 
  Serial.print( "Volts: ");
  Serial.print(voltage); 
  //convert to temperature 
  float temperature = (voltage -0.5) *100;
  Serial.print("ty nguyen C");
  Serial.println(temperature);

  // use if else to control light to go up when the temp is lower than basline temp +2
  if(temperature < baselineTemp +2){
    digitalWrite(2, LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
  }  
  else if(temperature >= baselineTemp + 2 && temperature < baselineTemp + 4 ){
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
  }
  else if (temperature >= baselineTemp + 4 && temperature < baselineTemp + 6){
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
  }
  else if ( temperature >= baselineTemp +6){
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
  }
  delay(250);
}
