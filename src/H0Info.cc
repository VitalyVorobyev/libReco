#include "H0Info.h"

#include <cmath>

using std::sin;
using std::cos;
using std::sqrt;

ClassImp(H0Info)

void H0Info::Clear(void) {
    p = 0., costh = 0., phi = 0.;  // Momentum
    mchi2 = 0.;
//    vchi2 = 0.;
    m = 0.;
}

double H0Info::pz(void) const { return p * costh;}
double H0Info::pt(void) const { return p * sqrt(1. - costh * costh);}
double H0Info::px(void) const { return pt() * cos(phi);}
double H0Info::py(void) const { return pt() * sin(phi);}

#ifdef __MAKECINT__
#pragma link C++ class H0Info;
#endif

