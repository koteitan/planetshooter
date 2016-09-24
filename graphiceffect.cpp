#include <Arduboy.h>
#include "graphiceffect.h"
GraphicEffect::GraphicEffect(Arduboy *_pArduboy){
	pArduboy = _pArduboy;
  vram = pArduboy->getBuffer();
}
GraphicEffect::~GraphicEffect(){
}
/* mosaic effect
   seq = 0:normal
   seq = 1:2x2 mosaic
   seq = 2:4x4 mosaic
   seq = 3:8x8 mosaic
*/
void GraphicEffect::mosaic(int seq){
  uint8_t *p = (uint8_t*)vram;
  for(int i=0;i<128*8>>seq;i++){
    const uint8_t mask[] = {0x55,0xAA,0x33,0xCC,0x0F,0xF0};
    uint8_t *m = (uint8_t*)&mask[0];
    uint8_t c=0x00;
    for(int j=0;j<1<<seq;j++) c |= *p++;
    for(int j=0;j<seq;j++) c |= ((c&(*m++))<<(1<<j)) |((c&(*m++))>>(1<<j));
    for(int j=0;j<1<<seq;j++) SPI.transfer(c);
  }//i
}

/* lightning effect
  seq =  0: normal
  seq =  1: lightning on the line y= 1
       ...
  seq = 63: lightning on the line y=63
*/
void GraphicEffect::lightning(int seq){
  int lybyte = seq/8;
  int lybit  = seq%8;
  
  //all bits are lightning
  for(int y=0;y<lybyte;y++){
    for(int x=0;x<128;x++){
      uint8_t c = (vram[lybyte*128+x]>>lybit & 0x01)?0xFF:0x00;
      SPI.transfer(c);
    }
  }
  //lybit bits are lightning
  for(int x=0;x<128;x++){
    uint8_t c = vram[lybyte*128+x]>>lybit;
    c = c<<lybit | ((c&1)?0xFF:0x00)>>(8-lybit);    
    SPI.transfer(c);
  }
  //all bits are normal
  for(int x=0;x<128*(8-lybyte-1);x++){
    SPI.transfer(vram[(lybyte+1)*128+x]);
  }
}

