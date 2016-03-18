#include "GammaInfo.h"
#include <cmath>

ClassImp(GammaInfo)

void GammaInfo::Clear(void){
  p[0] = -99; p[1] = -99; p[2] = -99;
}

double GammaInfo::P(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}

double GammaInfo::pt(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]);
}

double GammaInfo::costh(void) const{
  return p[2]/P();
}

