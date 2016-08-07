#include <Arduboy.h>
#define SX (128)
#define SY ( 64)
/* world coodinate:
  (-2.0,-1.0)--(+2.0,1.0)
*/
#define WX0 (-2.0f)
#define WX1 (+2.0f)
#define WY0 (-1.0f)
#define WY1 (+1.0f)
#define WX  (WX1-WX0)
#define WY  (WY1-WY0)
#define BX0 (-100.0f)
#define BX1 (+100.0f)
#define BY0 (-100.0f)
#define BY1 (+100.0f)
#define BX  (BX1-BX0)
#define BY  (BY1-BY0)
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

#define BGSTARS 30 // number of background stars
int frame_rate  = 60;  // frames/sec
float q_bgstar[2][BGSTARS]; // position of bg stars
float q_player[2]; //position of player
float v_player[2]; //velosity of player

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
  float vstep  = 0.01f;
  float vdecay = 0.95;
  float vmax   = +2;
  float vmin   = -2;
  if(keypressed[KEY_XM]){v_player[0]+= -vstep;}
  if(keypressed[KEY_XP]){v_player[0]+= +vstep;}
  if(keypressed[KEY_YM]){v_player[1]+= -vstep;}
  if(keypressed[KEY_YP]){v_player[1]+= +vstep;}
  v_player[0]=max(vmin,min(vmax,v_player[0]))*vdecay;
  v_player[1]=max(vmin,min(vmax,v_player[1]))*vdecay;
  q_player[0]=max(BX0,min(BX1,q_player[0]+v_player[0]));
  q_player[1]=max(BY0,min(BY1,q_player[1]+v_player[1]));
  float dx,dy,ivr;
  ivr=1.0f/sqrt(v_player[0]*v_player[0]+v_player[1]*v_player[1]);
  dx=v_player[0]*ivr;
  dy=v_player[1]*ivr;
  
  // draw clear------------
  arduboy.clear();
  // draw stars------------
  for(int s=0;s<BGSTARS;s++){
    int sy = (int)((q_bgstar[1][s]-q_player[1]-WY0+BY)*WY2SY)%SY;
    int sx = (int)((q_bgstar[0][s]-q_player[0]-WX0+BX)*WX2SX)%SX;
    int iy = sy / 8;
    int by = sy % 8;
    vram[iy*WIDTH + sx] |= 1<<by;
  }
  // draw player---------
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
  arduboy.fillTriangle(x0,y0,x1,y1,x2,y2,WHITE);
  arduboy.fillTriangle(x0,y0,x1,y1,x3,y3,WHITE);
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
  q_bgstar[0][s] = (float)random(0,SX-1)/(float)SX*WX+WX0;
  q_bgstar[1][s] = (float)random(0,SY-1)/(float)SY*WY+WY0;
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




