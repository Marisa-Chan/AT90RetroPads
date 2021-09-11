#include "Joystick2.h"

const int SELECT = 2;

bool sixButtonMode[2] = { false, false };
uint16_t currentState[4] = { 0, 0, 0, 0 };
uint16_t lastState[4] = { 0, 0, 0, 0 };


const int NES_LATCH = 13;
const int NES_CLOCK = 14;
const int NES_DATA1 = 15;
const int NES_DATA2 = 16;

//#define SERIALDEBUG


enum 
{
  BTN_ON    = 1 << 0,
  BTN_UP    = 1 << 1,
  BTN_DOWN  = 1 << 2,
  BTN_LEFT  = 1 << 3,
  BTN_RIGHT = 1 << 4,
  BTN_A     = 1 << 5,
  BTN_B     = 1 << 6,
  BTN_C     = 1 << 7, 
  BTN_X     = 1 << 8,
  BTN_Y     = 1 << 9,
  BTN_Z     = 1 << 10,
  BTN_START = 1 << 11,
  BTN_MODE  = 1 << 12,

  BTN_MASK = (BTN_A | BTN_B | BTN_C | BTN_X | BTN_Y | BTN_Z | BTN_START | BTN_MODE),

  BTN_NES_A      = 1 << 5,
  BTN_NES_B      = 1 << 6,
  BTN_NES_SELECT = 1 << 7,
  BTN_NES_START  = 1 << 8,
  
  BTN_NES_MASK = (BTN_NES_A | BTN_NES_B | BTN_NES_SELECT | BTN_NES_START),
};


const uint16_t masks[4] = {BTN_MASK, BTN_MASK, BTN_NES_MASK, BTN_NES_MASK};

typedef struct
{
  int player;
  int pin;
  uint16_t lowFlag;
  uint16_t highFlag;
  uint16_t pulse3Flag;
} input;
  
// Controller DB9 Pin to Button Flag Mappings
// First column is the controller index, second column
// is the Arduino pin that the controller's DB9 pin is
// attached to
input inputMap[] = {
  { 0,  3,  BTN_UP,        BTN_UP,     BTN_Z    }, // P0 DB9 Pin 1
  { 0,  5,  BTN_DOWN,      BTN_DOWN,   BTN_Y    }, // P0 DB9 Pin 2
  { 0,  6,  BTN_ON,        BTN_LEFT,   BTN_X    }, // P0 DB9 Pin 3
  { 0,  7,  BTN_ON,        BTN_RIGHT,  BTN_MODE }, // P0 DB9 Pin 4
  { 0,  4,  BTN_A,         BTN_B,      0    }, // P0 DB9 Pin 6
  { 0,  8,  BTN_START,     BTN_C,      0    }, // P0 DB9 Pin 9
  { 1,  17,  BTN_UP,        BTN_UP,     BTN_Z    }, // P1 DB9 Pin 1
  { 1,  19,  BTN_DOWN,      BTN_DOWN,   BTN_Y    }, // P1 DB9 Pin 2
  { 1,  20,  BTN_ON,        BTN_LEFT,   BTN_X    }, // P1 DB9 Pin 3
  { 1,  0,   BTN_ON,        BTN_RIGHT,  BTN_MODE }, // P1 DB9 Pin 4
  { 1,  18,  BTN_A,         BTN_B,      0    }, // P1 DB9 Pin 6
  { 1,  1,   BTN_START,     BTN_C,      0    }  // P1 DB9 Pin 9
};
  



void setup() {
  // put your setup code here, to run once:

  

  for (input &inpt :  inputMap )
  {
    pinMode(inpt.pin, INPUT);
    digitalWrite(inpt.pin, HIGH);
  }
   
  pinMode(SELECT, OUTPUT);
  digitalWrite(SELECT, HIGH);



  pinMode(NES_DATA1, INPUT_PULLUP);
  pinMode(NES_DATA2, INPUT_PULLUP);
   
  pinMode(NES_LATCH, OUTPUT);
  pinMode(NES_CLOCK, OUTPUT);
  digitalWrite(NES_LATCH, LOW);
  digitalWrite(NES_CLOCK, HIGH);

  //Sega
  Joystick[0].sendState(0, 0, 0);
  Joystick[1].sendState(0, 0, 0);
  //Nes
  Joystick[2].sendState(0, 0, 0);
  Joystick[3].sendState(0, 0, 0);

  #ifdef SERIALDEBUG
  Serial.begin(9600);
  Serial.println("Joystick!");
  #endif

}

void loop() {
  currentState[0] = 0;
  currentState[1] = 0;
  currentState[2] = 0;
  currentState[3] = 0;
  
  read6buttons();
  ReadNes();
  
  sendStates();

  delayMicroseconds(1950);
}



