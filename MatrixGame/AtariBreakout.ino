#include "LedControl.h" //  need the library
#include <LiquidCrystal.h>
#include <EEPROM.h>
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER


const int RSPin = 13;
const int EPin = 8;
const int D4Pin = 2;
const int D5Pin = 3;
const int D6Pin = 4;
const int D7Pin = 5;
LiquidCrystal lcd(RSPin, EPin, D4Pin, D5Pin, D6Pin, D7Pin);

const int platformSize = 3;
const int matrixSize = 8;
const int bricksRows = 3;
const int bricksColumns = 8;
const int menuLength = 4;


const int pinX = A0;
const int pinY = A1;
const int pinSW = 6;
int buttonValue;

int lastDisplayTime = 0;
int displayDelay = 300;
int arrow = 0;
bool joyMoved = false;
bool startFinished = false;
bool button = false;
bool gameExit = false;
bool scoreExit = false;
long timeSwitch = 0;
int score = 0;
int highscore = 0;
bool printHighscore = false;
int startingLevelValue = 1;
char highscorePlayer[4] = "XYZ";
char player[4] = "XYZ";
int letter = 0;
bool selectName = false;
bool settingsExit = false;
bool joyXMoved = false;
bool joyYMoved = false;
int infoDelay = 2000;
int switchDelay = 500;
bool infoExit = false;
int currentLevel;
int lastButtonValue = 1;
long switchTime = 0;
int swValue;
long debounce = 500;

int xValue = 0;
int yValue = 0;
bool movedPlatform = false;
int maxThreshold = 600;
int minThreshold = 400;
int posX = 0;
int platform[3];
int bricks = 24;
unsigned long prevTime = 0;
int gameSpeed = 150;
int startUpTime = 3000;
int startTime = 0;
int state = 0;

struct Ball {

  int x, y;
  int dirX, dirY;
};

struct arrowPositions {
  int x;
  int y;
} pos[4];

int bricksMatrix[matrixSize][matrixSize] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

Ball ball = {3, 3, 0, -1};

void freeSpace() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}

int buttonGotPressed(long switchTime) {
  int buttonValue = digitalRead(pinSW);
  if (buttonValue == 0 && lastButtonValue == 1 && millis() - switchTime > debounce) {
    switchTime = millis();
    lastButtonValue = 0;
    return 1;
  }
  if (buttonValue == 1 && lastButtonValue == 0) {
    lastButtonValue = 1;
  }
  return 0;
}

void resetGame() {

  for (int i = 0; i < matrixSize; i++)
    for (int j = 0; j < matrixSize; j++)
      lc.setLed(0, j, i, false);
  ball.x = 3;
  ball.y = 3;
  ball.dirY = 1;
  ball.dirX = 0;

  bricks = 24;
  if (startingLevelValue == 1) gameSpeed = 250;
  else if (startingLevelValue == 2) gameSpeed = 200;
  else if (startingLevelValue == 3) gameSpeed = 150;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < matrixSize; j++)
      bricksMatrix[i][j] = 1;

  platform[0] = 2;
  platform[1] = 3;
  platform[2] = 4;
  for (int i = 0; i < platformSize; i++)
    lc.setLed(0, platform[i], matrixSize - 1, true);

}

void showArrow(int arr) {
  if (millis() - lastDisplayTime > displayDelay)
  {
    for (int i = 0; i < menuLength; i++)
    {
      lcd.setCursor(pos[i].x, pos[i].y);
      lcd.print(" ");
    }
    lcd.setCursor(pos[arr].x, pos[arr].y);
    lcd.print(">");
    lastDisplayTime = millis();
  }
}

