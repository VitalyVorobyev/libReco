#ifndef __TRACKINFO_H__
#define __TRACKINFO_H__

#include "TObject.h"

class TrackInfo : public TObject{
public:
  TrackInfo(void) { Clear();}
  void Clear(void);

  double p[3];         // Momentum
  double r, z;         // Impact parameters
  int rz_svd, rphi_svd;// SVD hits
  double atckpi;       // Identification
  double atckp;
  double eid;
  double muid;

  double P(void)     const;
  double pt(void)    const;
  double costh(void) const;

  ClassDef(TrackInfo,1)
};

#ifdef __MAKECINT__
#pragma link C++ class TrackInfo;
#endif

#endif
