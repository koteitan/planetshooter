#pragma once
#include <Arduboy.h>
#include "common.h"
#include "camera.h"
#include "player.h"
#include "shot.h"
#include "enemy.h"
#include "bullet.h"
#include "debri.h"
#include "bgstar.h"
#include "graphiceffect.h"

class Camera;
class Player;
class Enemy;
class Bullet;
class Shot;
class BackGroundStar;
class Debri;
typedef enum{
  eGAME_STT_PLAY = 0,
  eGAME_STT_DIED = 1,
  eGAME_STTS     = 2
}eGAME_STT;
typedef enum{
  eGE_STT_IDLE    = 0,
  eGE_STT_PLAYING = 1,
  eGE_STTS        = 2
}eGE_STT;
#if 1 // release
#define BGSTARS      ( 5) // number of background stars
#define BGSTARLAYERS ( 3) // number of background star layers
#define ENEMIES      (10) // number of enemies
#define BULLETS      (10) // max number of bullets 
#define DEBRIS       ( 4) // max number of debris
#endif
#if 0 //debug
#define BGSTARS      (10) // number of background stars
#define BGSTARLAYERS ( 3) // number of background star layers
#define ENEMIES      ( 1) // number of enemies
#define BULLETS      (10) // max number of bullets 
#define DEBRIS       (10) // max number of debris
#endif
#if 0 // low cost
#define BGSTARS      ( 3) // number of background stars
#define BGSTARLAYERS ( 3) // number of background star layers
#define ENEMIES      ( 2) // number of enemies
#define BULLETS      ( 2) // max number of bullets 
#define DEBRIS       ( 2) // max number of debris
#endif
#define PLAYER_SIZE_DR (5.0f) // displayed radius of player [px]
#define PLAYER_SIZE_CR (2.0f) // collision radius of player [px]
#define ENEMY_SIZE_DR  (10.0f) // displayed radius of enemy [px]
#define ENEMY_SIZE_CR  (10.0f) // collision radius of enemy [px]
#define BULLET_SIZE_DR (2.0f) // displayed radius of bullet [px]
#define BULLET_SIZE_CR (2.0f) // collision radius of bullet [px]
class Game{
  public:
  int score  ;
  int hiscore;
  Arduboy *pA;
  bool *keypressed;
  Camera *pCamera                       ;
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
  int t_died;
  eGAME_STT state;
  eGE_STT geState;
  GraphicEffect *pGE;
  int geSeqNow  = 0;
  int geSeqMax  = 0;
  int geTimeNow = 0;
  int geTimeMax = 0;
  int geSeqStep = 0;
  int geType    = 0;
  int geTypeMax = 3;
  int geDamageTimeNow = 0;
  int geDamageTimeMax = 4;
  Game(Arduboy *_pA, bool *_kp);
  void reset    (void);
  void drawScore(void);
  void drawAll  (void);
  void loop     (void);
  void drawDebug(void);
  void drawEnemyHp(void);
  float fDbg[10];
};
