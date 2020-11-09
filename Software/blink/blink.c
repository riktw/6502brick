#pragma link("linker.ld")     //use linker.ld file in the same folder

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


volatile uint16_t counter;  //Need to define shared variable here, used in main and IRQ code
volatile via_reg* VIA1 = ((via_reg*)0xB000);    //VIA mapped to 0xB000


void main() {
  
    VIA1->DirectionB  = 0xFF;         //All IO on port B as output
    
    VIA1->TimerControl = 0x40;        //Continous mode
    VIA1->InterruptEnable = 0xC0;     //Timer 1 irq on
    
    VIA1->Timer1CounterHigh = 0x07;   //0x708, 1800, 1.8Mhz clk, 1ms irq
    VIA1->Timer1CounterLow = 0x08;    
       
    
    counter = 0;
    asm { cli }                       //Enable interrupts
    
    while(1) {
    }
}


#pragma code_seg(IRQ)     //Indicate to compiler that this cose has to go to IRQ section.

interrupt(hardware_stack) void IRQ() {
  
  counter++;
  if(counter < 500) {
    VIA1->PortB  = 0xFF;    //All LEDs on
  }
  else {
    VIA1->PortB  = 0x00;    //All LEDs off
  }
  if(counter >= 1000) {
    counter = 0;
  }
  VIA1->Timer1LatchHigh = 0x07;   //Writing to the LatchHigh resets the IRQ
}
