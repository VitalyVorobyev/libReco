#ifndef __SFWMINFO_H__
#define __SFWMINFO_H__

#include "TObject.h"

class SFWMInfo : public TObject{
public:
  SFWMInfo(void) {}
  void Clear(void);

  double kmm2;
  double ket;
  double khso00;
  double khso01;
  double khso02;
  double khso03;
  double khso04;
  double khso10;
  double khso12;
  double khso14;
  double khso20;
  double khso22;
  double khso24;
  double khoo0;
  double khoo1;
  double khoo2;
  double khoo3;
  double khoo4;

  ClassDef(SFWMInfo,1)
};

#ifdef __MAKECINT__
#pragma link C++ class SFWMInfo;
#endif

#endif
