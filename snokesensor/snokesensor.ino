/*******
 
 All the resources for this project:
 https://www.hackster.io/Aritro

*******/

// https://create.arduino.cc/projecthub/Aritro/smoke-detection-using-mq-2-gas-sensor-79c54a

int smokeA0 = A5;
// Your threshold value
int sensorThres = 400;

void setup() {
  pinMode(smokeA0, INPUT);
  Serial.begin(9600);
}

void loop() {
  int analogSensor = analogRead(smokeA0);

  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor > sensorThres)
  {
    
  }
  else
  {
   
  } 

}
