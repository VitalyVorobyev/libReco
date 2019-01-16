#ifndef __H0INFO_H__
#define __H0INFO_H__

#include "TObject.h"

/**
 * @brief The H0Info2 class. Class for composite neutral particles
 **/
class H0Info : public TObject {
 public:
    H0Info(void) { Clear();}
    void Clear(void);

    double m;
    double p;    // Momentum
    double costh;
    double phi;

    /** Vertex fit chi2/n.d.f **/
//    double vchi2;
    /** Mass fit chi2/n.d.f **/
    double mchi2;

    double px(void) const;
    double py(void) const;
    double pz(void) const;
    double pt(void) const;

    ClassDef(H0Info, 1)
};

#ifdef __MAKECINT__
#pragma link C++ class H0Info;
#endif

#endif

