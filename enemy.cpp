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

//----------------------------------------
bool Enemy::move(Game *pG){
  bool isPlayerAlive=true;
  float vstep  = 0.004f;
  float vdecay = 0.9f;
  float vmax   = +0.01f;
  float dx = pG->pPlayer->q[0]-q[0];
  float dy = pG->pPlayer->q[1]-q[1];
  float dr   = sqrt(dx*dx+dy*dy);
  float idr  = 1.0f/dr;
  // collision to player 
  if(pG->pPlayer->h>0 && dr<SX2WX*(PLAYER_SIZE_CR+ENEMY_SIZE_CR)){
    pG->pPlayer->h-=10;
    pG->geDamageTimeNow = pG->geDamageTimeMax;
    if(pG->pPlayer->h<=0){
      isPlayerAlive = false;
    }
  }
  // respawn at far
  if(abs(dx)>WX*3.0f||abs(dy)>WY*3.0f){
    respawn(pG);
  }
  // close to player 
  float esidr = idr*vstep;
  if(dt--<0){
    dt=random(0,120);
    dd=random(0,6);
  }
  switch(dd){
    case 1: // right angle to Player
      v[0]-=dy*esidr;
      v[1]+=dx*esidr;
    break;
    case 0: // right angle to Player
      v[0]+=dy*esidr;
      v[1]-=dx*esidr;
    break;
    default: // close to Player
      v[0]+=dx*esidr;
      v[1]+=dy*esidr;
    break;
  }
  // keep apart from other enemies 
  for(int e2=0;e2<ENEMIES;e2++){
    Enemy *pE2 = pG->pEnemy[e2];
    if(pE2!=this){
      float dx2=q[0]-pE2->q[0];
      float dy2=q[1]-pE2->q[1];
      float cs=SX2WX*10.0f;
      if(abs(dx2)<cs&&abs(dy2)<cs){
        v[0]+=dx2*esidr;
        v[1]+=dy2*esidr;
      }
    }
  }
  v[0]=max(-vmax,min(vmax,v[0]))*vdecay;
  v[1]=max(-vmax,min(vmax,v[1]))*vdecay;
  q[0]+=v[0];
  q[1]+=v[1];
  //fire 
  #define FIREFRAMES (64)
  #define BULLETSPEED (0.08f)
  float br=random(0,FIREFRAMES);
  if(br==0 && pG->bullets<BULLETS){
    int i=0;
    Bullet *pB = NULL;
    for(i=0;i<BULLETS;i++){
      pB = pG->pBullet[i];
      if(!pB->b) break;
    }
    if(i<BULLETS){
      pB->b=true;
      pB->q[0]=q[0];
      pB->q[1]=q[1];
      float bsidr = idr*BULLETSPEED;
      pB->v[0]=dx*bsidr;
      pB->v[1]=dy*bsidr;
    }
  }
  return isPlayerAlive;
}
//----------------------------------------
void Enemy::respawn(Game *pG){
  h=32;
  Player *pP=pG->pPlayer;
  int r=random(0,4);
  switch(r){
    case 0:
      q[0]=(((float)random(0,65536))/65536.0f)*WX*4.0f-WX*2.0f+pP->q[0];
      q[1]=pP->q[1]-WY*2.0f;
    return;
    case 1:
      q[0]=(((float)random(0,65536))/65536.0f)*WX*4.0f-WX*2.0f+pP->q[0];
      q[1]=pP->q[1]+WY*2.0f;
    return;
    case 2:
      q[0]=pP->q[0]-WX*2.0f;
      q[1]=(((float)random(0,65536))/65536.0f)*WY*4.0f-WY*2.0f+pP->q[1];
    return;
    default:
      q[0]=pP->q[0]+WX*2.0f;
      q[1]=(((float)random(0,65536))/65536.0f)*WY*4.0f-WY*2.0f+pP->q[1];
    return;
  }
  dt=(char)random(0,120);
  dd=(char)random(0,4);
}
//----------------------------------------
void Enemy::draw(Game *pG){
  float dx=q[0]-pG->pCamera->q[0];
  float dy=q[1]-pG->pCamera->q[1];
  int x=(int)(dx*WX2SX)+SX/2;
  int y=(int)(dy*WY2SY)+SY/2;
  if(abs(dx) < WX/2 && abs(dy) < WY/2){
    pG->pA->drawCircle(x,y,ENEMY_SIZE_DR,WHITE);
  }
}
//----------------------------------------
