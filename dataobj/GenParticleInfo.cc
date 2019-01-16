#include "GenParticleInfo.h"

ClassImp(GenParticleInfo)

void GenParticleInfo::Clear(void){
  id   = -99;
  flag = -99;
  for(int i=0; i<9; i++) chain[i] = -99;
}

