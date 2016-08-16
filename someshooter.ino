#include <Arduboy.h>
#include "common.h"
#include "game.h"
#include "actor.h"
// library version is below:
// git co https://github.com/Arduboy/pA->git 3c409fefb


int curkeys = 0;
int button[KEYS]={ LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, A_BUTTON, B_BUTTON};
bool keypressed[KEYS];
Arduboy *pA = new Arduboy();

int bullets = 0;        //     number of bullets now
int frame_rate  = 60;  // frames/sec
Game *pGame;

void setup(){
  pA->beginNoLogo();
  pA->initRandomSeed();
  pGame = new Game(pA, keypressed);
  pA->setFrameRate(frame_rate);
}

long msnow=0;
long msdif=0;
long msmax=0;
void loop(){
  for(int k=0;k<KEYS;k++) keypressed[k] |= pA->pressed(button[k]); // latch key
  if (!(pA->nextFrame())) return;  
  msnow=millis();
  pGame->loop();
  msdif=millis()-msnow;  
  if(msdif>msmax)msmax=msdif;
  for(int k=0;k<KEYS;k++) keypressed[k]=0; //clear key
}

