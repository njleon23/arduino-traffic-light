# Sensor-Triggered Traffic Light System (Arduino)

A smart traffic light simulation using an ultrasonic distance sensor (HC-SR04) and a finite state machine. This project detects nearby vehicles and changes the LED traffic lights accordingly, mimicking a real-world smart traffic light controller.

---

## Features

- Detects cars using the HC-SR04 ultrasonic sensor
- Controls red, yellow, and green LEDs based on car proximity
- Implements state machine logic with real-time transitions
- Uses `millis()` for **non-blocking timing**
- Built on an Elegoo Uno (Arduino-compatible)

---

## Components Used

- Arduino Uno (or compatible)
- HC-SR04 Ultrasonic Sensor
- 3 LEDs (Red, Yellow, Green)
- Resistors (220Ω recommended)
- Breadboard and jumper wires

---

## State Machine Logic

The system operates in four states:

1. **GREEN**: Default state, green LED on
2. **YELLOW**: Transition state after detecting a car nearby
3. **RED**: Red LED on, simulating stop
4. **COOLDOWN**: Reset delay before returning to green

A car detection triggers the transition if it's closer than `20cm`.

---

## Code Highlights

- **`millis()`** is used to implement **non-blocking delays** so the microcontroller remains responsive while waiting.
- A switch-case state machine is used to manage light transitions based on time and distance.

---

## Getting Started

1. Clone the repository or download the `.ino` file.
2. Open the sketch in the Arduino IDE.
3. Connect the components as described.
4. Select the correct **board** and **port** from the Arduino IDE.
5. Upload and run!

---

## License

This project is open-source and free to use under the MIT License.
