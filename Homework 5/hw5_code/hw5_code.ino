#include <EEPROM.h>
#include "DHT.h" //include the temperature and humidity sensor library
#define ledRPin 11
#define ledGPin 10
#define ledBPin 9
#define trigPin 8
#define echoPin 7
#define tempHumidityPin 6
#define lightSensorPin A0
#define DHTTYPE DHT11
#define maxDistanceValue 400
#define maxLEDValue 255
#define maxTempValue 100
#define minTempValue -25
#define maxHumidityValue 100
#define maxLightValue 100
#define minimumValue 0
#define ledColors 3
#define storedParametersCount 13 //how many parameters we store in the eeprom memory
#define ledRIndex 0
#define ledGIndex 1
#define ledBIndex 2
#define loggedValues 10
#define debugEnabled true
#define tempDefault 25
#define humidityDefault 50

int updateRate = 10; //default update value is 10 seconds
int distanceMinVal = 5;
int distanceMaxVal = 100 ;//units are in cm
int lightMinVal = 0;
int lightMaxVal = 100; //converted from the analog reading to 0-100 values for simplicity
int tempMinVal = 0;
int tempMaxVal = 40;//units are in degrees celsius
int humidityMinVal = 0;
int humidityMaxVal = 100; //units are in %
int ledValues[ledColors] = {0,0,0};
int adresses[storedParametersCount] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
int distanceLoggedValues [loggedValues] = {0,0,0,0,0,0,0,0,0,0};
int lightLoggedValues [loggedValues] = {0,0,0,0,0,0,0,0,0,0};
int tempLoggedValues [loggedValues] = {0,0,0,0,0,0,0,0,0,0};
int humidityLoggedValues [loggedValues] = {0,0,0,0,0,0,0,0,0,0};

enum storedParameters{
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

DHT dht(tempHumidityPin, DHTTYPE);

bool isTempOutOfRange = false;
bool isHumidityOutOfRange = false;
bool isDistanceOutOfRange = false;
bool isLightOutOfRange = false;
bool ledAutoMode = false;

void setup() {
  Serial.begin(9600);
  dht.begin(); //initialize the temp and humidity sensor
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
  int distance = readDistance();
  int light = readLightValue();
  int hum = 0;
  int tmp = 0;
  readTempAndHumidity(tmp, hum);
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Light: ");
  Serial.println(light);
  Serial.print("Temp: ");
  Serial.println(tmp);
  Serial.print("Humidity: ");
  Serial.println(hum);
  delay(1000);
  
}

int readDistance(){
  int computedDistance = 0;
  unsigned long duration = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  computedDistance = duration*0.034/2;
  if(computedDistance > maxDistanceValue){
    computedDistance = maxDistanceValue; //the datasheet for this sensor says the max distance this sensor can read is 4m so we cap the value at 400 cm to prevent weird readings
  }
  return computedDistance;
}

int readLightValue(){
  int photocellValue = analogRead(lightSensorPin);
  return map(photocellValue, 0, 1023, 0, 100);
}

void readTempAndHumidity(int &temp, int &humidity){
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humidity) || isnan(temp)) {
   Serial.println("Temp and humidity sensor read error, writing default values");
    temp = tempDefault;
    humidity = humidityDefault;
  }
}

bool isLEDValidValue(int val) {
  return (val >= minimumValue && val <= maxLEDValue);
} //check the value is between the constants defined at the start of the program

bool isDistanceValidValue (int val){
  return (val >= minimumValue && val <= maxDistanceValue);
} //check the value is between the constants defined at the start of the program

bool isTmpValidValue (int val){
  return (val >= minTempValue && val <= maxTempValue);
} //check the value is between the constants defined at the start of the program

bool isHumidityValidValue (int val){
  return (val >= minimumValue && val <= maxHumidityValue);
} //check the value is between the constants defined at the start of the program

bool isLightValidValue(int val) {
  return (val >= minimumValue && val <= maxLightValue);
} //check the value is between the constants defined at the start of the program
