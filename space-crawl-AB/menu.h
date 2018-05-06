#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "game.h"

void drawTitleScreen()
{
  sprites.drawSelfMasked(20, 8, logo_img, 0);
}

void stateMenuIntro()
{
  globalCounter++;
  sprites.drawSelfMasked(34, 4, trodoss_img, 0);
  if (globalCounter > 120)
  {
    gameState = STATE_MENU_MAIN;
  }
}

void stateMenuMain()
{
  drawTitleScreen();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) startGame();
}

#endif
