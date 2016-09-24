#include <Arduboy.h>
#include "graphiceffect.h"
GraphicEffect::GraphicEffect(Arduboy *_pArduboy){
	pArduboy = _pArduboy;
  vram = pArduboy->getBuffer();
}
GraphicEffect::~GraphicEffect(){
}
/* mosaic with 2^seq x 2^seq dots 
   seq = 0:normal
   seq = 1:2x2 mosaic
   seq = 2:4x4 mosaic
   seq = 3:8x8 mosaic
*/
void GraphicEffect::mosaic(int seq){
  uint8_t *p = (uint8_t*)(pArduboy->getBuffer());
  for(int i=0;i<128*8>>seq;i++){
    const uint8_t mask[] = {0x55,0xAA,0x33,0xCC,0x0F,0xF0};
    uint8_t *m = (uint8_t*)&mask[0];
    uint8_t c=0x00;
    for(int j=0;j<1<<seq;j++) c |= *p++;
    for(int j=0;j<seq;j++) c |= ((c&(*m++))<<(1<<j)) |((c&(*m++))>>(1<<j));
    for(int j=0;j<1<<seq;j++) SPI.transfer(c);
  }//i
}
