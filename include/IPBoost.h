#ifndef __IPBOOST_H__
#define __IPBOOST_H__

#include "TObject.h"

class IPBoost : public TObject{
public:
  IPBoost(void) { Clear();}
  void Clear(void);

  double ip[3]; //
  double boost[3]; //

  ClassDef(IPBoost,1)
};

#ifdef __MAKECINT__
#pragma link C++ class IPBoost;
#endif

#endif
