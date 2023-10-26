#include <SimpleStack.h>

#define firstFloorLED 7
#define secondFloorLED 8
#define thirdFloorLED 9
#define buzzer 3
#define elevatorStateLED 10
#define firstFloorBtn A0
#define secondFloorBtn A1
#define thirdFloorBtn A2
#define doorCloseTimer 1500
#define moveTime 4500
#define waitTime 2000
#define debounceTime 150

int currentFloor = 0;
bool elevatorMoving = false;
bool firstFloorBtnPressed=false;
bool secondFloorBtnPressed=false;
bool thirdFloorBtnPressed=false;
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
  if (isMoving) {
    //blink LED
  }
  floorIndicators(floor);
  return floor;
}

void buzzerMode(int state) {
  switch (state) {
    case 1:  //arrival
      tone(buzzer, 250, 100);
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
  elevatorState(0, elevatorMoving);
}

void loop() {
  // put your main code here, to run repeatedly:
}
