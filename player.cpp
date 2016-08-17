//--------------------------------------
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
//--------------------------------------
void Player::move(Game *pG){
  float vstep  = 0.012f;
  float vdecay = 0.8f;
  float vmax   = +10;
  if(pG->keypressed[KEY_XM]){v[0]+= -vstep;}
  if(pG->keypressed[KEY_XP]){v[0]+= +vstep;}
  if(pG->keypressed[KEY_YM]){v[1]+= -vstep;}
  if(pG->keypressed[KEY_YP]){v[1]+= +vstep;}
  v[0]=max(-vmax,min(vmax,v[0]))*vdecay;
  v[1]=max(-vmax,min(vmax,v[1]))*vdecay;
  q[0]=max(BX0,min(BX1,q[0]+v[0]));
  q[1]=max(BY0,min(BY1,q[1]+v[1]));
}
//--------------------------------------
void Player::draw(Game *pG){
  int cx = pG->pCamera->q[0]*WX2SX;
  int cy = pG->pCamera->q[1]*WY2SY;
  int px = pG->pPlayer->q[0]*WX2SX;
  int py = pG->pPlayer->q[1]*WY2SY;
  float _vr=sqrt(v[0]*v[0]+v[1]*v[1]);
  float ivr = 1.0f/_vr;
  float dx = v[0]*ivr;
  float dy = v[1]*ivr;
  if(_vr>10e-20){
    d[0]=dx;
    d[1]=dy;
    vr  =_vr;
  }else{
    dx=d[0];
    dy=d[1];
  }
  float cosm = cos((180.0f-60.0f)/180.0f*PI);
  float cosp = cos((180.0f+60.0f)/180.0f*PI);
  float sinm = sin((180.0f-60.0f)/180.0f*PI);
  float sinp = sin((180.0f+60.0f)/180.0f*PI);
  int x0=SX/2                              +(px-cx);
  int y0=SY/2                              +(py-cy);
  int x1=SX/2+(int)(dx*5.0f)               +(px-cx);
  int y1=SY/2+(int)(dy*5.0f)               +(py-cy);
  int x2=SX/2+(int)((cosm*dx-sinm*dy)*5.0f)+(px-cx);
  int y2=SY/2+(int)((sinm*dx+cosm*dy)*5.0f)+(py-cy);
  int x3=SX/2+(int)((cosp*dx-sinp*dy)*5.0f)+(px-cx);
  int y3=SY/2+(int)((sinp*dx+cosp*dy)*5.0f)+(py-cy);
  pG->pA->fillTriangle(x0,y0,x1,y1,x2,y2,WHITE);
  pG->pA->fillTriangle(x0,y0,x1,y1,x3,y3,WHITE);
}
//--------------------------------------
void Player::drawHp(Game *pG){
  pG->pA->drawLine(0,SY-1,h,SY-1,WHITE);
}
//--------------------------------------

