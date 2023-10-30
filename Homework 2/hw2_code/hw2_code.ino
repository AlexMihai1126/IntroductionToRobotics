#include <SimpleStack.h>

#define firstFloorLED 7
#define secondFloorLED 8
#define thirdFloorLED 9
#define buzzer 3
#define elevatorStateLED 10
#define firstFloorBtn 4
#define secondFloorBtn 12
#define thirdFloorBtn 13
#define doorCloseTimer 2500
#define moveTime 5000
#define debounceTime 150
#define elevatorLEDBlinkInterval 250

enum State {
  ARRIVAL,
  MOVING,
  DOORS_CLOSE,
  OFF
};

int currentFloor = 1; //we always start from the 1st floor
bool firstFloorBtnLastReading = LOW;
bool secondFloorBtnLastReading = LOW;
bool thirdFloorBtnLastReading = LOW;
bool firstFloorBtnState = LOW;
bool secondFloorBtnState = LOW;
bool thirdFloorBtnState = LOW;
bool elevatorLEDCurrentState = LOW;
unsigned long prevMillisMove = 0;
unsigned long prevMillisDoors = 0;
unsigned long lastDebounceTime = 0;
SimpleStack<int> floorStack(10);
int topFloorOnStack = 0;
State buzzerAction = OFF; //we use enum to avoid magic numbers

void floorIndicators(int floor) {
  switch (floor) {
    case 1:
      digitalWrite(firstFloorLED, HIGH);
      digitalWrite(secondFloorLED, LOW);
      digitalWrite(thirdFloorLED, LOW);
      break;
    case 2:
      digitalWrite(firstFloorLED, LOW);
      digitalWrite(secondFloorLED, HIGH);
      digitalWrite(thirdFloorLED, LOW);
      break;
    case 3:
      digitalWrite(firstFloorLED, LOW);
      digitalWrite(secondFloorLED, LOW);
      digitalWrite(thirdFloorLED, HIGH);
      break;
    default:
      digitalWrite(firstFloorLED, LOW);
      digitalWrite(secondFloorLED, LOW);
      digitalWrite(thirdFloorLED, LOW);
      break;
  }
}

void buzzerMode(int state) {
  switch (state) {
    case 0:  //arrival
      tone(buzzer, 1500, 500);
      Serial.println("Buzzer made an arrival sound");
      break;
    case 1:  //moving
      tone(buzzer, 800, 250);
      Serial.println("Buzzer made a moving sound");
      break;
    case 2:  //doors closing
      tone(buzzer, 2500, 300);
      Serial.println("Buzzer made a door closing sound");
      break;
    default:  //off
      digitalWrite(buzzer, LOW);
      Serial.println("Buzzer has been turned off");
      break;
  }
}

void elevatorLED(bool on) {
  if (on) {
    if (millis() - prevMillisMove >= elevatorLEDBlinkInterval) {
      prevMillisMove = millis();
      if (elevatorLEDCurrentState == LOW) {
        elevatorLEDCurrentState = HIGH;
      } else {
        elevatorLEDCurrentState = LOW;
      }
      digitalWrite(elevatorStateLED, elevatorLEDCurrentState);
    }
  }
}


void setup() {
  pinMode(firstFloorLED, OUTPUT);
  pinMode(secondFloorLED, OUTPUT);
  pinMode(thirdFloorLED, OUTPUT);
  pinMode(elevatorStateLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(firstFloorBtn, INPUT_PULLUP);
  pinMode(secondFloorBtn, INPUT_PULLUP);
  pinMode(thirdFloorBtn, INPUT_PULLUP);
  buzzerMode(buzzerAction=OFF);  //buzzer is OFF by default, we explicitly initialize it as such to avoid any noises during startup
  floorIndicators(currentFloor);
  Serial.begin(9600);
}

void loop() {
  bool firstFloorBtnReading = !digitalRead(firstFloorBtn);
  bool secondFloorBtnReading = !digitalRead(secondFloorBtn);
  bool thirdFloorBtnReading = !digitalRead(thirdFloorBtn);  //we use ! because we have a PULLUP configuration meaning we have to invert the output

  if (firstFloorBtnReading != firstFloorBtnLastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceTime) {
    if (firstFloorBtnReading != firstFloorBtnState) {
      firstFloorBtnState = firstFloorBtnReading;
      if (firstFloorBtnReading == HIGH) {
        Serial.println("Call from 1st floor");
        floorStack.push(1);
      }
    }
  }
  firstFloorBtnLastReading = firstFloorBtnReading;
  //pt buton 1

  if (secondFloorBtnReading != secondFloorBtnLastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceTime) {
    if (secondFloorBtnReading != secondFloorBtnState) {
      secondFloorBtnState = secondFloorBtnReading;
      if (secondFloorBtnReading == HIGH) {
        Serial.println("Call from 2nd floor");
        floorStack.push(2);
      }
    }
  }
  secondFloorBtnLastReading = secondFloorBtnReading;
  //pt buton 2


  if (thirdFloorBtnReading != thirdFloorBtnLastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceTime) {
    if (thirdFloorBtnReading != thirdFloorBtnState) {
      thirdFloorBtnState = thirdFloorBtnReading;
      if (thirdFloorBtnReading == HIGH) {
        Serial.println("Call from 3rd floor");
        floorStack.push(3);
      }
    }
  }
  thirdFloorBtnLastReading = thirdFloorBtnReading;
  //pt buton 3

  if (!floorStack.isEmpty()) {
    floorStack.peek(&topFloorOnStack);
    if (currentFloor != topFloorOnStack) {
      unsigned long currentMillisDoors = millis();
      Serial.println("Floor call is not from the current floor, executing");
      elevatorLED(false);
      if (currentMillisDoors - prevMillisDoors >= doorCloseTimer) {
        prevMillisDoors = currentMillisDoors;
        Serial.println("Doors have closed.");
        buzzerMode(buzzerAction=DOORS_CLOSE);
        elevatorLED(true);
      }
      //after a short delay elevator will start moving
      buzzerMode(buzzerAction=MOVING);
      unsigned long currentMillisMove = millis();
      if (currentMillisMove - prevMillisMove >= moveTime) {
        prevMillisMove = currentMillisMove;
        Serial.println("Elevator arrived at the desired floor.");
        currentFloor = topFloorOnStack;
        floorIndicators(currentFloor);
        buzzerMode(buzzerAction=ARRIVAL);
        elevatorLED(false);
      }
    } else {
      Serial.println("Ignoring call as we are already at the floor that the button was pressed from");
    }
    int objToRemove = topFloorOnStack;
    floorStack.pop(&objToRemove);
  }
}
