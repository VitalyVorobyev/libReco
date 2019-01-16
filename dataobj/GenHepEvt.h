#ifndef __GENHEPEVT_H__
#define __GENHEPEVT_H__

#include "TObject.h"
#include <vector>

class GenHepEvt : public TObject{
public:
  GenHepEvt(void) { Clear();}
  void Clear(void);

  std::vector<int> idhep;
  std::vector<int> daF;
  std::vector<int> daL;

  ClassDef(GenHepEvt,1)
};

#ifdef __MAKECINT__
#pragma link C++ class GenHepEvt;
#endif

#endif
