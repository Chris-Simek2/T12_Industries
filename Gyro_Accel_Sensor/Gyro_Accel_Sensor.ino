// Basic demo for accelerometer/gyro readings from Adafruit ISM330DHCX

#include <Adafruit_ISM330DHCX.h>
#include <Arduino.h>
#include <math.h>

// For SPI mode, we need a CS pin
#define LSM_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define LSM_SCK 13
#define LSM_MISO 12
#define LSM_MOSI 11

Adafruit_ISM330DHCX ism330dhcx;
void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit ISM330DHCX test!");

  if (!ism330dhcx.begin_I2C()) {
    // if (!ism330dhcx.begin_SPI(LSM_CS)) {
    // if (!ism330dhcx.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    Serial.println("Failed to find ISM330DHCX chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("ISM330DHCX Found!");

  // ism330dhcx.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  Serial.print("Accelerometer range set to: ");
  switch (ism330dhcx.getAccelRange()) {
  case LSM6DS_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case LSM6DS_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case LSM6DS_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case LSM6DS_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  // ism330dhcx.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  Serial.print("Gyro range set to: ");
  switch (ism330dhcx.getGyroRange()) {
  case LSM6DS_GYRO_RANGE_125_DPS:
    Serial.println("125 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  case ISM330DHCX_GYRO_RANGE_4000_DPS:
    Serial.println("4000 degrees/s");
    break;
  }

  // ism330dhcx.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Accelerometer data rate set to: ");
  switch (ism330dhcx.getAccelDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  // ism330dhcx.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Gyro data rate set to: ");
  switch (ism330dhcx.getGyroDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  ism330dhcx.configInt1(false, false, true); // accelerometer DRDY on INT1
  ism330dhcx.configInt2(false, true, false); // gyro DRDY on INT2
}


float acc_x;
float acc_y;
//float acc_z;
float acc_total_magnitude;
float acc_total_direction;
float zRot;
//float xRot;
//float yRot;
float errorZ;
//float errorX;
//float errorY;
float velocity;
float time;
//float xoffset;

void loop() {
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  ism330dhcx.getEvent(&accel, &gyro, &temp);

  //Serial.print("\t\tTemperature ");
  //Serial.print(temp.temperature);
  //Serial.print(" deg C");
  //Serial.print("\t\t");
  //Serial.print(temp.temperature*9/4+32);
  //Serial.println(" deg F");
  
  acc_x = accel.acceleration.x;  
  acc_y = accel.acceleration.y;  
  //acc_z = accel.acceleration.z;
  
  //*****************************************//
  //Calculate for acceleration due to gravity//
  //*****************************************//
  //yRot = gyro.gyro.y*0.25 + yRot + errorY*0.25; 
  //if (acc_z < 9.0){
      //xoffset=9.88*sin(yRot*180/PI);
      //acc_x = acc_x - xoffset;    
  //}

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tAccel X: ");
  //Serial.print(accel.acceleration.x);
  Serial.print(acc_x);
  Serial.print(" \tY: ");
  //Serial.print(accel.acceleration.y);
  Serial.print(acc_y);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2 ");  

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

  Serial.println();
  Serial.print("\t\tAccel Mag: ");
  Serial.print(acc_total_magnitude);
  Serial.println(" m/s^2 ");
  Serial.print("\t\tAccel Dir: ");
  Serial.print(acc_total_direction);
  Serial.println(" Degrees ");

//*****************************************//
//Calculating Speed//
//*****************************************//
  if (acc_total_direction<0){
    acc_total_magnitude = acc_total_magnitude*-1;
  }
  velocity = velocity + (acc_total_magnitude*0.25);      //for time we use 0.25 since that is our sample rate
  Serial.println();
  Serial.print("\t\tVelocity: ");
  Serial.print(velocity);
  Serial.println();
  
  /* Display the results (rotation is measured in degrees) */
  Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x*180/PI);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y*180/PI);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z*180/PI);
  Serial.println(" Degrees ");
  Serial.println();

//*****************************************//
//Calculating Head Rotation//
//*****************************************//
  errorZ = 0.00845;                                //0.00845 deg in positive direction = +0.48 in Rad to compesate
  //errorX = -0.00366;                               //0.00366 deg in negative direction = -0.21 in Rad to compesate
  //errorY = 0.01404;                                //0.01404 deg in positive direction = +0.80 in Rad to compesate
  zRot = gyro.gyro.z*0.25 + zRot + errorZ*0.25;    
  //xRot = gyro.gyro.x*0.25 + xRot + errorX*0.25; 
  //yRot = gyro.gyro.y*0.25 + yRot + errorY*0.25;    
  
  Serial.print("\t\tUpdated Z Rotation: ");
  Serial.print(zRot*180/PI);
  Serial.println(" Degrees ");
  Serial.println();
  //Serial.print("\t\tUpdated Y Rotation: ");
  //Serial.print(yRot*180/PI);
  //Serial.println(" Degrees ");
  //Serial.println();
  
  // X and Y Rotation is not too important right now because we care more about the rotation of the head.
  //Serial.print("\t\tUpdated X Rotation: ");
  //Serial.print(xRot*180/PI);
  //Serial.println(" Degrees ");
  //Serial.print("\t\tUpdated Y Rotation: ");
  //Serial.print(yRot*180/PI);
  //Serial.println(" Degrees ");
  //Serial.println();
  
  Serial.print("\t\tCurrent Time On: ");
  Serial.print(time);
  Serial.println();
  Serial.println();

  time = time + 0.25;
  delay(250);
}
