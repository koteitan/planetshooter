#include <Arduboy.h>
#include "graphiceffect.h"
GraphicEffect::GraphicEffect(Arduboy *_pArduboy){
	pArduboy = _pArduboy;
  vram = pArduboy->getBuffer();
	seq = 0;
  timenow = 0;
  timemax = 2;
}
GraphicEffect::~GraphicEffect(){
}
void GraphicEffect::init(void){
	seq=0;
}
bool GraphicEffect::mosaic(void){
  uint8_t *p = (uint8_t*)(pArduboy->getBuffer());
  int invseq=3-seq;
  for(int i=0;i<128*8>>invseq;i++){
    uint8_t mask[] = {0x55,0xAA,0x33,0xCC,0x0F,0xF0};
    uint8_t *m = (uint8_t*)&mask[0];
    uint8_t c=0x00;
    for(int j=0;j<1<<invseq;j++) c |= *p++;
    for(int j=0;j<invseq;j++) c |= ((c&(*m++))<<(1<<j)) |((c&(*m++))>>(1<<j));
    for(int j=0;j<1<<invseq;j++) SPI.transfer(c);
  }//i
  if(timenow>timemax){
    timenow=0;
    seq++;
  }else{
    timenow++;
  }
  return seq==4;
}
