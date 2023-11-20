#include <EEPROM.h>
#include "DHT.h"  //include the temperature and humidity sensor library
#define clearEEPROM false
#define storeDefaultsEEPROM false
#define ledRPin 11
#define ledGPin 10
#define ledBPin 9
#define trigPin 8
#define echoPin 7
#define tempHumidityPin 6
#define lightSensorPin A0
#define DHTTYPE DHT11
#define maxDistanceValue 255
#define maxLEDValue 255
#define maxTempValue 50
#define minTempValue -25
#define maxHumidityValue 100
#define maxLightValue 100
#define minimumValue 0
#define ledColors 3
#define storedParametersCount 13  //how many parameters we store in the eeprom memory
#define ledRIndex 0
#define ledGIndex 1
#define ledBIndex 2
#define loggedValues 10
#define tempDefault 25
#define humidityDefault 50
#define multiplyToMsec 1000

short updateRate = 10;  //default update value is 10 seconds
short distanceMinVal = 5;
short distanceMaxVal = 100;  //units are in cm
short lightMinVal = 0;
short lightMaxVal = 100;  //converted from the analog reading to 0-100 values for simplicity
short tempMinVal = 0;
short tempMaxVal = 40;  //units are in degrees celsius
short humidityMinVal = 0;
short humidityMaxVal = 100;  //units are in %
short ledValues[ledColors] = { 0, 0, 0 };
short adresses[storedParametersCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
short distanceLoggedValues[loggedValues] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
short lightLoggedValues[loggedValues] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
short tempLoggedValues[loggedValues] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
short humidityLoggedValues[loggedValues] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
short readValueMin = 0;
short readValueMax = 0;
short value1 = 0;
short value2 = 0;
short value3 = 0;
short logId = 0;

unsigned long prevMillisRefresh = 0;

enum storedParameters {
  UPDATE_RATE,
  LED_MODE,
  DIST_MIN,
  DIST_MAX,
  LIGHT_MIN,
  LIGHT_MAX,
  TEMP_MIN,
  TEMP_MAX,
  HUM_MIN,
  HUM_MAX,
  LED_R_VAL,
  LED_G_VAL,
  LED_B_VAL
};

enum menuState {
  MAIN_MENU,
  SENSOR_SETTINGS,
  RESET_LOGGER,
  SYSTEM_STATUS,
  RGB_LED_CONTROL,
  MANUAL_COLOR_CONTROL,
};

menuState currentState = MAIN_MENU;  //static makes the variable's value persistent

DHT dht(tempHumidityPin, DHTTYPE);

bool isTempOutOfRange = false;
bool isHumidityOutOfRange = false;
bool isDistanceOutOfRange = false;
bool isLightOutOfRange = false;
bool ledAutoMode = true;
bool inputSucceded = true;  //we assume input succeeds, we make it false on the way if something goes wrong
bool showValuesToConsole = false;

bool mainMenuPrinted = false;
bool sensorSettingsPrinted = false;
bool resetLoggerPrinted = false;
bool systemStatusPrinted = false;
bool rgbLedControlPrinted = false;

void setup() {
  if (clearEEPROM == true) {
    for (int i = 0; i < EEPROM.length(); i++) {
      EEPROM.update(i, 0);
    }
  }

  if(storeDefaultsEEPROM == true){
    EEPROM.update(adresses[UPDATE_RATE],updateRate);
    EEPROM.update(adresses[DIST_MIN], distanceMinVal);
    EEPROM.update(adresses[DIST_MAX], distanceMaxVal);
    EEPROM.update(adresses[TEMP_MIN], tempMinVal);
    EEPROM.update(adresses[TEMP_MAX], tempMaxVal);
    EEPROM.update(adresses[HUM_MIN], humidityMinVal);
    EEPROM.update(adresses[HUM_MAX], humidityMaxVal);
    EEPROM.update(adresses[LIGHT_MIN], lightMinVal);
    EEPROM.update(adresses[LIGHT_MAX], lightMaxVal);
    EEPROM.update(adresses[LED_MODE], ledAutoMode);
    EEPROM.update(adresses[LED_R_VAL], minimumValue);
    EEPROM.update(adresses[LED_G_VAL], minimumValue);
    EEPROM.update(adresses[LED_B_VAL], minimumValue);
  }
  Serial.begin(9600);
  dht.begin();  //initialize the temp and humidity sensor
  pinMode(ledRPin, OUTPUT);
  pinMode(ledGPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lightSensorPin, INPUT);
  updateRate = EEPROM.read(adresses[UPDATE_RATE]);
  ledAutoMode = EEPROM.read(adresses[LED_MODE]);
  distanceMinVal = EEPROM.read(adresses[DIST_MIN]);
  distanceMaxVal = EEPROM.read(adresses[DIST_MAX]);
  lightMinVal = EEPROM.read(adresses[LIGHT_MIN]);
  lightMaxVal = EEPROM.read(adresses[LIGHT_MAX]);
  tempMinVal = EEPROM.read(adresses[TEMP_MIN]);
  tempMaxVal = EEPROM.read(adresses[TEMP_MAX]);
  humidityMinVal = EEPROM.read(adresses[HUM_MIN]);
  humidityMaxVal = EEPROM.read(adresses[HUM_MAX]);
  ledValues[ledRIndex] = EEPROM.read(adresses[LED_R_VAL]);
  ledValues[ledGIndex] = EEPROM.read(adresses[LED_G_VAL]);
  ledValues[ledBIndex] = EEPROM.read(adresses[LED_B_VAL]);
}

void loop() {
  //Part of the loop was written with ChatGPT
  Serial.flush();  // Clear the serial buffer

  switch (currentState) {
    case MAIN_MENU:
      if (!mainMenuPrinted) {
        printMainMenu();
        mainMenuPrinted = true;
      }
      handleMainMenuInput();
      break;

    case SENSOR_SETTINGS:
      if (!sensorSettingsPrinted) {
        printSensorSettingsMenu();
        sensorSettingsPrinted = true;
      }
      handleSensorSettingsInput();
      break;

    case RESET_LOGGER:
      if (!resetLoggerPrinted) {
        printResetLoggerMenu();
        resetLoggerPrinted = true;
      }
      handleResetLoggerInput();
      break;

    case SYSTEM_STATUS:
      if (!systemStatusPrinted) {
        printSystemStatusMenu();
        systemStatusPrinted = true;
      }
      handleSystemStatusInput();
      break;

    case RGB_LED_CONTROL:
      if (!rgbLedControlPrinted) {
        printRGBLEDControlMenu();
        rgbLedControlPrinted = true;
      }
      handleRGBLEDControlInput();
      break;
  }
  readSensors();
  ledControl();
}


int readDistance() {
  int computedDistance = 0;
  unsigned long duration = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  computedDistance = duration * 0.034 / 2;
  if (computedDistance > maxDistanceValue) {
    computedDistance = maxDistanceValue;  //the datasheet for this sensor says the max distance this sensor can read is 4m so we cap the value at 400 cm to prevent weird readings
  }
  return computedDistance;
}

int readLightValue() {
  int photocellValue = analogRead(lightSensorPin);
  return map(photocellValue, 0, 1023, 0, 100);
}

void readTempAndHumidity(int &temp, int &humidity) {
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humidity) || isnan(temp)) {
    if (Serial.available() > 0) { Serial.println("Temp and humidity sensor read error, writing default values"); }
    temp = tempDefault;
    humidity = humidityDefault;
  }
}

