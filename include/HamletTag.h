#ifndef __HAMLETTAG_H__
#define __HAMLETTAG_H__

#include "TObject.h"

class HamletTag : public TObject{
public:
  HamletTag(void) { Clear();}
  void Clear(void);

//  double tag_NN,tag_NN_err; //
  double tag_LH,tag_LH_err; //
  int tagl, taglid;
  int flv;

  ClassDef(HamletTag,1)
};

#ifdef __MAKECINT__
#pragma link C++ class HamletTag;
#endif

#endif
