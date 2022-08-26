// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    Serial.begin(9600);
}
 
void loop()
{
    uint16_t xAxis = analogRead(A0);
  Serial.print("Xaxis: ");
  Serial.println(xAxis);
    uint16_t yAxis = analogRead(A1);
  Serial.print("Yaxis: ");
  Serial.println(yAxis);
    uint32_t xyAxis = ((uint32_t)xAxis<<16 & 0xFFFF0000 ) | (uint32_t)yAxis;     
//  Serial.print("XYaxis: ");
//  Serial.println(xyAxis);
    rf_driver.send((uint8_t *)&xyAxis, sizeof(xyAxis));
    rf_driver.waitPacketSent();
}
