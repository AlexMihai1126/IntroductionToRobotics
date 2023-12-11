#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#define matrixDIN 10
#define matrixCS 9
#define matrixCLK 8
#define displayD7 7
#define displayD6 6
#define displayD5 5
#define displayD4 4
#define displayBacklight 3  //PWM controllable
#define displayEN 12
#define displayRS 13
#define buzzerPin 2
#define redPushbtn A4
#define joystickX A1
#define joystickY A2
#define joystickSw A0
#define ambientLightSensor A3
#define matrixRows 16
#define matrixColumns 8
#define storedParametersCount 6
#define blinkInterval 250
#define maxCharsName 8
#define maxGameLevels 4
#define maxAccesibleMenuStates 5
#define maxAccesibleInternalMenus 2
#define maxAccesibleSettingsSubmenus 5

byte logicalMatrix[matrixRows][matrixColumns] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};


enum brightnessLevels {
  lowBrightness,
  midBrightness,
  highBrightness,
  ultraBrightness
};

enum mainMenuStates {
  startGame,
  highscores,
  settings,
  about,
  tutorial,
  intro,
  inSubmenu
};

mainMenuStates currentState = inSubmenu;
mainMenuStates previousState = inSubmenu;  // using this state as a startup state to save memory or handing over control to another menu state

enum internalMenuStates {
  inGame,
  endGame,
  inOtherMenu
};

internalMenuStates currInternalMenuState = inOtherMenu;

enum settingsSubmenus {
  nameInput,
  brightnessControl,
  soundControl,
  gameLevel,
  resetHighscores,
  inMainMenu
};

settingsSubmenus currentSettingsSubmenu = inMainMenu;

enum gameLevels {
  practice,
  easy,
  normal,
  hard
};

enum storedParameters {
  mtxBright,
  lcdBright,
  autoBright,
  soundOn,
  lastLevel,
  highscoreStartAddr
};

enum joystickState {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  STATIC
};

joystickState joyState = STATIC;
joystickState lastJoyState = STATIC;

struct highscore {
  char name[maxCharsName] = "";
  int scoreValue = 0;
};

short adresses[storedParametersCount] = { 0, 1, 2, 3, 4, 5 };

byte matrixBrightness = 2;
byte lcdBrightness = 127;
bool autoBrightness = false;
bool isSoundOn = true;  //defaults

bool introHasAppeared = false;
bool isInGame = false;
bool joystickMoved = false;
bool cmdExecuted = false;

byte joySwReading = LOW;
byte joySwState = LOW;
byte joySwStateLastReading = LOW;

byte redSwReading = LOW;
byte redSwState = LOW;
byte SwStateLastReading = LOW;

int minThreshold = 480;
int maxThreshold = 550;
int xValue = 0;
int yValue = 0;

const unsigned long debounceTime = 100;
const unsigned long introShowTime = 5000;

unsigned long startIntroTime = 0;
unsigned long lastDebounceTimeJoySw = 0;
unsigned long lastDebounceTimeRedSw = 0;

highscore hs1;
highscore hs2;
highscore hs3;

LiquidCrystal lcd(displayRS, displayEN, displayD4, displayD5, displayD6, displayD7);
LedControl matrix = LedControl(matrixDIN, matrixCLK, matrixCS, 2);

void loadParameters() {
  matrixBrightness = EEPROM.read(adresses[mtxBright]);
  lcdBrightness = EEPROM.read(adresses[lcdBright]);
  autoBrightness = EEPROM.read(adresses[autoBright]);
  isSoundOn = EEPROM.read(adresses[soundOn]);
}

void loadHighscores() {
  short readAdress = adresses[highscoreStartAddr] + sizeof(highscore);
  EEPROM.get(readAdress, hs1);
  readAdress += sizeof(highscore);
  EEPROM.get(readAdress, hs2);
  readAdress += sizeof(highscore);
  EEPROM.get(readAdress, hs3);
}

void displayMatrix() {
  for (int row = 0; row < matrixRows; row++) {
    for (int col = 0; col < matrixColumns; col++) {
      matrix.setLed(1, 15 - row, 7 - col, logicalMatrix[row][col]);
      matrix.setLed(0, 15 - row, 7 - col, logicalMatrix[row][col]);  // Update each LED state
    }
  }
}

