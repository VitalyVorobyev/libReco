#include "Ks0Info.h"

ClassImp(Ks0Info)

void Ks0Info::Clear(void){
  m = 0, m_raw = 0;
  p[0] = -99, p[1] = -99, p[2] = -99;
  dr = -99, dz = -99, dphi = -99, fl = -99;
}

