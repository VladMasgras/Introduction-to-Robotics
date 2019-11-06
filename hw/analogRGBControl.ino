int redPin=9;
int greenPin=11;
int bluePin=10;
int potPinBlue=A0;
int potPinRed=A1;
int potPinGreen=A2;
int potValueBlue=0;
int potValueRed=0;
int potValueGreen=0;
int ledValueBlue=0;
int ledValueRed=0;
int ledValueGreen=0;

void setColor(int red,int green,int blue)
{
  analogWrite(redPin,red);
  analogWrite(bluePin,blue);
  analogWrite(greenPin,green);
}
void setup() {
  // put your setup code here, to run once:
  pinMode(potPinBlue, INPUT);
  pinMode(potPinRed, INPUT);
  pinMode(potPinGreen, INPUT);
  pinMode(redPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  Serial.begin(9600);  
}

void loop() {
 
  potValueBlue=analogRead(potPinBlue);
  potValueRed=analogRead(potPinRed);
  potValueGreen=analogRead(potPinGreen);
  ledValueBlue=map(potValueBlue,0,1023,0,255);
  ledValueRed=map(potValueRed,0,1023,0,255);
  ledValueGreen=map(potValueGreen,0,1023,0,255);
  Serial.println(potValueRed);
  Serial.println(potValueBlue);
  Serial.println(potValueGreen);
  setColor(ledValueRed,ledValueGreen,ledValueBlue);
  // put your main code here, to run repeatedly:

}
