#pragma once
#include "common.h"
#include "game.h"
class Game;

// Camera ------------------------------
class Camera{
  public:
  float q[DIMS];
  float v[DIMS];
  float c[DIMS];
  void move(Game *pG);
  void draw(Game *pG);
};
