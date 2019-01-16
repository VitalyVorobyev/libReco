#ifndef __DELTAEMBC_H__
#define __DELTAEMBC_H__

#include "TObject.h"

class DeltaEMbc : public TObject{
public:
  DeltaEMbc(void) { Clear();}
  void Clear(void);

  double de;
  double mbc;

  ClassDef(DeltaEMbc,1)
};

#ifdef __MAKECINT__
#pragma link C++ class DeltaEMbc;
#endif

#endif
