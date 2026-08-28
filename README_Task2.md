# Keeping Watch Over Odysseus

## About the Project

This project is an Arduino-based state machine made for the **"Keeping Watch Over Odysseus"** task.

The idea is to have the Arduino keep track of the ship's current situation and react when it detects different problems. An LDR is used to detect a storm, an ultrasonic sensor is used to detect Charybdis, and a push button is used to drop or raise the anchor.

The current state of the ship is shown on a 16x2 LCD. An LED is used as a warning during a storm, while a buzzer gives a siren-like warning when Charybdis is detected.

The whole circuit was built and tested using **Tinkercad Circuits**.

## Components Used

* Arduino UNO R3
* 16x2 LCD
* LDR (Photoresistor)
* 10kΩ resistor
* HC-SR04 Ultrasonic Sensor
* Push button
* LED
* Buzzer

## Pin Connections

| Component       | Arduino Pin |
| --------------- | ----------- |
| LCD RS          | 12          |
| LCD Enable      | 11          |
| LCD D4          | 5           |
| LCD D5          | 4           |
| LCD D6          | 3           |
| LCD D7          | 2           |
| Ultrasonic Trig | 7           |
| Ultrasonic Echo | 8           |
| LED             | 9           |
| Buzzer          | 10          |
| Push Button     | 13          |
| LDR             | A0          |

The push button uses the Arduino's built-in pull-up resistor, so one side is connected to pin 13 and the other side is connected to GND.

## How the State Machine Works

The system has five states:

* OPEN SEA
* ANCHOR DROPPED
* STORM
* CHARYBDIS
* WRECKED

### OPEN SEA

This is the starting state.

While in OPEN SEA, the Arduino checks both sensors.

* If the light level goes below **512**, it changes to **STORM**.
* If something comes within **100 cm**, it changes to **CHARYBDIS**.
* Pressing the button changes the state to **ANCHOR DROPPED**.

If both conditions happen at the same time, STORM gets priority.

### ANCHOR DROPPED

When the button is pressed, the ship drops its anchor.

While in this state, the sensors are not checked, so the ship stays anchored even if the light level or distance changes.

Pressing the button again brings the ship back to OPEN SEA.

### STORM

A low light level is treated as a storm.

When STORM starts:

* The LCD displays `STORM`.
* The LED starts blinking.
* A 5-second timer starts.

If the light returns to normal before 5 seconds, the system goes back to OPEN SEA.

If the storm continues for 5 seconds, the ship becomes WRECKED.

### CHARYBDIS

If the ultrasonic sensor detects something less than **100 cm** away, the system enters CHARYBDIS.

When this happens:

* The LCD displays `CHARYBDIS`.
* The buzzer produces a rising and falling siren.
* A 5-second timer starts.

If the object moves away before 5 seconds, the system returns to OPEN SEA.

If it stays within 100 cm for 5 seconds, the ship becomes WRECKED.

### WRECKED

WRECKED is the final state.

Once the ship has been exposed to STORM or CHARYBDIS for 5 seconds, it enters WRECKED.

The system stays in this state and does not respond to the button or sensors. The simulation has to be restarted to begin again from OPEN SEA.

## State Flow

```text
                       Button
                         ↓
                  ANCHOR DROPPED
                         │
                       Button
                         ↓
                      OPEN SEA
                     /        \
            Light < 512      Distance < 100 cm
                 ↓                  ↓
              STORM             CHARYBDIS
                 │                  │
              5 sec              5 sec
                 ↓                  ↓
              WRECKED            WRECKED
```

If the danger goes away before 5 seconds:

```text
STORM ───────────→ OPEN SEA
       Light returns

CHARYBDIS ───────→ OPEN SEA
       Object moves away
```

## Testing the Project

The different states can be tested directly in Tinkercad:

1. Start the simulation. The LCD should show `OPEN SEA`.
2. Press the button to test `ANCHOR DROPPED`.
3. Increase the light level again and press the button to return to `OPEN SEA`.
4. Lower the LDR light level below the threshold to trigger `STORM`.
5. Keep the storm active for 5 seconds to trigger `WRECKED`.
6. Restart the simulation and set the ultrasonic sensor distance below 100 cm to test `CHARYBDIS`.
7. Move the detected object away before 5 seconds to return to `OPEN SEA`.
8. Keep it below 100 cm for 5 seconds to trigger `WRECKED`.

## Tools Used

* Arduino UNO R3
* Tinkercad Circuits
* Arduino C++

```
```
