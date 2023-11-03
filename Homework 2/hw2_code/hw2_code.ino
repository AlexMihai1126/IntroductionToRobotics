#define firstFloorLED 7
#define secondFloorLED 8
#define thirdFloorLED 9
#define buzzer 3
#define elevatorStateLED 10
#define firstFloorBtn 4
#define secondFloorBtn 12
#define thirdFloorBtn 13
#define doorCloseTimer 1000
#define moveTime 2500
#define debounceTime 150
#define elevatorLEDBlinkInterval 250

enum State {
  ARRIVAL,
  MOVING,
  DOORS_CLOSE,
  OFF
};

short int currentFloor = 1;  //we always start from the 1st floor
short int targetFloor = 1;
bool isAcceptingCmd = true;
bool firstFloorBtnLastReading = LOW;
bool secondFloorBtnLastReading = LOW;
bool thirdFloorBtnLastReading = LOW;
bool firstFloorBtnState = LOW;
bool secondFloorBtnState = LOW;
bool thirdFloorBtnState = LOW;
bool elevatorLEDCurrentState = LOW;
bool elevatorIsStatic = true;
bool movingSoundPlayed = true;  //prevent the sound from playing until the elevator gets moving
short int floorsToTravel = 0;   // we scale our move time based on this number
unsigned long startMillisMove = 0;
unsigned long startMillisDoors = 0;
bool startedMoveCounter = false;
bool startedDoorsCounter = false;
bool coomputedFloorsToTravel = false;
unsigned long lastDebounceTime = 0;
unsigned long prevMillisLED = 0;
State buzzerAction = OFF;  //we use enum to avoid magic numbers

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
      tone(buzzer, 800, (moveTime * floorsToTravel));
      Serial.println("Buzzer made a moving sound");
      break;
    case 2:  //doors closing
      tone(buzzer, 6000, 250);
      Serial.println("Buzzer made a door closing sound");
      break;
    default:  //off
      digitalWrite(buzzer, LOW);
      Serial.println("Buzzer has been turned off");
      break;
  }
}

void elevatorLED() {
  if (elevatorIsStatic == false) {
    if (millis() - prevMillisLED >= elevatorLEDBlinkInterval) {
      prevMillisLED = millis();
      if (elevatorLEDCurrentState == LOW) {
        elevatorLEDCurrentState = HIGH;
      } else {
        elevatorLEDCurrentState = LOW;
      }
      digitalWrite(elevatorStateLED, elevatorLEDCurrentState);
    }
  } else {
    digitalWrite(elevatorStateLED, HIGH);
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
  buzzerMode(buzzerAction = OFF);  //buzzer is OFF by default, we explicitly initialize it as such to avoid any noises during startup
  floorIndicators(currentFloor);
  Serial.begin(9600);
}

void loop() {
  bool firstFloorBtnReading = !digitalRead(firstFloorBtn);
  bool secondFloorBtnReading = !digitalRead(secondFloorBtn);
  bool thirdFloorBtnReading = !digitalRead(thirdFloorBtn);  //we use ! because we have a PULLUP configuration meaning we have to invert the output

  if (firstFloorBtnReading != firstFloorBtnLastReading && isAcceptingCmd == true) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceTime) {
    if (firstFloorBtnReading != firstFloorBtnState) {
      firstFloorBtnState = firstFloorBtnReading;
      if (firstFloorBtnReading == HIGH) {
        Serial.println("Call from 1st floor");
        isAcceptingCmd = false;
        targetFloor = 1;
      }
    }
  }
  firstFloorBtnLastReading = firstFloorBtnReading;
  //pt buton 1

  if (secondFloorBtnReading != secondFloorBtnLastReading && isAcceptingCmd == true) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceTime) {
    if (secondFloorBtnReading != secondFloorBtnState) {
      secondFloorBtnState = secondFloorBtnReading;
      if (secondFloorBtnReading == HIGH) {
        Serial.println("Call from 2nd floor");
        isAcceptingCmd = false;
        targetFloor = 2;
      }
    }
  }
  secondFloorBtnLastReading = secondFloorBtnReading;
  //pt buton 2


  if (thirdFloorBtnReading != thirdFloorBtnLastReading && isAcceptingCmd == true) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceTime) {
    if (thirdFloorBtnReading != thirdFloorBtnState) {
      thirdFloorBtnState = thirdFloorBtnReading;
      if (thirdFloorBtnReading == HIGH) {
        Serial.println("Call from 3rd floor");
        isAcceptingCmd = false;
        targetFloor = 3;
      }
    }
  }
  thirdFloorBtnLastReading = thirdFloorBtnReading;
  //pt buton 3
  elevatorLED();
  if (isAcceptingCmd == false) {
    if(currentFloor == targetFloor){
       Serial.println("Ignoring call as we are already at the floor that the button was pressed from");
        isAcceptingCmd = true; //we can only end up in this state if the floor call is from the same one we are stationed at so we need to reenable accepting commands.
    }
    if ((currentFloor != targetFloor) && elevatorIsStatic == true) {
      if (coomputedFloorsToTravel == false) {
        floorsToTravel = abs(targetFloor - currentFloor);
        Serial.print("Floors to travel: ");
        Serial.println(floorsToTravel);
        Serial.println("Floor call is not from the current floor, executing");
        coomputedFloorsToTravel = true;
      }
      if (startedDoorsCounter == false) {
        startMillisDoors = millis();
        startedDoorsCounter = true;
      }
      if ((millis() - startMillisDoors >= doorCloseTimer) && elevatorIsStatic == true) {
        Serial.println("Doors have closed.");
        buzzerMode(buzzerAction = DOORS_CLOSE);
        elevatorIsStatic = false;
        movingSoundPlayed = false;
      }
    }
    if (elevatorIsStatic == false) {
      if (movingSoundPlayed == false) {
        buzzerMode(buzzerAction = MOVING);
        movingSoundPlayed = true;
      }
      if (startedMoveCounter == false) {
        startMillisMove = millis();
        startedMoveCounter = true;
      }
      if ((millis() - startMillisMove >= (moveTime * floorsToTravel)) && elevatorIsStatic == false) {
        currentFloor = targetFloor;
        floorIndicators(currentFloor);
        buzzerMode(buzzerAction = ARRIVAL);
        Serial.println("Elevator arrived at the desired floor.");
        isAcceptingCmd = true;
        elevatorIsStatic = true;
        startedDoorsCounter = false;
        startedMoveCounter = false;
        coomputedFloorsToTravel = false;
      }
    }

  } 
}