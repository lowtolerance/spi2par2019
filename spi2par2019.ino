#include <LiquidCrystal.h>
#include "commandReducer.h"
#include "display.h"

//HD44780 LCD Setup
const uint8_t rs = 18, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4; //HD44780 compliant LCD display pin numbers
const uint8_t mosi = 16, sck = 15; 
uint8_t X = 0, Y = 0;

//On each successful SPI transaction, 
ISR (SPI_STC_vect) {
  queue[pos++] = SPDR;
}

void initSPI() {
  //Enable SPI slave mode
  SPCR |= _BV(SPE);
  //Interrupt on complete transaction
  SPCR |= _BV(SPIE);
  //SPI Clock is high when inactive
  SPCR |= _BV(CPOL); 
  //Data valid when clock is low
  SPCR |= _BV(CPHA);  
}

LiquidCrystal screen = hd44780(rs,en,d4,d5,d6,d7);

Display_t display = NULL;

void setup() {
  display = initDisplay()
}

uint8_t queue[100];
uint8_t pos = 0;

void loop() {
  pos &&
    commandReducer(getCommand());
}