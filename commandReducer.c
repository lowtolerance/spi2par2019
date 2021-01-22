#include "display.h"


cursor_t updateCursorPos(cursor, uint8_t x, uint8_t y) {
  cursor.x = x;
  cursor.y = y;
  return cursor;
}

bool setDisplayActiveFlag(state, bool active) {
  state.flags.active = active;
  return state.flags
}

typedef struct action_s {
  uint8_t type;
  uint8_t payload;
}

displayState_t stateManager(action_s action, displayState_t state) {
  switch (action.type) {
    case XeniumCursorHome:
      state.cursor = 
        updateCursorPos(
          state.cursor,
          0,
          0
        );
      state.cursor.x = 0
    case XeniumHideDisplay:
      state.flags = 
        setDisplayActive(false);
      break;
    case XeniumShowDisplay:
      state.flags = 
        setDisplayActive(true);
    case XeniumHideCursor:
      state.cursor.mode = HIDDEN;
      break;
    case XeniumShowBlockCursor:
    case XeniumShowInvertedCursor:
      state.cursor.mode = BLINK;
      break;
    case XeniumBackspace:
      state.buffer[sizeOf(buffer)] = " ";
      state.cursor.x = 
        state.cursor.x - 1,
      break;
    case XeniumLineFeed: //Move Cursor down one row, but keep column
      state.cursor.y = state.cursor.y + 1;
      break;
    case XeniumDeleteInPlace:
      state.buffer[sizeOf(buffer)1] = " ";
      break;
    case XeniumFormFeed: //Formfeed just clears the screen and resets the cursor.
      state.cursor = updateCursorPos(0,0);
      state.buffer = NULL;
      break;
    case XeniumCarriageReturn: //Carriage returns moves the cursor to the start of the current line
      state.cursor = 
        updateCursorPos(
          0, 
          state.cursor.y
        );
      break;
    case XeniumSetCursorPosition:
      state.cursor = 
        updateCursorPosition(
          getCommand(),
          getCommand()
        );
      break;
    case XeniumSetBacklight:
      break;
    case XeniumSetContrast:
      break;
    case XeniumReboot:
      state = initDisplayState;
      break;
    case XeniumCursorMove:
      if (getCommand() == 91) {
        uint8_t x = state.cursor.x;
        uint8_t y = state.cursor.y;
        switch (getCommand()) {
          case 65: //UP
            y = y - 1;
            break;
          case 66: //DOWN
            y = y + 1;
            break;
          case 67: //RIGHT
            x = x + 1;
            break;
          case 68: //LEFT
            x = x - 1;
            break;
          default:
            break;
        }
        state.cursor = 
          updateCursorPos(x,y)
      }
      break;
    case 32 ... 254:
      state.buffer = buffer + command;
      break;
    default:
      break;
  }
}

uint8_t getCommand(uint8_t *queue, uint8_t pos) {
  return pos && &queue[pos--];
}
