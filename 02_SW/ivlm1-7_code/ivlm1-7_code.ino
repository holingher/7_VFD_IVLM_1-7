int strobePin  = 10;    // Strobe Pin on the MSGEQ7
int resetPin   = 9;    // Reset Pin on the MSGEQ7
int outPin     = A0;   // Output Pin on the MSGEQ7
int level[7];          // An array to hold the values from the 7 frequency bands

const int tubeCount = 10;
const int segCount = 5;

typedef struct seg_t
{
  int seg_index;
  int port_index;
  char port;
};

typedef struct tube_t
{
  int tube_index;
  int port_index;
  char port;
};

seg_t segmentPins[segCount] = {
//seg_index,port_index,port
  { 1,      PORTD2,   'D'},
  { 2,      PORTD1,   'D'},
  { 3,      PORTD4,   'D'},
  { 4,      PORTD0,   'D'},
  { 5,      PORTD3,   'D'}
};

tube_t tubePins[tubeCount] = {
//tube_index,port_index,port
  { 1,       PORTC1,    'C'},
  { 2,       PORTC2,    'C'},
  { 3,       PORTC3,    'C'},
  { 4,       PORTC4,    'C'},
  { 5,       PORTC5,    'C'},
  { 6,       PORTD5,    'D'},
  { 7,       PORTD6,    'D'},
  { 8,       PORTD7,    'D'},
  { 9,       PORTB0,    'B'},
  { 10,      PORTB3,    'B'}
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
  
  DDRB = DDRB | B11111111;
  DDRC = DDRC | B11111110;
  DDRD = DDRD | B11111111;
}


/* from top to bottom
 * 1 - PORTD2
 * 2 - PORTD1
 * 1 - PORTD4
 * 2 - PORTD0
 * 1 - PORTD3
 */

/* from left to right
 *   1    -    2   -   3    -    4   -    5   -   6    -    7   -    8   -    9   -  10
 * PORTC1 - PORTC2 - PORTC3 - PORTC4 - PORTC5 - PORTD5 - PORTD6 - PORTD7 - PORTB0 - PORTB3
 */
#define ON true
#define OFF false
void portWrite(char select_option, int index, bool state);
int tube_index = 0;

void close_all_tubes()
{
    for(int tube_index = 0; tube_index<tubeCount; tube_index++)
    {
      portWrite('T', tube_index, OFF);
    }
}


void close_all_seg()
{
    for(int seg_index = 0; seg_index<segCount; seg_index++)
    {
      portWrite('S', seg_index, OFF);
    }
}
// Frequency for main loop function
#define READ_DELAY  50  // milliseconds

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
 
  //for (int tube_index = 0; tube_index < 7; tube_index++) 
  {
    //Serial.print       (level[tube_index]);
    //Serial.print       ("   ");

      //close_all_tubes();
      if(level[tube_index]<=50)
      {
        portWrite('T', tube_index, ON);
        portWrite('S', 0, ON);
        delay(100);
        portWrite('S', 0, OFF);
        portWrite('T', tube_index, OFF);
        delay(10);
      }
      else if((level[tube_index]>50) && (level[tube_index]<=100))
      {
        portWrite('T', tube_index, ON);
        portWrite('S', 1, ON);
        delay(100);
        portWrite('S', 1, OFF);
        portWrite('T', tube_index, OFF);
        delay(10);
      }
      else if((level[tube_index]>100) && (level[tube_index]<=150))
      {
        portWrite('T', tube_index, ON);
        portWrite('S', 2, ON);
        delay(100);
        portWrite('S', 2, OFF);
        portWrite('T', tube_index, OFF);
        delay(10);
      }
      else if((level[tube_index]>150) && (level[tube_index]<=200))
      {
        portWrite('T', tube_index, ON);
        portWrite('S', 3, ON);
        delay(100);
        portWrite('S', 3, OFF);
        portWrite('T', tube_index, OFF);
        delay(10);
      }
      else if(level[tube_index]>200)
      {
        portWrite('T', tube_index, ON);
        portWrite('S', 4, ON);
        delay(100);
        portWrite('S', 4, OFF);
        portWrite('T', tube_index, OFF);
        delay(10);
      }
    }
}

void portWrite(char select_option, int index, bool state)
{
  int port_internal;
  char local_string;

  if(select_option == 'S')
  {
    port_internal = segmentPins[index].port_index;
    local_string = segmentPins[index].port;  
  }
  else
  {
    port_internal = tubePins[index].port_index;
    local_string = tubePins[index].port;
  }
  
  if(state == ON)
  {
    if(local_string == 'B') 
    {
      PORTB |=(1<< port_internal);
    }
    else if(local_string == 'C')
    {
      PORTC |=(1<< port_internal);
    }
    else if(local_string == 'D')
    {
      PORTD |=(1<< port_internal);
    }
  }
  else
  {
    if(local_string == 'B') 
    {
      PORTB &=~ (1<<port_internal);
    }
    else if(local_string == 'C')
    {
      PORTC &=~ (1<<port_internal);
    }
    else if(local_string == 'D')
    {
      PORTD &=~ (1<<port_internal);
    }
  }
}
