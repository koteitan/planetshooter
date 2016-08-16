#pragma once
#include <Arduboy.h>
#include "common.h"
#include "actor.h"
class Player;
class Enemy;
class Bullet;
class Shot;
class BackGroundStar;
class Debri;
#define BGSTARS      (10) // number of background stars
#define BGSTARLAYERS ( 3) // number of background star layers
#define ENEMIES      ( 6) // number of enemies
#define BULLETS      (10) // max number of bullets 
#define DEBRIS       (20) // max number of debris
#define PLAYER_SIZE_DR (5.0f) // displayed radius of player [px]
#define PLAYER_SIZE_CR (2.0f) // collision radius of player [px]
#define ENEMY_SIZE_DR  (5.0f) // displayed radius of enemy [px]
#define ENEMY_SIZE_CR  (5.0f) // collision radius of enemy [px]
#define BULLET_SIZE_DR (2.0f) // displayed radius of bullet [px]
#define BULLET_SIZE_CR (2.0f) // collision radius of bullet [px]
class Game{
  public:
  int score  ;
  int hiscore;
  Arduboy *pA;
  bool *keypressed;
  
  Player *pPlayer                       ;
  Enemy  *pEnemy [ENEMIES]              ;
  Bullet *pBullet[BULLETS]              ;
  Shot   *pShot                         ;
  Debri  *pDebri [DEBRIS ]              ;
  BGStar *pBgstar[BGSTARS][BGSTARLAYERS];
  int tEnemyHp;
  int iEnemyHp;
  int bullets;
  int debris;
  int iDebris;
  int iAnime;
  int iAnimeMax;
 
  Game(Arduboy *_pA, bool *_kp);
  void reset    (void);
  void drawScore(void);
  void loop     (void);
  void drawDebug(void);
};
