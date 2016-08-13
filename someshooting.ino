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
int button[KEYS]={ LEFT_BUTTON, RIGHT_BUTTON, UP_BUTTON, DOWN_BUTTON, A_BUTTON, B_BUTTON};
boolean keypressed[KEYS];
Arduboy arduboy;
AbPrinter text(arduboy);

#define BGSTARS      (10) // number of background stars
#define BGSTARLAYERS ( 3) // number of background star layers
#define ENEMIES      ( 1) // number of enemies
#define BULLETS      (10) // max number of bullets 
#define PLAYER_SIZE_DR (5.0f) // displayed radius of player [px]
#define PLAYER_SIZE_CR (2.0f) // collision radius of player [px]
#define ENEMY_SIZE_DR  (5.0f) // displayed radius of enemy [px]
#define ENEMY_SIZE_CR  (5.0f) // collision radius of enemy [px]
#define BULLET_SIZE_DR (2.0f) // displayed radius of bullet [px]
#define BULLET_SIZE_CR (2.0f) // collision radius of bullet [px]
int bullets = 0;        //     number of bullets now
int frame_rate  = 60;  // frames/sec
float q_bgstar[2][BGSTARS][BGSTARLAYERS]; // position of bg stars
float q_player[2];          //position of player
float v_player[2];          //velosity of player
char h_player;             // hitpoint of player
float q_enemy[ENEMIES][2];  // position of enemy e
float v_enemy[ENEMIES][2];  // velosity of enemy e
char dd_enemy[ENEMIES]; // direction of enemy e
char dt_enemy[ENEMIES]; // direction left time of enemy e
char h_enemy[ENEMIES]; // hitpoint of enemy e 
char hi_enemy;         // hitpoint displayed enemy index
char ht_enemy;         // hitpoint displayed time
bool b_bullet[BULLETS]; // is bullet shot
float q_bullet[BULLETS][2]; // position of bullet b
float v_bullet[BULLETS][2]; // velosity of bullet b
char  t_shot=0;  // shot left time
char  p_shot=0;  // shot pattern
float d_shot[2]; // shot direction
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
  float vstep  = 0.004f;
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
  h_enemy[e]=32;
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
  dt_enemy[e]=(char)random(0,120);
  dd_enemy[e]=(char)random(0,4);
}

