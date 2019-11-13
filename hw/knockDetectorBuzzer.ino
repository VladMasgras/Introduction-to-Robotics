const int passiveBuzzerPin = A0;
int speakerValue = 0;
const int threshold = 400;

unsigned int previousTime = 0;
const int buzzInterval = 5000;

const int activeBuzzerPin =9;
int buzzerTone = 12000;
int buzzerValue = 0;
int knockValue=0;

const int pushButton = 2;
int pushButtonState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(passiveBuzzerPin, INPUT);
  pinMode(activeBuzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   unsigned int currentTime=millis();
   speakerValue = analogRead(passiveBuzzerPin);
   Serial.println(speakerValue);
   if (speakerValue > threshold)
   {
     Serial.println("Knock!");
    knockValue=1;
   }
   if (currentTime-previousTime >= buzzInterval && knockValue == 1)
   {
    previousTime=currentTime;
    tone(activeBuzzerPin, buzzerTone);
   }
   pushButtonState = digitalRead(pushButton);
   if(!pushButtonState){
  Serial.println("BUTTON PRESSED");
  noTone(activeBuzzerPin);
  knockValue=0;
  pushButtonState=!pushButtonState;
   }
}
