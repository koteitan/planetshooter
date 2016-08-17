#pragma once
#include "common.h"
#include "game.h"
class Game;
class Shot{
  public:
  float d[DIMS]; // direction
  char t;        // left time
  char dp;       // display pattern
  void move(Game *pG);
  void draw(Game *pG);
};
