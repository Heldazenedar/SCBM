#include <Keypad.h>
#include <Joystick.h>

#define ENABLE_PULLUPS
#define NUMROWS 6
#define NUMCOLS 5

byte buttons[NUMROWS][NUMCOLS] = {
  {0,1,2,3,4},
  {5,6,7,8,9},
  {10,11,12,13,14},
  {15,16,17,18,19},
  {20,21,22,23,24},
  {25,26,27,28,29},
};

byte rowPins[NUMROWS] = {5,4,3,2,1,0};
byte colPins[NUMCOLS] = {A4,A3,A2,A1,A0};

Keypad buttonmatrix = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK, 31, 0,
  false, false, false, false, false, false,
  false, false, false, false, false);

const int button31Pin = 7;
int button31State = LOW;
int lastButton31State = LOW;
unsigned long button31PressTime = 0;
const unsigned long button31PressDuration = 500;

void setup() {
  pinMode(button31Pin, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  Joystick.begin();
}

void loop() {
  CheckButton31();
  CheckAllButtons();
}

void ToggleLED(char key) {
  switch (key) {
    case '1':
      digitalWrite(8, !digitalRead(8));
      break;
    case '2':
      digitalWrite(9, !digitalRead(9));
      break;
    case '3':
      digitalWrite(10, !digitalRead(10));
      break;
    case '4':
      digitalWrite(11, !digitalRead(11));
      break;
    default:
      break;
  }
}

void CheckButton31() {
  button31State = digitalRead(button31Pin);

  if (button31State != lastButton31State) {
    if (button31State == HIGH) {
      Joystick.setButton(31, 1);
      button31PressTime = millis();
    } else {
      Joystick.setButton(31, 0);
    }
  }

  if (button31State == LOW && (millis() - button31PressTime) > button31PressDuration) {
    Joystick.setButton(31, 0);
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
            if (buttonmatrix.key[i].kchar >= '1' && buttonmatrix.key[i].kchar <= '4') {
              ToggleLED(buttonmatrix.key[i].kchar);
            }
            break;
          case RELEASED:
          case IDLE:
            Joystick.setButton(buttonmatrix.key[i].kchar, 0);
            break;
        }
      }
    }
  }
}



