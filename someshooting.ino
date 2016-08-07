#include <Arduboy.h>
#define WX 64
#define WY 32
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

int frame_rate  = 4;  // frames/sec

int ri=0;
/* 
  histP[i] = XXXX XXXD
  P       = player        : 0=own 1=enemy
  D       = dir    to walk: 0=x 1=y
  XXXXXXX = number to walk: -31 to +31
*/
#define HISTMAX 100 // oh my memory
// 0=player 1=COM h=head t=tail
uint8_t hist[2][HISTMAX];
int  x[2][2],  y[2][2];// {x,y}[p][c] =            current position of player p+1 of {head,tail}[c]
int dx[2][2], dy[2][2];// {x,y}[p][c] = ={-1,0,+1} current position of player p+1 of {head,tail}[c] 
int ih[2][2];          // ih[p][c] =         history buffer pointer of player p+1 of {head,tail}[c]
int ax, ay; // position of apple
int score[2];
int bet=0;

uint8_t isWall(int x, int y);
void addHist(uint8_t *hist, int *i, int d, uint8_t isy);
void incHist(uint8_t *hist, int *i);
void decHist(uint8_t *hist, int *i);
int16_t getLeft(uint8_t *hist, int *i);
void gameover(int iswin);

void setup(){
  score[0]=0;
  score[1]=0;
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
  if(keypressed[KEY_A]&&keypressed[KEY_B]) resetGame();
  if (!(arduboy.nextFrame())) return;
  
  msnow=millis();
  loopGame();
  msdif=millis()-msnow;
  
  if(msdif>msmax)msmax=msdif;
  for(int k=0;k<KEYS;k++) keypressed[k]=0; //clear key
}

