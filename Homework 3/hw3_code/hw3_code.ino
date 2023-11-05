#define pinA 12
#define pinB 10
#define pinC 9
#define pinD 8
#define pinE 7
#define pinF 6
#define pinG 5
#define pinDP 4
#define pinClickedLED 11
#define segSize 8
#define pinSW 2
#define pinX A0
#define pinY A1
#define debugEnabled false
#define noOfDigits 10
#define debounceTime 200
#define resetTime 1500
#define blinkInterval 350

enum joystickState {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  STATIC
};

enum segmentsEnum {
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  dotPoint
};

joystickState joyState = STATIC;
segmentsEnum currSegment = dotPoint;

bool isSegmentClicked[segSize] = {
  false, false, false, false, false, false, false, false
};

bool cmdExecuted = false;
bool gotJostickState = false;
bool btnHasTriggered = false;
bool boardHasBeenReset = false;

byte joySwReading = LOW;
byte joySwState = LOW;
byte joySwStateLastReading = LOW;
byte dotPointState = LOW;
byte segmentBlinkState = LOW;

unsigned long prevMillisBlink = 0;
unsigned long rstInitialTimeMoment = 0;
int minThreshold = 480;
int maxThreshold = 550;
int xValue = 0;
int yValue = 0;
unsigned long lastDebounceTime = 0;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

void resetBoard() {
  for (int i = 0; i < segSize; i++) {
    digitalWrite(segments[i], LOW);
    isSegmentClicked[i] = false;
  }
  currSegment = dotPoint;
  boardHasBeenReset = true;
  if (debugEnabled == true) {
    Serial.println("Reset the board.");
  }
}

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}
void loop() {
  cleanupUnusedSegments();
  checkWhichSegmentsAreClicked();
  checkIfCurrSegmentIsClicked();
  joySwReading = !digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  moveSegments();
  clickCurrSegIfSwClicked();
  blinkCurrentSegment();
}

void getSwitchState() {
  if (joySwReading != joySwStateLastReading) {
    lastDebounceTime = millis();
    rstInitialTimeMoment = lastDebounceTime;
  }

  if ((millis() - lastDebounceTime) >= debounceTime) {
    if (joySwReading != joySwState) {
      joySwState = joySwReading;
      if (joySwReading == HIGH) {
        joySwState = HIGH;
      } else {
        joySwState = LOW;
        btnHasTriggered = false;
      }
    }
  }
  joySwStateLastReading = joySwReading;
  // if((millis()-rstInitialTimeMoment) >= resetTime && boardHasBeenReset == false){
  //   resetBoard();
  // }
}

void getJoystickState() {
  if (gotJostickState == false) {
    if (xValue > maxThreshold) {
      if (debugEnabled == true) {
        Serial.println("UP");
      }
      gotJostickState = true;
      joyState = UP;
    }

    if (xValue < minThreshold) {
      if (debugEnabled == true) {
        Serial.println("DOWN");
      }
      gotJostickState = true;
      joyState = DOWN;
    }

    if (yValue > maxThreshold) {
      if (debugEnabled == true) {
        Serial.println("RIGHT");
      }
      gotJostickState = true;
      joyState = RIGHT;
    }

    if (yValue < minThreshold) {
      if (debugEnabled == true) {
        Serial.println("LEFT");
      }
      gotJostickState = true;
      joyState = LEFT;
    }
  }

  if (xValue >= minThreshold && xValue < maxThreshold && yValue >= minThreshold && yValue < maxThreshold) {
    if (debugEnabled == true) {
      //Serial.println("STATIC");
    }
    gotJostickState = false;
    cmdExecuted = false;
    joyState = STATIC;
  }
}


void blinkCurrentSegment() {
  if (millis() - prevMillisBlink >= blinkInterval) {
    prevMillisBlink = millis();
    if (segmentBlinkState == LOW) {
      segmentBlinkState = HIGH;
    } else {
      segmentBlinkState = LOW;
    }
    digitalWrite(segments[currSegment], segmentBlinkState);
    if (debugEnabled) {
      Serial.print("Blinking segment: ");
      Serial.println(currSegment);
    }
  }
}

void clickCurrSegIfSwClicked() {
  getSwitchState();
  if (joySwState == HIGH && btnHasTriggered == false) {
    isSegmentClicked[currSegment] = !isSegmentClicked[currSegment];
    btnHasTriggered = true;
  }
}

