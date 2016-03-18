#include "VertexInfo.h"

ClassImp(VertexInfo)

void VertexInfo::Clear(void){
  z = -99.; sz = -99.;
  ntrk = -1; ndf = -1;
  chisq = -99.; cl = -99.;
}
