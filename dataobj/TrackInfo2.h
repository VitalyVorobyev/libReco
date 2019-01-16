#ifndef __TRACKINFO2_H__
#define __TRACKINFO2_H__

#include "TObject.h"

/**
 * @brief The TrackInfo2 class.
 **/
class TrackInfo2 : public TObject {
 public:
    TrackInfo2(void) { Clear();}
    void Clear(void);

    double p;    // Momentum
    double costh;
    double phi;

    double r, z;           // Impact parameters
    int rz_svd, rphi_svd;  // SVD hits
    double atckpi;         // Identification
    double atckp;
    double eid;
    double muid;

    double px(void) const;
    double py(void) const;
    double pz(void) const;
    double pt(void) const;

    ClassDef(TrackInfo2, 1)
};

#ifdef __MAKECINT__
#pragma link C++ class TrackInfo2;
#endif

#endif  // __TRACKINFO2_H__
