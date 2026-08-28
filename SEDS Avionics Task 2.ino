```cpp
#include <LiquidCrystal.h>

// LCD connected to Arduino pins 12, 11, 5, 4, 3 and 2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pin assignments for all components
const int trigPin = 7;
const int echoPin = 8;
const int ledPin = 9;
const int buzzerPin = 10;
const int buttonPin = 13;
const int lightPin = A0;

// Sensor thresholds
const int lightThreshold = 512;
const int distanceThreshold = 100;

// Time limits and intervals
const unsigned long dangerTime = 5000;
const unsigned long debounceTime = 50;
const unsigned long blinkTime = 300;

// Possible states of the system
enum State {
  OPEN_SEA,
  ANCHOR_DROPPED,
  STORM,
  CHARYBDIS,
  WRECKED
};

State currentState = OPEN_SEA;

// Variables used for timing and outputs
unsigned long dangerStartTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long lastButtonTime = 0;

bool ledState = false;
bool lastButtonState = HIGH;

// Measures the distance detected by the ultrasonic sensor
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  // If no echo is received, assume there is no nearby object
  if (duration == 0) {
    return 999;
  }

  return duration * 0.0343 / 2;
}

// Displays the current system state on the LCD
void displayState() {
  lcd.clear();

  switch (currentState) {
    case OPEN_SEA:
      lcd.print("OPEN SEA");
      break;

    case ANCHOR_DROPPED:
      lcd.print("ANCHOR DROPPED");
      break;

    case STORM:
      lcd.print("STORM");
      break;

    case CHARYBDIS:
      lcd.print("CHARYBDIS");
      break;

    case WRECKED:
      lcd.print("WRECKED");
      break;
  }
}

// Changes the current state and resets the required timers and outputs
void changeState(int newState) {
  currentState = (State)newState;

  // Start the danger timer when entering a dangerous state
  if (currentState == STORM || currentState == CHARYBDIS) {
    dangerStartTime = millis();
  } else {
    dangerStartTime = 0;
  }

  // Turn off the LED when leaving the STORM state
  if (currentState != STORM) {
    digitalWrite(ledPin, LOW);
    ledState = false;
  }

  // Stop the buzzer when leaving the CHARYBDIS state
  if (currentState != CHARYBDIS) {
    noTone(buzzerPin);
  }

  displayState();
}

// Detects a button press and handles switch bouncing
bool buttonPressed() {
  bool buttonState = digitalRead(buttonPin);

  // With INPUT_PULLUP, a pressed button reads LOW
  if (buttonState == LOW && lastButtonState == HIGH) {
    if (millis() - lastButtonTime >= debounceTime) {
      lastButtonTime = millis();
      lastButtonState = buttonState;
      return true;
    }
  }

  lastButtonState = buttonState;
  return false;
}

void setup() {
  Serial.begin(9600);

  // Configure sensor and output pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Internal pull-up resistor is used for the button
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(lightPin, INPUT);

  lcd.begin(16, 2);
  displayState();
}

/*
void loop() {
  Serial.println(digitalRead(buttonPin));
  delay(200);
}
*/

// Main state machine
void loop() {

  // WRECKED is a terminal state
  if (currentState == WRECKED) {
    return;
  }

  // Pressing the button toggles between OPEN SEA and ANCHOR DROPPED
  if (buttonPressed()) {
    if (currentState == ANCHOR_DROPPED) {
      changeState(OPEN_SEA);
    } else {
      changeState(ANCHOR_DROPPED);
    }
    return;
  }

  // While anchored, the sensors are ignored
  if (currentState == ANCHOR_DROPPED) {
    return;
  }

  // Read the sensors
  int lightValue = analogRead(lightPin);
  Serial.println(lightValue);
  float distance = getDistance();

  switch (currentState) {

    // Normal operating state
    case OPEN_SEA:

      // Low light indicates a storm
      if (lightValue < lightThreshold) {
        changeState(STORM);

      // An object closer than 100 cm indicates Charybdis
      } else if (distance < distanceThreshold) {
        changeState(CHARYBDIS);
      }

      break;

    // Storm warning state
    case STORM:

      // Blink the LED continuously while the storm is active
      if (millis() - lastBlinkTime >= blinkTime) {
        lastBlinkTime = millis();
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
      }

      // If the storm lasts for 5 seconds, the ship is wrecked
      if (millis() - dangerStartTime >= dangerTime) {
        changeState(WRECKED);

      // If the light returns to normal, leave the storm state
      } else if (lightValue >= lightThreshold) {
        changeState(OPEN_SEA);
      }

      break;

    // Charybdis warning state
    case CHARYBDIS:
      {
        // Create a rising and falling siren effect
        unsigned long sirenTime = millis() % 2000;
        int frequency;

        if (sirenTime < 1000) {
          frequency = map(sirenTime, 0, 999, 500, 1800);
        } else {
          frequency = map(sirenTime, 1000, 1999, 1800, 500);
        }

        tone(buzzerPin, frequency);

        // If Charybdis remains within range for 5 seconds, the ship is wrecked
        if (millis() - dangerStartTime >= dangerTime) {
          changeState(WRECKED);

        // If the object moves away, return to open sea
        } else if (distance >= distanceThreshold) {
          changeState(OPEN_SEA);
        }
      }

      break;

    // This state is handled before the switch statement
    case ANCHOR_DROPPED:
      break;

    // WRECKED is handled before the switch statement
    case WRECKED:
      break;
  }
}