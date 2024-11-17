// libraries
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// initialize LCD
LiquidCrystal_I2C lcd(0x20,16,2); // set the LCD address to 0x20 for a 16 chars and 2 line display

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
int speakerPin = 8;
//const int buzzer = 8; //buzzer to arduino pin 8 for victory music 

// initialize led matrix
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 256// Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 20 // Set BRIGHTNESS to about 1/5 (max = 255)
int ledMAP[40] = {44, 43, 42, 41, 40, 39, 38, 37, 38, 35,
                  61, 66, 93, 98, 125, 130, 157, 162, 189, 194,
                  220, 219, 218, 217, 216, 215, 214, 213, 212, 211,
                  205, 178, 173, 146, 141, 114, 109, 82, 77, 50 };
                  
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

// this function plays song
/* Melody
 * (cleft) 2005 D. Cuartielles for K3
 *
 * This example uses a piezo speaker to play melodies.  It sends
 * a square wave of the appropriate frequency to the piezo, generating
 * the corresponding tone.
 *
 * The calculation of the tones is made following the mathematical
 * operation:
 *
 *       timeHigh = period / 2 = 1 / (2 * toneFrequency)
 *
 * where the different tones are described as in the table:
 *
 * note 	frequency 	period 	timeHigh
 * c 	        261 Hz 	        3830 	1915 	
 * d 	        294 Hz 	        3400 	1700
 * d#			311 Hz			3216	1608
 * e 	        329 Hz 	        3038 	1519 	
 * f 	        349 Hz 	        2864 	1432 	
 * g 	        392 Hz 	        2550 	1275
 * g#			415 Hz			2410    1205
 * a 	        440 Hz 	        2272 	1136
 * a#			466 Hz			2146	1073		
 * b 	        493 Hz 	        2028	1014	
 * C	        523 Hz	        1912 	956
 *
 * http://www.arduino.cc/en/Tutorial/Melody
 */
  