void checkCollisionPlat() {

  for (int i = 0; i < platformSize; i++)
    if (ball.y == matrixSize - 1 && platform[i] == ball.x)
    {
      ball.y--;
      break;
    }
  if (platform[0] == ball.x + 1 && ball.y == matrixSize - 2)
  {
    ball.dirX = -1;
    ball.dirY = -1;
  }
  if (platform[2] == ball.x - 1 && ball.y == matrixSize - 2)
  {
    ball.dirX = 1;
    ball.dirY = -1;
  }
  if (platform[1] == ball.x && ball.y == matrixSize - 2)
  {
    int chance = random(1, 10);
    if (chance <= 2)
    {
      ball.dirX = -1;
      ball.dirY = -1;
    }
    else if (chance <= 4)
    {
      ball.dirX = 1;
      ball.dirY = -1;
    }
    else {
      ball.dirX = 0;
      ball.dirY = -1;
    }
  }
  else if (platform[0] == ball.x  && ball.y == matrixSize - 2)
  {
    int chance = random(1, 10);
    if (chance <= 2)
    {
      ball.dirX = 0;
      ball.dirY = -1;
    }
    else if (chance <= 4)
    {
      ball.dirX = 1;
      ball.dirY = -1;
    }
    else {
      ball.dirX = -1;
      ball.dirY = -1;
    }
  }
  else if (platform[2] == ball.x && ball.y == matrixSize - 2)
  {
    int chance = random(1, 10);
    if (chance <= 2)
    {
      ball.dirX = -1;
      ball.dirY = -1;
    }
    else if (chance <= 4)
    {
      ball.dirX = -1;
      ball.dirY = -1;
    }
    else {
      ball.dirX = 1;
      ball.dirY = -1;
    }
  }
}

void checkCollisionBricks() {

  if (bricksMatrix[ball.y][ball.x] == 1)
  {
    bricksMatrix[ball.y][ball.x] = 0;
    ball.dirY = 1;
    bricks--;
    score++;
  }
}
void printBricks() {

  for (int i = 0; i < bricksRows; i++)
    for (int j = 0; j < bricksColumns; j++)
      if (bricksMatrix[i][j] == 1)
        lc.setLed(0, j, i, true);

}
void printPlatform()
{
  for (int i = 0; i < platformSize; i++)
    lc.setLed(0, platform[i], matrixSize - 1, true);
}
void printBall()
{
  lc.setLed(0, ball.x, ball.y, true);
}

void moveBall() {

  if (ball.dirY > 0) {
    lc.setLed(0, ball.x, ball.y, false);
    ball.y++;
    if (ball.y > 7)
    {
      ball.y = 6;
      ball.dirY = -1;
    }
  }
  else if (ball.dirY < 0)
  {
    lc.setLed(0, ball.x, ball.y, false);
    ball.y--;
    if (ball.y < 0) {
      ball.y = 1;
      ball.dirY = 1;
    }
  }
  if (ball.dirX > 0)
  {
    ball.x++;
    if (random(1, 10) == 1 && ball.x < 7) ball.x++;
    if (ball.x > 7)
    {
      ball.x = 6;
      ball.dirX = -1;
    }
  }
  else if (ball.dirX < 0)
  {
    ball.x--;
    if (random(1, 10) == 1 && ball.x > 0) ball.x--;
    if (ball.x < 0)
    {
      ball.x = 1;
      ball.dirX = 1;
    }
  }
}

void saveHighscore() {

  int address = 0;
  EEPROM.put(address, highscore);
  address += sizeof(int);
  EEPROM.put(address, highscorePlayer);
}

