#include <Keypad.h>
#include <Joystick.h>
#include <TimerOne.h> // Include the TimerOne library for timer functions

#define ENABLE_PULLUPS
#define NUMROWS 7
#define NUMCOLS 5
#define BUTTON31 30
#define LED1 8
#define LED2 9
#define LED3 10
#define LED4 11
#define LED5 12

byte buttons[NUMROWS][NUMCOLS] = {
  { 0, 1, 2, 3, 4 },
  { 5, 6, 7, 8, 9 },
  { 10, 11, 12, 13, 14 },
  { 15, 16, 17, 18, 19 },
  { 20, 21, 22, 23, 24 },
  { 25, 26, 27, 28, 29 },
};

byte rowPins[NUMROWS] = { 5, 4, 3, 2, 1, 0 };
byte colPins[NUMCOLS] = { A4, A3, A2, A1, A0 };

Keypad buttonmatrix = Keypad(makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK, 31, 0,
                   false, false, false, false, false, false,
                   false, false, false, false, false);

const int button31Pin = 7;
int lastButton31State = LOW;
unsigned long lastButtonPressTime = 0;
const unsigned long buttonPressDuration = 500; // 0.5 second

void SetLEDsLow() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
}

void setup() {
  pinMode(button31Pin, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);
  Joystick.begin();

  // Initialize Timer1 to call the TimerCallback function every 1 ms
  Timer1.initialize(1000); // 1 ms interval
  Timer1.attachInterrupt(TimerCallback);

  buttonmatrix.setDebounceTime(50); // Ignore bounce for 250ms after the first keypress
}

void loop() {
  CheckButton31();
  CheckAllButtons();
}

void ToggleLED(char key) {
  Serial.println("Enter ToggleLed");
  switch (key) {
    case 0:
      Serial.println("Toggled LED 1");
      digitalWrite(LED1, !digitalRead(LED1));
      break;
    case 1:
      Serial.println("Toggled LED 2");
      digitalWrite(LED2, !digitalRead(LED2));
      break;
    case 2:
      Serial.println("Toggled LED 3");
      digitalWrite(LED3, !digitalRead(LED3));
      break;
    case 3:
      Serial.println("Toggled LED 4");
      digitalWrite(LED4, !digitalRead(LED4));
      break;
    default:
      break;
  }
}

void CheckButton31() {
  int button31State = digitalRead(button31Pin);

  if (button31State != lastButton31State) {
    // State change detected

    // Button has gone from low to high or high to low
    Joystick.setButton(BUTTON31, 1);
    lastButtonPressTime = millis(); // Record the time of the button press

    if (button31State == LOW) {
      // When button31Pin is low, set the LEDs
      SetLEDsLow();
      delay(500); // 0.5-second delay
      Joystick.setButton(BUTTON31, 0); // Set the button to 0
    } else {
      // When button31Pin is high
    }
  } else {
    // No state change
    if (button31State == HIGH && millis() - lastButtonPressTime >= buttonPressDuration) {
      // Button is currently high, and the press duration has passed
      Joystick.setButton(BUTTON31, 0); // Set the button to 0
    }
  }

  lastButton31State = button31State;
}


void CheckAllButtons() {
  if (buttonmatrix.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {
      if (buttonmatrix.key[i].stateChanged) {
        switch (buttonmatrix.key[i].kstate) {
          case PRESSED:
          case HOLD:
            Joystick.setButton(buttonmatrix.key[i].kchar, 1);
            if (buttonmatrix.key[i].kchar >= 0 && buttonmatrix.key[i].kchar <= 3) {
              ToggleLED(buttonmatrix.key[i].kchar);
            }
            break;
          case RELEASED:
          case IDLE:
            Joystick.setButton(buttonmatrix.key[i].kchar, 0);
            if (buttonmatrix.key[i].kchar >= 0 && buttonmatrix.key[i].kchar <= 3) {
              ToggleLED(buttonmatrix.key[i].kchar);
            }
            break;
        }
      }
    }
  }
}

// Callback function for Timer1
void TimerCallback() {
  // You can use this function for any time-sensitive operations
  // It will be called every 1 ms
  // You may not need this function for the
}
