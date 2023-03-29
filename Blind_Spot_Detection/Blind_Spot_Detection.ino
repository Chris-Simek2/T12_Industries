/* 
  Project: Smart Helmet with Blind Spot Detection
  File: Smart_Helmet_BSD
  Date created: 03/05/2023
  Author: Christopher Simek
  Date last revised: 03/06/2023
  Last revised author: Christopher Simek
*/

/* ALWAYS UPDATE THE LAST REVISED AUTHOR AFTER EDITING THE FILE!*/

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


// Setup the Trig pins to be outputs and the Echo pins to be inputs for all ultrasonic sensors.
// Set serial monitor to 9600
void setup()
{
  Serial.begin (9600);

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
}

/* 
  - Function to get the duration of the time from when the trig pin sends
  out a pulse to when the echo pin receives the pulse back.
  - Set the delay to 10ms.
  - Converts the duration to cm.
  - Set Max distance to 450 cm.
*/

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


void loop() {
  /* 
    - Call the sonarDistance function to have the ultrasonic sensors send a pulse
    and return the distance.
    - Store the current and previous distances in the associated matrix.
  */
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

  // Serial.print(left_Matrix[0][0]);
  // Serial.print("-");
  // Serial.print(left_Matrix[1][0]);
  // Serial.print(" : ");
  // Serial.print(ML_Matrix[0][0]);
  // Serial.print("-");
  // Serial.print(ML_Matrix[1][0]);
  // Serial.print(" : ");
  // Serial.print(middle_Matrix[0][0]);
  // Serial.print("-");
  // Serial.print(middle_Matrix[1][0]);
  // Serial.print(" : ");
  // Serial.print(MR_Matrix[0][0]);
  // Serial.print("-");
  // Serial.print(MR_Matrix[1][0]);
  // Serial.print(" : ");
  // Serial.print(right_Matrix[0][0]);
  // Serial.print("-");
  // Serial.println(right_Matrix[1][0]);
}