void changeName() {

  while (selectName = true) {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    if (xValue < minThreshold && joyXMoved == false) {
      if (letter > 0) {
        letter--;
      }
      else {
        letter = 2;
      }
      joyXMoved = true;
    }
    if (xValue > maxThreshold && joyXMoved == false) {
      if (letter < 2) {
        letter++;
      }
      else {
        letter = 0;
      }

      joyXMoved = true;
    }
    if (xValue >= minThreshold && xValue <= maxThreshold) {
      joyXMoved = false;
    }
    if (yValue < minThreshold && joyYMoved == false) {
      if (player[letter] == 'A')
        player[letter] = 'Z';
      else
        player[letter]--;
      joyYMoved = true;
    }

    if (yValue > maxThreshold && joyYMoved == false) {
      if (player[letter] != 'Z')
        player[letter]++;
      else
        player[letter] = 'A';
      joyYMoved = true;
    }

    if (yValue >= minThreshold && yValue <= maxThreshold) {
      joyYMoved = false;
    }
    lcd.setCursor(0, 1);
    for (int i = 0; i < 3; i++)
      lcd.print(player[i]);
    lcd.setCursor(1, letter);
    if (buttonGotPressed(0)) {
      settingsExit = true;
      selectName = false;
      return ;
    }
  }
}
void settings() {

  lcd.clear();
  button = false;
  while (settingsExit == false)
  {
    lcd.setCursor(0, 0);
    lcd.print("Level ");
    lcd.print(startingLevelValue);
    yValue = analogRead(pinY);
    if (yValue < minThreshold && joyMoved == false)
    {
      if (startingLevelValue > 1)
        startingLevelValue--;
      joyMoved = true;
    }
    if (yValue > maxThreshold && joyMoved == false)
    {
      if (startingLevelValue < 3)
        startingLevelValue++;
      joyMoved = true;
    }
    if (yValue >= minThreshold && yValue <= maxThreshold)
    {
      joyMoved = false;
    }
    lcd.setCursor(0, 1);
    for (int i = 0; i < 3; i++)
      lcd.print(player[i]);
    if (buttonGotPressed(0)) selectName = true;
    if (selectName == true) {
      changeName();
    }
  }
  lcd.clear();
}

void highscoreMenu() {

  int address = 0;
  switchTime = 0;
  lcd.clear();
  button = false;
  while (scoreExit == false)
  {
    lcd.setCursor(0, 0);
    lcd.print("Highest score ");
    lcd.print(EEPROM[0]);
    address = sizeof(int);
    EEPROM.get(address, highscorePlayer);
    lcd.setCursor(0, 1);
    for (int i = 0; i < 3; i++)
    lcd.print(highscorePlayer[i]);
    if (buttonGotPressed(0)) {
      scoreExit = true;
    }
  }

  button = false;
  lcd.clear();
}

void play()
{
  lcd.clear();
  resetGame();
  button = false;
  while (state == 0)
  {

    xValue = analogRead(pinX);
    if (xValue > maxThreshold && movedPlatform == false)
    {
      if (platform[platformSize - 1] != 7)
        for (int i = 0; i < platformSize; i++)
        {
          lc.setLed(0, platform[i], 7, false);
          platform[i]++;
        }
      movedPlatform = true;
    }
    if (xValue < minThreshold && movedPlatform == false)
    {
      if (platform[0] != 0)
        for (int i = 0; i < platformSize; i++)
        {
          lc.setLed(0, platform[i], 7, false);
          platform[i]--;
        }
      movedPlatform = true;
    }
    if (xValue >= minThreshold && xValue <= maxThreshold)
    {
      movedPlatform = false;
    }
    checkCollisionBricks();
    checkCollisionPlat();
    if (millis() - prevTime > gameSpeed) {
      moveBall();
      prevTime = millis();
    }
    printBall();
    printPlatform();
    printBricks();
    //delay(150);
    //prevTime = millis();
    bool platformHit = false;
    if (bricks == 0) state = 2;
    if (ball.y == matrixSize - 1)
      state = 1;

    int address = 0;
    if (state == 1) {
      lcd.clear();
      while (gameExit == false) {
        EEPROM.get(address, highscore);
        if (score > highscore)
        {
          lcd.setCursor(0, 0);
          lcd.print("New");
          lcd.setCursor(0, 1);
          lcd.print("Highscore ");
          lcd.print(score);
        }
        else {
          lcd.setCursor(0, 0);
          lcd.print("Game Over");
          lcd.setCursor(0, 1);
          lcd.print("Wanna try again?");
        }
        if (buttonGotPressed(0)) gameExit = true;
      }
    }
    else if (state == 2)
    {
      lcd.clear();
      while (gameExit == false) {
        lcd.setCursor(0, 0);
        if (startingLevelValue == 1)
        { lcd.print("You beat the");
          lcd.setCursor(0, 1);
          lcd.print("first level");
        }
        else if (startingLevelValue == 2) {
          lcd.print("You beat the");
          lcd.setCursor(0, 1);
          lcd.print("second level");
        }
        else {
          lcd.print("You beat the");
          lcd.setCursor(0, 1);
          lcd.print("game");
        }
        if (buttonGotPressed(0)) gameExit = true;
      }
    }
    lcd.setCursor(0, 0);
    lcd.print("Score ");
    lcd.print(score);
    lcd.setCursor(0, 1);
    lcd.print("Level ");
    lcd.print(startingLevelValue);
  }
  if (score > highscore)
  {
    highscore = score;
    strcpy(highscorePlayer, player);
    saveHighscore();
  }
  if (state == 2 && startingLevelValue < 3)
  {
    state = 0;
    gameExit = false;
    startingLevelValue++;
    play();
  }
  
  lcd.clear();
}

