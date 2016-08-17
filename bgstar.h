#pragma once
#include "common.h"
#include "game.h"
class Game;

// background star effect------------------
class BGStar{
  public:
  float q[DIMS];
  void draw(Game *pG, int l);
};
