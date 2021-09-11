/*
 Modifided version of HVPE(original credits below) for erase and flash new fuses for AT90USB162 with Arduino Mega 2560 (used to have serial output)
 */

/*  
 HVFuse - Use High Voltage Programming Mode to Set Fuses on ATmega48/88/168
 09/23/08  Jeff Keyzer  http://mightyohm.com                
 The HV programming routines are based on those described in the
 ATmega48/88/168 datasheet 2545M-AVR-09/07, pg. 290-297
 This program should work for other members of the AVR family, but has only
 been verified to work with the ATmega168.  If it works for you, please
 let me know!  http://mightyohm.com/blog/contact/
 */

// Desired fuse configuration
#define  HFUSE  0xD9   // Default for ATmega48/88/168, for others see
#define  LFUSE  0xDC   // http://www.engbedded.com/cgi-bin/fc.cgi
#define  EFUSE  0xF4   // http://www.engbedded.com/cgi-bin/fc.cgi

// Pin Assignments
#define  DATA    PORTA // PORTD = Arduino Digital pins 0-7
#define  DATAD   DDRA  // Data direction register for DATA port
#define  DATAIN  PINA
#define  VCC     8
#define  RDY     12     // RDY/!BSY signal from target
#define  OE      11
#define  WR      10
#define  BS1     9
#define  XA0     13
#define  XA1     18    // Analog inputs 0-5 can be addressed as
#define  PAGEL   19    // digital outputs 14-19
#define  RST     14    // Output to level shifter for !RESET
#define  BS2     16
#define  XTAL1   17

#define  BUTTON  15    // Run button

void setup()  // run once, when the sketch starts
{
  Serial.begin(115200);
  // Set up control lines for HV parallel programming
  DATA = 0x00;  // Clear digital pins 0-7
  DATAD = 0xFF; // set digital pins 0-7 as outputs
  pinMode(VCC, OUTPUT);
  pinMode(RDY, INPUT);
  pinMode(OE, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(BS1, OUTPUT);
  pinMode(XA0, OUTPUT);
  pinMode(XA1, OUTPUT);
  pinMode(PAGEL, OUTPUT);
  pinMode(RST, OUTPUT);  // signal to level shifter for +12V !RESET
  pinMode(BS2, OUTPUT);
  pinMode(XTAL1, OUTPUT);

  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);  // turn on pullup resistor

  // Initialize output pins as needed
  digitalWrite(RST, HIGH);  // Level shifter is inverting, this shuts off 12V
}

