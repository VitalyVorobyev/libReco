#ifndef __EVTSHAPE_H__
#define __EVTSHAPE_H__

#include "TObject.h"
#include "SFWMInfo.h"

class EvtShape : public TObject{
public:
  EvtShape(void) { Clear();}
  void Clear(void);

  double cos_thr;
  double thr_sig;
  double thr_oth;

  SFWMInfo k0, k1;

  ClassDef(EvtShape,1)
};

#ifdef __MAKECINT__
#pragma link C++ class EvtShape;
#endif

#endif
