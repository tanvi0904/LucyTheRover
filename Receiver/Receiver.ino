  // Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

#define enA 9
#define in1 4
#define in2 5
#define enB 10
#define in3 6
#define in4 7

int motorSpeedA = 0;
int motorSpeedB = 0;

int IRSensor = 2;
int buzzer = 8;

int smokeA0 = A5;
// Smokw threshold value
int sensorThres =100;

void setup()
{
      // Initialize ASK Object
      rf_driver.init();
      // Setup Serial Monitor
      Serial.begin(9600);
      pinMode(enA, OUTPUT);
      pinMode(enB, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(in3, OUTPUT);
      pinMode(in4, OUTPUT);
    
      // Turn off motors - Initial state
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
  
      pinMode (IRSensor, INPUT); // sensor pin INPUT
  
      pinMode(smokeA0, INPUT);
}
 
void loop()
{
      int analogSensor = analogRead(smokeA0);
      //smoke value
      Serial.print("Pin A0: ");
      Serial.println(analogSensor);
      if (analogSensor > sensorThres)
      {
            tone(buzzer, 900, 1000);
      }
      else
      {
            noTone(buzzer);
      } 
    
      int statusSensor = digitalRead (IRSensor);
      if (statusSensor == 1)
      {
        noTone(buzzer);
          // Serial.println("No object");
      }
       else
      {
          tone(buzzer,  1000, 2000);
          // Serial.println("Object");
          motorSpeedA = 0;
          motorSpeedB = 0;
          // Turn off motors
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
      }
       // Check if received packet is correct size
       uint32_t xyAxis;
       uint8_t dataXYlen = sizeof(xyAxis);
       uint16_t xAxis, yAxis;
      if (rf_driver.recv((uint8_t*)&xyAxis, &dataXYlen))
      {
          // Serial.print("XYaxis: ");
          // Serial.println(xyAxis);
          yAxis = (xyAxis & 0xFFFF0000) >> 16;
          xAxis = xyAxis & 0x0000FFFF;
      }
//       Serial.print("Xaxis: ");
//       Serial.println(xAxis);
//       Serial.print("Yaxis: ");
//       Serial.println(yAxis);
      // X-axis used for left and right control
      if (xAxis < 470 && statusSensor == 1) 
      {
          noTone(buzzer);
          // Set Motor A 
          digitalWrite(in1, HIGH);
          digitalWrite(in2, LOW);
          // Set Motor B 
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          // Convert the declining X-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
          motorSpeedA = map(xAxis, 470, 0, 0, 255);
          motorSpeedB = map(xAxis, 470, 0, 0, 255);
      }
        else if (xAxis > 550 && statusSensor == 1) 
        {
          noTone(buzzer);
          // Set Motor A 
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          // Set Motor B 
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW  );
          // Convert the increasing X-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
          motorSpeedA = map(xAxis, 550, 1023, 0, 255);
          motorSpeedB = map(xAxis, 550, 1023, 0, 255);
        }
        
        // If joystick stays in middle the motors do not moving
        else 
        {
          noTone(buzzer);
          motorSpeedA = 0;
          motorSpeedB = 0;
          // Turn off motors
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
        }
        
        // Y-axis used for front and back control
        if (yAxis < 470&& statusSensor == 1) 
        {
          noTone(buzzer);
          // Set Motor B forward
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          // Convert the declining Y-axis readings from 470 to 0 into increasing 0 to 255 value
          int yMapped = map(yAxis, 470, 0, 0, 255);
          // Move to left - decrease left motor speed, increase right motor speed
          // motorSpeedA = motorSpeedA - yMapped;
          motorSpeedB = motorSpeedB + yMapped;
          // Confine the range from 0 to 255
          // if (motorSpeedA < 0) 
          // {
          //    motorSpeedA = 0;
          // }
          
          if (motorSpeedB > 255) 
          {
              motorSpeedB = 255;
          }
        }
        
        // Set Motor B backward
        if (yAxis > 550) 
        {
          digitalWrite(in3, LOW);
          digitalWrite(in4, HIGH);
          // Convert the increasing Y-axis readings from 550 to 1023 into 0 to 255 value
          int yMapped = map(yAxis, 550, 1023, 0, 255);
          // motorSpeedA = motorSpeedA + yMapped;
          motorSpeedB = motorSpeedB - yMapped;
          tone(buzzer, 900, 1000);
          // Confine the range from 0 to 255
          // if (motorSpeedA > 255) 
          // {
          //      motorSpeedA = 255;
          // }
          if (motorSpeedB < 0) 
          {
            motorSpeedB = 0;
          }
        }
        motorSpeedA = 255;
        motorSpeedB = 255;
        analogWrite(enA, motorSpeedA); // Send PWM signal to motor A          
        analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
}
