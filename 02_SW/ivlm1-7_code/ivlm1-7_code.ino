#include <ShiftRegister74HC595.h>

#define DS_PIN 9
#define SH_CP_PIN 8
#define ST_CP_PIN 7
#define NB_OF_SHIFT_REGISTERS 5

// create a global shift register object
// parameters: (number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 sr (NB_OF_SHIFT_REGISTERS, DS_PIN, SH_CP_PIN, ST_CP_PIN); 

int strobePin  = 10;    // Strobe Pin on the MSGEQ7
int resetPin   = 9;    // Reset Pin on the MSGEQ7
int outPin     = A0;   // Output Pin on the MSGEQ7
int level[7];          // An array to hold the values from the 7 frequency bands

const int tubeCount = 8;
const int segCount = 5;


int table[segCount][tubeCount] = {
  {32,33,34,35,36,37,38,39},//top row
  {24,25,26,27,28,29,30,31},
  {16,17,18,19,20,21,22,23},
  {8, 9, 10,11,12,13,14,15},
  {0, 1, 2, 3, 4, 5, 6, 7 }//bottom row
  };



void setup() 
{
  // Define our pin modes
  pinMode      (strobePin, OUTPUT);
  pinMode      (resetPin,  OUTPUT);
  pinMode      (outPin,    INPUT);
 
  // Create an initial state for our pins
  digitalWrite (resetPin,  LOW);
  digitalWrite (strobePin, LOW);
  delay        (1);
 
  // Reset the MSGEQ7 as per the datasheet timing diagram
  digitalWrite (resetPin,  HIGH);
  delay        (1);
  digitalWrite (resetPin,  LOW);
  digitalWrite (strobePin, HIGH);
  delay        (1);
  
  //DDRB = DDRB | B11111111;
  //DDRC = DDRC | B11111110;
  //DDRD = DDRD | B11111111;
}



void loop() 
{
  int tube_index = 6;
  
  // Cycle through each frequency band by pulsing the strobe.
  for (int i = 0; i < 7; i++) 
  {
    digitalWrite       (strobePin, LOW);
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram
    //map(value, fromLow, fromHigh, toLow, toHigh)
    level[i] = /*map(*/analogRead (outPin)/*, 0, 255, 0, 5)*/;
    digitalWrite       (strobePin, HIGH);
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram  
  }

  
  sr.setAllLow(); // set all pins LOW
  delay(10);
  
  if(level[tube_index]<=50)
  {
    //sr.set(i, HIGH); // set single pin HIGH
  }
  else if((level[tube_index]>50) && (level[tube_index]<=100))
  {

  }
  else if((level[tube_index]>100) && (level[tube_index]<=150))
  {

  }
  else if((level[tube_index]>150) && (level[tube_index]<=200))
  {

  }
  else if(level[tube_index]>200)
  {
   
  }

  // setting single pins
  //for (int i = 0; i < 8; i++) {
    
  //   sr.set(i, HIGH); // set single pin HIGH
  //   delay(250); 
  //}
  // set all pins at once
  uint8_t pinValues[] = { B00111100,
                          B01000010,
                          B10011001,
                          B01000010,
                          B00111100};
  sr.setAll(pinValues); 
  delay(100);

  sr.setAllLow(); // set all pins LOW
  delay(10);

  // set pins without immediate update
  // sr.setNoUpdate(0, HIGH);
  // sr.setNoUpdate(1, LOW);
  // at this point of time, pin 0 and 1 did not change yet
  sr.updateRegisters(); // update the pins to the set values
}

