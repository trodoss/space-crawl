#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "elements.h"

void stateGamePlaying() {
 gameState = STATE_GAME_PLAYING;
 
 globalCounter++;
 if (globalCounter > 20) {
  globalCounter = 0;
  if (view_y < 128) view_y++;
 }
 draw_crawl();

 if (arduboy.pressed(RIGHT_BUTTON)) {
  move_player(STATE_PLAYER_RIGHT);
 }
 
 if (arduboy.pressed(LEFT_BUTTON)) {
  move_player(STATE_PLAYER_LEFT);
 }
 
 if (arduboy.pressed(DOWN_BUTTON)) {
  move_player(STATE_PLAYER_DOWN);
 }
 
 if (arduboy.pressed(UP_BUTTON)) {
  move_player(STATE_PLAYER_UP);
 }

 if (arduboy.pressed(B_BUTTON)) {
  fire_laser();
 }

 if (arduboy.pressed(A_BUTTON)) {
  teleport_player();
 }
   
  level_element_handle();

};

void startGame()
{
  level_element_add(TYPE_PLAYER, 0,0, STATE_PLAYER_RIGHT, FACING_RIGHT);
  level_element_add(TYPE_LASER, 0,0, STATE_HIDDEN, FACING_RIGHT);

  level_element_add(TYPE_ENEMY, 0,0, STATE_HIDDEN, FACING_RIGHT);
  level_element_add(TYPE_ENEMY, 0,0, STATE_HIDDEN, FACING_RIGHT);
  level_element_add(TYPE_ENEMY, 0,0, STATE_HIDDEN, FACING_RIGHT);
  add_enemy(2);
  add_enemy(3);
  add_enemy(4);
  stateGamePlaying();
 
};

void stateGameOver() {
   gameState = STATE_GAME_OVER;
   sprites.drawSelfMasked(20, 8, game_over_img, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    view_x = 0;
    view_y = 0;
    globalCounter = 0;

    level_element_reset();
    gameState = STATE_MENU_MAIN;
  }
}

#endif
