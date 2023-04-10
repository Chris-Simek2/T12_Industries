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

////////////////////////////////////////////////////////////////////////////////////////////////
// Both LED and SPEAKER pin numbers
const int LED_PIN = 1; // LED PIN: Arduino Uno = 13, Adafruit RP2040 = GPIO1
const int SPEAKER_PIN = 0; // SPEAKER PIN: Arduino Uno = 12, Adafruit RP2040 = GPIO0

// Ultrasonic sensors trig and echo pin numbers
const int trigPin1 = 2; // Left Ultrasonic Trig pin: Arduino Uno = 2, Adafruit RP2040 = GPIO2
const int echoPin1 = 3; // Left Ultrasonic Echo pin: Arduino Uno = 3, Adafruit RP2040 = GPIO3
const int trigPin2 = 6; // Middle-Left Ultrasonic Trig pin: Arduino Uno = 4, Adafruit RP2040 = GPIO6
const int echoPin2 = 7; // Middle-Left Ultrasonic Echo pin: Arduino Uno = 5, Adafruit RP2040 = GPIO7
const int trigPin3 = 8; // Middle Ultrasonic Trig pin: Arduino Uno = 6, Adafruit RP2040 = GPIO8
const int echoPin3 = 9; // Middle Ultrasonic Echo pin: Arduino Uno = 7, Adafruit RP2040 = GPIO9
const int trigPin4 = 10; // Middle-Right Ultrasonic Trig pin: Arduino Uno = 8, Adafruit RP2040 = GPIO10
const int echoPin4 = 11; // Middle-Right Ultrasonic Echo pin: Arduino Uno = 9, Adafruit RP2040 = GPIO11
const int trigPin5 = 12; // Right Ultrasonic Trig pin: Arduino Uno = 10, Adafruit RP2040 = GPIO12
const int echoPin5 = 13; // Right Ultrasonic Echo pin: Arduino Uno = 11, Adafruit RP2040 = GPIO13

// FOR TESTING PURPOSES
int matrix4[4][1] = {{0},{0},{0},{0}}; 
int matrix10[10][1] = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};

// Matrix of the current and previous distances from the ultrasonic sensors
int left_Matrix[4][1] = {{0},{0},{0},{0}};
int ML_Matrix[4][1] = {{0},{0},{0},{0}};
int middle_Matrix[4][1] = {{0},{0},{0},{0}};
int MR_Matrix[4][1] = {{0},{0},{0},{0}};
int right_Matrix[4][1] = {{0},{0},{0},{0}};
/////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void) {
  //////////////////////////////////////////////////////////////////////////////////////////////
  // Ultrasonic sensors
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  pinMode(trigPin5, OUTPUT);
  pinMode(echoPin5, INPUT);

  // LED and SPEAKER
  pinMode(LED_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  //////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////
long duration; // pulse length from trig to echo
int distance = duration * 0.034 / 2; // converting duration to cm
void sonarDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*0.034)/2;
  if (distance > 450) {
    distance = 450;
  }
}

/*
- Algorithm for when an object is within 250cm with a +5cm addition for accuracy.
- As an objects currVal and prevVal decrease, the object is getting closer. If so,
blindSpotDectection return true.
- As an objects currVal and prevVal increase, the object is moving away. If so,
blindSpotDectection returns false.
- An added + or - 5 cm is added to account for accuracy of the ultrasonic sensors.
*/
bool blindSpotDetection(int currVal, int prevVal1, int prevVal2, int prevVal3) {
  while (currVal  <= 240) {
    if (currVal - prevVal1 > 5 && prevVal2 - prevVal3 > 5) {
      return false;
    } else {
      return true;
    }
  }
  return false;
}

