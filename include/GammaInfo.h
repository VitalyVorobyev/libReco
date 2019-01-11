#ifndef __GAMMAINFO_H__
#define __GAMMAINFO_H__

#include "TObject.h"

class GammaInfo : public TObject{
public:
  GammaInfo(void) { Clear();}
  void Clear(void);

  double e;
  double costh;

  ClassDef(GammaInfo, 2)
};

#ifdef __MAKECINT__
#pragma link C++ class GammaInfo;
#endif

#endif
