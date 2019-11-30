#include <LiquidCrystal.h>
#include <EEPROM.h>

const int RSPin = 12;
const int EPin = 11;
const int D4Pin = 5;
const int D5Pin = 4;
const int D6Pin = 3;
const int D7Pin = 2;
LiquidCrystal lcd(RSPin, EPin, D4Pin, D5Pin, D6Pin, D7Pin);

const int pinSW = 10;
const int pinX = A0;
const int pinY = A1;
int swValue = LOW;
int xValue = 0;
int yValue = 0;
int arrow = 0;

bool joyMoved = false;
bool button = false;
int minThreshold = 400;
int maxThreshold = 600;
int displayDelay = 200;
int lastDisplayTime = 0;
int startTime = 0;
int lastPlayTime = 0;
int playTime = 10000;
int currentTime;
int prevTime = 0;
int levelUpTime = 2000;
int startingLevelValue = 0;

int level = 0;
int score = 0;
bool settingsExit = false;
int highestScore = 0;
bool scoreExit = false;

struct arrowPositions {
  int x;
  int y;
} pos[3];

void settings()
{
  lcd.clear();
  delay(500);
  while(1)
  {
    lcd.setCursor(0,0);
    lcd.print("Choose level:");
    lcd.setCursor(14,0);
    lcd.print(startingLevelValue);
    lcd.setCursor(11,1);
    lcd.print("Exit");
    yValue = analogRead(pinY);
    if(yValue < minThreshold && joyMoved == false)
    {
      if(startingLevelValue > 1)
        startingLevelValue--;
      joyMoved = true;
      lcd.setCursor(14,0);
      lcd.print(startingLevelValue);
    }
    if(yValue > maxThreshold && joyMoved == false)
    {
      startingLevelValue++;
      joyMoved = true;
      lcd.setCursor(14,0);
      lcd.print(startingLevelValue);
    }
    if(yValue >= minThreshold && yValue <= maxThreshold) 
    {
      joyMoved = false; 
    }
    swValue = digitalRead(pinSW);
    if(swValue == LOW && button == false)
      {
        button = true;
        settingsExit = true;
      }
    else if(swValue == LOW)
      button = false;
  if(settingsExit == true)
    {
      arrow = 1;
      button = false;
      lcd.clear();
      delay(500);
      break;
    }
  }
}
void highscore()
{
  lcd.clear();
  delay(500);
  while(1)
  {
    lcd.setCursor(0,0);
    lcd.print("Highest score");
    if (score>highestScore) highestScore = score;
    lcd.setCursor(2,1);
    lcd.print(highestScore);
    lcd.setCursor(10,1);
    lcd.print("Exit");
    swValue = digitalRead(pinSW);
    if(swValue == LOW && button == false)
    {
      button = true;
      scoreExit = true;
    }
    else if(swValue == LOW){
      button = false;
    }
    if (scoreExit == true)
    {
      arrow = 2;
      button = false;
      lcd.clear();
      delay(500);
      break;
    }
  }
}
void start()
{
  lcd.clear();
  delay(300);
  startTime = millis();
  level = startingLevelValue;
  while(1)
  {
    currentTime = millis();
    if(currentTime - startTime < playTime)
    {
      //Serial.println(currentTime);
      lcd.setCursor(5,0);
      lcd.print("Lives: 3");
      lcd.setCursor(0,1);
      lcd.print("Level:");
      lcd.print(level);
      lcd.setCursor(8,1);
      lcd.print("Score: ");
      lcd.setCursor(14,1);
      score = level*3;
      lcd.print(score);
      delay(500);
      if(currentTime - prevTime >= levelUpTime)
      {
        prevTime = currentTime;
        level++;
      }
    }
    else
    {
      endGame();
      button = false;
      lcd.clear();
      arrow = 0;
      delay(500);
      break;
    }
  }
}
void endGame()
{
  lcd.clear();
  delay(500);

  while(1)
  {
    lcd.setCursor(0,0);
    lcd.print("Congratulations");
    lcd.setCursor(0,1);
    lcd.print("you won!    Exit");
    swValue = digitalRead(pinSW);
    if (swValue == LOW && button ==false)
    {
      button =true;
      break;
    }
    else if(swValue == LOW)
    button = false;
  }
}
void showArrow(int arr) {
  if(millis() - lastDisplayTime > displayDelay)
  {
      for(int i = 0; i < 3; i++) 
    {
      lcd.setCursor(pos[i].x, pos[i].y);
      lcd.print(" ");
    }
     lcd.setCursor(pos[arr].x, pos[arr].y);
     lcd.print(">");
     lastDisplayTime = millis();
  }
}
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  pos[0].x=0;
  pos[0].y=0;
  pos[1].x=7;
  pos[1].y=0;
  pos[2].x=0;
  pos[2].y=1;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(1,0);
  lcd.print("Start");
  
  lcd.setCursor(8,0);
  lcd.print("Settings");

  lcd.setCursor(1,1);
  lcd.print("Highscore");
  xValue = analogRead(pinX);
  if(xValue < minThreshold && joyMoved == false) 
  {
    if(arrow > 0) 
    {
      arrow--;
    }
    else 
    {
      arrow = 2;
    }
    joyMoved = true;
  }

  if (xValue > maxThreshold && joyMoved == false) 
  {
    if (arrow < 2) 
     {
      arrow++;  
     }
     else 
     {
      arrow = 0; 
     } 
       
     joyMoved = true;
  }
  
  if(xValue >= minThreshold && xValue <= maxThreshold) 
  {
    joyMoved = false;
  }
  showArrow(arrow);
  swValue = digitalRead(pinSW);
  if(swValue == LOW && button == false)
    button = true;
  else if(swValue == LOW)
    button = false;
  settingsExit = false;
  scoreExit = false;
   if (button == true)
   {
      if(arrow == 0)
        start();
       else if(arrow == 1)
        settings();
       else highscore();
   }
}
