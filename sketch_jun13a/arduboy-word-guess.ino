#include <Arduboy2.h>
Arduboy2 arduboy;

char *wordList[] = {"ABOUT", "ALMOST", "ANSWER", "AWAKE", "BASIN", "BASKET", "BIRTH", "BLACK", "BLOOD", "BOARD", "BRAKE", "CANVAS", "CAUSE", "CHEAP", "CHIEF", "CLOCK", "COLLAR", "CURVE", "DRINK", "ERROR", "EXPERT", "FATHER", "FINGER", "FIXED", "FLIGHT", "FRIEND", "FRONT", "FRUIT", "HAPPY", "ISLAND", "MONTH", "MOTHER", "NATION", "NERVE", "NORMAL", "NORTH", "NUMBER", "OFFER", "PENCIL", "PLOUGH", "QUITE", "RECORD", "REGRET", "REWARD", "RHYTHM", "RIVER", "ROUGH", "SCALE", "SCHOOL", "SENSE", "SHARP", "SHEEP", "SHELF", "SHORT", "SILVER", "SKIRT", "SNAKE", "SPONGE", "SPRING", "STAGE", "STEEL", "STONE", "SUDDEN", "SYSTEM", "THEORY", "TONGUE", "TOUCH", "VESSEL", "WASTE", "YELLOW"};
#define WORD_LIST_SIZE 70
char currentLetter;
String currentWord;
String life;
String playerWord;

#define GAME_STATE_PLAYING         0
#define GAME_STATE_CORRECT_GUESS   1
#define GAME_STATE_INCORRECT_GUESS 2
#define GAME_STATE_LOSE            3
#define GAME_STATE_WIN             4
#define GAME_STATE_NEW_GAME        5
byte gameState;

#define SCREEN_WIDTH                        25
#define SCREEN_HEIGHT                       8
#define CHARACTER_WIDTH                     6
#define CHARACTER_HEIGHT                    8
#define MID_GAME_MESSAGE_DELAY_MILLISECONDS 2000
#define END_GAME_MESSAGE_DELAY_MILLISECONDS 5000

void setup() {
  arduboy.begin();
  arduboy.clear();
  arduboy.initRandomSeed();

  gameState = GAME_STATE_NEW_GAME;
}

void loop() {
  arduboy.clear();
  arduboy.pollButtons();

  if(gameState == GAME_STATE_NEW_GAME || gameState == GAME_STATE_LOSE || gameState == GAME_STATE_WIN) {
    byte randomWordIndex = random(0, WORD_LIST_SIZE - 1);

    currentWord = String(wordList[randomWordIndex]);
    playerWord = String(wordList[randomWordIndex]);
    life = String("########");

    currentLetter = 'A';

    for(byte i = 0; i < playerWord.length(); i++) {
      playerWord.setCharAt(i, '_');
    }
  }
  
  gameState = GAME_STATE_PLAYING;

  updateSelectedLetter();
  makeGuess();
  checkWinLoss();

  displayGame();
  
  arduboy.display();

  if(gameState == GAME_STATE_CORRECT_GUESS || gameState == GAME_STATE_INCORRECT_GUESS) {
    delay(MID_GAME_MESSAGE_DELAY_MILLISECONDS);
  } else if(gameState == GAME_STATE_LOSE || gameState == GAME_STATE_WIN) {
    delay(END_GAME_MESSAGE_DELAY_MILLISECONDS);
  }
}

void updateSelectedLetter() {
  if(arduboy.justPressed(LEFT_BUTTON)) {
    if(currentLetter == 'A') {
      currentLetter = 'Z';
    } else {
      currentLetter--;
    }
  }

  if(arduboy.justPressed(RIGHT_BUTTON)) {
    if(currentLetter == 'Z') {
      currentLetter = 'A';
    } else {
      currentLetter++;
    }
  }
}

void makeGuess() {
  if(arduboy.justPressed(B_BUTTON)) {
    gameState = GAME_STATE_INCORRECT_GUESS;
    
    for(byte i = 0; i < currentWord.length(); i++) {
      if(currentWord.charAt(i) == currentLetter && playerWord.charAt(i) == '_') {
        playerWord.setCharAt(i, currentWord.charAt(i));
        gameState = GAME_STATE_CORRECT_GUESS;
      }
    }

    if(gameState == GAME_STATE_INCORRECT_GUESS) {
      byte guessChunk = life.indexOf('#');
      life.setCharAt(guessChunk, currentLetter);
    }
  }
}

void checkWinLoss() {
  if(playerWord.indexOf('_') < 0) {
    gameState = GAME_STATE_WIN;
  }

  if(life.indexOf('#') < 0) {
    gameState = GAME_STATE_LOSE;
  }
}

void displayGame() {
  byte wordCenter;
  // Player Word
  wordCenter = floor((SCREEN_WIDTH - playerWord.length()) / 2) - 1;
  arduboy.setCursor(wordCenter * CHARACTER_WIDTH, 1 * CHARACTER_HEIGHT);
  arduboy.print(playerWord);

  // Life
  wordCenter = floor((SCREEN_WIDTH - sizeof("Life: ") - life.length()) / 2) - 1;
  arduboy.setCursor(wordCenter * CHARACTER_WIDTH, 3 * CHARACTER_HEIGHT);
  arduboy.print("Life: ");
  arduboy.print(life);

  // Letter
  if(gameState == GAME_STATE_PLAYING) {
    arduboy.setCursor(8 * CHARACTER_WIDTH, 5 * CHARACTER_HEIGHT);
    arduboy.print("->");
    arduboy.print(currentLetter);
    arduboy.print("<-");
  }

  // Result
  if(gameState == GAME_STATE_CORRECT_GUESS) {
    arduboy.setCursor(8 * CHARACTER_WIDTH, 5 * CHARACTER_HEIGHT);
    arduboy.print("Correct!");
  } else if(gameState == GAME_STATE_INCORRECT_GUESS) {
    arduboy.setCursor(6 * CHARACTER_WIDTH, 5 * CHARACTER_HEIGHT);
    arduboy.print("Incorrect...");
  } else if(gameState == GAME_STATE_WIN) {
    arduboy.setCursor(CHARACTER_WIDTH, 5 * CHARACTER_HEIGHT);
    arduboy.print("Great job! You win!");
  } else if(gameState == GAME_STATE_LOSE) {
    arduboy.setCursor(3 * CHARACTER_WIDTH, 5 * CHARACTER_HEIGHT);
    arduboy.print("Game over. Answer:");
    wordCenter = floor((SCREEN_WIDTH - currentWord.length()) / 2) - 1;
    arduboy.setCursor(wordCenter * CHARACTER_WIDTH, 6 * CHARACTER_HEIGHT);
    arduboy.print(currentWord);
  }
  
  // DEBUG
  //arduboy.setCursor(0, 0);
  //arduboy.print(currentWord);
}