void loopGame(){
  uint8_t iy=0, by=0;
  int gotApple = 2; // 2 = nobody, 0=P1, 1=P2
  bet++;
  
  //input for P1 (key -> dx,dy)
  if(!(keypressed[KEY_XM]||keypressed[KEY_XP]||keypressed[KEY_YM]||keypressed[KEY_YP])){
    incHist(hist[0],&ih[0][0]);
  }else{
    if(keypressed[KEY_XM]){
      if(dx[0][0]<0){
        incHist(hist[0],&ih[0][0]);
      }else if(dx[0][0]==0){
        addHist(hist[0],&ih[0][0],-1,0);
        dx[0][0]=-1;
        dy[0][0]= 0;
      }
    }
    if(keypressed[KEY_XP]){
      if(dx[0][0]>0){
        incHist(hist[0],&ih[0][0]);
      }else if(dx[0][0]==0){
        addHist(hist[0],&ih[0][0],+1,0);
        dx[0][0]=+1;
        dy[0][0]= 0;
      }
    }
    if(keypressed[KEY_YM]){
      if(dy[0][0]<0){
        incHist(hist[0],&ih[0][0]);
      }else if(dy[0][0]==0){
        addHist(hist[0],&ih[0][0],-1,1);
        dx[0][0]= 0;
        dy[0][0]=-1;
      }
    }
    if(keypressed[KEY_YP]){
      if(dy[0][0]>0){
        incHist(hist[0],&ih[0][0]);
      }else if(dy[0][0]==0){
        addHist(hist[0],&ih[0][0],+1,1);
        dx[0][0]= 0;
        dy[0][0]=+1;
      }
    }
  }
  
  //judge P1
  if(x[0][0]+dx[0][0]==ax && y[0][0]+dy[0][0]==ay){
    gotApple = 0;
  }else if(isWall(x[0][0]+dx[0][0], y[0][0]+dy[0][0])){
    gameover(1);
  }
  
  //input for P2 (AI -> dx,dy)
  if(x[1][0]+dx[1][0]==ax && x[1][0]+dy[1][0]==ay){
    // if got apple
    gotApple = 1;
    incHist(hist[1],&ih[1][0]);
  }else if(!isWall(x[1][0]+dx[1][0], y[1][0]+dy[1][0]) && random(0,100)>1){
    // if p2 can go straight
    incHist(hist[1],&ih[1][0]);
  }else{
    // when p2 can not go straight
    int d  = random(0,2)*2-1; // randomize the trying order =-1 or +1 
    int md = -d;
    if(dx[1][0]==0){
      if     (!isWall(x[1][0]+ d, y[1][0])){
        addHist(hist[1],&ih[1][0],d,0);
        dx[1][0]= d;
        dy[1][0]= 0;
      }
      else if(!isWall(x[1][0]+md, y[1][0])){
        addHist(hist[1],&ih[1][0],d,0);
        dx[1][0]=md;
        dy[1][0]= 0;}
      else{
        gameover(0);
      }
    }else{ //dy[1][0]==0
      if     (!isWall(x[1][0], y[1][0]+ d)){
        addHist(hist[1],&ih[1][0],d,1);
        dx[1][0]= 0;
        dy[1][0]= d;
      }
      else if(!isWall(x[1][0], y[1][0]+md)){
        addHist(hist[1],&ih[1][0],d,1);
        dx[1][0]= 0;
        dy[1][0]=md;
      }
      else{
        gameover(0);
      }
    }
  }
  
  for(int p=0;p<2;p++){
    //draw head
    iy = (y[p][0]*2+dy[p][0]*1) / 8;
    by = (y[p][0]*2+dy[p][0]*1) % 8;
    vram[iy*WIDTH + x[p][0]*2+dx[p][0]*1] |= 1<<by;
    iy = (y[p][0]*2+dy[p][0]*2) / 8;
    by = (y[p][0]*2+dy[p][0]*2) % 8;
    vram[iy*WIDTH + x[p][0]*2+dx[p][0]*2] |= 1<<by;
    
    //draw tail
    /*
    iy = (y[p][0]*2+dy[p][0]*1) / 8;
    by = (y[p][0]*2+dy[p][0]*1) % 8;
    vram[iy*WIDTH + x[p][0]*2+dx[p][0]*1] &= ~(1<<by);
    iy = (y[p][0]*2+dy[p][0]*2) / 8;
    by = (y[p][0]*2+dy[p][0]*2) % 8;
    vram[iy*WIDTH + x[p][0]*2+dx[p][0]*2] &= ~(1<<by);
*/
//  for(int p=0;p<2;p++){
  }
  for(int p=0;p<2;p++){
  //move head
    x[p][0] += dx[p][0];
    y[p][0] += dy[p][0];
    
    if(p==gotApple){
      ax=random(0,WX);
      ay=random(0,WY);
      while(!isWall(ax,ay)){
        ax=random(0,WX);
        ay=random(0,WY);
      }
    }else{
      //move tail
      x[p][1] += dx[p][1];
      y[p][1] += dy[p][1];
      // new direction of tail
      if(getLeft(hist[p],&ih[p][1])==0) ih[p][1] = (ih[p][1]+1) % HISTMAX;
    }
  }
  
  //display apple
  iy = (ay*2) / 8;
  by = (ay*2) % 8;
  vram[iy*WIDTH + ax*2] |= 1<<by;  

  //display score
  text.setCursor(0,0);
  String str="";
  str+="   "+ralign(score[0],4)+":"+ralign(bet,4)+":"+ralign(score[1],4)+"   ";
  text.print(str);

  //display debug
  if(1){
    int i=0;
//    vram[i++]=0x55; vram[i++]=(uint8_t)msdif;
//    vram[i++]=0x55; vram[i++]=(uint8_t)msmax;
//    vram[i++]=0x55; vram[i++]=random(0,2);
//    vram[i++]=0x55; vram[i++]=dy[1][0];
//    vram[i++]=0x55; vram[i++]=x[1][0];
//    vram[i++]=0x55; vram[i++]=y[1][0];
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

uint8_t isWall(int _x, int _y){
  return arduboy.getPixel(_x*2,_y*2);
}

void addHist(uint8_t *hist, int *i, int d, uint8_t isy){
  *i = (*i+1) % HISTMAX;
  hist[*i] =  (((uint8_t)d)<<1)|isy;
}
void incHist(uint8_t *hist, int *i){
  int8_t s = ((int8_t)hist[*i])>>1;
  if(s>0)s++;else s--;
  hist[*i] =  (((uint8_t)s)<<1)|hist[*i]&1;
}
void decHist(uint8_t *hist, int *i){
  int8_t s = ((int8_t)hist[*i])>>1;
  if(s>0)s--;else s++;
  hist[*i] =  (((uint8_t)s)<<1)|hist[*i]&1;
}
int16_t getLeft(uint8_t *hist, int *i){
   return (int16_t)((int8_t)hist[*i])>>1;
}

void gameover(int iswin){
  score[iswin]+=bet;
  bet=0;
  restartGame();
}
void resetGame(){
  score[0]=0;
  score[1]=0;
  restartGame();
}

void restartGame(){
  ax=random(0,WX);
  ay=random(0,WY);

  x[0][0]=(WX/4*1)>>1<<1; y[0][0]=(WY/2+10/2)>>1<<1; //P1 head
  x[1][0]=(WX/4*3)>>1<<1; y[1][0]=(WY/2+10/2)>>1<<1; //P1 tail
  x[0][1]=(WX/4*1)>>1<<1; y[0][1]=(WY/2+10/2)>>1<<1; //P2 head
  x[1][1]=(WX/4*3)>>1<<1; y[1][1]=(WY/2+10/2)>>1<<1; //P2 tail
  
  dx[0][0]=+1; dy[0][0]=0; //P1 tail
  dx[1][0]=-1; dy[1][0]=0; //P1 head
  dx[0][1]=+1; dy[0][1]=0; //P2 tail
  dx[1][1]=-1; dy[1][1]=0; //P2 head
  
  for(int i=0;i<HISTMAX;i++){
    hist[0][i]=0x00;
    hist[1][i]=0x00;
  }
  hist[0][0]=0x01; // x+1
  hist[1][0]=0x7F; // x-1
  
  ih[0][0]=0;
  ih[0][1]=0;
  ih[1][0]=0;
  ih[1][1]=0;
  
  arduboy.clear();
  arduboy.drawRect(0, 10, WIDTH-1, HEIGHT-1-10, 1);
  arduboy.display();
}




