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
void BGStar::draw(Game *pG, int l){
      int sy = (int)(((q[1]-pG->pCamera->q[1])/((double)l+1)-WY0+BY)*WY2SY)%SY;
      int sx = (int)(((q[0]-pG->pCamera->q[0])/((double)l+1)-WX0+BX)*WX2SX)%SX;
      int iy = sy / 8;
      int by = sy % 8;
      pG->pA->getBuffer()[iy*WIDTH + sx] |= 1<<by;
}
