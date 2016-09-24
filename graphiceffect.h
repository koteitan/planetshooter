#pragma once
#include <Arduboy.h>
class GraphicEffect{
private:
  char *vram;
public:
	Arduboy *pArduboy;
	int seq;
  int timenow;
  int timemax;

	GraphicEffect(Arduboy *_pArduboy);
	~GraphicEffect(void);
	void init(void);
  bool mosaic(void);
};