/* 
Turns on the LED and Speaker notification system when the blindSpotDectection algorithm 
detects an object moving within 250cm of the rider.
*/
void BSD_notification(int currVal, int prevVal1, int prevVal2, int prevVal3) {
  if (blindSpotDetection(currVal, prevVal1, prevVal2, prevVal3) == true) {
    digitalWrite(LED_PIN, HIGH);
    tone(SPEAKER_PIN, 100);
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(SPEAKER_PIN);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////




//Define Variables
float zRot;
float errorZ;
float turntime;
float sttime;

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

  // float headingRadians = atan2(mag.magnetic.y, mag.magnetic.x);
  // float headingDegrees = headingRadians * 180 / PI;
  // float declinationAngle = 11.41666666666667;

  // headingDegrees += declinationAngle;

  // Serial.print(" \t\tHeading Degrees: ");
  // Serial.print(headingDegrees);
  // Serial.println(" Degrees");

Serial.print("\t\tGyro z: ");
Serial.print(gyro.gyro.z*180/PI);  
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
    sttime = sttime + 0.100;
    if(sttime > 6){
      zRot = 0;
      sttime =0;
    }

 //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  sonarDistance(trigPin1, echoPin1);
  left_Matrix[1][0] = left_Matrix[0][0];
  delay(10); 
  left_Matrix[0][0] = distance;

  sonarDistance(trigPin2, echoPin2);
  ML_Matrix[1][0] = ML_Matrix[0][0];
  delay(10); 
  ML_Matrix[0][0] = distance;

  sonarDistance(trigPin3, echoPin3);
  for (int i = 3; i > 0; i--){
    middle_Matrix[i][0] = middle_Matrix[i-1][0];
  }
  delay(10);
  middle_Matrix[0][0] = distance;

  // middle_Matrix[1][0] = middle_Matrix[0][0];
  // delay(10); 
  // middle_Matrix[0][0] = distance;
  
  sonarDistance(trigPin4, echoPin4);
  for (int i = 3; i > 0; i--){
    matrix4[i][0] = matrix4[i-1][0];
  }
  delay(10);
  matrix4[0][0] = distance;

  // MR_Matrix[1][0] = MR_Matrix[0][0];
  // delay(50); 
  // MR_Matrix[0][0] = distance;

  sonarDistance(trigPin5, echoPin5);
  right_Matrix[1][0] = right_Matrix[0][0];
  delay(10); 
  right_Matrix[0][0] = distance;

  /*
  - Blind spot dectection algorithm is called for detection.
  */
  // blindSpotDetection(left_Matrix[0][0], left_Matrix[1][0], left_Matrix[2][0], left_Matrix[3][0]);
  // BSD_notification(left_Matrix[0][0], left_Matrix[1][0], left_Matrix[2][0], left_Matrix[3][0]);

  // blindSpotDetection(ML_Matrix[0][0], ML_Matrix[1][0], ML_Matrix[2][0], ML_Matrix[3][0]);
  // BSD_notification(ML_Matrix[0][0], ML_Matrix[1][0], ML_Matrix[2][0], ML_Matrix[3][0]);

  blindSpotDetection(middle_Matrix[0][0], middle_Matrix[1][0], middle_Matrix[2][0], middle_Matrix[3][0]);
  BSD_notification(middle_Matrix[0][0], middle_Matrix[1][0], middle_Matrix[2][0], middle_Matrix[3][0]);

  // blindSpotDetection(MR_Matrix[0][0], MR_Matrix[1][0], MR_Matrix[2][0], MR_Matrix[3][0]);
  // BSD_notification(MR_Matrix[0][0], MR_Matrix[1][0], MR_Matrix[2][0], MR_Matrix[3][0]);

  // blindSpotDetection(right_Matrix[0][0], right_Matrix[1][0], right_Matrix[2][0], right_Matrix[3][0]);
  // BSD_notification(right_Matrix[0][0], right_Matrix[1][0], right_Matrix[2][0], right_Matrix[3][0]);


  // PRINTING THE MATRICES FOR TESTING PURPOSES
  for (int i = 0; i < 3; i++) {
    Serial.print(middle_Matrix[i][0]);
    Serial.print("-");
  }
  Serial.println(middle_Matrix[3][0]);
  ////////////////////////////////////////////////////////////////////////////////////////////////////    

  }
  Serial.println();
  Serial.println();
  Serial.println();

  delay(100);
}
