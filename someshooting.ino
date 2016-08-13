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
#define SY2WY ((float)WY/SY)
#define SX2WX ((float)WX/SX)
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

#define BGSTARS      10 // number of background stars
#define BGSTARLAYERS  3 // number of background star layers
#define ENEMIES       6 //
int frame_rate  = 60;  // frames/sec
float q_bgstar[2][BGSTARS][BGSTARLAYERS]; // position of bg stars
float q_player[2]; //position of player
float v_player[2]; //velosity of player
float q_enemy[ENEMIES][2];
float v_enemy[ENEMIES][2];
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
void movePlayer(){
  float vstep  = 0.005f;
  float vdecay = 0.9f;
  float vmax   = +10;
  float vmin   = -10;
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
}

void respawnEnemy(int e){
  int r=random(0,4);
  switch(r){
    case 0:
      q_enemy[e][0]=(((float)random(0,65536))/65536.0f)*WX*4.0f-WX*2.0f+q_player[0];
      q_enemy[e][1]=q_player[1]-WY;
    return;
    case 1:
      q_enemy[e][0]=(((float)random(0,65536))/65536.0f)*WX*4.0f-WX*2.0f+q_player[0];
      q_enemy[e][1]=q_player[1]+WY;
    return;
    case 2:
      q_enemy[e][0]=q_player[0]-WX;
      q_enemy[e][1]=(((float)random(0,65536))/65536.0f)*WY*4.0f-WY*2.0f+q_player[1];
    return;
    default:
      q_enemy[e][0]=q_player[0]+WX;
      q_enemy[e][1]=(((float)random(0,65536))/65536.0f)*WY*4.0f-WY*2.0f+q_player[1];
    return;

  }
}

void moveEnemies(){
  for(int e=0;e<ENEMIES;e++){
    float vr  = 0.01f;
    float dx = q_player[0]-q_enemy[e][0];
    float dy = q_player[1]-q_enemy[e][1];
    if(abs(dx)>WX*2||abs(dy)>WY*2){
      respawnEnemy(e);
    }else{
      vr = vr/sqrt(dx*dx+dy*dy);
      v_enemy[e][0]=dx*vr;
      v_enemy[e][1]=dy*vr;
      q_enemy[e][0]+=v_enemy[e][0];
      q_enemy[e][1]+=v_enemy[e][1];
    }
  }
}

void drawStars(){
  // draw stars------------
  float layerscale[3]={1.0f, 0.5f, 0.25f};
  for(int l=0;l<BGSTARLAYERS;l++){
    for(int s=0;s<BGSTARS;s++){
      int sy = (int)((q_bgstar[1][s][l]-q_player[1]*layerscale[l]-WY0+BY)*WY2SY)%SY;
      int sx = (int)((q_bgstar[0][s][l]-q_player[0]*layerscale[l]-WX0+BX)*WX2SX)%SX;
      int iy = sy / 8;
      int by = sy % 8;
      vram[iy*WIDTH + sx] |= 1<<by;
    }
  }
}
void drawPlayer(){
  // draw player---------
  float ivr=1.0f/sqrt(v_player[0]*v_player[0]+v_player[1]*v_player[1]);
  float dx=v_player[0]*ivr;
  float dy=v_player[1]*ivr;
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
}
void drawEnemies(){
  // draw enemies---------
  for(int e=0;e<ENEMIES;e++){
    float cs=SX2WX*5.0f;
    float dx=q_enemy[e][0]-q_player[0];
    float dy=q_enemy[e][1]-q_player[1];
    if(abs(dx) < WX/2-cs && abs(dy) < WY/2-cs){
      arduboy.drawCircle((int)(dx*WX2SX)+SX/2,(int)(dy*WY2SY)+SY/2,5,WHITE);
    }
  }
}
void drawDebug(){
#if 0
  for(int e=0;e<ENEMIES;e++){
    char *p;
    p=(char*)&q_enemy[e][0];
    vram[e+SX*0]=*p++;
    vram[e+SX*1]=*p++;
    vram[e+SX*2]=*p++;
    vram[e+SX*3]=*p++;
    p=(char*)&q_enemy[e][1];
    vram[e+SX*4]=*p++;
    vram[e+SX*5]=*p++;
    vram[e+SX*6]=*p++;
    vram[e+SX*7]=*p++;
  }
  vram[ENEMIES+0+SX*0]=0x55;
  vram[ENEMIES+0+SX*1]=0x55;
  vram[ENEMIES+0+SX*2]=0x55;
  vram[ENEMIES+0+SX*3]=0x55;
  vram[ENEMIES+0+SX*4]=0x55;
  vram[ENEMIES+0+SX*5]=0x55;
  vram[ENEMIES+0+SX*6]=0x55;
  vram[ENEMIES+0+SX*7]=0x55;
#endif
}
void loopGame(){
  // move ------------
  movePlayer();
  moveEnemies();
  // draw ------------
  arduboy.clear();
  drawStars();
  drawPlayer();
  drawEnemies();
  drawDebug();
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
for(int l=0;l<BGSTARLAYERS;l++){
  for(int s=0;s<BGSTARS;s++){
    q_bgstar[0][s][l] = (float)random(0,SX-1)/(float)SX*WX+WX0;
    q_bgstar[1][s][l] = (float)random(0,SY-1)/(float)SY*WY+WY0;
  }
}
  q_player[0] = 0.0f;
  q_player[1] = 0.0f;
  v_player[0] = 0.0f;
  v_player[1] = 0.0f;
  for(int e=0;e<ENEMIES;e++) respawnEnemy(e);
  restartGame();
}

void restartGame(){
  arduboy.clear();
  arduboy.display();
}




