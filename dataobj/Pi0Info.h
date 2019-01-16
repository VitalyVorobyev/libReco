#ifndef __PI0INFO_H__
#define __PI0INFO_H__

#include "TObject.h"

class Pi0Info : public TObject{
public:
  Pi0Info(void) { Clear();}
  void Clear();

  double m, m_raw;
  double p[3];
  double eg1,eg2;
  double chisq;
//  double width;
//  double e9oe25;
//  int nhits;

  double asym(void)  const;
  double P(void)     const;
  double pt(void)    const;
  double costh(void) const;
  double egmin(void) const;
  double egmax(void) const;

  ClassDef(Pi0Info,1)
};

#ifdef __MAKECINT__
#pragma link C++ class Pi0Info;
#endif

#endif
