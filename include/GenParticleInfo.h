#ifndef __GENPARTICLEINFO_H__
#define __GENPARTICLEINFO_H__

#include "TObject.h"

class GenParticleInfo : public TObject{
public:
  GenParticleInfo(void) { Clear();}
  void Clear(void);

  int id;
  int flag;
  int chain[9];

  ClassDef(GenParticleInfo,1)
};

#ifdef __MAKECINT__
#pragma link C++ class GenParticleInfo;
#endif

#endif
