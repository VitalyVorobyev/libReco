#ifndef __DALITZVARS_H__
#define __DALITZVARS_H__

#include "TObject.h"

class DalitzVars : public TObject{
public:
  DalitzVars(void) { Clear();}
  void Clear(void);

  double msq[3];
  double cosh[3];

  ClassDef(DalitzVars,1)
};

#ifdef __MAKECINT__
#pragma link C++ class DalitzVars;
#endif

#endif
