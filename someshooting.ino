#include <Arduboy.h>
#define SX (128)
#define SY ( 64)
/* world coodinate:
  (-2.0,-1.0)--(+2.0,1.0)
*/
#define WX  ( 4.0f)
#define WY  ( 2.0f)
#define WX0 (-2.0f)
#define WX1 (+2.0f)
#define WY0 (-1.0f)
#define WY1 (+1.0f)
#define WY2SY ((float)SY/WY)
#define WX2SX ((float)SX/WX)
unsigned char *vram; // =arduboy.getBuffer()

#define KEY_XM 0
#define KEY_XP 1
#define KEY_YM 2
#define KEY_YP 3
#define KEY_B  4
#define KEY_A  5
#define KEYS   6
int curkeys = 0;
int button[KEYS]={ LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, B_BUTTON, A_BUTTON};
boolean keypressed[KEYS];
Arduboy arduboy;
AbPrinter text(arduboy);

#define BGSTARS 100 // number of background stars
int frame_rate  = 60;  // frames/sec
float q_bgstar[2][BGSTARS]; // position of bg stars
float q_player[2]; //position of player
float v_player[2]; //verosity of player

void setup(){
  arduboy.begin();
  arduboy.initRandomSeed();
  vram = arduboy.getBuffer();
  initGame();
  arduboy.setFrameRate(frame_rate);
}
void initGame(){
  resetGame();
}

long msnow=0;
long msdif=0;
long msmax=0;

void loop(){
  for(int k=0;k<KEYS;k++) keypressed[k] |= arduboy.pressed(button[k]); // latch key
  if (!(arduboy.nextFrame())) return;
  
  msnow=millis();
  loopGame();
  msdif=millis()-msnow;
  
  if(msdif>msmax)msmax=msdif;
  for(int k=0;k<KEYS;k++) keypressed[k]=0; //clear key
}

void loopGame(){
  float vstep  = 0.001f;
  float vdecay = 0.9;
  float vmax   = 1;
  if(keypressed[KEY_XM]){v_player[0]+= -vstep;}
  if(keypressed[KEY_XP]){v_player[0]+= +vstep;}
  if(keypressed[KEY_YM]){v_player[1]+= -vstep;}
  if(keypressed[KEY_YP]){v_player[1]+= +vstep;}
  v_player[0]=min(vmax,v_player[0])*vdecay;
  v_player[1]=min(vmax,v_player[1])*vdecay;
  q_player[0]+=v_player[0];
  q_player[1]+=v_player[1];

  arduboy.clear();
  for(int s=0;s<BGSTARS;s++){
    int sy = (int)((q_bgstar[1][s]-q_player[1]-WY0)*WY2SY)%SY;
    int sx = (int)((q_bgstar[0][s]-q_player[0]-WX0)*WX2SX)%SX;
    int iy = sy / 8;
    int by = sy-iy*8;
    vram[iy*WIDTH + sx] |= 1<<by;
  }
  arduboy.display();
}

String ralign(int i, int n){
  int spc=0;
  if(    0<=+i && +i<=    9) spc=n-1;
  if(   10<=+i && +i<=   99) spc=n-2;
  if(  100<=+i && +i<=  999) spc=n-3;
  if( 1000<=+i && +i<= 9999) spc=n-4;
  if(10000<=+i             ) spc=n-5;
  if(    0< -i && -i<=    9) spc=n-2;
  if(   10<=-i && -i<=   99) spc=n-3;
  if(  100<=-i && -i<=  999) spc=n-4;
  if( 1000<=-i && -i<= 9999) spc=n-5;
  if(10000<=-i             ) spc=n-6;
  String str="";
  for(int c=0;c<spc;c++)str+=" ";
  return str+String(i);
}


void resetGame(){
for(int s=0;s<BGSTARS;s++){
  q_bgstar[0][s] = (float)random(0,SX-1)/(float)SX;
  q_bgstar[1][s] = (float)random(0,SY-1)/(float)SY;
}
  q_player[0] = 0.0f;
  q_player[1] = 0.0f;
  v_player[0] = 0.0f;
  v_player[1] = 0.0f;
  restartGame();
}

void restartGame(){
  arduboy.clear();
  arduboy.display();
}




