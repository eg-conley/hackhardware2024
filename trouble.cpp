#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()

using namespace std;

struct player {
    string color;
    int start;
    int end;
    int currPosition = 0;
};

int rollDie() {
    srand(time(0)); // seed random number with current time
    int randomNum =  (rand() % 6) + 1; // random # 1-6
    cout << "Random number: " << randomNum << endl;
    return randomNum;
}

int main() {
    // initialize the 4 players corresponding to colors at position 0
    player p1 = {"red"};
    player p2 = {"yellow"};
    player p3 = {"green"};
    player p4 = {"blue"};

    cout << "First player rolls." << endl;

    // determine first player`
    while (rollDie() != 6) {
        cout << "Next player rolls." << endl;
        // keep rolling


    }
    rollDie();





    return 0;
}



class Player
{
    protected: 
        string color;
        int currPos;
        bool home;
        int start;
        int end;
    public:
        

}


class Student
{
   public:
      Student(char division, int roll_no);
      void add_science_marks(int marks);
      int get_roll_no();
   private:
      char _division;
      int _roll_no;
      int _science_marks;
};

Student::Student(char division, int roll_no){
   _division = division;
   _roll_no = roll_no;
}

void Student::add_science_marks(int marks){
   _science_marks = marks;
}

int Student::get_roll_no(){
   return _roll_no;
}

void setup() {
   // put your setup code here, to run once:
   Serial.begin(9600);
   Serial.println();
   Student Yash('A',26);

   Serial.print("Roll number of the student is: ");
   Serial.println(Yash.get_roll_no());
}

void loop() {
   // put your main code here, to run repeatedly:
}