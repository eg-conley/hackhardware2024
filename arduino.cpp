// libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/// initialize LCD
LiquidCrystal_I2C lcd(0x20,16,2); // set the LCD address to 0x20 for a 16 chars and 2 line display

// button pins
const int moveRedButton;
const int moveYellowButton;
const int moveGreenButton;
const int moveBlueButton;
const int rollButton;

// LED matrix pins

// other variables
const int numPlayers = 4;
const int gameBoardSize = 40; // can change the how many spaces on board

// functions and Piece class

// this function generates a random integer between 1 and 6
int rollDie() {
  return random(1,7);
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
  
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Press the button to roll the die");
  while (digitalRead(diceRoll) == LOW) {
    // does nothing until button is HIGH
  }

  delay(50);

   int randomNumber = random(1, 7); // Generate random integer between 1 and 6
  Serial.print("You rolled: ");
  Serial.println(randomNumber);

  if (randomNumber == 6) {
    Serial.println("Color starts.");
    while (true) {
      // End the game or freeze the program for demonstration purposes
    }
  } else {
    Serial.println("Next player rolls.");
  }

  // Wait for button release
  while (digitalRead(diceRoll) == HIGH) {
    // Does nothing until button is released (HIGH state)
  }

  delay(50); // Debounce delay to prevent false triggers
}





