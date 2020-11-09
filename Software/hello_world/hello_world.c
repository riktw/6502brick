#pragma link("linker.ld")

#include <stdint.h>
#include <stdio.h>

typedef struct 
{
  volatile uint8_t PortB;
  volatile uint8_t PortA;
  volatile uint8_t DirectionB;
  volatile uint8_t DirectionA;
  volatile uint8_t Timer1CounterLow;
  volatile uint8_t Timer1CounterHigh;
  volatile uint8_t Timer1LatchLow;
  volatile uint8_t Timer1LatchHigh;
  volatile uint8_t Timer2Latches;
  volatile uint8_t Timer2Counter;
  volatile uint8_t ShiftControl;
  volatile uint8_t TimerControl;
  volatile uint8_t Handshake;
  volatile uint8_t InterruptFlag;
  volatile uint8_t InterruptEnable;
} via_reg;    //VIA register mapping

typedef struct
{
  volatile uint8_t ControlStatus;
  volatile uint8_t ReceiveTransmit;
} acia_reg;   //UART ACIA register mapping


volatile uint16_t counter;
volatile via_reg* VIA1 = ((via_reg*)0xB000);    //VIA mapped to 0xB000
volatile acia_reg* ACIA1 = ((acia_reg*)0xA000); //ACIA UART mapped to 0xA000

#define LCD_E_PIN   0x80
#define LCD_RW_PIN  0x40
#define LCD_RS_PIN  0x20
#define LCD_NO_PINS 0x00

void sendCharacter(char payload) {
  //first check if transmit buffer is empty before filling it
  uint8_t status = ACIA1->ControlStatus;
  while(!(status & 0x02)) {     
    status = ACIA1->ControlStatus;
  };
  
  ACIA1->ReceiveTransmit = payload; 
}

void WaitForScreen() {
  // to chec if screen is ready for a new command, status has to be read.
  VIA1->DirectionB  = 0x00;     //PORTB input to read data
  
  while(1) {
    VIA1->PortA = LCD_RW_PIN;       //RW high to read data 
    VIA1->PortA = LCD_E_PIN | LCD_RW_PIN;  //Set E to Read status reg   
    uint8_t status = VIA1->PortB;
    uint8_t flag = (status & 0x80); //Check if bit 7 is high, indicating busy
    if(flag == 0x80) {
    }
    else {
      break;
    }
  }
  VIA1->PortA = LCD_RW_PIN;     //Clear E pin
  VIA1->DirectionB  = 0xFF;     //PORTB ouput again, done with reads
}

void ScreenSendInstruction(uint8_t instruction) {
  WaitForScreen();
  
  VIA1->PortB  = instruction;  
  VIA1->PortA = LCD_NO_PINS;   //RW and RS low, send instruction
  VIA1->PortA = LCD_E_PIN;     //Set E to send instruction
  VIA1->PortA = LCD_NO_PINS;   //Clear E pin
}

void ScreenPrintChar(char charToPrint) {
  WaitForScreen();
  
  VIA1->PortB  = charToPrint; 
  VIA1->PortA = LCD_RS_PIN;               //set rs pin to write character
  VIA1->PortA = LCD_E_PIN | LCD_RS_PIN;   //Set E to send instruction
  VIA1->PortA = LCD_RS_PIN;               //clear E pin
}

void main() {
    
    ACIA1->ControlStatus = 0x03;    //reset
    
    VIA1->DirectionB  = 0xFF;     //8 bit databus to display
    VIA1->DirectionA  = 0xE0;     //upper 3 pins for RS, RW, E pin of screen
    
    ScreenSendInstruction(0x38);  //8 bit mode, 2 line, 5x8 font 
    ScreenSendInstruction(0x0F);  //display on, cursor on, blink on
    ScreenSendInstruction(0x06);  //increment cursor, don't shift display
    ScreenSendInstruction(0x02);  //cursor to start
    ScreenSendInstruction(0x01);  //clear dislay
    
    uint8_t hello[] = "Hello world!"z;    //z means string without 0 termination
    for(uint8_t i = 0; i < sizeof(hello); ++i) {
      ScreenPrintChar(hello[i]);
    }
    
  
    ACIA1->ControlStatus = 0x15;    //divide clock by 16, 8 bit, 1 stop bit
    for(uint8_t i = 0; i < sizeof(hello); ++i) {
      sendCharacter(hello[i]);
    }

    while(1) { }
}

//no interrupts used here.
#pragma code_seg(IRQ)
interrupt(hardware_stack) void IRQ() {
}
