#pragma once
#include "common.h"
#include "game.h"
class Game;

// Player ------------------------------
class Player{
  public:
  float q[DIMS]; //position
  float v[DIMS]; //verosity
  float vr     ; //speed     (=  |v|)
  float d[DIMS]; //direction (=v/|v|)
  int h;         //hit point
  void move(Game *pG);   // move and shot
  void draw(Game *pG);   // draw
  void drawHp(Game *pG); // draw hit point
};
