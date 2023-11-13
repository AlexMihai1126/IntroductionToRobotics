//part of this code was written during the lab
#define latchPin 11
#define clockPin 10
#define dataPin 12
#define segD1 4
#define segD2 5
#define segD3 6
#define segD4 7
#define displayCount 4
#define encodingsNumber 10
#define maxLaps 4
#define startStopButton 2
#define lapButton 3
#define resetButton 8
#define tenthsSecondsDisplay 2

int displayDigits[] = { segD1, segD2, segD3, segD4 };
volatile short int currentLapNumber = 0;
volatile short int savedLaps = 0;

byte resetBtnLastReading = LOW;
byte resetBtnState = LOW;
byte byteEncodings[encodingsNumber] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110   // 9
};

volatile bool timerIsStarted = false;
volatile bool resetBtnEnabled = true;
volatile bool canSaveLaps = false; //we consider the timer is "paused" even at 0.0
volatile bool timerHasBeenReset = false;

unsigned long lastIncrement = 0;
unsigned long delayCount = 100;
volatile unsigned long stopwatch = 0;
unsigned long lastDebounceTime = 0;
volatile unsigned long laps[] = { 0, 0, 0, 0 };
volatile unsigned long lastInterruptTime = 0;
const unsigned long debounceTime = 100;


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(startStopButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(lapButton, INPUT_PULLUP);
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  Serial.begin(9600);
  writeReg(B00000000);
  attachInterrupt(digitalPinToInterrupt(startStopButton), handleStartStop, RISING);
  attachInterrupt(digitalPinToInterrupt(lapButton), handleLap, RISING);
}

void loop() {
  checkResetBtn();
  if (millis() - lastIncrement > delayCount && timerIsStarted == true) {
    stopwatch++;
    stopwatch %= 10000;
    lastIncrement = millis();
  }
  writeNumber(stopwatch);
}


void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}
void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}
void writeNumber(int number) {
  int currentNumber = number;
  int lastDigit = 0;
  int digits [displayCount] = {0,0,0,0};
  short int counter = displayCount-1;
  byte data = B00000000;
  while (currentNumber != 0) {
    lastDigit = currentNumber % 10;
    digits[counter] = lastDigit;
    counter--;
    currentNumber /= 10;
  }
  for(int i=0; i < displayCount ;i++){
    activateDisplay(i);
    data = byteEncodings[digits[i]];
    if(i == tenthsSecondsDisplay){
      data += 1; //we activate the dot point for the 3rd display to separate seconds from tenths of a second
    }
    writeReg(data);
    writeReg(B00000000);
  }
}

void checkResetBtn() {
  bool resetBtnReading = !digitalRead(resetButton);
  if (resetBtnReading != resetBtnLastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime > debounceTime) && resetBtnEnabled == true) {
    if (resetBtnReading != resetBtnState) {
      resetBtnState = resetBtnReading;
      if (resetBtnReading == HIGH) {
        stopwatch = 0; 
      }
    }
  }
  resetBtnLastReading = resetBtnReading;
}

void handleStartStop() {
  static unsigned long interruptTime = 0;
  interruptTime = micros();
  if (interruptTime - lastInterruptTime > debounceTime * 1000) {
    timerIsStarted = !timerIsStarted;
    resetBtnEnabled = !resetBtnEnabled;
    canSaveLaps = !canSaveLaps;
  }
  lastInterruptTime = interruptTime;
}

void handleLap() {
  static unsigned long interruptTime = 0;
  if(savedLaps > maxLaps){
      currentLapNumber = 0; //we overwrite the saved laps is we exceed the limit
      savedLaps = 1;
      Serial.println("Overwriting laps");
    }
  interruptTime = micros();
  if ((interruptTime - lastInterruptTime > debounceTime * 1000) && canSaveLaps == true) {
    laps[currentLapNumber] = stopwatch; //save the current time displayed
    savedLaps +=1;
    currentLapNumber+=1;
  }
  lastInterruptTime = interruptTime;
  for(int i=0; i<maxLaps; i++){
    Serial.println(laps[i]);
  }
}