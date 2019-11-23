#include <ShiftRegister74HC595.h>

#define DS_PIN 4
#define SH_CP_PIN 2
#define ST_CP_PIN 3
#define NB_OF_SHIFT_REGISTERS 5

// create a global shift register object
// parameters: (number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 sr (NB_OF_SHIFT_REGISTERS, DS_PIN, SH_CP_PIN, ST_CP_PIN); 

int strobePin  = 10;    //PB2 Strobe Pin on the MSGEQ7
int resetPin   = 9;    //PB3 Reset Pin on the MSGEQ7
int outPin     = A0;   // Output Pin on the MSGEQ7

const int tubeCount = 8;
const int segCount = 5;

int table[segCount][tubeCount] = {
  {0, 1, 2, 3, 4, 5, 6, 7 },//bottom row
  {8, 9, 10,11,12,13,14,15},
  {16,17,18,19,20,21,22,23},
  {24,25,26,27,28,29,30,31},
  {32,33,34,35,36,37,38,39}//top row
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
  DDRD = DDRD | B11111111;
}
byte next_cycle_seg6 = 0;
void loop() 
{
  int temp_level_7 = 0;
  byte lvl[7]={0,0,0,0,0,0,0};
  byte row_value[4];
  byte level[7];          // An array to hold the values from the 7 frequency bands
  byte level_to_seg[7];          // An array to hold the values from the 7 frequency bands
  int currentSegmentLevel = 0;
  // Cycle through each frequency band by pulsing the strobe.
  for (int i = 0; i < 7; i++) 
  {
    //digitalWrite       (strobePin, LOW);
    PORTB = PORTB & 0b11111011;
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram
    //map(value, fromLow, fromHigh, toLow, toHigh)
    level[i] = analogRead(outPin);
    //digitalWrite       (strobePin, HIGH);
    PORTB = PORTB | 0b00000100;
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram  
  }

  for(int index_tube = 0; index_tube<8; index_tube++)
  {
    level_to_seg[index_tube] = level[index_tube]/50;
  }
  
  level_to_seg[7]=(level_to_seg[6] + next_cycle_seg6)/2;
  next_cycle_seg6 = level_to_seg[6];
  for(int index_row = 0; index_row<5; index_row++)
  {
    if(level_to_seg[0] == index_row){lvl[0] = 1;} else {lvl[0] = 0;}
    if(level_to_seg[1] == index_row){lvl[1] = 1;} else {lvl[1] = 0;}
    if(level_to_seg[2] == index_row){lvl[2] = 1;} else {lvl[2] = 0;}
    if(level_to_seg[3] == index_row){lvl[3] = 1;} else {lvl[3] = 0;}
    if(level_to_seg[4] == index_row){lvl[4] = 1;} else {lvl[4] = 0;}
    if(level_to_seg[5] == index_row){lvl[5] = 1;} else {lvl[5] = 0;}
    if(level_to_seg[6] == index_row){lvl[6] = 1;} else {lvl[6] = 0;}
    if(level_to_seg[7] == index_row){lvl[7] = 1;} else {lvl[7] = 0;}

    row_value[index_row]=(byte)(lvl[0] |
            (lvl[1] << 1) | 
            (lvl[2] << 2) | 
            (lvl[3] << 3) | 
            (lvl[4] << 4) |
            (lvl[5] << 5) | 
            (lvl[6] << 6) | 
            (lvl[7] << 7));
  }

  // set all pins at once
  uint8_t pinValues[] = { row_value[0],
                          row_value[1],
                          row_value[2],
                          row_value[3],
                          row_value[4],}; 
  sr.setAll(pinValues); 
  delay(70);
}

/*
byte foo(int index_row)
{
  byte lvl[7]={0,0,0,0,0,0,0};
  for(int index = 0;index<8;index++)
  {
    if(local_level_to_seg[index] == index_row){lvl[index] = 1;}
    else{lvl[index] = 0;}
  }

  return (byte)(lvl[0] |
            (lvl[1] << 1) | 
            (lvl[2] << 2) | 
            (lvl[3] << 3) | 
            (lvl[4] << 4) |
            (lvl[5] << 5) | 
            (lvl[6] << 6) | 
            (lvl[7] << 7));
}*/
