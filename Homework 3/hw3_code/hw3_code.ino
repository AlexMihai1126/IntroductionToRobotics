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

enum joystickState {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  STATIC
};

joystickState joyState = STATIC;

bool isAcceptingChanges = true;

byte joySwReading = LOW;
byte joySwState = LOW;
byte joySwStateLastReading = LOW;
byte dotPointState = LOW;

int minThreshold = 480;
int maxThreshold = 550;
int xValue = 0;
int yValue = 0;
int lastDebounceTime = 0;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

void getJoystickState() {
  if (xValue > maxThreshold && isAcceptingChanges == true) {
    isAcceptingChanges = false;
    if (debugEnabled == true) {
      Serial.println("UP");
    }
    joyState = UP;
  }

  if (xValue < minThreshold && isAcceptingChanges == true) {
    isAcceptingChanges = false;
    if (debugEnabled == true) {
      Serial.println("DOWN");
    }
    joyState = DOWN;
  }

  if (yValue > maxThreshold && isAcceptingChanges == true) {
    isAcceptingChanges = false;
    if (debugEnabled == true) {
      Serial.println("RIGHT");
    }
    joyState = RIGHT;
  }

  if (yValue < minThreshold && isAcceptingChanges == true) {
    isAcceptingChanges = false;
    if (debugEnabled == true) {
      Serial.println("LEFT");
    }
    joyState = LEFT;
  }

  if (xValue >= minThreshold && xValue < maxThreshold && yValue >= minThreshold && yValue < maxThreshold) {
    isAcceptingChanges = true;
    if (debugEnabled == true) {
      Serial.println("STATIC");
    }
    joyState = STATIC;
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
      }
      else {
      joySwState = LOW;
      }
    }
  }
  joySwStateLastReading = joySwReading;
  
  if(debugEnabled == true){
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
  getSwitchState();
}
