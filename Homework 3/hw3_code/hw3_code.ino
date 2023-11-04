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
#define debugEnabled true
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
segmentsEnum prevSegment = dotPoint;

bool isAClicked = false;
bool isBClicked = false;
bool isCClicked = false;
bool isDClicked = false;
bool isEClicked = false;
bool isFClicked = false;
bool isGClicked = false;
bool isDPClicked = false;

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
  if (xValue > maxThreshold) {
    if (debugEnabled == true) {
      Serial.println("UP");
    }
    joyState = UP;
  }

  if (xValue < minThreshold) {
    if (debugEnabled == true) {
      Serial.println("DOWN");
    }
    joyState = DOWN;
  }

  if (yValue > maxThreshold) {
    if (debugEnabled == true) {
      Serial.println("RIGHT");
    }
    joyState = RIGHT;
  }

  if (yValue < minThreshold) {
    if (debugEnabled == true) {
      Serial.println("LEFT");
    }
    joyState = LEFT;
  }

  if (xValue >= minThreshold && xValue < maxThreshold && yValue >= minThreshold && yValue < maxThreshold) {
    if (debugEnabled == true) {
      Serial.println("STATIC");
    }
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
  }
}

void shutdownPrevSegment (segmentsEnum segment){
  digitalWrite(segments[segment], LOW);
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
  joySwReading = !digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  getJoystickState();
  //getSwitchState();
  if(joyState == UP){
    prevSegment = currSegment;
    currSegment = A;
    shutdownPrevSegment(prevSegment);
  }
  if(joyState == DOWN){
    prevSegment = currSegment;
    currSegment = dotPoint;
    shutdownPrevSegment(prevSegment);
  }
  if(joyState == LEFT){
    prevSegment = currSegment;
    currSegment = E;
    shutdownPrevSegment(prevSegment);
  }
  if(joyState == RIGHT){
    prevSegment = currSegment;
    currSegment = C;
    shutdownPrevSegment(prevSegment);
  }
    blinkCurrentSegment();
}
