#include <Arduboy.h>
#pragma once

#define DIMS   2 // number of dimension
/* screen coodinate:
  (0,0)--(128,64) 
*/
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
/* border coodinate:
  (-100,-100)--(+100,+100)
*/
#define BX0 (-100.0f)
#define BX1 (+100.0f)
#define BY0 (-100.0f)
#define BY1 (+100.0f)
#define BX  (BX1-BX0)
#define BY  (BY1-BY0)

// scale change
#define WY2SY ((float)SY/WY)
#define WX2SX ((float)SX/WX)
#define SY2WY ((float)WY/SY)
#define SX2WX ((float)WX/SX)

// key
#define KEY_XM 0
#define KEY_XP 1
#define KEY_YM 2
#define KEY_YP 3
#define KEY_B  4
#define KEY_A  5
#define KEYS   6

/* return String converted from integer i with length n */
String ralign(int i, int n); 