void info() {

  bool printInfo = false;
  lcd.clear();
  long functionStart = millis();
  long functionTime = 0;
  while (infoExit == false) {

    if (functionTime < infoDelay) {
      lcd.setCursor(0, 0);
      lcd.print("Atari Breakout");
      lcd.setCursor(0, 1);
      lcd.print("Vlad Masgras");
    }
    else if (functionTime < infoDelay * 2)
    {
      if (printInfo == false) {
        lcd.clear();
        printInfo = true;
      }
      lcd.setCursor(0, 0);
      lcd.print("https://github.c");
      lcd.setCursor(0, 1);
      lcd.print("om/VladMasgras");
    }
    else if (functionTime < infoDelay * 3)
    {
      if (printInfo == true) {
        lcd.clear();
        printInfo = false;
      }
      lcd.setCursor(0, 0);
      lcd.print("@ UnibucRobotics");
    }
    else infoExit = true;
    functionTime = millis() - functionStart;
  }
  button = false;
  lcd.clear();
}

void setup() {
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  platform[0] = 2;
  platform[1] = 3;
  platform[2] = 4;
  for (int i = 0; i < platformSize; i++)
    lc.setLed(0, platform[i], matrixSize - 1, true);

  for (int i = 0; i < bricksRows; i++)
    for (int j = 0; j < bricksColumns; j++)
      lc.setLed(0, j, i, true);

  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  pos[0].x = 0;
  pos[0].y = 0;
  pos[1].x = 7;
  pos[1].y = 0;
  pos[2].x = 0;
  pos[2].y = 1;
  pos[3].x = 11;
  pos[3].y = 1;
  //freeSpace();

  Serial.begin(9600);
}


void loop() {

  if (millis() - startTime <= startUpTime)
  {
    lcd.setCursor(0, 0);
    lcd.print("Alo pronto");
    lcd.setCursor(0, 1);
    lcd.print("Sunt contactul");
  }
  else
  {
    if (startFinished == false) lcd.clear();
    startFinished = true;
    lcd.setCursor(1, 0);
    lcd.print("Start");

    lcd.setCursor(8, 0);
    lcd.print("Settings");

    lcd.setCursor(1, 1);
    lcd.print("Highscore");

    lcd.setCursor(12, 1);
    lcd.print("Info");

    xValue = analogRead(pinX);
    if (xValue < minThreshold && joyMoved == false)
    {
      if (arrow > 0)
      {
        arrow--;
      }
      else
      {
        arrow = 3;
      }
      joyMoved = true;
    }

    if (xValue > maxThreshold && joyMoved == false)
    {
      if (arrow < 3)
      {
        arrow++;
      }
      else
      {
        arrow = 0;
      }

      joyMoved = true;
    }

    if (xValue >= minThreshold && xValue <= maxThreshold)
    {
      joyMoved = false;
    }
    showArrow(arrow);
    state = 0;
    gameExit = false;
    scoreExit = false;
    selectName = false;
    settingsExit = false;
    infoExit = false;
    score = 0;
    if (buttonGotPressed(0))
    {
      if (arrow == 0)
        play();
      else if (arrow == 1)
        settings();
      else if (arrow == 2) highscoreMenu();
      else info();
    }
  }
}
