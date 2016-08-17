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
void Shot::move(Game *pG){
  float knockback = 0.6f; // enemy knockback
  Player *pP = pG->pPlayer;
  if(t>0){
    t--;
    for(int e=0;e<ENEMIES;e++){
      Enemy *pE=pG->pEnemy[e];
      float dx=pE->q[0]-pP->q[0]; // E-P
      float dy=pE->q[1]-pP->q[1]; // E-P
      float s=d[0]*dx + d[1]*dy; // s = D(E-P)'/|D| = D(E-P)'
      float cx = dx-s*d[0]; // (E-P)-sD = E-(P+sD)
      float cy = dy-s*d[1]; // (E-P)-sD = E-(P+sD)
      float dr=sqrt(cx*cx+cy*cy);
      if(dr<ENEMY_SIZE_CR*SX2WX && s>0){
        pE->h--;
        char newdebris;
        if(pE->h>0){
          pG->tEnemyHp=60;
          pG->iEnemyHp=e;
          newdebris=2;
          pE->v[0]+=dx*knockback;
          pE->v[1]+=dy*knockback;
        }else{
          //enemy dies
          pG->score += 100;
          pE->respawn(pG);
          pG->tEnemyHp=0;
          pG->iEnemyHp=ENEMIES;
          newdebris=4;
        }
        for(int n=0;n<newdebris||pG->debris<DEBRIS;n++){
          Debri *pD=pG->pDebri[pG->iDebris];
          pD->t=20;
          pD->v[0]=d[0]*0.05f+((float)random(0,200)/100.0f-1.0f)*0.02f;
          pD->v[1]=d[1]*0.05f+((float)random(0,200)/100.0f-1.0f)*0.02f;
          pD->q[0]=pE->q[0]-cx;
          pD->q[1]=pE->q[1]-cy;
          pG->iDebris=(pG->iDebris+1)%DEBRIS;
          pG->debris++;
        }
      }
    }
  }else{
    if(pG->keypressed[KEY_A] && pG->state!=eGAME_STT_DIED){
      d[0]=pP->d[0];
      d[1]=pP->d[1];
      dp = random(0,4);
      t  = 1;
    }
  }
}
//----------------------------------------
void Shot::draw(Game *pG){
  if(t){
    int px = pG->pPlayer->q[0]*WX2SX;
    int py = pG->pPlayer->q[1]*WY2SY;
    int cx = pG->pCamera->q[0]*WX2SX;
    int cy = pG->pCamera->q[1]*WY2SY;
    int dx=(int)(d[0]*(float)WX2SX*1);
    int dy=(int)(d[1]*(float)WY2SY*1);
    pG->pA->drawLine(SX/2+dx*(dp+0)+(px-cx),
                     SY/2+dy*(dp+0)+(py-cy),
                     SX/2+dx*(dp+1)+(px-cx),
                     SY/2+dy*(dp+1)+(py-cy),WHITE);
  }
}
//----------------------------------------
