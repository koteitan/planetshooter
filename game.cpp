#undef DEBUG
#include <Arduboy.h>
#include "common.h"
#include "game.h"

//--------------------------------------
Game::Game(Arduboy *_pA, bool *_kp){
  state = eGAME_STT_PLAY;
  keypressed = _kp;
  pA = _pA;
  hiscore=0;
  pCamera = new Camera();
  pPlayer = new Player();
  pShot   = new Shot  ();
  for(int i=0;i<ENEMIES;i++) pEnemy [i]=new Enemy ();
  for(int i=0;i<BULLETS;i++) pBullet[i]=new Bullet();
  for(int i=0;i<DEBRIS ;i++) pDebri [i]=new Debri ();
  for(int l=0;l<BGSTARLAYERS;l++){
    for(int s=0;s<BGSTARS;s++) pBgstar[s][l] = new BGStar();
  }
  iAnime    = 0;
  iAnimeMax = 2;
  geState = eGE_STT_IDLE;
  pGE = new GraphicEffect(pA);
  reset();
}
//--------------------------------------
void Game::reset(void){
  for(int l=0;l<BGSTARLAYERS;l++){
    for(int s=0;s<BGSTARS;s++){
      pBgstar[s][l]->q[0] = (float)random(0,SX-1)/(float)SX*WX*BGSTARLAYERS+WX0;
      pBgstar[s][l]->q[1] = (float)random(0,SY-1)/(float)SY*WY*BGSTARLAYERS+WY0;
    }
  }
  pPlayer->q[0] =  0.0f;
  pPlayer->q[1] =  0.0f;
  pPlayer->v[0] = +10e-20;
  pPlayer->v[1] =  0.0f;
  pPlayer->d[0] = +1.0f;
  pPlayer->d[1] =  0.0f;
  
  pCamera->q[0] = pPlayer->q[0];
  pCamera->q[1] = pPlayer->q[1];
  pCamera->v[0] =  0.0f;
  pCamera->v[1] =  0.0f;
  pPlayer->h = 127;
  for(int i=0;i<ENEMIES;i++) {pEnemy [i]->respawn(this);}
  for(int i=0;i<BULLETS;i++) pBullet[i]->b=false;
  tEnemyHp  = 0;
  iEnemyHp  = 0;
  debris    = 0;
  iDebris   = 0;
  bullets   = 0;

  score = 0;
  t_died = 60;
  state = eGAME_STT_PLAY;
  geState = eGE_STT_PLAYING;  
  geTimeNow = 0;
  geSeqNow  = 0;
  geType = random(0,geTypeMax);
  switch(geType){
    case 0:
      geSeqMax  = 4;
      geTimeMax = 6; 
      geSeqStep = 1; 
    break;
    case 1:
      geSeqMax  = 64;
      geTimeMax =  0; 
      geSeqStep =  1; 
    break;
    case 2:
      geSeqMax  = 16;
      geTimeMax =  0; 
      geSeqStep =  2; 
    break;
    default:
    break;
  }
  geDamageTimeNow=0;
  pA->clear();
}
void Game::drawScore(void){
  char str [6];
  pA->setCursor(0*6,SY-8-3);
  pA->print("SCORE:");
  pA->setCursor(6*6,SY-8-3);
  sprintf(str, "% 5d",score);
  pA->print(str);

  pA->setCursor(12*6,SY-8-3);
  pA->print("HI:");
  pA->setCursor(15*6,SY-8-3);
  sprintf(str, "% 5d",hiscore);
  pA->print(str);
}
//--------------------------------------
void Game::loop(void){

  if(geState==eGE_STT_PLAYING){
    //do graphic effect
    drawAll();
    switch(geType){
      case 0:
      pGE->mosaic(geSeqMax-geSeqNow-1);
      break;
      case 1:
      pGE->lightning(geSeqMax-geSeqNow-1);
      break;
      case 2:
      pGE->devide(geSeqMax-geSeqNow-1);
      break;
      default:
      break;
    }
    if(geTimeNow>geTimeMax){
      geTimeNow=0;
      geSeqNow += geSeqStep;
    }else{
      geTimeNow++;
    }
    if(geSeqNow>geSeqMax){
      geState=eGE_STT_IDLE;
    }
    return;
  }

  // move 
  pCamera->move(this);
  if(state==eGAME_STT_PLAY) pPlayer->move(this);
  bool isAlive = true;
  for(int i=0;i<ENEMIES;i++) isAlive &= pEnemy [i]->move(this);
  for(int i=0;i<BULLETS;i++) isAlive &= pBullet[i]->move(this);
  pShot->move(this);
  Debri::moveAll(this);

  // score
  hiscore = max(score, hiscore);

  // draw 
  drawAll();
  
  // inclement anime 
  iAnime=(iAnime+1) % iAnimeMax;
  if(!isAlive){
    state=eGAME_STT_DIED;
    int newdebris = 10;
    for(int n=0;n<newdebris||debris<DEBRIS;n++){
      Debri *pD=pDebri[iDebris];
      pD->t=20;
      pD->v[0]=pPlayer->v[0]*0.05f+((float)random(0,200)/100.0f-1.0f)*0.02f;
      pD->v[1]=pPlayer->v[1]*0.05f+((float)random(0,200)/100.0f-1.0f)*0.02f;
      pD->q[0]=pPlayer->q[0];
      pD->q[1]=pPlayer->q[1];
      iDebris=(iDebris+1)%DEBRIS;
      debris++;
    }
  }

  // state
  if(state==eGAME_STT_DIED){
    if(t_died--<=0){
      reset();
    }
  }
}
void Game::drawAll(){
  pA->clear();
//  pCamera->draw(this); // for debug
  if(state==eGAME_STT_PLAY) pPlayer->draw(this);
  for(int i=0;i<ENEMIES;i++) pEnemy [i]->draw(this);
  for(int i=0;i<BULLETS;i++) pBullet[i]->draw(this);
  Debri::drawAll(this);
  pShot->draw(this);
  for(int l=0;l<BGSTARLAYERS;l++){
    for(int s=0;s<BGSTARS;s++){
      pBgstar[s][l]->draw(this,l);
    }
  }
  pPlayer->drawHp(this);
  drawEnemyHp();
  drawDebug();// debug
  drawScore();
  if(geState != eGE_STT_PLAYING){
    if(geDamageTimeNow>0){
      pGE->glitch(random(0,1024),random(0,1024));
      geDamageTimeNow--;
    }else{
      pA->display();
    }
  }
}
//--------------------------------------
void Game::drawEnemyHp(){
  if(tEnemyHp>0){
    pA->drawLine(0,SY-3,pEnemy[iEnemyHp]->h,SY-3,WHITE);
    tEnemyHp--;
  }
}
//--------------------------------------
void Game::drawDebug(void){
#if 1
  int i=0;
  char str[20];
#endif
//pA->setCursor(0,i*8);pA->print(getMargin());i++;
#if 0
  pA->setCursor(0,i*8);pA->print(fDbg[i]);i++;
  pA->setCursor(0,i*8);pA->print(fDbg[i]);i++;
  pA->setCursor(0,i*8);pA->print(fDbg[i]);i++;
  pA->setCursor(0,i*8);pA->print(fDbg[i]);i++;
  pA->setCursor(0,i*8);pA->print(pPlayer->d[0]);i++;
  pA->setCursor(0,i*8);pA->print(pPlayer->d[1]);i++;
#endif
}
//--------------------------------------
