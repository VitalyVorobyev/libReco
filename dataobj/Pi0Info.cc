#include "Pi0Info.h"
#include <cmath>

ClassImp(Pi0Info)

void Pi0Info::Clear(void){
  m = 0, m_raw = 0;
  p[0] = -99, p[1] = -99, p[2] = -99;
  eg1 = -99, eg2 = -99;
  chisq = -1;
}

double Pi0Info::P(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}

double Pi0Info::pt(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]);
}

double Pi0Info::costh(void) const{
  return p[2]/P();
}

double Pi0Info::asym(void) const{
  return (egmax()-egmin())/(eg1+eg2);
}

double Pi0Info::egmin(void) const{
  return eg1 > eg2 ? eg2 : eg1;
}

double Pi0Info::egmax(void) const{
  return eg1 > eg2 ? eg1 : eg2;
}

