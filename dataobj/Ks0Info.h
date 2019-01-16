#ifndef __KS0INFO_H__
#define __KS0INFO_H__

#include "TObject.h"

class Ks0Info : public TObject{
public:
  Ks0Info(void) { Clear();}
  void Clear(void);

  double m, m_raw;
  double p[3];
  double dr,dz,dphi,fl;

  ClassDef(Ks0Info,1)
};

#ifdef __MAKECINT__
#pragma link C++ class Ks0Info;
#endif

#endif
