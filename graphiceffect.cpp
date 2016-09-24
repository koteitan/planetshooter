#include <Arduboy.h>
#include "graphiceffect.h"
GraphicEffect::GraphicEffect(Arduboy *_pArduboy){
	pArduboy = _pArduboy;
  vram = pArduboy->getBuffer();
	seq = 0;
  timenow = 0;
  timemax = 4;
}
GraphicEffect::~GraphicEffect(){
}
void GraphicEffect::init(void){
	seq=0;
}
bool GraphicEffect::mosaic(void){
  uint8_t *p = (uint8_t*)(pArduboy->getBuffer());
  int invseq=3-seq;
  switch(invseq){
    case 0:
      for(int i=0;i<128*8;i++){
        SPI.transfer(*p++);
      }//i
    break;
    case 1:
      for(int i=0;i<128*8/2;i++){
        uint8_t c;
        c = *p++;
        c |= *p++;
        c |= ((c&0x55)>>1) |((c&0xAA)<<1);
        SPI.transfer(c);
        SPI.transfer(c);
      }//i
    break;
    case 2:
      for(int i=0;i<128*8/4;i++){
        uint8_t c;
        c = *p++;
        c |= *p++;
        c |= *p++;
        c |= *p++;
        c |= ((c&0x55)<<1) |((c&0xAA)>>1);
        c |= ((c&0x33)<<2) |((c&0xCC)>>2);
        SPI.transfer(c);
        SPI.transfer(c);
        SPI.transfer(c);
        SPI.transfer(c);
      }//i
    break;
    case 3:
      for(int i=0;i<128*8/8;i++){
        uint8_t c;
        c = *p++;
        c |= *p++;
        c |= *p++;
        c |= *p++;
        c |= *p++;
        c |= *p++;
        c |= *p++;
        c |= *p++;
        c=(c!=0)?0xFF:0x00;
        SPI.transfer(c);
        SPI.transfer(c);
        SPI.transfer(c);
        SPI.transfer(c);
        SPI.transfer(c);
        SPI.transfer(c);
        SPI.transfer(c);
        SPI.transfer(c);
      }//i
    break;
    default:
    break;
  }
  if(timenow>timemax){
    timenow=0;
    seq++;
  }else{
    timenow++;
  }
  return seq==4;
}
