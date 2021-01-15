#include "xeniumspi.h"
#include "src/SMWire/SMWire.h"
#include <LiquidCrystal.h>

//HD44780 LCD Setup
const uint8_t rs = 18, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4; //HD44780 compliant LCD display pin numbers
const uint8_t mosi = 16, sck = 15; 
uint8_t X = 0, Y = 0;

LiquidCrystal hd44780(rs, en, d4, d5, d6, d7); //Constructor for the LCD.


//SPI Data
int16_t queue[256];
uint8_t pos = 0;

char command;
//SPI Bus Receiver Interrupt Routine
ISR (SPI_STC_vect) {
  queue[pos++] = SPDR;
}

void setup() {
  Serial.begin(9600);
  for (uint8_t i = 0; i < 8; i++) {
    hd44780.createChar(i, &chars[i][0]);
  }
  hd44780.begin(20, 4);
  hd44780.noCursor();
  SPCR |= _BV(SPE);   //Turn on SPI. We don't set the MSTR bit so it's slave.
  SPCR |= _BV(SPIE);  //Enable to SPI Interrupt Vector
  SPCR |= _BV(CPOL);  //SPI Clock is high when inactive
  SPCR |= _BV(CPHA);  //Data is Valid on Clock Trailing Edge
  hd44780.setCursor(0, 0);
}

void stateManager(char command) {
  switch (command) {
    case XeniumCursorHome:
      X = 0;
      Y = 0;
      hd44780.setCursor(X, Y);
      break;
    case XeniumHideDisplay:
      hd44780.noDisplay();
      break;
    case XeniumShowDisplay:
      hd44780.display();
      break;
    case XeniumHideCursor:
      hd44780.noCursor();
      break;
    case XeniumShowBlockCursor:
    case XeniumShowInvertedCursor:
      hd44780.cursor();
      hd44780.blink();
      break;
    case XeniumBackspace:
      X--;
      hd44780.setCursor(X, Y);
      hd44780.print(" ");
      hd44780.setCursor(X, Y);
      break;
    case XeniumLineFeed: //Move Cursor down one row, but keep column
      X++;
      hd44780.setCursor(X, Y);
      break;
    case XeniumDeleteInPlace:
      hd44780.print(" ");
      hd44780.setCursor(X, Y);
      break;
    case XeniumFormFeed: //Formfeed just clears the screen and resets the cursor.
      hd44780.clear();
      X = 0;
      Y = 0;
      hd44780.setCursor(X, Y);
      break;
    case XeniumCarriageReturn: //Carriage returns moves the cursor to the start of the current line
      X = 0;
      hd44780.setCursor(X, Y);
      break;
    case XeniumSetCursorPosition:
          hd44780.setCursor(getCommand(), getCommand());
      break;
    case XeniumSetBacklight:
      break;
    case XeniumSetContrast:
      break;
    case XeniumReboot:
      X = 0;
      Y = 0;
      hd44780.begin(20, 4);
      break;
    case XeniumCursorMove:
      if (getCommand() == 27) {
        switch (getCommand()) {
          case 65: //UP
            Y--;
            break;
          case 66: //DOWN
            Y++;
            break;
          case 67: //RIGHT
            X++;
            break;
          case 68: //LEFT
            X--;
            break;
          default:
            break;
        }
        hd44780.setCursor(X, Y);
      }
      break;
    case  32 ... 254:
      hd44780.setCursor(X, Y);
      hd44780.write(command);
      X++;
      break;
    default:
      break;
  }
}

void loop() {
  if (pos > 0) {
    stateMachine(getCommand());
  } else {
    Serial.print(".")
  }
}

uint8_t getCommand() {
  return queue[pos--];
}