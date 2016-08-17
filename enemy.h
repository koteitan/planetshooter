#pragma once
#include "common.h"
#include "game.h"
class Game;
class Enemy{
  public:
  float q[DIMS]; // position
  float v[DIMS]; // velosity
  char dd;       // direction
  char dt;       // direction left time
  char h;        // hitpoint
  bool move(Game *pG);      // move with player p / returns if player is alive
  void draw(Game *pG);      // draw
  void respawn(Game *pG);   // respawn
};


