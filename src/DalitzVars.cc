#include "DalitzVars.h"

ClassImp(DalitzVars)

void DalitzVars::Clear(void){
  for(int i=0; i<3; i++){
    msq[i] = -99.;
    cosh[i] = -99.;
  }
}
