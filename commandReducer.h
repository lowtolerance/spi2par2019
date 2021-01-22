#import <LiquidCrystal.h>

void stateManager(uint8_t command, LiquidCrystal *lcd);
uint8_t getCommand(uint8_t *queue, uint8_t pos);