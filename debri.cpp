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
//-----------------------------------
void Debri::init(Game *pG){
  pG->iDebris=0;
  pG->debris =0;
}
void Debri::moveAll(Game *pG){
  for(int n=0;n<pG->debris;n++){
    int d=(pG->iDebris+n)%DEBRIS;
    pG->pDebri[d]->move(pG);
  };
}
void Debri::drawAll(Game * pG){
  for(int n=0;n<pG->debris;n++){
    int d=(pG->iDebris+n)%DEBRIS;
    pG->pDebri[d]->draw(pG);
  };
}
//-----------------------------------
void Debri::move(Game *pG){
  q[0]+=v[0];
  q[1]+=v[1];
  v[0]*=0.95;
  v[1]*=0.95;
  t--;
  if(t==0){
    pG->iDebris=(pG->iDebris+1)%DEBRIS;
    pG->debris--;
  }
}
void Debri::draw(Game *pG){
  float dx=q[0]-pG->pCamera->q[0];
  float dy=q[1]-pG->pCamera->q[1];
  if(abs(dx)<WX && abs(dy)<WY){
    pG->pA->drawPixel(dx*WX2SX+SX/2,dy*WY2SY+SY/2,WHITE);
  }
}
