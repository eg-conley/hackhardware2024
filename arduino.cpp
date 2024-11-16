// libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

/// initialize LCD
LiquidCrystal_I2C lcd(0x20,16,2); // set the LCD address to 0x20 for a 16 chars and 2 line display

#define PIN        2
#define NUMPIXELS 256// Popular NeoPixel ring size

// button pins
const int moveRedButton;
const int moveYellowButton;
const int moveGreenButton;
const int moveBlueButton;
const int diceRoll = 10 ; 

//button state 
int button_state; 

// LED matrix pins

// other variables
const int numPlayers = 4;
const int gameBoardSize = 40; // can change the how many spaces on board

// functions and Piece class

// this function generates a random integer between 1 and 6
int rollDie() {
  return random(1,7);
}

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

// this class represents the Players' board piece
class Piece {
  public:
    // ATTRIBUTES
    String colorID;
    int currPos;
    int startPos;
    int endPos;
    bool isHome;
    bool hasWon;

    // METHODS
    // constructor
    Player(String color, int start, int end) : colorId(color), currPos(50), startPos(start), endPos(end), isHome(true), hasWon(false) {}

    // this function moves the piece forward one space on the board
    void moveOne() {
      // if statement prevents piece from moving forward if it is at its end position on the board
      if (currPos+1) % gameBoardSize != endPos) // modulus functionality to create circularity for board
        currPos = (currPos + 1) % gameBoardSize;
    }

    // this function checks if another piece is in the same position on the board and sends that piece back to home
    void checkOverlaps(Piece* ) {
      if (currPos == other)


    }






};




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // serial number for debugging
  randomSeed(analogRead(0)); // Seed the random number generator with noise from an analog pin

  // button code
  pinMode(moveRed, INPUT);
  pinMode(moveYellow, INPUT);
  pinMode(moveGreen, INPUT);
  pinMode(moveBlue, INPUT);
  pinMode(diceRoll, INPUT);

  // LCD code
  lcd.init(); // initialize lcd
  lcd.clear(); // clear what was last on it
  lcd.backlight(); // turn on backlight

  // loop to scroll the message across the LCD
  /*String welcome = "Welcome to Trouble";
  for (int i = 0; i < welcome.length(); i++) {
    lcd.clear();
    lcd.setCursor(0, 0); // start from beginning
    lcd.print(welcome.substring(i)); // display substring
    delay(300); // adjust speed of scroll
  }
  String instruction1 = "First person to roll a 6 goes first";
  for (int i = 0; i < instruction1.length(); i++) {
    lcd.clear();
    lcd.setCursor(0, 0); // start from beginning
    lcd.print(instruction1.substring(i)); // display substring
    delay(300); // adjust speed of scroll
  }*/
  lcd.clear();
  lcd.print("Roll the die!");
  pixels.begin(); 
  //SET HOME BASES 
  homeBase(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  button_state= digitalRead(diceRoll); 
  Serial.println("Press the button to roll the die");
  while (digitalRead(diceRoll) == LOW) {
    // does nothing until button is HIGH
  }

  delay(50);

  if (randomNumber == 6) {
    Serial.println("Color starts.");
    while (true) {
      // End the game or freeze the program for demonstration purposes
    }
  } else {
    Serial.println("Next player rolls.");
  }

  // Wait for button release
  while (button_state == HIGH) {
    int randomNumber = rollDie(); 
    lcd.clear();    
    lcd.print("ROLLED A ");  // Print the string
    lcd.print(randomNumber); // Print the value of randomNumber
    setDicePad(randomNumber); 
  }

  delay(50); // Debounce delay to prevent false triggers
}