void readSensors() {
  if (logId > loggedValues) {
    logId = 0;  //reset and overwrite previous stored values
  }
  int hum = 0;
  int tmp = 0;

  if ((millis() - prevMillisRefresh) >= (updateRate * multiplyToMsec)) {
    prevMillisRefresh = millis();
    readTempAndHumidity(tmp, hum);
    distanceLoggedValues[logId] = readDistance();
    lightLoggedValues[logId] = readLightValue();
    tempLoggedValues[logId] = tmp;
    humidityLoggedValues[logId] = hum;
    if (distanceLoggedValues[logId] < distanceMinVal && distanceLoggedValues[logId] > distanceMaxVal) {
      isDistanceOutOfRange = true;
    } else {
      isDistanceOutOfRange = false;
    }
    if (tempLoggedValues[logId] < tempMinVal && tempLoggedValues[logId] > tempMaxVal) {
      isTempOutOfRange = true;
    } else {
      isTempOutOfRange = false;
    }
    if (humidityLoggedValues[logId] < humidityMinVal && humidityLoggedValues[logId] > humidityMaxVal) {
      isHumidityOutOfRange = true;
    } else {
      isHumidityOutOfRange = false;
    }
    if (lightLoggedValues[logId] < lightMinVal && lightLoggedValues[logId] > lightMaxVal) {
      isLightOutOfRange = true;
    } else {
      isLightOutOfRange = false;
    }

    if (showValuesToConsole == true) {
      Serial.println(F("Press 1 to exit data display mode."));
      Serial.print("Distance: ");
      Serial.println(distanceLoggedValues[logId]);
      Serial.print("Light: ");
      Serial.println(lightLoggedValues[logId]);
      Serial.print("Temp: ");
      Serial.println(tempLoggedValues[logId]);
      Serial.print("Humidity: ");
      Serial.println(humidityLoggedValues[logId]);
    }
    logId += 1;
  }
}

