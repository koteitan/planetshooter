#pragma once
#include "common.h"
#include "game.h"
class Game;
// debri of enemy shot effect-----------------
class Debri{
  public:
  float q[DIMS];
  float v[DIMS];
  char t;
  void init(Game *pG);
  void move(Game *pG);
  void draw(Game *pG);

  static char i     ;
  static char debris;
  static void moveAll(Game *pG);
  static void drawAll(Game *pG);
};
