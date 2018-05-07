#ifndef Sensor_h
#define Sensor_h

#include <Wire.h>
// Defines for easy Use
#define MPU_ADDRESS_GYRO_CONFIG 0x1B
#define MPU_ADDRESS_ACCEL_CONFIG 0x1C
#define MPU_ADDRESS_GYX 0x43
#define MPU_ADDRESS_GYY 0x45
#define MPU_ADDRESS_GYZ 0x47
#define MPU_ADDRESS_ACX 0x3B
#define MPU_ADDRESS_ACY 0x3D
#define MPU_ADDRESS_ACZ 0x3F
#define MPU_ADDRESS_TEMP 0x41

#define MPU_ADDRESS_AD0_GND 0x68
#define MPU_ADDRESS_AD0_VCC 0x69
#define MPU_ADDRESS_PWR_MGMT_1 0x6B
//Defines for FS_SEL
#define FS_SEL_0 0x0
#define FS_SEL_1 0x1
#define FS_SEL_2 0x2
#define FS_SEL_3 0x3

#define STEPSIZEX  200
#define STEPSIZEY  200



int16_t RawAcX,RawAcY,RawAcZ,RawTmp,RawGyX,RawGyY,RawGyZ;
float PhysAcX,PhysAcY,PhysAcZ,PhysTmp,PhysGyX,PhysGyY,PhysGyZ;
int16_t deggyrox;
int16_t deggyroy;
int16_t stepNull, stepOne, stepTwo, stepThree, stepFour , stepSizeY, stepSizeX;
long time_1,time_2;
long loop_timer;

//----------------------------------funtcions---------------------
//Setup Sensor
void SetupSensor()
{
  //Setup Registers of the GY-521
  Wire.begin();
  Wire.beginTransmission(MPU_ADDRESS_AD0_GND);
  Wire.write(MPU_ADDRESS_PWR_MGMT_1);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //Setup Registers of the Accelerometer
  Wire.beginTransmission(MPU_ADDRESS_AD0_GND);
  Wire.write(MPU_ADDRESS_ACCEL_CONFIG);  // PWR_MGMT_1 register
  Wire.write(FS_SEL_0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  //Setup Registers of the Gyroscope
  Wire.beginTransmission(MPU_ADDRESS_AD0_GND);
  Wire.write(MPU_ADDRESS_GYRO_CONFIG);  // PWR_MGMT_1 register
  Wire.write(FS_SEL_0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

//requests actual Sensor data and updates variables
long ReadRawGyroData()
{
  Wire.beginTransmission(MPU_ADDRESS_AD0_GND);
  Wire.write(MPU_ADDRESS_GYX);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS_AD0_GND,6,true);  // request a total of 14 registers
  long time = micros();
  RawGyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  RawGyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  RawGyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  return(time);
}
void printDegData(long int time)
{
  deggyrox=((RawGyX)/2)*time;
  deggyroy=((RawGyY)/2)*time;
}
long ReadRawAccData()
{
  Wire.beginTransmission(MPU_ADDRESS_AD0_GND);
  Wire.write(MPU_ADDRESS_ACX);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS_AD0_GND,6,true);  // request a total of 14 registers
  long time = micros();
  RawAcX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  RawAcY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  RawAcZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  return(time);
}
void ReadRawTempData()
{
  Wire.beginTransmission(MPU_ADDRESS_AD0_GND);
  Wire.write(MPU_ADDRESS_TEMP);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDRESS_AD0_GND,1,true);  // request a total of 14 registers
  RawTmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
}
void RawToPhysData() //Umrechnung von Rohdaten in Physikalische Daten
{
  PhysAcX=RawAcX/16384.;
  PhysAcY=RawAcY/16384.;
  PhysAcZ=RawAcZ/16384.;

  PhysGyX=RawGyX/131.;
  PhysGyY=RawGyY/131.;
  PhysGyZ=RawGyZ/131.;
}

//prints sensor data
void standartReadout()
{
  Serial.print("AcX = "); Serial.print(RawAcX);
  Serial.print(" | AcY = "); Serial.print(RawAcY);
  Serial.print(" | AcZ = "); Serial.println(RawAcZ);
  Serial.print(" | Tmp = "); Serial.print(RawTmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  //Serial.print(" |DegreeX = "); Serial.print(deggyrox);
  Serial.print(" | RawGyX = "); Serial.print(RawGyX);
  Serial.print(" | PhysGyX = "); Serial.print(PhysGyX);
  //Serial.print(" |Time = "); Serial.print(time);
  //Serial.print(" | DegreeY = "); Serial.print(deggyroy);
  Serial.print(" | RawGyY = "); Serial.print(RawGyY);
  Serial.print(" | PhysGyY = "); Serial.print(PhysGyY);
  Serial.print(" | RawGyZ = "); Serial.println(RawGyZ);
  Serial.print(" | PhysGyZ = "); Serial.print(PhysGyZ);
  }


//geschwindigkeitsstufen



void yASteps()
{
    int stepIs=0;
    int steps = 0;
    
  while(true)
  {  
    if(RawAcY <0)
    {
      if(RawAcY > (STEPSIZEY*(steps-1)))
     {
       Serial.print("SpeedStep ");
       Serial.println(steps);
       break;
      }
    else
      steps--;
    }
    if(RawAcY >0)
    {
      if(RawAcY < (STEPSIZEY*(steps+1)))
      {
        Serial.print("SpeedStep ");
        Serial.println(steps);
        break;
      }
    else
      steps++; 
    }
    
  }
}

void xASteps(){
  int stepIs=0;
  int steps = 0;
  
while(true){
  if(RawAcX <0){
  if(RawAcX > (STEPSIZEX*(steps-1)))
   {
    Serial.print("Right ");
    Serial.println(steps);
    break;
  }
  else
  steps--;
  }
  
  if(RawAcX >0){
  if(RawAcX < (STEPSIZEX*(steps+1)))
  {
    Serial.print("Left  ");
    Serial.println(steps);
    break;
  }
  else
  steps++; 
  }
  
}
}
  
#endif
