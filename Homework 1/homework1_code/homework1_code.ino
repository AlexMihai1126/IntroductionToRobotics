const int ledPinR=3;
const int ledPinG=5;
const int ledPinB=6;
const int controlRed=A1;
const int controlGreen=A0;
const int controlBlue=A2;

void setup() {
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  Serial.begin(9600);
}

void printValues(){
  Serial.print("Red: ");
  Serial.println(analogRead(controlRed));
  Serial.print("Green: ");
  Serial.println(analogRead(controlGreen));
  Serial.print("Blue: ");
  Serial.println(analogRead(controlBlue));
}//function used for debugging an issue I encountered with reading bad values on the analog pins

void loop() {

  //printValues();

  int inputRed=analogRead(controlRed);
  int inputGreen=analogRead(controlGreen);
  int inputBlue=analogRead(controlBlue);
  
  int ledValueRed = map(inputRed,0,1023,0,255);
  int ledValueGreen = map(inputGreen,0,1023,0,255);
  int ledValueBlue = map(inputBlue,0,1023,0,255);

  analogWrite(ledPinR,ledValueRed);
  analogWrite(ledPinG,ledValueGreen);
  analogWrite(ledPinB,ledValueBlue);
}
