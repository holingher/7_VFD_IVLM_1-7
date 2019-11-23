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

const int segCount = 5;

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
 
  DDRD = DDRD | B00011111;
}

byte next_cycle_seg6 = 0;
char display_type = 'a';
void loop() 
{
  byte lvl[7]={0,0,0,0,0,0,0};
  byte row_value[4];
  int level[7];          // An array to hold the values from the 7 frequency bands
  byte level_to_seg[7];          // An array to hold the values from the 7 frequency bands

bool value_met_0;
bool value_met_1;
bool value_met_2;
bool value_met_3;
bool value_met_4;
bool value_met_5;
bool value_met_6;
bool value_met_7;

  // Cycle through each frequency band by pulsing the strobe.
  for (int i = 0; i < 7; i++) 
  {
    digitalWrite       (strobePin, LOW);
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram
    level[i] = analogRead(outPin);
    digitalWrite       (strobePin, HIGH);
    delayMicroseconds  (100);                    // Delay necessary due to timing diagram  
  }

  level_to_seg[0] = level[0]/200;
  level_to_seg[1] = level[1]/200;
  level_to_seg[2] = level[2]/200;
  level_to_seg[3] = level[3]/200;
  level_to_seg[4] = level[4]/200;
  level_to_seg[5] = level[5]/200;
  level_to_seg[6] = level[6]/200;
  level_to_seg[7]=(level_to_seg[6] + next_cycle_seg6)/2;
  next_cycle_seg6 = level_to_seg[6];

value_met_0 = false;
value_met_1 = false;
value_met_2 = false;
value_met_3 = false;
value_met_4 = false;
value_met_5 = false;
value_met_6 = false;
value_met_7 = false;
  for(int index_row = 0; index_row<segCount; index_row++)
  {
    if(display_type == 'a')
    {
      if(level_to_seg[0] == index_row){lvl[0] = 1;} else {lvl[0] = 0;}
      if(level_to_seg[1] == index_row){lvl[1] = 1;} else {lvl[1] = 0;}
      if(level_to_seg[2] == index_row){lvl[2] = 1;} else {lvl[2] = 0;}
      if(level_to_seg[3] == index_row){lvl[3] = 1;} else {lvl[3] = 0;}
      if(level_to_seg[4] == index_row){lvl[4] = 1;} else {lvl[4] = 0;}
      if(level_to_seg[5] == index_row){lvl[5] = 1;} else {lvl[5] = 0;}
      if(level_to_seg[6] == index_row){lvl[6] = 1;} else {lvl[6] = 0;}
      if(level_to_seg[7] == index_row){lvl[7] = 1;} else {lvl[7] = 0;}
    }
    else if(display_type == 'b')
    {
      if((level_to_seg[0] == index_row) && (!value_met_0)){lvl[0] = 1;value_met_0 = true;} 
      else if((level_to_seg[0] != index_row) && (!value_met_0)){lvl[0] = 1;}
      else if((level_to_seg[0] != index_row) && (value_met_0)){lvl[0] = 0;}
      
      if((level_to_seg[1] == index_row) && (!value_met_1)){lvl[1] = 1;value_met_1 = true;} 
      else if((level_to_seg[1] != index_row) && (!value_met_1)){lvl[1] = 1;}
      else if((level_to_seg[1] != index_row) && (value_met_1)){lvl[1] = 0;}
      
      if((level_to_seg[2] == index_row) && (!value_met_2)){lvl[2] = 1;value_met_2 = true;} 
      else if((level_to_seg[2] != index_row) && (!value_met_2)){lvl[2] = 1;}
      else if((level_to_seg[2] != index_row) && (value_met_2)){lvl[2] = 0;}
      
      if((level_to_seg[3] == index_row) && (!value_met_3)){lvl[3] = 1;value_met_3 = true;} 
      else if((level_to_seg[3] != index_row) && (!value_met_3)){lvl[3] = 1;}
      else if((level_to_seg[3] != index_row) && (value_met_3)){lvl[3] = 0;}
      
      if((level_to_seg[4] == index_row) && (!value_met_4)){lvl[4] = 1;value_met_4 = true;} 
      else if((level_to_seg[4] != index_row) && (!value_met_4)){lvl[4] = 1;}
      else if((level_to_seg[4] != index_row) && (value_met_4)){lvl[4] = 0;}
      
      if((level_to_seg[5] == index_row) && (!value_met_5)){lvl[5] = 1;value_met_5 = true;} 
      else if((level_to_seg[5] != index_row) && (!value_met_5)){lvl[5] = 1;}
      else if((level_to_seg[5] != index_row) && (value_met_5)){lvl[5] = 0;}
      
      if((level_to_seg[6] == index_row) && (!value_met_6)){lvl[6] = 1;value_met_6 = true;} 
      else if((level_to_seg[6] != index_row) && (!value_met_6)){lvl[6] = 1;}
      else if((level_to_seg[6] != index_row) && (value_met_6)){lvl[6] = 0;}
      
      if((level_to_seg[7] == index_row) && (!value_met_7)){lvl[7] = 1;value_met_7 = true;} 
      else if((level_to_seg[7] != index_row) && (!value_met_7)){lvl[7] = 1;}
      else if((level_to_seg[7] != index_row) && (value_met_7)){lvl[7] = 0;}
    }
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
  delay(100);

    // set pins without immediate update
  sr.setNoUpdate(DS_PIN, HIGH);
  sr.setNoUpdate(SH_CP_PIN, LOW);
}
