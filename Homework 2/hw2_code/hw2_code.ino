#include <SimpleStack.h>

#define firstFloorLED 7
#define secondFloorLED 8
#define thirdFloorLED 9
#define buzzer 3
#define elevatorStateLED 10
#define firstFloorBtn 4
#define secondFloorBtn 12
#define thirdFloorBtn 13
#define doorCloseTimer 1500
#define moveTime 5000
#define waitTime 2000
#define debounceTime 150

int currentFloor = 0;
bool elevatorMoving = false;
bool firstFloorBtnLastReading=LOW;
bool secondFloorBtnLastReading=LOW;
bool thirdFloorBtnLastReading=LOW;
bool firstFloorBtnState=LOW;
bool secondFloorBtnState=LOW;
bool thirdFloorBtnState=LOW;
bool elevatorLEDCurrentState = LOW;
unsigned long prevMillis=0;
const int elevatorLEDBlinkInterval=250;
unsigned long lastDebounceTime = 0;
SimpleStack<int> floorStack(10);

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

int elevatorState(int floor, bool isMoving) {
  if (isMoving == true) {
    unsigned long currentMillis=millis();
    if(currentMillis - prevMillis >= elevatorLEDBlinkInterval){
      prevMillis=currentMillis;
      if(elevatorLEDCurrentState==LOW){
        elevatorLEDCurrentState = HIGH;
      }
      else{
        elevatorLEDCurrentState= LOW;
      }
      digitalWrite(elevatorStateLED, elevatorLEDCurrentState);
    }
  }
  floorIndicators(floor);
  buzzerMode(0);
  return floor;
}

void buzzerMode(int state) {
  switch (state) {
    case 1:  //arrival
      tone(buzzer, 5000, 1000);
      break;
    case 2:  //moving
      tone(buzzer, 250, 100);
      break;
    case 3:  //doors closing
      tone(buzzer, 250, 100);
      break;
    default:  //off
      digitalWrite(buzzer, LOW);
      break;
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
  buzzerMode(0);
  elevatorState(1, true);
}

void loop() {
  bool firstFloorBtnReading = digitalRead(firstFloorBtn);
  bool secondFloorBtnReading = digitalRead(secondFloorBtn);
  bool thirdFloorBtnReading digitalRead(thirdFloorBtn);

  if (firstFloorBtnReading != firstFloorBtnLastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceTime) {
    if (firstFloorBtnReading != firstFloorBtnState) {
      firstFloorBtnState = firstFloorBtnReading;
      if (firstFloorBtnReading == HIGH) {
        //cod pt ce sa se intample cand butonul este HIGH
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
        //cod pt ce sa se intample cand butonul este HIGH
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
        //cod pt ce sa se intample cand butonul este HIGH
      }
    }
  }
  thirdFloorBtnLastReading = thirdFloorBtnReading;

  //pt buton 3
}