void LatchPulse()
{
  digitalWrite(NES_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(NES_LATCH, LOW);
}

void ClockPulse()
{
  digitalWrite(NES_CLOCK, LOW);
  digitalWrite(NES_CLOCK, HIGH);
}

void ReadNes()
{
  LatchPulse();
  
  if ( digitalRead(NES_DATA1) == LOW )
    currentState[ 2 ] |= BTN_NES_A;
    
  if ( digitalRead(NES_DATA2) == LOW )
    currentState[ 3 ] |= BTN_NES_A;
    
  ClockPulse();

  if ( digitalRead(NES_DATA1) == LOW )
    currentState[ 2 ] |= BTN_NES_B;
    
  if ( digitalRead(NES_DATA2) == LOW )
    currentState[ 3 ] |= BTN_NES_B;

  ClockPulse();

  if ( digitalRead(NES_DATA1) == LOW )
    currentState[ 2 ] |= BTN_NES_SELECT;
    
  if ( digitalRead(NES_DATA2) == LOW )
    currentState[ 3 ] |= BTN_NES_SELECT;

  ClockPulse();

  if ( digitalRead(NES_DATA1) == LOW )
    currentState[ 2 ] |= BTN_NES_START;
    
  if ( digitalRead(NES_DATA2) == LOW )
    currentState[ 3 ] |= BTN_NES_START;

  ClockPulse();

  if ( digitalRead(NES_DATA1) == LOW )
    currentState[ 2 ] |= BTN_UP;
    
  if ( digitalRead(NES_DATA2) == LOW )
    currentState[ 3 ] |= BTN_UP;

  ClockPulse();

  if ( digitalRead(NES_DATA1) == LOW )
    currentState[ 2 ] |= BTN_DOWN;
    
  if ( digitalRead(NES_DATA2) == LOW )
    currentState[ 3 ] |= BTN_DOWN;

  ClockPulse();

  if ( digitalRead(NES_DATA1) == LOW )
    currentState[ 2 ] |= BTN_LEFT;
    
  if ( digitalRead(NES_DATA2) == LOW )
    currentState[ 3 ] |= BTN_LEFT;

  ClockPulse();

  if ( digitalRead(NES_DATA1) == LOW )
    currentState[ 2 ] |= BTN_RIGHT;
    
  if ( digitalRead(NES_DATA2) == LOW )
    currentState[ 3 ] |= BTN_RIGHT;
}





void read3buttons()
{
  // Set SELECT LOW and read lowFlag
  digitalWrite(SELECT, LOW);
      
  delayMicroseconds(20);
  
  for (input &inpt : inputMap )
  {
    if (digitalRead(inpt.pin) == LOW)
    {
      currentState[ inpt.player ] |= inpt.lowFlag;
    }
  }
  
  // Set SELECT HIGH and read highFlag
  digitalWrite(SELECT, HIGH);
      
  delayMicroseconds(20);
      
  for (input &inpt : inputMap )
  {
    if (digitalRead(inpt.pin) == LOW)
    {
      currentState[ inpt.player ] |= inpt.highFlag;
    }
  }
}

void read6buttons()
{
  // Poll for three-button states twice
  read3buttons();
  read3buttons();
   
  // After two three-button polls, pulse the SELECT line
  // so the six-button reports the higher button states
  digitalWrite(SELECT, LOW);

  // When a six-button first connects, it'll spam UP and DOWN,
  // which signals the game to switch to 6-button polling
  for (int i = 0; i < 2; i++)
  {
    if ((currentState[i] & BTN_ON) != 0 )
    {
      if (digitalRead(inputMap[i * 6].pin) == LOW && digitalRead(inputMap[i * 6 + 1].pin) == LOW)
        sixButtonMode[i] = true;
    }
    // When a controller disconnects, revert to three-button polling
    else
    {
      sixButtonMode[i] = false;
    }
  }
   
  delayMicroseconds(20);
  digitalWrite(SELECT, HIGH);
   
  for (input &inpt : inputMap)
  {
    if (sixButtonMode[ inpt.player ] )
    {
      if (digitalRead(inpt.pin) == LOW)
      {
        currentState[inpt.player] |= inpt.pulse3Flag;
      }
    }
  }
   
  
}

void sendStates()
{
  for (int i = 0; i < 4; i++)
  {
    uint16_t state = currentState[i];
    if (lastState[i] != state)
    {
      
      int8_t ay = ((state & BTN_UP) != 0 ? -127 : 0) + ((state & BTN_DOWN) != 0 ? 127 : 0);
      int8_t ax = ((state & BTN_LEFT) != 0 ? -127 : 0) + ((state & BTN_RIGHT) != 0 ? 127 : 0);

      #ifdef SERIALDEBUG
      Serial.print(i);
      Serial.print(' ');
      Serial.print(state, HEX);
      Serial.print(' ');
      Serial.print(ax);
      Serial.print(' ');
      Serial.print(ay);
      Serial.print(' ');
      Serial.println((state & masks[i]) >> 5, HEX);
      #endif
      
      Joystick[i].sendState(ax, ay, (state & masks[i]) >> 5);
    }    
    
    lastState[i] = state;
  }
}
