#pragma once
#include "common.h"
#include "game.h"
class Game;
// player -------------------------------
class Camera;
class Player;
class Enemy;
class Bullet;
class Shot;
class BackGroundStar;
class Debri;

// Camera ------------------------------
class Camera{
  public:
  float q[DIMS];
  float v[DIMS];
  float c[DIMS];
  void move(Game *pG);
  void draw(Game *pG);
};
// Player ------------------------------
class Player{
  public:
  float q[DIMS]; //position
  float v[DIMS]; //verosity
  int h;         //hit point
  void move(Game *pG);   // move and shot
  void draw(Game *pG);   // draw
  void drawHp(Game *pG); // draw hit point
};
// enemies ------------------------------
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

// bullet of enemies ----------------------
class Bullet{ // bullets of enemies
  public:
  bool b;     // is shot
  float q[2]; // position
  float v[2]; // velosity
  static void init(void);
  bool move(Game *pG); // move with player p / returns if player is alive
  void draw(Game *pG); // draw
};
// shot of player--------------------------
class Shot{
  public:
  float d[DIMS]; // direction
  char t;        // left time
  char dp;       // display pattern
  void move(Game *pG);
  void draw(Game *pG);
};
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
// background star effect------------------
class BGStar{
  public:
  float q[DIMS];
  void draw(Game *pG, int l);
};
