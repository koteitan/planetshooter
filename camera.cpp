//Camera --------------------------------------
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
//---------------------------------------------
void Camera::move(Game *pG){
  float rate   = 0.75f;
  float vmax   = 1.0f;
  float vstep  = 0.08f;
  Player *pP = pG->pPlayer;
  c[0] = pP->q[0] + pP->d[0]*rate*2.0f;
  c[1] = pP->q[1] + pP->d[1]*rate*1.0f + 4.0f*SX2WX; // for score charactor
  v[0]=max(-vmax,min(vmax,(c[0]-q[0])*vstep));
  v[1]=max(-vmax,min(vmax,(c[1]-q[1])*vstep));
  q[0]+=v[0];
  q[1]+=v[1];
}
//---------------------------------------------
void Camera::draw(Game *pG){
  int cx = pG->pCamera->c[0]*WX2SX - pG->pCamera->q[0]*WX2SX;
  int cy = pG->pCamera->c[1]*WY2SY - pG->pCamera->q[1]*WY2SY;
  pG->pA->drawLine(SX/2+cx-2,SY/2+cy-2,SX/2+cx+2,SY/2+cy+2,WHITE);
  pG->pA->drawLine(SX/2+cx-2,SY/2+cy+2,SX/2+cx+2,SY/2+cy-2,WHITE);
}

