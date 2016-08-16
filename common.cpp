#include "common.h"
String ralign(int i, int n){
  int spc=0;
  if(        0<=+i && +i<=       9) spc=n-1;
  else if(       10<=+i && +i<=      99) spc=n-2;
  else if(      100<=+i && +i<=     999) spc=n-3;
  else if(     1000<=+i && +i<=    9999) spc=n-4;
  else if(    10000<=+i && +i<=   99999) spc=n-5;
  else if(   100000<=+i && +i<=  999999) spc=n-6;
  else if(  1000000<=+i && +i<= 9999999) spc=n-7;
  else if( 10000000<=+i && +i<=99999999) spc=n-8;
  else if(100000000<=+i                ) spc=n-9;
  else if(        0< -i && -i<=         9) spc=n-2;
  else if(       10<=-i && -i<=        99) spc=n-3;
  else if(      100<=-i && -i<=       999) spc=n-4;
  else if(     1000<=-i && -i<=      9999) spc=n-5;
  else if(    10000<=-i && -i<=     99999) spc=n-6;
  else if(   100000<=-i && -i<=    999999) spc=n-7;
  else if(  1000000<=-i && -i<=   9999999) spc=n-8;
  else if( 10000000<=-i && -i<=  99999999) spc=n-9;
  else if(100000000<=-i                  ) spc=n-10;
  String str="";
  for(int c=0;c<spc;c++)str+=" ";
  return str+String(i);
}
