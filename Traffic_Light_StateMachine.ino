/* Sensor-Triggered Traffic Light System ------------------------------
   This Arduino project uses an ultrasonic sensor (HC-SR04) to detect
   approaching vehicles and trigger traffic light changes using a 
   finite state machine (FSM) model.

   States:
   - GREEN: default state with green LED on
   - YELLOW: warning before stopping
   - RED: stop state
   - COOLDOWN: minimum delay to prevent rapid retriggering

   Timing is controlled using millis() for non-blocking logic. 
   This allows sensor checks and LED changes to happen without pausing the system.
*/


// Pin Definitions for LEDs
const int red = 8;
const int yellow = 13;
const int green = 12;

// Pin Definitions for Ultrasonic Sensor
const int trigPin = 7;
const int echoPin = 6;

// Detection and Timing Settings
const int carDetectedDistance = 20; // centimeters - threshold to detect a nearby car
const unsigned long yellowDuration = 2000;    // yellow light duration (2 seconds)
const unsigned long redDuration = 5000;       // red light duration (5 seconds)
const unsigned long cooldownDuration = 5000;  // minimum time before detecting another car

// Finite State Machine for traffic light logic
enum TrafficState { 
  GREEN, 
  YELLOW, 
  RED, 
  COOLDOWN 
};

TrafficState currentState = GREEN; // start in green state
unsigned long stateStartTime = 0;  // tracks when the current state started

void setup() {
  // Initialize LED pins as outputs
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Begin Serial communication for debugging
  Serial.begin(9600);

  // Turn on green at start
  digitalWrite(green, HIGH); // start with green
}

void loop() {
  unsigned long currentTime = millis(); // Get the current time since program start

  // --- Get Distance Reading from Ultrasonic Sensor ---
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);               // short pause
  digitalWrite(trigPin, HIGH);        // trigger a 10µs pulse
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);  // measure echo return time
  distance = duration * 0.034 / 2;    // convert to cm (speed of sound = 0.034 cm/µs)

  Serial.print("Distance (cm) : ");
  Serial.println(distance);

  // --- FSM State Control ---
  switch (currentState) {
    case GREEN: 
      // Default state - green light on
      digitalWrite(green, HIGH);
      digitalWrite(yellow, LOW);
      digitalWrite(red, LOW);

      // If a car is detected nearby and green has been on long enough, switch to yellow
      if (distance > 0 && distance < carDetectedDistance && (currentTime - stateStartTime >= cooldownDuration)) {
        currentState = YELLOW;
        stateStartTime = currentTime;
      }
      break;

    case YELLOW:
      // Transition state - yellow light on
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
      digitalWrite(red, LOW);


      //Move to red after yellowDuration
      if (currentTime - stateStartTime >= yellowDuration) {
        currentState = RED;
        stateStartTime = currentTime;
      }
      break;

    case RED: 
      // Stop state - red light on
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(red, HIGH);

      // Move to cooldown after redDuration
      if (currentTime - stateStartTime >= redDuration) {
        currentState = COOLDOWN;
        stateStartTime = currentTime;
      }
      break;

    case COOLDOWN: 
      // Allow cooldown before green resets
      digitalWrite(green, HIGH);
      digitalWrite(yellow, LOW);
      digitalWrite(red, LOW);

      if (currentTime - stateStartTime >= 5000) {
        currentState = GREEN;
        stateStartTime = currentTime;
      }
      break;
  }

  delay(250); // Delay to pace ultrasonic sensor readings and serial output
}
