#include <Arduboy.h>
#include "common.h"
#include "game.h"
#include "camera.h"
#include "player.h"
#include "shot.h"
#include "enemy.h"
#include "bullet.h"
#include "debri.h"
#include "bgstar.h"
//-----------------------------------------
bool Bullet::move(Game *pG){
  bool isPlayerAlive = true;
  Player *pP=pG->pPlayer;
  if(b){
    q[0]+=v[0];
    q[1]+=v[1];
    float dx = pP->q[0]-q[0];
    float dy = pP->q[1]-q[1];
    float dr = sqrt(dx*dx+dy*dy);
    if(abs(dx)>WX||abs(dy)>WY){
      b=false;
    }
    if(dr<SX2WX*(BULLET_SIZE_CR+PLAYER_SIZE_CR)){
      pP->h-=5;
      b=false;
      if(pP->h<=0) isPlayerAlive = false;
    }
  }
  return isPlayerAlive;
}
//-----------------------------------------
void Bullet::draw(Game *pG){
  if(b){
    float cs=SX2WX*BULLET_SIZE_DR;
    float dx=q[0]-pG->pCamera->q[0];
    float dy=q[1]-pG->pCamera->q[1];
    if(abs(dx) < WX/2-cs && abs(dy) < WY/2-cs){
      int x=(int)(dx*WX2SX)+SX/2;
      int y=(int)(dy*WY2SY)+SY/2;
      pG->pA->drawCircle(x,y,BULLET_SIZE_DR-pG->iAnime,WHITE);
    }
  }
}
//-----------------------------------------
