// libraries
/* #include <Wire.h>
#include <LiquidCrystal_I2C.h> */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// initialize LCD
//LiquidCrystal_I2C lcd(0x20,16,2); // set the LCD address to 0x20 for a 16 chars and 2 line display

// initialize button pins
const int moveRedButton = 4;
int moveRedButtonState;
const int moveYellowButton = 5;
int moveYellowButtonState;
const int moveGreenButton = 6;
int moveGreenButtonState;
const int moveBlueButton = 7;
int moveBlueButtonState;
const int diceButton = 10 ;
int diceButtonState;
const int buzzer = 8; //buzzer to arduino pin 8 for victory music 

// initialize led matrix
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 256// Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

// other variables
const int numPlayers = 4;
const int gameBoardSize = 40; // can change the how many spaces on board
const int offBoardPos = 50; // can change depending on board size
int gameBoard[gameBoardSize];

// functions and Piece class
// this function generates a random integer between 1 and 6
int rollDie() {
  return random(1,7);
}

// this class represents the Players' board piece
class Piece {
  public:
    // ATTRIBUTES
    String colorId;
    int currPos; // this will be set to a value not on the board (ex: 50)
    int startPos;
    int endPos;
    bool isHome;
    bool hasWon;

    // METHODS
    // constructor
    Piece(String color, int start, int end) : colorId(color), currPos(offBoardPos), startPos(start), endPos(end), isHome(true), hasWon(false) {}

    // this function moves the piece forward one space on the board
    void moveOne() {
      // if statement prevents piece from moving forward if it is at its end position on the board
      if ((currPos+1) % gameBoardSize != endPos) // modulus functionality to create circularity for board
        currPos = (currPos + 1) % gameBoardSize;

        // update the board with the correct color
        switch (colorId) {
          case "red":
            break;

          case "yellow"
            break;
          
          case "green"
            break;

          case "blue"

            break;
        }
    }
  
    // this function checks if another piece is in the same position on the board and sends that piece back to home
    void checkOverlaps(Piece* otherPiece) {
      if (currPos == otherPiece->currPos) {
        otherPiece->isHome = true;
        otherPiece->currPos = offBoardPos;
        // add LED matrix color updaes here also
      }
    }

    // this function starts the corresponding Player's turn
    void takeTurn(int moveCount) {
      // if the piece is home, need to roll a 6 to get on board
      if (isHome) {
        if (moveCount == 6) {
          isHome = false;
          currPos = startPos; // puts piece on the board
          moveOne();
        }
      }
      // if the piece is at end, need to roll a 1 to win
      else if (currPos == endPos -1) {
        if (moveCount == 1) {
          hasWon = true;
        }
      }
      // if piece is on board, roll
      else {
        for (int i = 0; i < moveCount; i++) {
          moveOne();
        }
      }
    }


};

// set home base function 
void homeBase(){ 
    pixels.setPixelColor(0, pixels.Color(150, 150, 0)); // GREEN HOME 
    pixels.show(); 
    pixels.setPixelColor(15, pixels.Color(150, 0, 0)); // BLUE HOME 
    pixels.show();   // Send the updatedpixel colors to the hardware.
    pixels.setPixelColor(240, pixels.Color(0, 0, 150)); // RED HOME 
    pixels.show();   // Send the updatedpixel colors to the hardware.
    pixels.setPixelColor(255, pixels.Color(0, 150, 0)); //YELLOW  HOME 
   pixels.show();   // Send the updatedpixel colors to the hardware. 
   delay(DELAYVAL); // Pause before next pass through loop
}

// clear pad
void pixelPadClear(){ 
  for(int i=103; i<106; i++){ 
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // GREEN HOME 
    pixels.show(); 
  }
  for(int i=118; i<121; i++){ 
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // GREEN HOME 
    pixels.show(); 
  }
  for(int i=135; i<138; i++){ 
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // GREEN HOME 
    pixels.show(); 
  }
}

// set dice pad
void setDicePad(int roll){ 
  Serial.println("DICE BUTTON PRESSED"); 
      //CLEAR DICE PAD 
      pixelPadClear(); 
      Serial.println(roll); 
      if(roll == 1){ 
        pixels.setPixelColor(119, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
      }
      else if(roll == 2){ 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(137, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
      }
      else if(roll == 3){ 
        pixels.setPixelColor(119, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(137, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
      }
      else if(roll == 4){ 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(105, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(137, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(135, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
      }
      else if(roll == 5){ 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(105, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(137, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(135, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(119, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
      }
      else if(roll == 6){ 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(105, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(137, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(135, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(118, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(120, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        
      }
      delay(1000); 
      diceButtonState = 0; 
    }

void setup() {
 pinMode(diceButton,INPUT);
 pinMode(moveRedButton, INPUT);
 pinMode(moveYellowButton, INPUT);
 pinMode(moveGreenButton, INPUT);
 pinMode(moveBlueButton, INPUT);
 pinMode(buzzer, OUTPUT); // Set buzzer - pin 8 as an output
 pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 homeBase(); // set home base

  Serial.begin(9600);
  
  // initialize random number generator
  randomSeed(analogRead(0));

  // create board with indices
  for (int i = 0; i < gameBoardSize; i++)
    gameBoard[i] = i;
}

  // create all 4 pieces and initialize corresponding board values
  Piece* Red = new Piece("red", 1, 40);
  Piece* Yellow = new Piece("yellow", 11, 10);
  Piece* Green = new Piece("green", 21, 20);
  Piece* Blue = new Piece("blue", 31, 30);
  Piece* Players[numPlayers] = {Red, Yellow, Green, Blue};
  int currPlayerInd = 0;
  Piece* currPlayer = Players[currPlayerInd]; // default first player is red

  void loop() {
    diceButtonState = digitalRead(diceButton);
    while (diceButtonState == HIGH) {
      int roll = rollDie(); 
      setDicePad(roll); 

      Serial.print("Player ");
      Serial.print(currPlayer->colorId);
      Serial.print(" rolled: ");
      Serial.println(roll);

      // wait for correct player to their move press button
      if (currPlayer->colorId == "red" && digitalRead(moveRedButton) == HIGH) {
        currPlayer->takeTurn(roll); // start the player's turn with their roll
      }
      else if (currPlayer->colorId == "yellow" && digitalRead(moveYellowButton) == HIGH) {
       currPlayer->takeTurn(roll); // start the player's turn with their roll
      }
      else if (currPlayer->colorId == "green" && digitalRead(moveGreenButton) == HIGH) {
        currPlayer->takeTurn(roll); // start the player's turn with their roll
      }
      else if (currPlayer->colorId == "blue" && digitalRead(moveBlueButton) == HIGH) {
        currPlayer->takeTurn(roll); // start the player's turn with their roll
      }

      // check all other player pieces and if they are in the same spot
      int checkIndex = (currPlayerInd + 1) % numPlayers;
      while (currPlayerInd != checkIndex) {
         currPlayer->checkOverlaps(Players[checkIndex]);
         checkIndex = (checkIndex + 1) % numPlayers; // increment the checkIndex
      }

      // check if current player has won
      if (currPlayer->hasWon) {
        Serial.print("Player ");
        Serial.print(currPlayer->colorId);
        Serial.println(" has won!");
        while (true) {} // ends the game
      }

      // otherwise move to the next player
      currPlayerInd = (currPlayerInd + 1) % numPlayers;
      currPlayer = Players[currPlayerInd];
      delay (500); // debouncer
      }
  }




