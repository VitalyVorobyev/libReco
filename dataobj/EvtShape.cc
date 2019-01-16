#include "EvtShape.h"

ClassImp(EvtShape)

void EvtShape::Clear(void){
  cos_thr = -2.;
  thr_sig = -2.;
  thr_oth = -2.;
  k0.Clear();
  k1.Clear();
}

