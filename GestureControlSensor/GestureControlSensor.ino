// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domainv
#include<Wire.h>

#include "Sensor.h"
//#include "Mqtt.h"




void setup()
{
  SetupSensor();
  //mqttSetup();
  Serial.begin(115200);
}
void loop()
{
  //Get Sensor Data
  ReadRawAccData();
  ReadRawTempData();
  ReadRawGyroData();
  standartReadout();
  xASteps();
  yASteps();

  //publishSensorData();
  
  while(micros() - loop_timer < 4000);                                 //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
  loop_timer = micros();//Reset the loop timer
}




