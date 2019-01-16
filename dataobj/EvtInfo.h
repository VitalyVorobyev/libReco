#ifndef __EVTINFO_H__
#define __EVTINFO_H__

#include "TObject.h"

class EvtInfo : public TObject {
 public:
    EvtInfo(void);
    void Clear();

    int exp;
    int run;
    int evtn;

    int SVD(void) const;
    ClassDef(EvtInfo, 1)
};

#ifdef __MAKECINT__
#pragma link C++ class EvtInfo;
#endif

#endif
