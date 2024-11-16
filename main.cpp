

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// LED MATRIX 
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 256// Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

//DICE BUTTON 
const int dice_button = 10 ;
int dice_button_state;


//BUZZER 
const int buzzer = 8; //buzzer to arduino pin 8 for victory music 

//roll die
int rollDie() {
  return random(1,7);
}

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

 pinMode(dice_button,INPUT);

 pinMode(buzzer, OUTPUT); // Set buzzer - pin 8 as an output
 pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 homeBase(); // set home base 
}


void loop() {
    dice_button_state= digitalRead(dice_button); 
    while (dice_button_state == HIGH) {
        int randomNumber = rollDie(); 
        setDicePad(randomNumber); 
    }
}
