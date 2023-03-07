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
const int LED_PIN = 13; // LED PIN: Arduino Uno = 13, Adafruit RP2040 = GPIO1
const int SPEAKER_PIN = 12; // SPEAKER PIN: Arduino Uno = 12, Adafruit RP2040 = GPIO0

// Ultrasonic sensors trig and echo pin numbers
const int trigPin1 = 2; // Left Ultrasonic Trig pin: Arduino Uno = 2, Adafruit RP2040 = GPIO2
const int echoPin1 = 3; // Left Ultrasonic Echo pin: Arduino Uno = 3, Adafruit RP2040 = GPIO3
const int trigPin2 = 4; // Middle-Left Ultrasonic Trig pin: Arduino Uno = 4, Adafruit RP2040 = GPIO6
const int echoPin2 = 5; // Middle-Left Ultrasonic Echo pin: Arduino Uno = 5, Adafruit RP2040 = GPIO7
const int trigPin3 = 6; // Middle Ultrasonic Trig pin: Arduino Uno = 6, Adafruit RP2040 = GPIO8
const int echoPin3 = 7; // Middle Ultrasonic Echo pin: Arduino Uno = 7, Adafruit RP2040 = GPIO9
const int trigPin4 = 8; // Middle-Right Ultrasonic Trig pin: Arduino Uno = 8, Adafruit RP2040 = GPIO10
const int echoPin4 = 9; // Middle-Right Ultrasonic Echo pin: Arduino Uno = 9, Adafruit RP2040 = GPIO11
const int trigPin5 = 10; // Right Ultrasonic Trig pin: Arduino Uno = 10, Adafruit RP2040 = GPIO12
const int echoPin5 = 11; // Right Ultrasonic Echo pin: Arduino Uno = 11, Adafruit RP2040 = GPIO13

// int distance_Left, distance_ML, distance_Middle, distance_MR, distance_Right;


// Matrix containing ALL the curr and prev distances from the ultrasonic sensors
// FOR TESTING PURPOSES 
int ultra_matrix[10][1] = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};

// Matrix of the current and previous distances from the ultrasonic sensors
int left_Matrix[2][1] = {{0},{0}};
int ML_Matrix[2][1] = {{0},{0}};
int middle_Matrix[2][1] = {{0},{0}};
int MR_Matrix[2][1] = {{0},{0}};
int right_Matrix[2][1] = {{0},{0}};


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

  // LEDs
  pinMode(LED_PIN, OUTPUT);
}

/* 
  - Function to get the duration of the time from when the trig pin sends
  out a pulse to when the echo pin receives the pulse back.
  - Set the delay to 10ms.
  - Converting the duration to cm with distance variable.
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
- Algorithm for when an object is within 250cm with a +5cm difference
- As an objects currVal and prevVal decrease, the object is getting closer. If so,
blindSpotDectection return true.
- As an objects currVal and prevVal increase, the object is moving away. If so,
blindSpotDectection returns false.
- An added + or - 5 cm is added to account for accuracy of the ultrasonic sensors.
*/
bool blindSpotDetection(int currVal, int prevVal) {
  while (currVal  <= 250 + 5) {
    if (currVal - prevVal <= 5 || currVal - prevVal >= -5) {
      if (currVal - prevVal > 5) {
        return false;
        } else {
        return true;
      }
    }
  }
  return false;
}

/* 
Turns on the LED and Speaker notification system when the blindSpotDectection algorithm 
detects an object moving within 250cm of the rider.
*/
void BSD_notification() {
  if (blindSpotDetection(MR_Matrix[0][0], MR_Matrix[1][0]) == true) {
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
  delay(250); 
  left_Matrix[0][0] = distance;

  sonarDistance(trigPin2, echoPin2);
  ML_Matrix[1][0] = ML_Matrix[0][0];
  delay(250); 
  ML_Matrix[0][0] = distance;

  sonarDistance(trigPin3, echoPin3);
  middle_Matrix[1][0] = middle_Matrix[0][0];
  delay(250); 
  middle_Matrix[0][0] = distance;
  
  sonarDistance(trigPin4, echoPin4);
  MR_Matrix[1][0] = MR_Matrix[0][0];
  delay(250); 
  MR_Matrix[0][0] = distance;

  sonarDistance(trigPin5, echoPin5);
  right_Matrix[1][0] = right_Matrix[0][0];
  delay(250); 
  right_Matrix[0][0] = distance;


  /*
  - Blind spot dectection algorithm is called for detection.
  */
  blindSpotDetection(left_Matrix[0][0], left_Matrix[1][0]);
  BSD_notification();

  blindSpotDetection(ML_Matrix[0][0], ML_Matrix[1][0]);
  BSD_notification();

  blindSpotDetection(middle_Matrix[0][0], middle_Matrix[1][0]);
  BSD_notification();

  blindSpotDetection(MR_Matrix[0][0], MR_Matrix[1][0]);
  BSD_notification();

  blindSpotDetection(right_Matrix[0][0], right_Matrix[1][0]);
  BSD_notification();

}
