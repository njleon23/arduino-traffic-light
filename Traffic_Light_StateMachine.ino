// Pin Definitions
const int red = 8;
const int yellow = 13;
const int green = 12;

const int trigPin = 7;
const int echoPin = 6;

//Detection and Timing Settings
const int carDetectedDistance = 20; //cm
const unsigned long yellowDuration = 2000;
const unsigned long redDuration = 5000;
const unsigned long cooldownDuration = 5000;

//State Machine Definition
enum TrafficState { 
  GREEN, 
  YELLOW, 
  RED, 
  COOLDOWN 
};

TrafficState currentState = GREEN; //initial state 
unsigned long stateStartTime = 0;  //tracks when we entered the current state

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600); //baud rate
  digitalWrite(green, HIGH); //start with green
}

void loop() {
  unsigned long currentTime = millis();

  //Get Distance from Ultrasonic Sensor
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance (cm) : ");
  Serial.println(distance);

  //State Machine
  switch (currentState) {

    case GREEN: 
      //Turns on green, others off
      digitalWrite(green, HIGH);
      digitalWrite(yellow, LOW);
      digitalWrite(red, LOW);

      //Check if car is detected and green has been on for at least 5 seconds
      if (distance > 0 && distance < carDetectedDistance && (currentTime - stateStartTime >= cooldownDuration)) {
        currentState = YELLOW;
        stateStartTime = currentTime;
      }
      break;

    case YELLOW:
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
      digitalWrite(red, LOW);

      if (currentTime - stateStartTime >= yellowDuration) {
        currentState = RED;
        stateStartTime = currentTime;
      }
      break;

    case RED: 
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(red, HIGH);

      if (currentTime - stateStartTime >= redDuration) {
        currentState = COOLDOWN;
        stateStartTime = currentTime;
      }
      break;

    case COOLDOWN: 
      digitalWrite(green, HIGH);
      digitalWrite(yellow, LOW);
      digitalWrite(red, LOW);

      if (currentTime - stateStartTime >= 5000) {
        currentState = GREEN;
        stateStartTime = currentTime;
      }
      break;
  }

  delay(250); //sensor check pacing

}