void autoBrightnessController() {
  //todo
}

void getJoystickState() {
  xValue = analogRead(joystickX);
  yValue = analogRead(joystickY);
  if(joystickMoved == false){
    if (xValue > maxThreshold) {
    joyState = UP;
    joystickMoved = true;
  }

  if (xValue < minThreshold) {
    joyState = DOWN;
    joystickMoved = true;
  }

  if (yValue > maxThreshold) {
    joyState = RIGHT;
    joystickMoved = true;
  }

  if (yValue < minThreshold) {
    joyState = LEFT;
    joystickMoved = true;
  }
  }
  

  if (xValue >= minThreshold && xValue < maxThreshold && yValue >= minThreshold && yValue < maxThreshold) {
    joyState = STATIC;
    joystickMoved = false;
    cmdExecuted = false;
  }
}

void getJoySwitchState() {
  joySwReading = !digitalRead(joystickSw);
  if (joySwReading != joySwStateLastReading) {
    lastDebounceTimeJoySw = millis();
  }

  if ((millis() - lastDebounceTimeJoySw) >= debounceTime) {
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
}


void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPushbtn, INPUT_PULLUP);
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  pinMode(ambientLightSensor, INPUT);
  pinMode(displayBacklight, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
  loadParameters();
  loadHighscores();
  analogWrite(displayBacklight, lcdBrightness);
  matrix.shutdown(0, false);
  matrix.setIntensity(0, matrixBrightness);
  matrix.clearDisplay(0);
  matrix.shutdown(1, false);
  matrix.setIntensity(1, matrixBrightness);
  matrix.clearDisplay(1);
  randomSeed(analogRead(A5));
  currentState = intro;  //switch to intro state after the setup runs
}

void loop() {
  getJoystickState();
  if (!isInGame) {
    handleMenuNavigation();
    handleMenu();
  }
}

void handleMenu() {
  if(currentState != previousState){
    lcd.clear();
    switch (currentState) {
    case intro:
      handleIntro();
      break;
    case startGame:
      handleStartGame();
      break;
    case highscores:
      handleHighscores();
      break;
    case settings:
      handleSettings();
      break;
    case about:
      handleAbout();
      break;
    case tutorial:
      handleTutorial();
      break;
    default:
      break;
  }
  }
  previousState = currentState;
}

void handleIntro() {
  lcd.print("Welcome!");
}

void handleStartGame() {
  lcd.print("START GAME:");
  delay(100);
  lcd.setCursor(0, 0);
  lcd.print("Press the RED");
  lcd.setCursor(1, 0);
  lcd.print("button to start");
}

void handleHighscores() {
  lcd.print("View highscores");
  lcd.setCursor(1, 0);
  lcd.print("Press joystick");
  //on joystick press will enter a function that will display all highscores, one at a time, for 3 sec each, then go back to main menu;
}

void handleSettings() {
  lcd.print("SETTINGS");
  lcd.setCursor(1, 0);
  lcd.print("Press joystick");
  //on click will move to inSubmenu state then handleSettingsSubmenus() will be called
}

void handleAbout() {
  lcd.print("About the game");
  lcd.print("Created by Alexandru Mihai");  //will scroll
  lcd.print("GitHub: AlexMihai1126");
}

void handleTutorial() {
  lcd.print("Tutorial:");
}

void handleMenuNavigation() {
  switch (joyState) {
    case UP:
      navigateMenuUp();
      break;
    case DOWN:
      navigateMenuDown();
      break;
    default:
      break;
  }
}

void navigateMenuUp() {
  if(cmdExecuted == false){
      cmdExecuted = true;
      if (currentState > 0) {
    currentState = static_cast<mainMenuStates>(currentState - 1);
  } else {
    currentState = static_cast<mainMenuStates>(maxAccesibleMenuStates - 1);
  }

  }

}

void navigateMenuDown() {
  if(cmdExecuted == false){
    cmdExecuted = true;
  currentState = static_cast<mainMenuStates>((currentState + 1) % maxAccesibleMenuStates);
  }
}


