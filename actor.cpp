#include <Arduboy.h>
#include "common.h"
#include "game.h"
#include "actor.h"
//BGStars ---------------------------
//Player --------------------------------------
void Player::move(Game *pG){
  float vstep  = 0.008f;
  float vdecay = 0.9f;
  float vmax   = +10;
  float vmin   = -10;
  if(pG->keypressed[KEY_XM]){v[0]+= -vstep;}
  if(pG->keypressed[KEY_XP]){v[0]+= +vstep;}
  if(pG->keypressed[KEY_YM]){v[1]+= -vstep;}
  if(pG->keypressed[KEY_YP]){v[1]+= +vstep;}
  v[0]=max(vmin,min(vmax,v[0]))*vdecay;
  v[1]=max(vmin,min(vmax,v[1]))*vdecay;
  q[0]=max(BX0,min(BX1,q[0]+v[0]));
  q[1]=max(BY0,min(BY1,q[1]+v[1]));
  float dx,dy,ivr;
  ivr=1.0f/sqrt(v[0]*v[0]+v[1]*v[1]);
  dx=v[0]*ivr;
  dy=v[1]*ivr;
}
void Player::draw(Game *pG){
  float ivr=1.0f/sqrt(v[0]*v[0]+v[1]*v[1]);
  float dx=v[0]*ivr;
  float dy=v[1]*ivr;
  float cosm = cos((180.0f-60.0f)/180.0f*PI);
  float cosp = cos((180.0f+60.0f)/180.0f*PI);
  float sinm = sin((180.0f-60.0f)/180.0f*PI);
  float sinp = sin((180.0f+60.0f)/180.0f*PI);
  int x0=SX/2;
  int y0=SY/2;
  int x1=SX/2+(int)(dx*5.0f);
  int y1=SY/2+(int)(dy*5.0f);
  int x2=SX/2+(int)((cosm*dx-sinm*dy)*5.0f);
  int y2=SY/2+(int)((sinm*dx+cosm*dy)*5.0f);
  int x3=SX/2+(int)((cosp*dx-sinp*dy)*5.0f);
  int y3=SY/2+(int)((sinp*dx+cosp*dy)*5.0f);
  pG->pA->fillTriangle(x0,y0,x1,y1,x2,y2,WHITE);
  pG->pA->fillTriangle(x0,y0,x1,y1,x3,y3,WHITE);
}
void Player::drawHp(Game *pG){
  pG->pA->drawLine(0,SY-1,h,SY-1,WHITE);
}
//shot----------------------------------------
void Shot::move(Game *pG){
  Player *pP = pG->pPlayer;
  if(t>0){
    t--;
    for(int e=0;e<ENEMIES;e++){
      Enemy *pE=pG->pEnemy[e];
      float dx=pE->q[0]-pP->q[0]; // E-P
      float dy=pE->q[1]-pP->q[1]; // E-P
      float s=d[0]*dx + d[1]*dy; // s = D(E-P)'/|D| = D(E-P)'
      dx -= s*d[0]; // (E-P)-sD = E-(P+sD)
      dy -= s*d[1]; // (E-P)-sD = E-(P+sD)
      float dr=sqrt(dx*dx+dy*dy);
      if(dr<ENEMY_SIZE_CR*SX2WX && s>0){
        pE->h--;
        char newdebris;
        if(pE->h>0){
          pG->tEnemyHp=60;
          pG->iEnemyHp=e;
          newdebris=1;
        }else{
          //enemy dies
          pG->score+=100;
          pG->hiscore=max(pG->score,pG->hiscore);
          pE->respawn(pG);
          pG->tEnemyHp=0;
          newdebris=10;
        }
        for(int n=0;n<newdebris||pG->debris<DEBRIS;n++){
          Debri *pD=pG->pDebri[pG->iDebris];
          pD->t=20;
          pD->v[0]=d[0]*0.05f+((float)random(0,200)/100.0f-1.0f)*0.02f;
          pD->v[1]=d[1]*0.05f+((float)random(0,200)/100.0f-1.0f)*0.02f;
          pD->q[0]=pE->q[0];
          pD->q[1]=pE->q[1];
          pG->iDebris=(pG->iDebris+1)%DEBRIS;
          pG->debris++;
        }
      }
    }
  }else{
    if(pG->keypressed[KEY_A]){
      d[0]=pP->v[0];
      d[1]=pP->v[1];
      float idr=1.0f/sqrt(d[0]*d[0]+d[1]*d[1]);
      d[0]*=idr;
      d[1]*=idr;
      dp = random(0,3);
      t  = 3;
    }
  }
}
//Enemy ----------------------------------------
bool Enemy::move(Game *pG){
  bool isPlayerAlive=true;
  float vstep  = 0.002f;
  float vdecay = 0.9f;
  float vmax   = +10;
  float vmin   = -10;
  float dx = pG->pPlayer->q[0]-q[0];
  float dy = pG->pPlayer->q[1]-q[1];
  float dr   = sqrt(dx*dx+dy*dy);
  float idr  = 1.0f/dr;
  if(abs(dx)>WX*1.1||abs(dy)>WY*1.1){
    respawn(pG);
  }else{
    // collision to player ----------
    if(dr<SX2WX*(PLAYER_SIZE_CR+ENEMY_SIZE_CR)){
      pG->pPlayer->h-=10;
      if(pG->pPlayer->h<=0){
        isPlayerAlive = false;
      }
    }
    // close to player ----------
    float esidr = idr*vstep;
    if(dt--<0){
        dt=random(0,120);
        dd=random(0,4);
    }
    switch(dd){
      case 0: case 1:
        v[0]+=dx*esidr;
        v[1]+=dy*esidr;
      break;
      case 2:
        v[0]+=dy*esidr;
        v[1]-=dx*esidr;
      break;
      default:
        v[0]-=dy*esidr;
        v[1]+=dx*esidr;
      break;
    }
    // keep apart from other enemies ------------
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
    v[0]=max(vmin,min(vmax,v[0]))*vdecay;
    v[1]=max(vmin,min(vmax,v[1]))*vdecay;
    q[0]+=v[0];
    q[1]+=v[1];
  }
  //fire -----------------
  #define FIREFRAMES (32)
  #define BULLETSPEED (0.04f)
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
void Enemy::draw(Game *pG){
  float cs=SX2WX*ENEMY_SIZE_DR;
  float dx=q[0]-pG->pPlayer->q[0];
  float dy=q[1]-pG->pPlayer->q[1];
  if(abs(dx) < WX/2-cs && abs(dy) < WY/2-cs){
    pG->pA->drawCircle((int)(dx*WX2SX)+SX/2,(int)(dy*WY2SY)+SY/2,ENEMY_SIZE_DR,WHITE);
  }
}
void Enemy::respawn(Game *pG){
  h=32;
  Player *pP=pG->pPlayer;
  int r=random(0,4);
  switch(r){
    case 0:
      q[0]=(((float)random(0,65536))/65536.0f)*WX*4.0f-WX*2.0f+pP->q[0];
      q[1]=pP->q[1]-WY;
    return;
    case 1:
      q[0]=(((float)random(0,65536))/65536.0f)*WX*4.0f-WX*2.0f+pP->q[0];
      q[1]=pP->q[1]+WY;
    return;
    case 2:
      q[0]=pP->q[0]-WX;
      q[1]=(((float)random(0,65536))/65536.0f)*WY*4.0f-WY*2.0f+pP->q[1];
    return;
    default:
      q[0]=pP->q[0]+WX;
      q[1]=(((float)random(0,65536))/65536.0f)*WY*4.0f-WY*2.0f+pP->q[1];
    return;
  }
  dt=(char)random(0,120);
  dd=(char)random(0,4);
}
//Bullet ----------------------------------
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
void Bullet::draw(Game *pG){
  if(b){
    float cs=SX2WX*BULLET_SIZE_DR;
    float dx=q[0]-pG->pPlayer->q[0];
    float dy=q[1]-pG->pPlayer->q[1];
    if(abs(dx) < WX/2-cs && abs(dy) < WY/2-cs){
      pG->pA->drawCircle((int)(dx*WX2SX)+SX/2,(int)(dy*WY2SY)+SY/2,BULLET_SIZE_DR-pG->iAnime,WHITE);
    }
  }
}
// Back Ground Stars-----------------------
void BGStar::draw(Game *pG, int l){
  float layerscale[3]={1.0f, 0.5f, 0.25f};
      int sy = (int)((q[1]-pG->pPlayer->q[1]*layerscale[l]-WY0+BY)*WY2SY)%SY;
      int sx = (int)((q[0]-pG->pPlayer->q[0]*layerscale[l]-WX0+BX)*WX2SX)%SX;
      int iy = sy / 8;
      int by = sy % 8;
      pG->pA->getBuffer()[iy*WIDTH + sx] |= 1<<by;
}
//shot --------------------------
void Shot::draw(Game *pG){
  if(t){
    int dx=(int)(d[0]*(float)SX/6);
    int dy=(int)(d[1]*(float)SY/6);
    pG->pA->drawLine(SX/2+dx*(dp+0),
                     SY/2+dy*(dp+0),
                     SX/2+dx*(dp+1),
                     SY/2+dy*(dp+1),WHITE);
  }
}
//Debri ---------------------------
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
  float dx=q[0]-pG->pPlayer->q[0];
  float dy=q[1]-pG->pPlayer->q[1];
  if(abs(dx)<WX && abs(dy)<WY){
    pG->pA->drawPixel(dx*WX2SX+SX/2,dy*WY2SY+SY/2,WHITE);
  }
}