void loop()  // run over and over again
{
  Serial.println("WaitBtn");
  while(digitalRead(BUTTON) == HIGH) {  // wait until button is pressed
  }

  Serial.println("Prog");
  // Initialize pins to enter programming mode
  
  // Enter programming mode
  digitalWrite(VCC, HIGH);  // Apply VCC to start programming process
  digitalWrite(RST, HIGH);
  delay(10);
  for(int i = 0; i < 12; i++)
  {
    digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
    delay(1);
    digitalWrite(XTAL1, LOW);
    delay(1);
  }

  digitalWrite(PAGEL, LOW);
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, LOW);
  digitalWrite(BS1, LOW);
  digitalWrite(BS2, LOW);

  delay(10);

  digitalWrite(WR, HIGH);  // Now we can assert !OE and !WR
  digitalWrite(OE, HIGH);
  delay(1);

  digitalWrite(RST, LOW);   // Apply 12V to !RESET thru level shifter
  delay(1);

  delay(1000);

  Serial.println("Sign");

  for(int i = 0; i < 3; i++)
  {
    sendcmd(B00001000);
    sendloaddr(i);
    digitalWrite(OE, LOW);
    digitalWrite(BS1, LOW);
    digitalWrite(BS2, LOW);
    delay(10);
    DATAD = 0;
    DATA = 0xFF;
    
    delay(1);
    Serial.print(i);
    Serial.print(" ");
    Serial.println(DATAIN, HEX);
    DATA = 0x00;  // Clear digital pins 0-7
    DATAD = 0xFF; // set digital pins 0-7 as outputs
    
    digitalWrite(OE, HIGH);
    delay(10);
  }

    sendcmd(B00000100);
    digitalWrite(OE, LOW);
    digitalWrite(BS1, LOW);
    digitalWrite(BS2, LOW);

    delay(10);
    DATAD = 0;
    DATA = 0xFF;
    
    delay(1);
    Serial.println(DATAIN, HEX);

    digitalWrite(BS1, HIGH);
    digitalWrite(BS2, HIGH);
    delay(10);
    Serial.println(DATAIN, HEX);

    digitalWrite(BS1, LOW);
    digitalWrite(BS2, HIGH);
    delay(10);
    Serial.println(DATAIN, HEX);

    digitalWrite(BS1, HIGH);
    digitalWrite(BS2, LOW);
    delay(10);
    Serial.println(DATAIN, HEX);

    
    digitalWrite(BS1, LOW);
    digitalWrite(BS2, LOW);
    DATA = 0x00;  // Clear digital pins 0-7
    DATAD = 0xFF; // set digital pins 0-7 as outputs
    
    digitalWrite(OE, HIGH);
    delay(10);

  
  sendcmd(B10000000);

  digitalWrite(WR, LOW);
  delay(10);
  digitalWrite(WR, HIGH);
  
  while(digitalRead(RDY) == LOW) {  // wait until button is pressed
    Serial.println("erase");
    digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
    delay(1);
    digitalWrite(XTAL1, LOW);
    delay(100);
  }


  
  
  // Now we're in programming mode until RST is set HIGH again
  
  // First we program HFUSE
  sendcmd(B01000000);  // Send command to enable fuse programming mode
  writefuse(HFUSE, 1);

  digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
  delay(1);
  digitalWrite(XTAL1, LOW);

  // Now we program LFUSE
  sendcmd(B01000000);
  writefuse(EFUSE, 2);

  digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
  delay(1);
  digitalWrite(XTAL1, LOW);

  // Now we program LFUSE
  sendcmd(B01000000);
  writefuse(LFUSE, 0);

  digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
  delay(1);
  digitalWrite(XTAL1, LOW);

  delay(1000);  // wait a while to allow button to be released

  // Exit programming mode
  digitalWrite(RST, HIGH);

  // Turn off all outputs
  DATA = 0x00;
  digitalWrite(OE, LOW);
  digitalWrite(WR, LOW);
  digitalWrite(PAGEL, LOW);
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, LOW);
  digitalWrite(BS1, LOW);
  digitalWrite(BS2, LOW);
  digitalWrite(VCC, LOW);
}

void sendcmd(byte command)  // Send command to target AVR
{
  // Set controls for command mode
  digitalWrite(XA1, HIGH);
  digitalWrite(XA0, LOW);
  digitalWrite(BS1, LOW);
  //DATA = B01000000;  // Command to load fuse bits
  DATA = command;
  digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
  delay(1);
  digitalWrite(XTAL1, LOW);
  //delay(1);
}

void sendloaddr(byte addr)  // Send command to target AVR
{
  // Set controls for command mode
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, LOW);
  digitalWrite(BS1, LOW);
  digitalWrite(BS2, LOW);
  //DATA = B01000000;  // Command to load fuse bits
  DATA = addr;
  digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
  delay(1);
  digitalWrite(XTAL1, LOW);
  //delay(1);
}

void writefuse(byte fuse, int highbyte)  // write high or low fuse to AVR
{
  // if highbyte = true, then we program HFUSE, otherwise LFUSE
  
  // Enable data loading
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, HIGH);
  delay(1);
  // Write fuse
  DATA = fuse;  // set desired fuse value
  digitalWrite(XTAL1, HIGH);
  delay(1);
  digitalWrite(XTAL1, LOW);
  if(highbyte == 0)
  {
    digitalWrite(BS2, LOW);  // program HFUSE
    digitalWrite(BS1, LOW);  // program HFUSE
  }
  else if(highbyte == 1)
  {
    digitalWrite(BS2, LOW);  // program HFUSE
    digitalWrite(BS1, HIGH);  // program HFUSE
  }
  else if(highbyte == 2)
  {
    digitalWrite(BS2, HIGH);  // program HFUSE
    digitalWrite(BS1, LOW);
  }
  digitalWrite(WR, LOW);
  delay(1);
  digitalWrite(WR, HIGH);
  delay(100);
  digitalWrite(BS2, LOW);  // program HFUSE
  digitalWrite(BS1, LOW);  // program HFUSE
  delay(100);
}
