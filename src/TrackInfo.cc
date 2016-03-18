#include "TrackInfo.h"
#include <cmath>

ClassImp(TrackInfo)

void TrackInfo::Clear(void){
  p[0] = -99, p[1] = -99, p[3] = -99;   // Momentum
  r = -99, z = -99;         // Impact parameters
  rz_svd = -1, rphi_svd = -1;// SVD hits
  atckpi = -2;       // Identification
  atckp  = -2;
  eid    = -2;
  muid   = -2;
}

double TrackInfo::P(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}

double TrackInfo::pt(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]);
}

double TrackInfo::costh(void) const{
  return p[2]/P();
}

#ifdef __MAKECINT__
#pragma link C++ class TrackInfo;
#endif