int length = 10; // the number of notes
char notes[] = "fefeca a#a "; // a space represents a rest
int beats[] = { 4, 1, 1, 1, 1, 1, 1, 2, };
int tempo = 300;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'd#', 'e', 'f', 'g', 'g#', 'a', 'a#' };
  int tones[] = { 1915, 1700, 1608, 1519, 1432, 1275, 1205, 1136, 1073 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
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
        if ((currPos) % gameBoardSize != endPos)
          currPos = (currPos + 1) % gameBoardSize;
      /*// if statement prevents piece from moving forward if it is at its end position on the board
      if ((currPos+1) % gameBoardSize != endPos ) // modulus functionality to create circularity for board
        currPos = (currPos + 1) % gameBoardSize;

        if ((currPos+1) % gameBoardSize == endPos)
        currPos = endPos;*/
    }
    
    // this function checks if another piece is in the same position on the board and sends that piece back to home
    void checkOverlaps(Piece* otherPiece) {
      if (currPos == otherPiece->currPos) {
        otherPiece->isHome = true;
        otherPiece->currPos = offBoardPos;
        if (!isHome) {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(otherPiece->colorId);
          lcd.print(" sent home");
          lcd.setCursor(5,1);
          lcd.print(":(");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
        }
      }
    }

    // this function starts the corresponding Player's turn
    void takeTurn(int rollNum) {
        //int moveCounter = 0; 
      // if the piece is home, need to roll a 6 to get on board
      if (isHome) {
        if (rollNum == 6) {
          isHome = false;
          currPos = startPos; // puts piece on the board and colors in 
          if (colorId == "red") {
            pixels.setPixelColor(ledMAP[currPos], pixels.Color(150, 0, 0)); // RED
            Serial.print("current position and led map current position: "); 
            Serial.println(currPos); 
            Serial.println(ledMAP[currPos]); 
            pixels.show(); 
          }
          else if (colorId == "yellow") {
            pixels.setPixelColor(ledMAP[currPos], pixels.Color(150, 150, 0)); // YELLOW
            Serial.print("current position and led map current position: "); 
            Serial.println(currPos); 
            Serial.println(ledMAP[currPos]); 
            pixels.show(); 
          }
          else if (colorId == "green") {
            pixels.setPixelColor(ledMAP[currPos], pixels.Color(0, 150, 0)); // GREEN
            Serial.print("current position and led map current position: "); 
            Serial.println(currPos); 
            Serial.println(ledMAP[currPos]); 
            pixels.show(); 
          }
          else if (colorId == "blue") {
            pixels.setPixelColor(ledMAP[currPos], pixels.Color(0, 0, 150)); // BLUE
            Serial.print("current position and led map current position: "); 
            Serial.println(currPos); 
            Serial.println(ledMAP[currPos]); 
            pixels.show();
          }      
    }
      }
      // if the piece is at end, need to roll a 1 to win
      else if (currPos == endPos) {
        if (rollNum == 1) {
          hasWon = true;
        }
      }
      // if piece is on board, roll
      else {
        if ((currPos + rollNum) == (endPos+1)) {
          hasWon = true;
        }
        pixels.setPixelColor(ledMAP[currPos], pixels.Color(0, 0, 0)); // clear board
        pixels.show();

        for (int i = 0; i < rollNum; i++) {
          moveOne();  
        }   
        delay(200); // delay so person can check  

        if (colorId == "red") {
            pixels.setPixelColor(ledMAP[currPos], pixels.Color(150, 0, 0)); // RED
            Serial.print("current position and led map current position: "); 
            Serial.println(currPos); 
            Serial.println(ledMAP[currPos]); 
            pixels.show(); 
          }
          else if (colorId == "yellow") {
            pixels.setPixelColor(ledMAP[currPos], pixels.Color(150, 150, 0)); // YELLOW
            Serial.print("current position and led map current position: "); 
            Serial.println(currPos); 
            Serial.println(ledMAP[currPos]); 
            pixels.show(); 
          }
          else if (colorId == "green") {
            pixels.setPixelColor(ledMAP[currPos], pixels.Color(0, 150, 0)); // GREEN
            Serial.print("current position and led map current position: "); 
            Serial.println(currPos); 
            Serial.println(ledMAP[currPos]); 
            pixels.show(); 
          }
          else if (colorId == "blue") {
            pixels.setPixelColor(ledMAP[currPos], pixels.Color(0, 0, 150)); // BLUE
            Serial.print("current position and led map current position: "); 
            Serial.println(currPos); 
            Serial.println(ledMAP[currPos]); 
            pixels.show();
          } 

          
      }     
        
          //moveCounter++; 
          //if(rollNum-moveCounter ==1 && currPos == endPos){ 
          //  hasWon= true; 
         //}
    }


};

