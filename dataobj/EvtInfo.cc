#include "EvtInfo.h"

ClassImp(EvtInfo)

EvtInfo::EvtInfo(void) {
    Clear();
}

void EvtInfo::Clear(void) {
    exp = 0;
    run = 0;
    evtn = 0;
}

int EvtInfo::SVD(void) const {
    return exp > 30 ? 2 : 1;
}

