#ifndef __VERTEXINFO_H__
#define __VERTEXINFO_H__

#include "TObject.h"

class VertexInfo : public TObject{
public:
  VertexInfo(void) { Clear();}
  void Clear(void);

  double z,sz;
  int ntrk,ndf;
  double chisq,cl;

  ClassDef(VertexInfo,1)
};

#ifdef __MAKECINT__
#pragma link C++ class VertexInfo;
#endif

#endif