// set home base function 
void homeBase(){ 
    pixels.setPixelColor(0, pixels.Color(150, 150, 0)); // YELLOw HOME 
    pixels.show(); 
    pixels.setPixelColor(15, pixels.Color(150, 0, 0)); // RED HOME 
    pixels.show();   // Send the updatedpixel colors to the hardware.
    pixels.setPixelColor(240, pixels.Color(0, 0, 150)); // BLUE HOME 
    pixels.show();   // Send the updatedpixel colors to the hardware.
    pixels.setPixelColor(255, pixels.Color(0, 150, 0)); // GREEN  HOME 
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
 pinMode(moveRedButton, INPUT_PULLUP);
 pinMode(moveYellowButton, INPUT_PULLUP);
 pinMode(moveGreenButton, INPUT_PULLUP);
 pinMode(moveBlueButton, INPUT_PULLUP);
 pinMode(speakerPin, OUTPUT);
 //pinMode(buzzer, OUTPUT); // Set buzzer - pin 8 as an output
 pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 pixels.setBrightness(BRIGHTNESS);
 homeBase(); // set home base

  lcd.init(); // initialize lcd
  lcd.clear(); // clear what was last on it
  lcd.backlight();
  lcd.print("welcome to chase!");
  delay(1500);
  lcd.clear(); 
  /*String instruction1 = "press the corner buttons to roll";
  // scroll the message across the screen
  for (int position = 0; position < instruction1.length(); position++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    // Print a substring of the message starting at 'position'
    lcd.print(String(instruction1).substring(position));
    delay(300); // Delay for scrolling effect
  }
  lcd.clear();
  delay(300);
  String instruction2 = "roll a 6 to get on the board";
  // scroll the message across the screen
  for (int position = 0; position < instruction2.length(); position++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    // Print a substring of the message starting at 'position'
    lcd.print(String(instruction2).substring(position));
    delay(300); // Delay for scrolling effect
  }
  lcd.clear();
  delay(600);
  String instruction3 = "if someone lands on you ";
  // scroll the message across the screen
  for (int position = 0; position < instruction3.length(); position++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    // Print a substring of the message starting at 'position'
    lcd.print(String(instruction3).substring(position));
    delay(300); // Delay for scrolling effect
  }
  lcd.clear();
  delay(300);
  String instruction4 = "you get sent home ";
  // scroll the message across the screen
  for (int position = 0; position < instruction4.length(); position++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    // Print a substring of the message starting at 'position'
    lcd.print(String(instruction4).substring(position));
    delay(300); // Delay for scrolling effect
  }*/

  delay(1500);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("red turn");

  Serial.begin(9600);
  
  // initialize random number generator
  randomSeed(analogRead(0));

  // create board with indices
  for (int i = 0; i < gameBoardSize; i++)
    gameBoard[i] = i;

   /* const char message[] = "Hello, world! Scrolling Text! ";
    int messageLength = sizeof(message) - 1;
    // Scroll the message across the screen
    for (int position = 0; position < messageLength; position++) {
      lcd.clear();
      // Print a substring of the message starting at 'position'
      lcd.print(String(message).substring(position));
      delay(300); // Delay for scrolling effect
    
  }*/
}

  // create all 4 pieces and initialize corresponding board values
  Piece* Red = new Piece("red", 0, 39);
  Piece* Yellow = new Piece("yellow", 10, 9);
  Piece* Green = new Piece("green", 20, 19);
  Piece* Blue = new Piece("blue", 30, 29);
  Piece* Players[numPlayers] = {Red, Yellow, Green, Blue};
  int currPlayerInd = 0;
  Piece* currPlayer = Players[currPlayerInd]; // default first player is red

  void loop() {
    diceButtonState = digitalRead(diceButton);
    while (diceButtonState == HIGH) {
      Red->hasWon = true;
      lcd.clear();
      int roll = rollDie(); 
      setDicePad(roll); 

      Serial.print("Player ");
      Serial.print(currPlayer->colorId);
      Serial.print(" rolled: ");
      Serial.println(roll);

      // wait for correct player to their move press button
      if (currPlayer->colorId == "red" && digitalRead(moveRedButton) == LOW) {
        currPlayer->takeTurn(roll); // start the player's turn with their roll
      }
      else if (currPlayer->colorId == "yellow" && digitalRead(moveYellowButton) == LOW) {
       currPlayer->takeTurn(roll); // start the player's turn with their roll
      }
      else if (currPlayer->colorId == "green" && digitalRead(moveGreenButton) == LOW) {
        currPlayer->takeTurn(roll); // start the player's turn with their roll
      }
      else if (currPlayer->colorId == "blue" && digitalRead(moveBlueButton) == LOW) {
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
        lcd.clear();
        lcd.print(currPlayer->colorId);
        lcd.print(" won!");
        Serial.print("Player ");
        Serial.print(currPlayer->colorId);
        Serial.println(" has won!");
        if (currPlayer->colorId == "red")
        {
          pixels.setPixelColor(103, pixels.Color(150, 0, 0)); 
          pixels.setPixelColor(104, pixels.Color(150, 0, 0));
          pixels.setPixelColor(105, pixels.Color(150, 0, 0)); 
          pixels.setPixelColor(135, pixels.Color(150, 0, 0)); 
          pixels.setPixelColor(136, pixels.Color(150, 0, 0));
          pixels.setPixelColor(137, pixels.Color(150, 0, 0)); 
          pixels.setPixelColor(118, pixels.Color(150, 0, 0));
          pixels.setPixelColor(119, pixels.Color(150, 0, 0));
          pixels.setPixelColor(120, pixels.Color(150, 0, 0)); 
          pixels.show();
        }
        if (currPlayer->colorId == "yellow")
        {
          pixels.setPixelColor(103, pixels.Color(150, 150, 0)); 
          pixels.setPixelColor(104, pixels.Color(150, 150, 0));
          pixels.setPixelColor(105, pixels.Color(150, 150, 0)); 
          pixels.setPixelColor(135, pixels.Color(150, 150, 0)); 
          pixels.setPixelColor(136, pixels.Color(150, 150, 0));
          pixels.setPixelColor(137, pixels.Color(150, 150, 0)); 
          pixels.setPixelColor(118, pixels.Color(150, 150, 0));
          pixels.setPixelColor(119, pixels.Color(150, 150, 0));
          pixels.setPixelColor(120, pixels.Color(150, 150, 0)); 
          pixels.show();
        }
        if (currPlayer->colorId == "green")
        {
          pixels.setPixelColor(103, pixels.Color(0, 150, 0)); 
          pixels.setPixelColor(104, pixels.Color(0, 150, 0));
          pixels.setPixelColor(105, pixels.Color(0, 150, 0)); 
          pixels.setPixelColor(135, pixels.Color(0, 150, 0)); 
          pixels.setPixelColor(136, pixels.Color(0, 150, 0));
          pixels.setPixelColor(137, pixels.Color(0, 150, 0)); 
          pixels.setPixelColor(118, pixels.Color(0, 150, 0));
          pixels.setPixelColor(119, pixels.Color(0, 150, 0));
          pixels.setPixelColor(120, pixels.Color(0, 150, 0)); 
          pixels.show();
        }
        if (currPlayer->colorId == "blue")
        {
          pixels.setPixelColor(103, pixels.Color(0, 0, 150)); 
          pixels.setPixelColor(104, pixels.Color(0, 0, 150));
          pixels.setPixelColor(105, pixels.Color(0, 0, 150)); 
          pixels.setPixelColor(135, pixels.Color(0, 0, 150)); 
          pixels.setPixelColor(136, pixels.Color(0, 0, 150));
          pixels.setPixelColor(137, pixels.Color(0, 0, 150)); 
          pixels.setPixelColor(118, pixels.Color(0, 0, 150));
          pixels.setPixelColor(119, pixels.Color(0, 0, 150));
          pixels.setPixelColor(120, pixels.Color(0, 0, 150)); 
          pixels.show();
        }

        // "we are the champions" taken from https://codebender.cc/sketch:79392#We%20are%20the%20champions.ino
        for (int i = 0; i < length; i++) {
          if (notes[i] == ' ') {
            delay(beats[i] * tempo); // rest
          } 
          else {
            playNote(notes[i], beats[i] * tempo);
          } 
          // pause between notes
          delay(tempo / 2); 
        }
        delay(500);
        while (true) {} // ends the game
      }

      // otherwise move to the next player
      currPlayerInd = (currPlayerInd + 1) % numPlayers;
      currPlayer = Players[currPlayerInd];

      lcd.setCursor(3,0);
      lcd.print(currPlayer->colorId);
      lcd.print(" turn");
      delay(300); // debouncer
      }
  }