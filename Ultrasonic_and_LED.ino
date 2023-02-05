// defines pins numbers for ultrasonic
const int trigPin = 9;
const int echoPin = 10;

// define pins numbers for LED
int redPin= 7;
int greenPin = 6;
int bluePin = 5;

// defines variables for ultrasonic 
long duration;
int distance;

void setup() {
  // ultrasonic pin setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // LED pin setup
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600); // Starts the serial communication
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance in cm
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance <= 35) {
    setColor(255, 165, 0); // Orange Color
    delay(500); // helps get rid of flickering 
  } else {
    setColor(0,0,0);
  }
}

// funciton to set color of RGB LED
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}