#undef DEBUG
#include <Arduboy.h>
#include "common.h"
#include "game.h"
Game::Game(Arduboy *_pA, bool *_kp){
  keypressed = _kp;
  pA = _pA;
  hiscore=0;
  pPlayer = new Player();
  pShot   = new Shot  ();
  for(int i=0;i<ENEMIES;i++) pEnemy [i]=new Enemy ();
  for(int i=0;i<BULLETS;i++) pBullet[i]=new Bullet();
  for(int i=0;i<DEBRIS ;i++) pDebri [i]=new Debri ();
  for(int l=0;l<BGSTARLAYERS;l++){
    for(int s=0;s<BGSTARS;s++) pBgstar[s][l] = new BGStar();
  }
  tEnemyHp  = 0;
  iEnemyHp  = 0;
  debris    = 0;
  iDebris   = 0;
  iAnime    = 0;
  iAnimeMax = 2;
  reset();
}
void Game::reset(void){
  for(int l=0;l<BGSTARLAYERS;l++){
    for(int s=0;s<BGSTARS;s++){
      pBgstar[s][l]->q[0] = (float)random(0,SX-1)/(float)SX*WX+WX0;
      pBgstar[s][l]->q[1] = (float)random(0,SY-1)/(float)SY*WY+WY0;
    }
  }
  pPlayer->q[0] = 0.0f;
  pPlayer->q[1] = 0.0f;
  pPlayer->v[0] = 0.0f;
  pPlayer->v[1] = 0.0f;
  pPlayer->h = 127;
  for(int i=0;i<ENEMIES;i++) {pEnemy [i]->respawn(this);}
  for(int i=0;i<BULLETS;i++) pBullet[i]->b=false;
  score=0;
  pA->clear();
  pA->display();
  iAnime=0;
}
void Game::drawScore(void){
  pA->setCursor(0,SY-8-3);
  pA->print("SCORE:"+ralign(score,5)+" HI:"+ralign(hiscore,5));
}
void Game::loop(void){
  // move ------------
  pPlayer->move(this);
  bool isAlive = true;
  for(int i=0;i<ENEMIES;i++) isAlive &= pEnemy [i]->move(this);
  for(int i=0;i<BULLETS;i++) isAlive &= pBullet[i]->move(this);
  pShot->move(this);
  Debri::moveAll(this);
  // draw ------------
  pA->clear();
  drawScore();
  pPlayer->draw(this);
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
//  drawDebug();// debug
  pA->display();
  // inclement anime
  iAnime=(iAnime+1) % iAnimeMax;
  if(!isAlive){
    reset();
  }
}
void Game::drawEnemyHp(){
  if(tEnemyHp>0){
    pA->drawLine(0,SY-3,pEnemy[iEnemyHp]->h,SY-3,WHITE);
    iEnemyHp--;
  }
}
void Game::drawDebug(void){
#ifdef DEBUG
  int y=0;
  pA->setCursor(0,y+=8);
  pA->print(String((long)pEnemy[0])+" "+String(fDebug[0]));
  pA->setCursor(0,y+=8);
  pA->print(String((long)pEnemy[1])+" "+String(fDebug[1]));
  pA->setCursor(0,y+=8);
  pA->print(String((long)pEnemy[2])+" "+String(fDebug[2]));
#endif
}




