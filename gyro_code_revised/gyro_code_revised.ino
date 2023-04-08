// Basic demo for accelerometer readings from Adafruit ICM20948

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_ICM20948 icm;
uint16_t measurement_delay_us = 65535; // Delay between measurements for testing
// For SPI mode, we need a CS pin
#define ICM_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define ICM_SCK 13
#define ICM_MISO 12
#define ICM_MOSI 11

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit ICM20948 test!");

  // Try to initialize!
  if (!icm.begin_I2C()) {
    // if (!icm.begin_SPI(ICM_CS)) {
    // if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {

    Serial.println("Failed to find ICM20948 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("ICM20948 Found!");
  // icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
  Serial.print("Accelerometer range set to: ");
  switch (icm.getAccelRange()) {
  case ICM20948_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case ICM20948_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case ICM20948_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case ICM20948_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  Serial.println("OK");

  // icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
  Serial.print("Gyro range set to: ");
  switch (icm.getGyroRange()) {
  case ICM20948_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  }

  //  icm.setAccelRateDivisor(4095);
  uint16_t accel_divisor = icm.getAccelRateDivisor();
  float accel_rate = 1125 / (1.0 + accel_divisor);

  Serial.print("Accelerometer data rate divisor set to: ");
  Serial.println(accel_divisor);
  Serial.print("Accelerometer data rate (Hz) is approximately: ");
  Serial.println(accel_rate);

  //  icm.setGyroRateDivisor(255);
  uint8_t gyro_divisor = icm.getGyroRateDivisor();
  float gyro_rate = 1100 / (1.0 + gyro_divisor);

  Serial.print("Gyro data rate divisor set to: ");
  Serial.println(gyro_divisor);
  Serial.print("Gyro data rate (Hz) is approximately: ");
  Serial.println(gyro_rate);

  // icm.setMagDataRate(AK09916_MAG_DATARATE_10_HZ);
  Serial.print("Magnetometer data rate set to: ");
  switch (icm.getMagDataRate()) {
  case AK09916_MAG_DATARATE_SHUTDOWN:
    Serial.println("Shutdown");
    break;
  case AK09916_MAG_DATARATE_SINGLE:
    Serial.println("Single/One shot");
    break;
  case AK09916_MAG_DATARATE_10_HZ:
    Serial.println("10 Hz");
    break;
  case AK09916_MAG_DATARATE_20_HZ:
    Serial.println("20 Hz");
    break;
  case AK09916_MAG_DATARATE_50_HZ:
    Serial.println("50 Hz");
    break;
  case AK09916_MAG_DATARATE_100_HZ:
    Serial.println("100 Hz");
    break;
  }
  Serial.println();

}


//Playground Variables
float velocity = 0;
float lastVelocity = 0;




//Define Variables
float acc_x;
float acc_y;
float acc_z;
float acc_total_magnitude;
float acc_total_direction;
float zRot;
float xRot;
float yRot;
float errorZ;
float errorX;
float errorY;
float turntime;
float time;
float xoffset;

void loop() {

  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  icm.getEvent(&accel, &gyro, &temp, &mag);

  //*****************************************//
  //Calculate for Magnetic reference (Compass)//
  //*****************************************//
  //Serial.print("\t\tMag X: ");
  //Serial.print(mag.magnetic.x);
  //Serial.print(" \tY: ");
  //Serial.print(mag.magnetic.y);
  //Serial.print(" \tZ: ");
  //Serial.print(mag.magnetic.z);
  //Serial.println(" uT");

  float headingRadians = atan2(mag.magnetic.y, mag.magnetic.x);
  float headingDegrees = headingRadians * 180 / PI;
  float declinationAngle = 11.41666666666667;

  headingDegrees += declinationAngle;

  Serial.print(" \t\tHeading Degrees: ");
  Serial.print(headingDegrees);
  Serial.println(" Degrees");


  acc_x = accel.acceleration.x;  
  acc_y = accel.acceleration.y;  
  acc_z = accel.acceleration.z;
  
  //*****************************************//
  //Calculate for acceleration due to gravity//
  //*****************************************//
  //errorX = -0.00366;                               //0.00366 deg in negative direction = -0.21 in Rad to compesate
  //errorY = 0.01404;                                //0.01404 deg in positive direction = +0.80 in Rad to compesate
  //xRot = gyro.gyro.x*0.100 + xRot + errorX*0.100; 
  //yRot = gyro.gyro.y*0.100 + yRot + errorY*0.100; 
  if (acc_z < 9.6){
      xoffset=9.88*sin(yRot*180/PI);
      acc_x = acc_x - xoffset;    
  }

  /* Display the results (acceleration is measured in m/s^2) */
  //Serial.print("\t\tAccel X: ");
  //Serial.print(accel.acceleration.x);
  //Serial.print(acc_x);
  //Serial.print(" \tY: ");
  //Serial.print(accel.acceleration.y);
  //Serial.print(acc_y);
  //Serial.print(" \tZ: ");
  //Serial.print(accel.acceleration.z);
  //Serial.println(" m/s^2 ");  

//*****************************************//
//Calculating acceleration and direction//
//*****************************************//
  acc_total_magnitude = sqrt((acc_x*acc_x)+(acc_y*acc_y));  //Calculate the total accelerometer vector
  acc_total_direction = atan(acc_y/acc_x);                  //Result comes out in Radians
  acc_total_direction = (acc_total_direction*180/PI);       //Converts to Radians
  
//**********************************************************************//
//Cases for different cuadrants of operation. See Presentation Slide 12//
//**********************************************************************//
  if (acc_x > 0 && acc_y > 0){
    acc_total_direction = 90 - acc_total_direction; 
  }
  if (acc_x > 0 && acc_y < 0){
    acc_total_direction = 90 - acc_total_direction; 
  }
  if (acc_x < 0 && acc_y < 0){
    acc_total_direction = 270 - acc_total_direction; 
  }
  if (acc_x < 0 && acc_y > 0){
    acc_total_direction = 270 - acc_total_direction; 
  }

  //Serial.println();
  Serial.print("\t\tAccel Mag: ");
  Serial.print(acc_total_magnitude);
  //Serial.println(" m/s^2 ");
  Serial.print("\t\tAccel Dir: ");
  Serial.print(acc_total_direction);
  Serial.println(" Degrees ");

//*****************************************//
//Calculating Speed//
//*****************************************//

  if ((acc_total_direction>180 && acc_total_direction<359){
    acc_total_magnitude = acc_total_magnitude*-1;
  }
  
  velocity = lastVelocity + (acc_total_magnitude * 0.1);
  lastVelocity = velocity;


  Serial.println();
  Serial.print("\t\tVelocity: ");
  Serial.print(velocity);
  Serial.println();
  
  /* Display the results (rotation is measured in degrees) */
  //Serial.print("\t\tGyro X: ");
  //Serial.print(gyro.gyro.x*180/PI);
  //Serial.print(" \tY: ");
  //Serial.print(gyro.gyro.y*180/PI);
  //Serial.print(" \tZ: ");
  //Serial.print(gyro.gyro.z*180/PI);
  //Serial.println(" Degrees ");
  //println();

//*****************************************//
//Calculating Head Rotation//
//*****************************************//
  errorZ = 0.0013;                                //0.0013 deg in positive direction  in Rad to compesate
  zRot = gyro.gyro.z*0.100 + zRot + errorZ*0.100;    
    
  
  Serial.print("\t\tUpdated Z Rotation: ");
  Serial.print(zRot*180/PI);
  Serial.println(" Degrees ");
  Serial.println();

  if (zRot*180/PI < -35 && zRot*180/PI > -125){
    Serial.print("\t\tHead is Turning Right ");
    Serial.println("\t\tTurn Off Sensors ");
    Serial.println();
    turntime = turntime + 0.100;
    if(turntime > 4){
      zRot = 0;
      turntime =0;
    }
  }  
    if (zRot*180/PI > 35 && zRot*180/PI < 125){
    Serial.print("\t\tHead is Turning Left ");
    Serial.println("\t\tTurn Off Sensors ");
    Serial.println();
    turntime = turntime + 0.100;
    if(turntime > 4){
      zRot = 0;
      turntime = 0;
    }
  }
  if (zRot*180/PI > -30 && zRot*180/PI < 30){
    Serial.print("\t\tGoing Straight ");
    Serial.println("\t\tNo Action Needed");
    Serial.println();
    time = time + 0.100;
    if(time > 6){
      zRot = 0;
      time =0;
    }
  } 
  // X and Y Rotation is not too important right now because we care more about the rotation of the head.
  //Serial.print("\t\tUpdated X Rotation: ");
  //Serial.print(xRot*180/PI);
  //Serial.println(" Degrees ");
  //Serial.print("\t\tUpdated Y Rotation: ");
  //Serial.print(yRot*180/PI);
  //Serial.println(" Degrees ");
  //Serial.println();
  
  //Serial.print("\t\tCurrent Time On: ");
  //Serial.print(time);
  Serial.println();
  Serial.println();
  Serial.println();

  delay(100);
}