void checkWhichSegmentsAreClicked() {
  for (int i = 0; i < segSize; i++) {
    if (isSegmentClicked[i] == true && i != currSegment) {
      digitalWrite(segments[i], HIGH);
      if (debugEnabled == true) {
        Serial.print("Segment clicked: ");
        Serial.println(i);
      }
    }
  }
}


void checkIfCurrSegmentIsClicked() {
  if (isSegmentClicked[currSegment] == true) {
    digitalWrite(pinClickedLED, HIGH);
    return;
  } else {
    digitalWrite(pinClickedLED, LOW);
  }
}

void shutdownPrevSegment(segmentsEnum segment) {
  if (isSegmentClicked[segment] == true) {
    return;
  } else {
    digitalWrite(segments[segment], LOW);
    if (debugEnabled == true) {
      Serial.print("Reset segment: ");
      Serial.println(segment);
    }
  }
}

void cleanupUnusedSegments() {
  for (int i = 0; i < segSize; i++) {
    if (isSegmentClicked[i] == false && i != currSegment) {
      digitalWrite(segments[i], LOW);
    }
  }
}

void moveSegments() {
  getJoystickState();
  if (currSegment == A && cmdExecuted == false) {
    switch (joyState) {
      case DOWN:
        {
          cmdExecuted = true;
          shutdownPrevSegment(A);
          currSegment = G;
          break;
        }
      case LEFT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(A);
          currSegment = F;
          break;
        }
      case RIGHT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(A);
          currSegment = B;
          break;
        }
      default:
        {
          break;
        }
    }
  }
  if (currSegment == B && cmdExecuted == false) {
    switch (joyState) {
      case UP:
        {
          cmdExecuted = true;
          shutdownPrevSegment(B);
          currSegment = A;
          break;
        }
      case DOWN:
        {
          cmdExecuted = true;
          shutdownPrevSegment(B);
          currSegment = G;
          break;
        }
      case LEFT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(B);
          currSegment = F;
          break;
        }
      default:
        {
          break;
        }
    }
  }
  if (currSegment == C && cmdExecuted == false) {
    switch (joyState) {
      case UP:
        {
          cmdExecuted = true;
          shutdownPrevSegment(C);
          currSegment = G;
          break;
        }
      case DOWN:
        {
          cmdExecuted = true;
          shutdownPrevSegment(C);
          currSegment = D;
          break;
        }
      case LEFT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(C);
          currSegment = E;
          break;
        }
      case RIGHT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(C);
          currSegment = dotPoint;
          break;
        }
      default:
        {
          break;
        }
    }
  }
  if (currSegment == D && cmdExecuted == false) {
    switch (joyState) {
      case UP:
        {
          cmdExecuted = true;
          shutdownPrevSegment(D);
          currSegment = G;
          break;
        }
      case LEFT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(D);
          currSegment = E;
          break;
        }
      case RIGHT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(D);
          currSegment = C;
          break;
        }
      default:
        {
          break;
        }
    }
  }
  if (currSegment == E && cmdExecuted == false) {
    switch (joyState) {
      case UP:
        {
          cmdExecuted = true;
          shutdownPrevSegment(E);
          currSegment = G;
          break;
        }
      case DOWN:
        {
          cmdExecuted = true;
          shutdownPrevSegment(E);
          currSegment = D;
          break;
        }
      case RIGHT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(E);
          currSegment = C;
          break;
        }
      default:
        {
          break;
        }
    }
  }
  if (currSegment == F && cmdExecuted == false) {
    switch (joyState) {
      case UP:
        {
          cmdExecuted = true;
          shutdownPrevSegment(F);
          currSegment = A;
          break;
        }
      case DOWN:
        {
          cmdExecuted = true;
          shutdownPrevSegment(F);
          currSegment = G;
          break;
        }
      case RIGHT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(F);
          currSegment = B;
          break;
        }
      default:
        {
          break;
        }
    }
  }
  if (currSegment == G && cmdExecuted == false) {
    switch (joyState) {
      case UP:
        {
          cmdExecuted = true;
          shutdownPrevSegment(G);
          currSegment = A;
          break;
        }
      case DOWN:
        {
          cmdExecuted = true;
          shutdownPrevSegment(G);
          currSegment = D;
          break;
        }
      default:
        {
          break;
        }
    }
  }
  if (currSegment == dotPoint && cmdExecuted == false) {
    switch (joyState) {
      case LEFT:
        {
          cmdExecuted = true;
          shutdownPrevSegment(dotPoint);
          currSegment = C;
          break;
        }
      default:
        {
          break;
        }
    }
  }
}
