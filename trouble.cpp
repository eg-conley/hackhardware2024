#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <string>
#include <vector>

using namespace std;

// this function generates and returns a random integer (1-6), as well as displays it on the LED matrix
int rollDie() {
    srand(time(0)); // seed random number with current time
    int randomNum =  (rand() % 6) + 1; // random # 1-6
    // in Arduino code, would change LED matrix here

    return randomNum;
}

// Player class to initiate the 4 players by colorId
class Player
{
    public: 
        string colorId;
        int currPos;
        int startPos;
        int endPos;
        bool isHome;
        bool hasWon;

        // construtor iniitalizes Player
        Player(string colorId, int startPos, int endPos) : colorId(colorId), startPos(startPos), endPos(endPos), isHome(true), hasWon(false) {}

        void moveOne() {
            currPos++;
            // in Arduino code, would change LED matrix here
        }

        void checkOverlaps(Player* otherPlayer) {
            if (currPos == otherPlayer->currPos);
                otherPlayer->isHome = true; // don't need to update the currPos beecause it will be handled in other loops
        }

        void takeTurn() { // in Arduino code, this would be initiated by the respective player's button push
            int moveCount;

            // if the Player is stil at home
            if (currPos == isHome) {
                moveCount = rollDie();
                // depending on die roll...
                if (moveCount != 6) {
                    // next Player's turn
                }
                else {
                    currPos = startPos;
                    moveCount = rollDie();
                    int actualMoves;
                    while (actualMoves < moveCount) {
                        moveOne();
                        actualMoves++;
                    }
                    // next Player's turn
                }
            }
            // if the Player is at the endPos of their cycle
            else if (currPos == endPos) {
                moveCount = rollDie();
                if (moveCount != 1) {
                    // next Player's turn
                }
                else {
                    hasWon = true;
                    // in Arduino, play music with buzzer
                }
            }
            // if the Player is at startPos or on the board
            else {
                moveCount = rollDie();
                int actualMoves;
                while (actualMoves < moveCount) { // in Arduino, this would be reliant on if the button is pushed
                        moveOne();
                        actualMoves++;
                }

                    // next Player's turn
            }  
        }
};

// this initiates the 4 players and keeps track of Player turns
void gameSetup() {
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
   vector <Player*> turns;
   turns.push_back(Red);
   turns.push_back(Yellow);
   turns.push_back(Green);
   turns.push_back(Blue);

   // determines current Player
   Player* currPlayerTurn = turns[0]; // in Arudino, this would be decided based on who rolls the 6 first
   while (currPlayerTurn->hasWon == false) {
     currPlayerTurn.takeTurn()
   }
}

int main() {
   return 0;
}