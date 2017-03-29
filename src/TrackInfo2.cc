#include "TrackInfo2.h"

#include <cmath>

using std::sin;
using std::cos;
using std::sqrt;

ClassImp(TrackInfo2)

void TrackInfo2::Clear(void) {
    p = 0., costh = 0., phi = 0.;   // Momentum
    r = 0., z = 0.;         // Impact parameters
    rz_svd = -1, rphi_svd = -1;  // SVD hits
    atckpi = -2;       // Identification
    atckp  = -2;
    eid    = -2;
    muid   = -2;
}

double TrackInfo2::pz(void) const { return p * costh;}
double TrackInfo2::pt(void) const { return p * sqrt(1. - costh * costh);}
double TrackInfo2::px(void) const { return pt() * cos(phi);}
double TrackInfo2::py(void) const { return pt() * sin(phi);}

#ifdef __MAKECINT__
#pragma link C++ class TrackInfo2;
#endif

