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

// Player class to initiate the 4 players by color
class Player
{
    public: 
        string color;
        int currPos;
        int start;
        int end;
        bool isHome;
        bool hasWon;

        // construtor iniitalizes home and win varibles
        Player() {
            isHome = true;
            hasWon = false;
        }
         
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
                    currPos = start;
                    moveCount = rollDie();
                    int actualMoves;
                    while (actualMoves < moveCount) {
                        moveOne();
                        actualMoves++;
                    }
                    // next Player's turn
                }
            }
            // if the Player is at the end of their cycle
            else if (currPos == end) {
                moveCount = rollDie();
                if (moveCount != 1) {
                    // next Player's turn
                }
                else {
                    hasWon = true;
                    // in Arduino, play music with buzzer
                }
            }
            // if the Player is at start or on the board
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
    // create all four players and assign corresponding board values for start and end
    Player* Red = new Player();
    Red->color = "red";
    Red->start = 1; // 1st spot on board going clockwise
    Red->end = 40; // last postition on board going clockwise;

    Player* Yellow = new Player();
    Yellow->color = "yellow";
    Yellow->start = 11;
    Yellow->end = 10;

    Player* Green = new Player();
    Green->color = "green";
    Green->start = 21;
    Green->end = 20;

    Player* Blue = new Player();
    Blue->color = "blue";
    Blue->start = 31;
    Blue->end = 30;

    // creates a vector of Player objects in order to track whose turn it us
    vector <Player*> turns;
    turns.push_back(Red);
    turns.push_back(Yellow);
    turns.push_back(Green);
    turns.push_back(Blue);

    // determines current Player
    Player* currPlayer = turns[0];
    while (currPlayer->hasWon == false);


    
}

int main() {
    // initialize the 4 players corresponding to colors at position 0
    Player Red;
    Player Yellow;
    Player Green;
    Player Blue;

    cout << "First player rolls." << endl;

    rollDie();

    return 0;
}