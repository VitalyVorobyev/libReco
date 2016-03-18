#ifndef __GAMMAINFO_H__
#define __GAMMAINFO_H__

#include "TObject.h"

class GammaInfo : public TObject{
public:
  GammaInfo(void) { Clear();}
  void Clear(void);

  double p[3];

  double P(void)     const;
  double pt(void)    const;
  double costh(void) const;

  ClassDef(GammaInfo,1)
};

#ifdef __MAKECINT__
#pragma link C++ class GammaInfo;
#endif

#endif
