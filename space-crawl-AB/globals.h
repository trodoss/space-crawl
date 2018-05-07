#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
#include <Sprites.h>
#include "bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_GAME_PLAYING           2
#define STATE_GAME_OVER              3

Arduboy2Base arduboy;
Sprites sprites;

short view_x;
short view_y;

byte gameState = STATE_MENU_INTRO; 
byte globalCounter = 0;

#endif