bool isLEDValidValue(int val) {
  return (val >= minimumValue && val <= maxLEDValue);
}  //check the value is between the constants defined at the start of the program

bool isDistanceValidValue(int val) {
  return (val >= minimumValue && val <= maxDistanceValue);
}  //check the value is between the constants defined at the start of the program

bool isTmpValidValue(int val) {
  return (val >= minTempValue && val <= maxTempValue);
}  //check the value is between the constants defined at the start of the program

bool isHumidityValidValue(int val) {
  return (val >= minimumValue && val <= maxHumidityValue);
}  //check the value is between the constants defined at the start of the program

bool isLightValidValue(int val) {
  return (val >= minimumValue && val <= maxLightValue);
}  //check the value is between the constants defined at the start of the program

void eraseLoggerData() {
  for (int i = 0; i < loggedValues; i++) {
    distanceLoggedValues[i] = 0;
    lightLoggedValues[i] = 0;
    tempLoggedValues[i] = 0;
    humidityLoggedValues[i] = 0;
  }
  Serial.println("Log data cleared.");
}

void ledControl() {
  if (ledAutoMode == true) {
    if (isDistanceOutOfRange || isHumidityOutOfRange || isLightOutOfRange || isTempOutOfRange) {
      digitalWrite(ledRPin, HIGH);
      digitalWrite(ledGPin, LOW);
      digitalWrite(ledBPin, LOW);
    } else {
      digitalWrite(ledGPin, HIGH);
      digitalWrite(ledRPin, LOW);
      digitalWrite(ledBPin, LOW);
    }

  } else {
    analogWrite(ledRPin, ledValues[ledRIndex]);
    analogWrite(ledGPin, ledValues[ledGIndex]);
    analogWrite(ledBPin, ledValues[ledBIndex]);
  }
}

//--------------------------------------------------- Functions below were generated with ChatGPT but modified accordingly to fit the use case here.
void handleMainMenuInput() {

  if (Serial.available() > 0) {
    Serial.flush();
    short userInput = Serial.parseInt();
    // Clear the remaining characters in the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
    switch (userInput) {
      case 1:
        currentState = SENSOR_SETTINGS;
        break;

      case 2:
        currentState = RESET_LOGGER;
        break;

      case 3:
        currentState = SYSTEM_STATUS;
        break;

      case 4:
        currentState = RGB_LED_CONTROL;
        break;
      default:
        Serial.println(F("Incorrect choice."));
        currentState = MAIN_MENU;
        mainMenuPrinted = false;
        break;
    }
  }
}

