// libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// initialize LCD
LiquidCrystal_I2C lcd(0x20,16,2); // set the LCD address to 0x20 for a 16 chars and 2 line display

// initialize button pins
const int moveRedButton;
const int moveYellowButton;
const int moveGreenButton;
const int moveBlueButton;
const int diceButton = 10 ;
int dice_button_state;
const int buzzer = 8; //buzzer to arduino pin 8 for victory music 

// initialize led matrix
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 256// Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

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
    }

    // this function checks if another piece is in the same position on the board and sends that piece back to home
    void checkOverlaps(Piece* otherPiece) {
      if (currPos == otherPiece->currPos) {
        otherPiece->isHome = true;
        otherPiece->currPos = offBoardPos;
      }
    }

    // this function starts the corresponding Player's turn
    void takeTurn(int moveCount) {
      // if the piece is home, need to roll a 6 to get on board
      if (isHome) {
        if (moveCount == 6) {
          isHome = false;
          currPos = startPos; // puts piece on the board
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
        for (int i = 0; i < moveCount; i++)
          moveOne();
      }
    }
};

//set home base function 
void homeBase(){ 
    pixels.setPixelColor(0, pixels.Color(0, 150, 0)); // GREEN HOME 
    pixels.show(); 
    pixels.setPixelColor(15, pixels.Color(150, 0, 0)); // BLUE HOME 
    pixels.show();   // Send the updatedpixel colors to the hardware.
    pixels.setPixelColor(240, pixels.Color(0, 0, 150)); // RED HOME 
    pixels.show();   // Send the updatedpixel colors to the hardware.
    pixels.setPixelColor(255, pixels.Color(150, 150, 0)); //YELLOW  HOME 
   pixels.show();   // Send the updatedpixel colors to the hardware. 
   delay(DELAYVAL); // Pause before next pass through loop
}

//clear pad
void pixelPadClear(){ 
  for(int i=103; i<106; i++){ 
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // GREEN HOME 
    pixels.show(); 
  }
  for(int i=118; i<121; i++){ 
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // GREEN HOME 
    pixels.show(); 
  }
  for(int i=135; i<137; i++){ 
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // GREEN HOME 
    pixels.show(); 
  }
}

//set dice pad
void setDicePad(int randomNumber){ 
  Serial.println("DICE BUTTON PRESSED"); 
      //CLEAR DICE PAD 
      pixelPadClear(); 
      Serial.println(randomNumber); 
      if(randomNumber == 1){ 
        pixels.setPixelColor(119, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
      }
      else if(randomNumber == 2){ 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(137, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
      }
      else if(randomNumber == 3){ 
        pixels.setPixelColor(119, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(103, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
        pixels.setPixelColor(137, pixels.Color(150, 150, 150)); // GREEN HOME 
        pixels.show(); 
      }
      else if(randomNumber == 4){ 
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
      else if(randomNumber == 5){ 
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
      else if(randomNumber == 6){ 
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
      dice_button_state = 0; 
    }

void setup() {
 pinMode(diceButton,INPUT);
 pinMode(buzzer, OUTPUT); // Set buzzer - pin 8 as an output
 pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 homeBase(); // set home base

  Serial.begin(9600);
  
  // initialize random number generator
  randomSeed(analogRead(0));

  // create board with indices
  int gameBoard[gameBoardSize];
  for (int i = 0; i < gameBoardSize; i++)
    gameBoard[i] = i;

  // create all 4 pieces and initialize corresponding board values
  Players[0] = new Piece("red", 1, 40);
  Players[1] = new Piece("yellow", 11, 10);
  Players[2] = new Piece("green", 21, 20);
  Players[3] = new Piece("blue", 31, 30);
}


void loop() {
    dice_button_state= digitalRead(diceButton); 
    while (dice_button_state == HIGH) {
        int randomNumber = rollDie(); 
        setDicePad(randomNumber); 
    }
}
