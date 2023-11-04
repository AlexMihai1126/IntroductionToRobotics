#define pinA 12
#define pinB 10
#define pinC 9
#define pinD 8
#define pinE 7
#define pinF 6
#define pinG 5
#define pinDP 4
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

bool movementHasTriggered = false;

byte joySwReading = LOW;
byte joySwState = LOW;
byte joySwStateLastReading = LOW;
byte dotPointState = LOW;
byte segmentBlinkState = LOW;

unsigned long prevMillisBlink = 0;
int minThreshold = 480;
int maxThreshold = 550;
int xValue = 0;
int yValue = 0;
unsigned long lastDebounceTime = 0;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

void getJoystickState() {
  if (xValue > maxThreshold && movementHasTriggered == false) {
    if (debugEnabled == true) {
      Serial.println("UP");
    }
    movementHasTriggered = true;
    joyState = UP;
  }

  if (xValue < minThreshold && movementHasTriggered == false) {
    if (debugEnabled == true) {
      Serial.println("DOWN");
    }
    movementHasTriggered = true;
    joyState = DOWN;
  }

  if (yValue > maxThreshold && movementHasTriggered == false) {
    if (debugEnabled == true) {
      Serial.println("RIGHT");
    }
    movementHasTriggered = true;
    joyState = RIGHT;
  }

  if (yValue < minThreshold && movementHasTriggered == false) {
    if (debugEnabled == true) {
      Serial.println("LEFT");
    }
    movementHasTriggered = true;
    joyState = LEFT;
  }

  if (xValue >= minThreshold && xValue < maxThreshold && yValue >= minThreshold && yValue < maxThreshold) {
    if (debugEnabled == true) {
      //Serial.println("STATIC");
    }
    movementHasTriggered = false;
    joyState = STATIC;
  }
}

void blinkCurrentSegment() {
  // if (isSegmentClicked[currSegment] == true) {
  //   return;
  // }
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

void checkSegmentClicked() {
  for (int i = 0; i < segSize; i++) {
    if (isSegmentClicked[i] == true) {
      digitalWrite(segments[i], HIGH);
      if (debugEnabled == true) {
        Serial.print("Segment clicked: ");
        Serial.println(i);
      }
    }
  }
}

void shutdownPrevSegment(segmentsEnum segment) {
  digitalWrite(segments[segment], LOW);
  if (debugEnabled == true) {
    Serial.print("Reset segment: ");
    Serial.println(segment);
  }
}

void resetBoard() {
  for (int i = 0; i < segSize; i++) {
    digitalWrite(segments[i], LOW);
  }
  currSegment = dotPoint;
  if (debugEnabled == true) {
    Serial.println("Reset the board.");
  }
}

void getSwitchState() {
  if (joySwReading != joySwStateLastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceTime) {
    if (joySwReading != joySwState) {
      joySwState = joySwReading;
      if (joySwReading == HIGH) {
        joySwState = HIGH;
      } else {
        joySwState = LOW;
      }
    }
  }
  joySwStateLastReading = joySwReading;

  if (debugEnabled == true) {
    Serial.print("Switch state is: ");
    Serial.println(joySwState);
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
  checkSegmentClicked();
  joySwReading = !digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  getJoystickState();
  //getSwitchState();
  if (joyState == UP) {
    shutdownPrevSegment(currSegment);
    currSegment = A;
  }
  if (joyState == DOWN) {
    shutdownPrevSegment(currSegment);
    currSegment = dotPoint;
  }
  if (joyState == LEFT) {
    shutdownPrevSegment(currSegment);
    currSegment = E;
  }
  if (joyState == RIGHT) {
    shutdownPrevSegment(currSegment);
    currSegment = C;
  }
  blinkCurrentSegment();
}
