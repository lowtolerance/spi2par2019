#include <LiquidCrystal.h>
struct cursor_s {
  uint8_t x, y;
  uint8_t mode;
};

struct cursor_s initCursor = {
  .x = 0, 
  .y = 0, 
  .mode = NO_SHOW
};
struct displayFlags_s {
  bool active : 1
}
struct displayState_s {
  uint8_t width;
  uint8_t height;
  cursor_t cursor;
  uint8_t contents[];
  uint8_t pixelSize;
  bool active = false;
};


struct displayState_s initDisplayState = {
  .width = 20,
  .height = 4,
  .cursor = initCursor,
  .contents = NULL;
  .pixelSize = 1;
  
};

typedef struct displayState_s displayState_t;

typedef struct hd44780_s {
  LiquidCrystal screen;
  displayState_s state;
} hd44780_t;

struct Display_s {
  .screen = NULL,
  .state = initDisplayState
};

typedef struct Display_s Display_t;

Display_t initDisplay(
  LiquidCrystal screen,   
  displayState_t state
) {
  state.buffer = memset(
    state.buffer, 
    0, 
    state.width * state.height * state.pixelSize
  );  
  screen.begin(state.width, state.height);
  screen.noCursor();
  screen.setCursor(cursor.x, cursor.y);
  return {.screen = screen, .state = state};
}