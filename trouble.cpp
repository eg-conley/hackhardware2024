#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <string>
#include <vector>

using namespace std;

// this function generates and returns a random integer (1-6), as well as displays it on the LED matrix
int rollDie() {
    int randomNum = (rand() % 6) + 1; // random # 1-6
    // in Arduino code, would change LED matrix here to display the die number

    return randomNum;
}

// for each of the player pieces
class Player {
    public: 
        string colorId;
        int currPos;
        int startPos;
        int endPos;
        bool isHome;
        bool hasWon;

        // construtor iniitalizes Player
        Player(string colorId, int startPos, int endPos) : colorId(colorId), currPos(50), startPos(startPos), endPos(endPos), isHome(true), hasWon(false) {}

        void moveOne() {
            // move Player piece to next position, looping back to 0 when necessary
            // if Player is at their end position, make sure they don't loop around again
            if ((currPos + 1) % 40 != endPos)
               currPos = (currPos + 1) % 40;
            // in Arduino code, would change LED matrix here
        }

        void checkOverlaps(Player* otherPlayer) {
            if (currPos == otherPlayer->currPos) {
               otherPlayer->isHome = true;
               otherPlayer->currPos = 50; // reset their position to a value that is not on board
               cout << otherPlayer->colorId << " was sent home :( " << endl;
            }
        }

        void takeTurn() { // in Arduino code, this would be initiated by the respective player's button push
            cout << "Player " << colorId << " turn"<< endl;

            int moveCount;

            // if the Player is stil at home
            if (isHome) {
               moveCount = rollDie();
               cout << "Player still home, need 6 to start. Roll count: " << moveCount << endl;
               // depending on die roll...
               if (moveCount != 6) {
                    // display instructions on LCD
                }
                else {
                  isHome = false;
                  currPos = startPos;
                  moveCount = rollDie();
                  cout << "Player rolled a 6. Roll again for spaces: " << moveCount << endl;
                  int actualMoves = 0;
                  while (actualMoves < moveCount) {
                     moveOne();
                     actualMoves++;
                  }
                }
                cout << "Player is currently at position: " << currPos << endl;
            }
            // if the Player is at the endPos of their cycle
            else if (currPos == endPos-1) {
               moveCount = rollDie();
               cout << "Player at end, need 1 to win. Roll count: " << moveCount << endl;
               if (moveCount != 1) { // need to roll a one to win
                  // display instructions on LCD
               }
               else {
                  hasWon = true;
               }
               cout << "Player is currently at position: " << currPos << endl;
            }
            // if the Player is at startPos or on the board
            else {
                moveCount = rollDie();
               cout << "Player on board. Roll count: " << moveCount << endl;
                int actualMoves = 0;
                while (actualMoves < moveCount) { // in Arduino, this would be reliant on if the button is pushed
                        moveOne();
                        actualMoves++;
                }
                cout << "Player is currently at position: " << currPos << endl;
            }  
            cout << endl;
        }
};

// this initiates the 4 players and keeps track of Player turns
void gameSetup() {
   srand(time(0)); // seed random number with current time

   // game board represented by array of 40 numbers (index 0-39)
   vector<int> gameBoard[40];
   for (int i = 0; i < 40; i++)
      gameBoard[i].push_back(i);

   // create all four players and assign corresponding board values for startPos and endPos
   Player* Red = new Player("red", 1, 40);
   Player* Yellow = new Player("yellow", 11, 10);
   Player* Green = new Player("green", 21, 20);
   Player* Blue = new Player("blue", 31, 30);

   // creates a vector of Player objects in order to track whose turn it is
   vector<Player*> turns = {Red, Yellow, Green, Blue};
   int currIndex = 0;
   Player* currPlayerTurn = turns[currIndex]; // default first player is red

   // game loop until a player wins
   int turnCount = 0; // Optional: safeguard
   const int maxTurns = 10; // Prevent infinite loop for debugging

   while (true) {
      currPlayerTurn->takeTurn();

      // check all other player pieces and if they are in the same spot
      int checkIndex = (currIndex + 1) % turns.size();
      //cout << "check Index: " << checkIndex << " current Index: " << currIndex << endl;
      while (currIndex != checkIndex)
      {
         currPlayerTurn->checkOverlaps(turns[checkIndex]);
         checkIndex = (checkIndex + 1) % turns.size(); // increment the checkIndex
         //cout << "next index to check in while loop: " << checkIndex << endl;
      }

      // check win condition
      if (currPlayerTurn->hasWon) {
         break;
      }
      // move to the next Player's turn, looping back to first
      currIndex = (currIndex + 1) % turns.size();
      currPlayerTurn = turns[currIndex];
    
      /*if (++turnCount > maxTurns) { // Safeguard
        cout << "Error: Infinite loop detected. Exiting game." << endl;
        break;
      }*/
    }

    // game winning sequence
      // iterate through to check who won
      for (int i = 0; i < turns.size(); i++) {
         if (turns[i]->hasWon == true)
            cout << "Player " << turns[i]->colorId << " has won!" << endl;
      }

}

int main() { // this is the setup code in Arudino that would run once
   gameSetup();

   return 0;
}