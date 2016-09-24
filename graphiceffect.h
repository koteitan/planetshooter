#pragma once
#include <Arduboy.h>
class GraphicEffect{
private:
  char *vram;
public:
	Arduboy *pArduboy;

	GraphicEffect(Arduboy *_pArduboy);
	~GraphicEffect(void);
  void mosaic(int);
  void lightning(int);
  void glitch(int,int);
  void devide(int);
};
