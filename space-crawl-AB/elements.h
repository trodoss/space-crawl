#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "globals.h"

#define STATE_HIDDEN 0

#define STEP_LENGTH  2
#define COUNTER_START 16

#define TYPE_PLAYER 0
#define TYPE_LASER  1
#define TYPE_ENEMY  2

#define STATE_PLAYER_RIGHT 1
#define STATE_PLAYER_LEFT  2
#define STATE_PLAYER_UP    3
#define STATE_PLAYER_DOWN  4

#define FACING_RIGHT 0
#define FACING_LEFT 2

#define STATE_ENEMY_RIGHT 10
#define STATE_ENEMY_LEFT  11
#define STATE_ENEMY_UP    12
#define STATE_ENEMY_DOWN  13

#define STATE_LASER      20

#define MAX_ELEMENT_RECORDS 12

struct LevelElement
{
  unsigned char id;          //elment identifier
  unsigned char type;        //element type
  unsigned char x;           // y position
  unsigned char y;           // x position
  unsigned char state;       //determines the state of the the element
  unsigned char step;        //used to maintain animation step
  unsigned char facing;
  unsigned char counter;
};

LevelElement levelElements[MAX_ELEMENT_RECORDS];
char element_count = 0;

void level_element_add(char type, char x, char y, char state, char facing)
{
      levelElements[element_count].id = element_count;
      levelElements[element_count].type = type;
      levelElements[element_count].x = x;
      levelElements[element_count].y = y;
      levelElements[element_count].step = 0;
      levelElements[element_count].state = state;
      levelElements[element_count].facing = facing;
      levelElements[element_count].counter = 0;
      element_count++;  
}

void level_element_reset() {
  //clear the array of the previous contents
  memset(levelElements, 0, sizeof(levelElements));
  element_count = 0;
}

LevelElement level_element_get(char id) {
  return levelElements[id];
}

level_element_update(char id, LevelElement element) {
  levelElements[id] = element;
}

bool level_test_element (LevelElement element, char testX, char testY)
{
   if ((element.y + 4) < testY) return false;
   if (element.y > (testY + 4)) return false;
   if ((element.x + 4) < testX) return false;
   if (element.x > (testX + 4)) return false;
   return true;
}

//-----------------

void draw_crawl()
{
  short mapx, mapy;
  short map_xoff, map_yoff;
  unsigned char tile;

  mapx = view_x / 4;
  mapy = view_y / 4;
  
  map_xoff = view_x & 3;
  map_yoff = view_y & 3;
  
  for (int i=0; i < 32; i++)
  {
    for (int j=0; j < 14; j++)
    {
      tile = pgm_read_byte(crawl[mapy + j] + mapx + i);
      if (tile > 32) sprites.drawOverwrite(i*4 - map_xoff, j*5 - map_yoff, font_4x4_img, tile - 65);
    }
  }
}

bool check_move(uint8_t x, uint8_t y) {
  short mapx = (view_x + x) / 4;
  short mapy = (view_y + y) / 4;  
  char chk = pgm_read_byte(crawl[mapy] + mapx);
  if (chk == 32) return true;
  return false;
}
//-------------------------------------

void move_player(char action) {
  LevelElement player = level_element_get(0);
  player.state = action;
  player.step++;
  if (player.step > 1) player.step = 0;
  switch (action) {
    case STATE_PLAYER_RIGHT:
       player.facing = FACING_RIGHT;
       if (player.x < 124) {
        if (check_move(player.x + 4, player.y)) player.x += 2;
       }
       break;

    case STATE_PLAYER_LEFT:
       player.facing = FACING_LEFT;
       if (player.x > 0) {
        if (check_move(player.x - 2, player.y))player.x -= 2;
       }
       break;

    case STATE_PLAYER_UP:
       if (player.y > 0) {
        if (check_move(player.x, player.y - 2))player.y -= 2;
       }
       break;

    case STATE_PLAYER_DOWN:
       if (player.y < 90) {
        if (check_move(player.x, player.y + 6)) player.y += 2;
       }
       break;
  }
  level_element_update(0, player);
}

void fire_laser() {
  LevelElement player = level_element_get(0);
  LevelElement laser = level_element_get(1); 
  if (laser.state == STATE_HIDDEN) {
    laser.state = STATE_LASER;
    laser.x = player.x;
    laser.y = player.y;
    laser.facing = player.facing;
    laser.counter = COUNTER_START;
    level_element_update(1, laser);
  }
}