void moveEnemies(){
    //enemy motion -----------------
  for(int e=0;e<ENEMIES;e++){
    float vstep  = 0.0000f;
    float vdecay = 0.9f;
    float vmax   = +10;
    float vmin   = -10;
    float dx = q_player[0]-q_enemy[e][0];
    float dy = q_player[1]-q_enemy[e][1];
    float dr   = sqrt(dx*dx+dy*dy);
    float idr  = 1.0f/dr;
    if(abs(dx)>WX*1.1||abs(dy)>WY*1.1){
      respawnEnemy(e);
    }else{
      // collision to player ----------
      if(dr<SX2WX*(PLAYER_SIZE_CR+ENEMY_SIZE_CR)){
        h_player--;
        if(h_player==0) resetGame();
      }
      // close to player ----------
      float esidr = idr*vstep;
      if(dt_enemy[e]--<0){
        dt_enemy[e]=random(0,120);
        dd_enemy[e]=random(0,4);
      }
      switch(dd_enemy[e]){
        case 0: case 1:
        v_enemy[e][0]+=dx*esidr;
        v_enemy[e][1]+=dy*esidr;
        break;
        case 2:
        v_enemy[e][0]+=dy*esidr;
        v_enemy[e][1]-=dx*esidr;
        break;
        default:
        v_enemy[e][0]-=dy*esidr;
        v_enemy[e][1]+=dx*esidr;
        break;
      }
      // keep apart from other enemies ------------
      for(int e2=0;e2<ENEMIES;e2++){
        if(e2!=e){
          float dx2=q_enemy[e][0]-q_enemy[e2][0];
          float dy2=q_enemy[e][1]-q_enemy[e2][1];
          float cs=SX2WX*10.0f;
          if(abs(dx2)<cs&&abs(dy2)<cs){
            v_enemy[e][0]+=dx2*esidr;
            v_enemy[e][1]+=dy2*esidr;
          }
        }
      }
      v_enemy[e][0]=max(vmin,min(vmax,v_enemy[e][0]))*vdecay;
      v_enemy[e][1]=max(vmin,min(vmax,v_enemy[e][1]))*vdecay;
      q_enemy[e][0]+=v_enemy[e][0];
      q_enemy[e][1]+=v_enemy[e][1];
    }
    //fire -----------------
#define FIREFRAMES (32)
#define BULLETSPEED (0.04f)
    float br=random(0,FIREFRAMES);
    if(br==0 && bullets<BULLETS){
      int b=0;
      for(b=0;b<BULLETS;b++){
        if(!b_bullet[b]) break;
      }
      b_bullet[b]=true;
      q_bullet[b][0]=q_enemy[e][0];
      q_bullet[b][1]=q_enemy[e][1];
      float bsidr = idr*BULLETSPEED;
      v_bullet[b][0]=dx*bsidr;
      v_bullet[b][1]=dy*bsidr;
    }
  }//e
}
void moveShot(){
  if(t_shot>0){
    t_shot--;
    for(int e=0;e<ENEMIES;e++){
      float dx=q_enemy[e][0]-q_player[0]; // E-P
      float dy=q_enemy[e][1]-q_player[1]; // E-P
      float s=d_shot[0]*dx + d_shot[1]*dy; // s = D(E-P)'/|D| = D(E-P)'
      dx -= s*d_shot[0]; // (E-P)-sD = E-(P+sD)
      dy -= s*d_shot[1]; // (E-P)-sD = E-(P+sD)
      float dr=sqrt(dx*dx+dy*dy);
      if(dr<ENEMY_SIZE_CR*SX2WX){
        h_enemy[e]--;
        if(h_enemy[e]>0){
          ht_enemy=60;
          hi_enemy=e;
        }else{
          respawnEnemy(e);
          ht_enemy=0;
        }
      }
    }
  }else{
    if(keypressed[KEY_A]){
      d_shot[0]=v_player[0];
      d_shot[1]=v_player[1];
      float idr=1.0f/sqrt(d_shot[0]*d_shot[0]+d_shot[1]*d_shot[1]);
      d_shot[0]*=idr;
      d_shot[1]*=idr;
      p_shot = random(0,2);
      t_shot = 3;
    }
  }
}
void moveBullets(){
  for(int b=0;b<BULLETS;b++){
    if(b_bullet[b]){
      q_bullet[b][0]+=v_bullet[b][0];
      q_bullet[b][1]+=v_bullet[b][1];
      float dx = q_player[0]-q_bullet[b][0];
      float dy = q_player[1]-q_bullet[b][1];
      float dr = sqrt(dx*dx+dy*dy);
      if(abs(dx)>WX||abs(dy)>WY){
        b_bullet[b]=false;
      }
      if(dr<SX2WX*(BULLET_SIZE_CR+PLAYER_SIZE_CR)){
        h_player--;
        if(h_player==0) resetGame();
      }
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
    float cs=SX2WX*ENEMY_SIZE_DR;
    float dx=q_enemy[e][0]-q_player[0];
    float dy=q_enemy[e][1]-q_player[1];
    if(abs(dx) < WX/2-cs && abs(dy) < WY/2-cs){
      arduboy.drawCircle((int)(dx*WX2SX)+SX/2,(int)(dy*WY2SY)+SY/2,ENEMY_SIZE_DR,WHITE);
    }
  }
}
void drawBullets(){
  // draw enemies---------
  for(int b=0;b<BULLETS;b++){
    if(b_bullet[b]){
      float cs=SX2WX*BULLET_SIZE_DR;
      float dx=q_bullet[b][0]-q_player[0];
      float dy=q_bullet[b][1]-q_player[1];
      if(abs(dx) < WX/2-cs && abs(dy) < WY/2-cs){
        arduboy.drawCircle((int)(dx*WX2SX)+SX/2,(int)(dy*WY2SY)+SY/2,BULLET_SIZE_DR,WHITE);
      }
    }
  }
}
void drawShot(){
  if(t_shot){
    char dx,dy;
    if(abs(d_shot[0])*WY<abs(d_shot[1])*WX){
      if(d_shot[0]>0){
        dx = +SX/2/3;
        dy = (char)(SY/2+(int)(d_shot[1]/d_shot[0]*((float)SX)/2.0f)/3.0f);
      }else{
        dx = -SX/2/3;
        dy = (char)(SY/2+(int)(d_shot[1]/d_shot[0]*((float)SX)/2.0f)/3.0f);
      }
    }else{
      if(d_shot[1]>0){
        dx = (char)(SX/2+(int)(d_shot[0]/d_shot[1]*((float)SY)/2.0f)/3.0f);
        dy = +SY/2/3;
      }else{
        dx = (char)(SX/2+(int)(d_shot[0]/d_shot[1]*((float)SY)/2.0f)/3.0f);
        dy = -SY/2/3;
      }
    }
//    arduboy.drawLine(SX/2+dx*p_shot,SY/2+dy*p_shot,SX/2+dx*(p_shot+1),SY/2+dy*(p_shot+1),WHITE);
    arduboy.drawLine(SX/2,SY/2,SX/2+(int)(d_shot[0]*(float)SX/2),SY/2+(int)(d_shot[1]*(float)SY/2),WHITE);
  }
}
void drawHp(){
  arduboy.drawLine(0,0,h_player,0,WHITE);
  if(ht_enemy>0){
    arduboy.drawLine(0,3,h_enemy[hi_enemy],3,WHITE);
    ht_enemy--;
  }
}
void drawDebug(){
#if 1
#endif
}
void loopGame(){
  // move ------------
  movePlayer();
  moveEnemies();
  moveBullets();
  moveShot();
  // draw ------------
  arduboy.clear();
  drawStars();
  drawPlayer();
  drawEnemies();
  drawBullets();
  drawShot();
  drawHp();
  drawDebug();// debug
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
  h_player = 127;
  hi_enemy = 0;
  ht_enemy = 0;
  for(int b=0;b<BULLETS;b++) b_bullet[b]=false;
  for(int e=0;e<ENEMIES;e++) respawnEnemy(e);
  restartGame();
}

void restartGame(){
  arduboy.clear();
  arduboy.display();
}




