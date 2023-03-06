/* 
  Project: Smart Helmet with Blind Spot Detection
  File: Smart_Helmet_BSD
  Date created: 03/05/2023
  Author: Christopher Simek
  Date last revised: 03/05/2023
  Last revised author: Christopher Simek
*/

/* ALWAYS UPDATE THE HEADER AFTER EDITING THE FILE!!!!*/

// LED pin
const int LED_PIN = 13;

const int trigPin1 = 2; // Left Ultrasonic Trig pin: Arduino Uno = 2, Adafruit RP2040 = 2
const int echoPin1 = 3; // Left Ultrasonic Echo pin: Arduino Uno = 3, Adafruit RP2040 = 3
const int trigPin2 = 4; // Middle-Left Ultrasonic Trig pin: Arduino Uno = 4, Adafruit RP2040 = 6
const int echoPin2 = 5; // Middle-Left Ultrasonic Echo pin: Arduino Uno = 5, Adafruit RP2040 = 7
const int trigPin3 = 6; // Middle Ultrasonic Trig pin: Arduino Uno = 6, Adafruit RP2040 = 8
const int echoPin3 = 7; // Middle Ultrasonic Echo pin: Arduino Uno = 7, Adafruit RP2040 = 9
const int trigPin4 = 8; // Middle-Right Ultrasonic Trig pin: Arduino Uno = 8, Adafruit RP2040 = 10
const int echoPin4 = 9; // Middle-Right Ultrasonic Echo pin: Arduino Uno = 9, Adafruit RP2040 = 11
const int trigPin5 = 10; // Right Ultrasonic Trig pin: Arduino Uno = 10, Adafruit RP2040 = 12
const int echoPin5 = 11; // Right Ultrasonic Echo pin: Arduino Uno = 11, Adafruit RP2040 = 13

// long int for the duration of the pulse
// int for the duration to be converted into a distance in ccm
long duration; 
int distance_Left, distance_ML, distance_Middle, distance_MR, distance_Right;

int distance;
// Matrix containing ALL the curr and prev distances from the ultrasonic sensors 
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
*/
long sonarDuration(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  if (distance > 450) {
    distance = 450;
  }
}

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

bool blindSpotDetection(int currVal, int prevVal) {
  while (currVal  <= 255) {
    if (currVal - prevVal <= 5 || currVal - prevVal >= -5) {
      if (currVal - prevVal < -5) {
        return false;
        } else {
        return true;
      }
    }
  }
  return false;
}


void loop() {
  /* 
    - Call the sonarDuration function to have the ultrasonic sensors send a pulse
    and return the duration.
    - Calculate the distance is cm using the equation: distance = duration * 0.034 / 2 
  */
/*
  sonarDistance(trigPin1, echoPin1);
  Serial.print(distance);

  sonarDistance(trigPin2, echoPin2);
  Serial.print("-");
  Serial.print(distance);

  sonarDistance(trigPin3, echoPin3);
  Serial.print("-");
  Serial.print(distance);
*/
  
  sonarDistance(trigPin4, echoPin4);

  for (int i = 9; i > 0; i--) {
    ultra_matrix[i][0] = ultra_matrix[i-1][0];
  }
  ultra_matrix[0][0] = distance;

  MR_Matrix[1][0] = MR_Matrix[0][0];
  MR_Matrix[0][0] = distance;
  if (blindSpotDetection(MR_Matrix[0][0], MR_Matrix[1][0]) == true) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  Serial.print(MR_Matrix[0][0]);
  Serial.print("-"); 
  Serial.println(MR_Matrix[1][0]);

  // sonarDistance(trigPin5, echoPin5);
  // Serial.print("-");
  // Serial.println(distance);
 
  // sonarDuration(trigPin1, echoPin1);
  // distance_Left = (duration*0.034)/2;
  // Serial.print(distance_Left);
  // Serial.print(distance);

  // sonarDuration(trigPin2, echoPin2);
  // distance_ML = (duration*0.034)/2;
  // Serial.print(" - ");
  // Serial.print(distance_ML);

  // sonarDuration(trigPin3, echoPin3);
  // distance_Middle = (duration*0.034)/2;
  // Serial.print(" - ");
  // Serial.print(distance_Middle);

  // sonarDuration(trigPin4, echoPin4);
  // distance_MR = (duration*0.034)/2;
  // Serial.print(" - ");
  // Serial.print(distance_MR);

  // sonarDuration(trigPin5, echoPin5);
  // distance_Right = (duration*0.034)/2;
  // Serial.print(" - ");
  // Serial.println(distance_Right);

  // left_Matrix[1][0] = left_Matrix[0][0];
  // left_Matrix[0][0] = distance_Left;

  // Serial.println(left_Matrix[0][0]);
  // Serial.println(left_Matrix[1][0]);

}