void teleport_player() {
  bool is_valid = false;
  LevelElement player = level_element_get(0);
  while (!is_valid) {
    player.x = (random() % 128); 
    player.y = (random() % 64);
    if (check_move(player.x, player.y )) is_valid = true; 
  }
  level_element_update(0, player);
}

LevelElement handle_laser(LevelElement laser) {
  if (laser.state != STATE_HIDDEN) {
    laser.counter--;
    if (laser.counter < 1) {
      laser.state = STATE_HIDDEN;
    } else {
      if (laser.facing == FACING_RIGHT) {
        if (laser.x < 124) {
          laser.x += 4;
        } else {
          laser.state = STATE_HIDDEN;
        }
      } else {
        if (laser.x > 0) {
          laser.x -= 4;
        } else {
          laser.state = STATE_HIDDEN;
        }
      }
    }
  }
  if (laser.state != STATE_HIDDEN) sprites.drawSelfMasked(laser.x, laser.y, laser_img, 0);
  return laser;
}

void add_enemy (char id) {
  bool is_valid = false;
  LevelElement enemy = level_element_get(id);
  int x = 0; 
  int y = 0;
  char state = (random() % 4) + STATE_ENEMY_RIGHT;
  while (!is_valid) {
    enemy.x = (random() % 128); 
    enemy.y = (random() % 64);
    if (check_move(enemy.x, enemy.y )) is_valid = true; 
  }
  enemy.state = state;
  level_element_update(id, enemy);
}

LevelElement handle_enemy(LevelElement enemy) {
  if (enemy.state > STATE_HIDDEN) {
    enemy.step++;
    bool change_direction = false;
    if (enemy.step > 1) enemy.step = 0;
    switch (enemy.state) {
      case STATE_ENEMY_RIGHT:
         if (enemy.x < 124) {
          if (check_move(enemy.x + 4, enemy.y)) {
            enemy.x += 2;
          } else {
            change_direction = true;
          }
         } else {
          change_direction = true;
         }
         break;
  
      case STATE_ENEMY_LEFT:
         if (enemy.x > 0) {
          if (check_move(enemy.x - 2, enemy.y)) {
            enemy.x -= 2;
          } else {
            change_direction = true;
          }
         } else {
          change_direction = true;
         }
         break;
  
      case STATE_ENEMY_UP:
         if (enemy.y > 0) {
          if (check_move(enemy.x, enemy.y - 2)) {
            enemy.y -= 2;
          } else {
            change_direction = true;
          }
         } else {
          change_direction = true;
         }
         break;
  
      case STATE_ENEMY_DOWN:
         if (enemy.y < 60) {
          if (check_move(enemy.x, enemy.y + 6)) {
            enemy.y += 2;
          } else {
            change_direction = true;
          }
         } else {
          change_direction = true;
         }
         break;
    }
    //check for laser
    LevelElement laser = level_element_get(1);
    if (laser.state != STATE_HIDDEN) {
      if (level_test_element(laser, enemy.x, enemy.y)) {
        laser.state = STATE_HIDDEN;
        level_element_update(1, laser);

        enemy.state = STATE_HIDDEN;
        add_enemy(enemy.id);
      }
    }
    
    //check for player hit
    LevelElement player = level_element_get(0);
    if (level_test_element(player, enemy.x, enemy.y)) {
      gameState = STATE_GAME_OVER;
    }
    
    if (enemy.state > STATE_HIDDEN) {
      if (change_direction) enemy.state = (random() % 4) + STATE_ENEMY_RIGHT;
      sprites.drawSelfMasked(enemy.x, enemy.y, ailen_img, enemy.step);
    }
  }
  return enemy;
}

void level_element_handle()
{
  for (char i=0; i < element_count; i++)
  {
     switch(levelElements[i].type) {
        case TYPE_PLAYER:
          if (!check_move(levelElements[i].x, levelElements[i].y)) {
              levelElements[i].y -= 2;
              if (levelElements[i].y < 1) gameState = STATE_GAME_OVER;
          }
          
          sprites.drawSelfMasked(levelElements[i].x, levelElements[i].y, trooper_img, levelElements[i].step + levelElements[i].facing);
          break;

        case TYPE_LASER:
          levelElements[i] = handle_laser(levelElements[i]);
          break;

        case TYPE_ENEMY:
          if (!check_move(levelElements[i].x, levelElements[i].y)) {
              levelElements[i].y -= 2;
              add_enemy(levelElements[i].id);
          }
          levelElements[i] = handle_enemy(levelElements[i]);
          break;
     }
     
  }
}

#endif
