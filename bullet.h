#pragma once
#include "common.h"
#include "game.h"
class Game;

// bullet of enemies ----------------------
class Bullet{ // bullets of enemies
  public:
  bool b=false;     // is shot
  float q[2]; // position
  float v[2]; // velosity
  static void init(void);
  bool move(Game *pG); // move with player p / returns if player is alive
  void draw(Game *pG); // draw
};

