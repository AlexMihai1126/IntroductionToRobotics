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

int updateRate = 10; //default update value is 10 seconds
int distanceMinVal = 5;
int distanceMaxVal = 100 ;//units are in cm
int lightMinVal = 0;
int lightMaxVal = 255; //converted from the analog reading to 0-255 values
int tempMinVal = 0;
int tempMaxVal = 40;//units are in degrees celsius
int humidityMinValue = 0;
int humidityMaxValue = 100; //units are in %

DHT dht(tempHumidityPin, DHTTYPE);

bool debugEnabled = true;
bool isTempOutOfRange = false;
bool isHumidityOutOfRange = false;
bool isDistanceOutOfRange = false;
bool isLightOutOfRange = false;
void setup() {
  Serial.begin(9600);
  dht.begin(); //initialize the temp and humidity sensor
  pinMode(ledRPin, OUTPUT);
  pinMode(ledGPin, OUTPUT);
  pinMode(ledBPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(lightSensorPin, INPUT);
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
  return computedDistance;
}

int readLightValue(){
  int photocellValue = analogRead(lightSensorPin);
  return map(photocellValue, 0, 1023, 0, 255);
}

void readTempAndHumidity(int &temp, int &humidity){
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(humidity) || isnan(temp)) {
   Serial.println("Temp and humidity sensor read error");
  }
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