void handleSensorSettingsInput() {
  if (Serial.available() > 0) {
    Serial.flush();
    short userInput = Serial.parseInt();
    // Clear the remaining characters in the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }
    switch (userInput) {
      case 1:
        // Prompt user for the sampling interval and update the variable
        Serial.println("Enter sampling interval (1-10 seconds): ");
        while (!Serial.available()) {
          // Wait for user input
        }
        updateRate = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }
        Serial.println("Sampling interval set to: " + String(updateRate) + " seconds");
        EEPROM.update(adresses[UPDATE_RATE], updateRate);
        currentState = SENSOR_SETTINGS;
        sensorSettingsPrinted = false;
        break;

      case 2:
        // Handle distance sensor threshold settings
        Serial.println("Enter distance minimum and maximum values in the format min,max: ");
        while (!Serial.available()) {
          // Wait for user input
        }

        inputSucceded = true;
        Serial.flush();
        readValueMin = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (!isDistanceValidValue(readValueMin)) {
          Serial.println("Error: Minimum is out of range (0-255)");
          inputSucceded = false;
          return;
        }

        if (Serial.read() != ',') {
          Serial.println("Error: Incorrect input format");
          inputSucceded = false;
          return;
        }
        Serial.flush();
        readValueMax = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (!isDistanceValidValue(readValueMax)) {
          Serial.println("Error: Maximum is out of range (0-255)");
          inputSucceded = false;
          return;
        }
        if (inputSucceded == true) {  //we update the values only if the input succeds (same for the other functions)
          distanceMinVal = readValueMin;
          distanceMaxVal = readValueMax;
          Serial.println("Minimum value set to: " + String(distanceMinVal) + " cm " + "and maximum value set to: " + String(distanceMinVal) + " cm ");
          EEPROM.update(adresses[DIST_MIN], distanceMinVal);
          EEPROM.update(adresses[DIST_MAX], distanceMaxVal);
        }
        currentState = SENSOR_SETTINGS;
        sensorSettingsPrinted = false;
        break;
      case 3:
        // Handle light sensor threshold settingss
        Serial.println("Enter light minimum and maximum values in the format min,max: ");
        while (!Serial.available()) {
          // Wait for user input
        }

        inputSucceded = true;
        Serial.flush();
        readValueMin = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (!isLightValidValue(readValueMin)) {
          Serial.println("Error: Minimum is out of range (0-100)");
          inputSucceded = false;
          return;
        }

        if (Serial.read() != ',') {
          Serial.println("Error: Incorrect input format");
          inputSucceded = false;
          return;
        }
        Serial.flush();
        readValueMax = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (!isLightValidValue(readValueMax)) {
          Serial.println("Error: Maximum is out of range (0-100)");
          inputSucceded = false;
          return;
        }
        if (inputSucceded == true) {  //we update the values only if the input succeds (same for the other functions)
          lightMinVal = readValueMin;
          lightMaxVal = readValueMax;
          Serial.println("Minimum value set to: " + String(lightMinVal) + " % " + "and maximum value set to: " + String(lightMaxVal) + " % ");
          EEPROM.update(adresses[LIGHT_MIN], lightMinVal);
          EEPROM.update(adresses[LIGHT_MAX], lightMaxVal);
        }
        currentState = SENSOR_SETTINGS;
        sensorSettingsPrinted = false;
        break;
      case 4:
        // Handle temperature sensor threshold settings
        Serial.println("Enter temperature minimum and maximum values in the format min,max: ");
        while (!Serial.available()) {
          // Wait for user input
        }

        inputSucceded = true;
        Serial.flush();
        readValueMin = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (!isTmpValidValue(readValueMin)) {
          Serial.println("Error: Minimum is out of range (0-50)");
          inputSucceded = false;
          return;
        }

        if (Serial.read() != ',') {
          Serial.println(F("Error: Incorrect input format"));
          inputSucceded = false;
          return;
        }
        Serial.flush();
        readValueMax = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (!isTmpValidValue(readValueMax)) {
          Serial.println(F("Error: Maximum is out of range (0-50)"));
          inputSucceded = false;
          return;
        }
        if (inputSucceded == true) {  //we update the values only if the input succeds (same for the other functions)
          tempMinVal = readValueMin;
          tempMaxVal = readValueMax;
          Serial.println("Minimum value set to: " + String(tempMinVal) + " degrees C " + "and maximum value set to: " + String(tempMaxVal) + " degrees C ");
          EEPROM.update(adresses[TEMP_MIN], tempMinVal);
          EEPROM.update(adresses[TEMP_MAX], tempMaxVal);
        }
        currentState = SENSOR_SETTINGS;
        sensorSettingsPrinted = false;
        break;
      case 5:
        // Handle humidity sensor threshold settings
        Serial.println(F("Enter humidity minimum and maximum values in the format min,max: "));
        while (!Serial.available()) {
          // Wait for user input
        }

        inputSucceded = true;
        Serial.flush();
        readValueMin = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (!isHumidityValidValue(readValueMin)) {
          Serial.println(F("Error: Minimum is out of range (0-100)"));
          inputSucceded = false;
          return;
        }

        if (Serial.read() != ',') {
          Serial.println(F("Error: Incorrect input format"));
          inputSucceded = false;
          return;
        }
        Serial.flush();
        readValueMax = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }

        if (!isHumidityValidValue(readValueMax)) {
          Serial.println(F("Error: Maximum is out of range (0-100)"));
          inputSucceded = false;
          return;
        }
        if (inputSucceded == true) {  //we update the values only if the input succeds (same for the other functions)
          humidityMinVal = readValueMin;
          humidityMaxVal = readValueMax;
          Serial.println("Minimum value set to: " + String(humidityMinVal) + " % " + "and maximum value set to: " + String(humidityMaxVal) + " % ");
          EEPROM.update(adresses[HUM_MIN], humidityMinVal);
          EEPROM.update(adresses[HUM_MAX], humidityMaxVal);
        }
        currentState = SENSOR_SETTINGS;
        sensorSettingsPrinted = false;
        break;

      case 6:
        currentState = MAIN_MENU;  // Go back
        mainMenuPrinted = false;
        break;

      default:
        Serial.println(F("Incorrect choice."));
        currentState = SENSOR_SETTINGS;
        sensorSettingsPrinted = false;
    }
  }
}

void handleResetLoggerInput() {
  if (Serial.available() > 0) {
    Serial.flush();
    short userInput = Serial.parseInt();
    // Clear the remaining characters in the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }

    switch (userInput) {
      case 1:
        eraseLoggerData();
        currentState = MAIN_MENU;
        resetLoggerPrinted = false;
        break;
      case 2:
        currentState = MAIN_MENU;  //go back if the user enters "NO"
        mainMenuPrinted = false;
        break;
      default:
        Serial.println(F("Incorrect choice."));
        currentState = RESET_LOGGER;
        resetLoggerPrinted = false;
        break;
    }
  }
}

void handleSystemStatusInput() {
  if (Serial.available() > 0) {
    Serial.flush();
    short userInput = Serial.parseInt();
    // Clear the remaining characters in the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }

    switch (userInput) {
      case 1:
        showValuesToConsole = !showValuesToConsole;
        currentState = SYSTEM_STATUS;
        systemStatusPrinted = false;
        break;
      case 2:
        Serial.print(F("Update rate (sec) :"));
        Serial.println(String(updateRate));
        Serial.print(F("Is LED in automatic mode :"));
        Serial.println(String(ledAutoMode));
        Serial.print(F("Distance minimum (cm) :"));
        Serial.println(String(distanceMinVal));
        Serial.print(F("Distance maximum (cm) :"));
        Serial.println(String(distanceMaxVal));
        Serial.print(F("Light miminum (%) :"));
        Serial.println(String(lightMinVal));
        Serial.print(F("Light maximum (%) :"));
        Serial.println(String(lightMaxVal));
        Serial.print(F("Temperature miminum (degrees C) :"));
        Serial.println(String(tempMinVal));
        Serial.print(F("Temperature maximum (degrees C) :"));
        Serial.println(String(tempMaxVal));
        Serial.print(F("Humidity miminum (%) :"));
        Serial.println(String(humidityMinVal));
        Serial.print(F("Humidity maximum (%) :"));
        Serial.println(String(humidityMaxVal));
        currentState = SYSTEM_STATUS;
        systemStatusPrinted = false;
        break;
      case 3:
        for (int i = 0; i < loggedValues; i++) {
          Serial.println("Log " + String(i + 1) + " :" + "Distance (cm): " + String(distanceLoggedValues[i]) + " " + "Light(%): " + String(lightLoggedValues[i]) + " " + "Temperature(degrees C): " + String(tempLoggedValues[i]) + " " + "Humidity(%): " + String(humidityLoggedValues[i]) + " ");
        }
        currentState = SYSTEM_STATUS;
        systemStatusPrinted = false;
        break;
      case 4:
        currentState = MAIN_MENU;  // Go back
        mainMenuPrinted = false;
        break;
      default:
        Serial.println(F("Incorrect choice."));
        currentState = SYSTEM_STATUS;
        systemStatusPrinted = false;
        break;
    }
  }
}

void handleRGBLEDControlInput() {
  if (Serial.available() > 0) {
    Serial.flush();
    short userInput = Serial.parseInt();
    // Clear the remaining characters in the serial buffer
    while (Serial.available() > 0) {
      Serial.read();
    }

    switch (userInput) {
      case 1:
        // Manual color control
        inputSucceded = true;
        Serial.println(F("Enter RGB values (comma-separated): "));
        while (!Serial.available()) {
          // Wait for user input
        }
        Serial.flush();
        value1 = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }
        if (!isLEDValidValue(value1)) {
          Serial.println(F("Error: Value 1 is out of range (0-255)"));
          inputSucceded = false;
          return;
        }

        // Check for the comma separator
        if (Serial.read() != ',') {
          Serial.println(F("Error: Incorrect input format"));
          inputSucceded = false;
          return;
        }
        Serial.flush();
        value2 = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }
        if (!isLEDValidValue(value2)) {
          Serial.println(F("Error: Value 2 is out of range (0-255)"));
          inputSucceded = false;
          return;
        }
        if (Serial.read() != ',') {
          Serial.println(F("Error: Incorrect input format"));
          inputSucceded = false;
          return;
        }
        Serial.flush();
        value3 = Serial.parseInt();
        // Clear the remaining characters in the serial buffer
        while (Serial.available() > 0) {
          Serial.read();
        }
        if (!isLEDValidValue(value3)) {
          Serial.println(F("Error: Value 3 is out of range (0-255)"));
          inputSucceded = false;
          return;
        }
        if (Serial.read() != '\n') {
          Serial.println(F("Error: Incorrect input format"));
          inputSucceded = false;
          return;
        }

        if (inputSucceded == true) {
          ledValues[ledRIndex] = value1;
          ledValues[ledGIndex] = value2;
          ledValues[ledBIndex] = value3;
        }
        Serial.println("RGB values set to: " + String(ledValues[ledRIndex]) + "," + String(ledValues[ledGIndex]) + "," + String(ledValues[ledBIndex]));
        EEPROM.update(adresses[LED_R_VAL], ledValues[ledRIndex]);
        EEPROM.update(adresses[LED_G_VAL], ledValues[ledGIndex]);
        EEPROM.update(adresses[LED_B_VAL], ledValues[ledBIndex]);
        currentState = RGB_LED_CONTROL;
        rgbLedControlPrinted = false;
        break;

      case 2:
        ledAutoMode = !ledAutoMode;
        Serial.println("Automatic LED control toggled " + String(ledAutoMode));
        currentState = RGB_LED_CONTROL;
        rgbLedControlPrinted = false;
        break;
      case 3:
        currentState = MAIN_MENU;  // Go back
        mainMenuPrinted = false;
        break;
      default:
        Serial.println(F("Incorrect choice."));
        currentState = RGB_LED_CONTROL;
        rgbLedControlPrinted = false;
        break;
    }
  }
}


void printMainMenu() {
  Serial.println(F("1. Sensor Settings"));
  Serial.println(F("2. Reset Logger Data"));
  Serial.println(F("3. System Status"));
  Serial.println(F("4. RGB LED Control"));
}

void printSensorSettingsMenu() {
  Serial.println(F("1.1 Sensors Sampling Interval. Set value between 1 and 10 seconds."));
  Serial.println(F("1.2 Distance sensor minimum and maximum threshold."));
  Serial.println(F("1.3 Light sensor minimum and maximum threshold."));
  Serial.println(F("1.4 Temperature sensor minimum and maximum threshold."));
  Serial.println(F("1.5 Humidity sensor minimum and maximum threshold."));
  Serial.println(F("1.6 Back"));
}

void printResetLoggerMenu() {
  Serial.println(F("2. Reset Logger Data. Are you sure?"));
  Serial.println(F("2.1 Yes"));
  Serial.println(F("2.2 No"));
}

void printSystemStatusMenu() {
  Serial.println(F("3. System Status"));
  Serial.println(F("3.1 Current Sensor Readings"));
  Serial.println(F("3.2 Current Sensor Settings"));
  Serial.println(F("3.3 Display Logged Data"));
  Serial.println(F("3.4 Back"));
}

void printRGBLEDControlMenu() {
  Serial.println(F("4. RGB LED Control"));
  Serial.println(F("4.1 Manual Color Control"));
  Serial.println(F("4.2 LED: Toggle Automatic ON/OFF"));
  Serial.println(F("4.3 Back"));
}
