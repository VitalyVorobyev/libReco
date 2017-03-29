// Base on example from J. Tanaka

#include <cmath>
#include <vector>

#include "uisetter.h"
#include "recotools.h"

#include "particle/Particle.h"
#include "kid/atc_pid.h"
#include "mdst/mdst.h"
#include "belle.h"
#include "CLHEP/Geometry/Point3D.h"
#include <CLHEP/Vector/ThreeVector.h>
#include "helix/Helix.h"
#include "ip/IpProfile.h"
#include MDST_H
#include <mdst/Muid_mdst.h>
#include "geninfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

using std::fabs;
using std::cerr;
using std::endl;
using std::abs;
using std::vector;

bool   UISetter::m_mc_flag  = false;
int    UISetter::m_rphi_svd = 1;
int    UISetter::m_rz_svd   = 2;
double UISetter::m_trk_pt   = 0.05;
double UISetter::m_atckpi_k = 0.2;
double UISetter::m_r_ip     = 2;
double UISetter::m_z_ip     = 5;

int UISetter::CheckTrk(Particle& trk, const bool wUI) {
    if (trk.p().perp() < m_trk_pt) return 1;
    if (wUI) {
        const TrkUserInfo& tinfo = static_cast<const TrkUserInfo&>(trk.userInfo());
        if (tinfo.rphi_svd_hits() < m_rphi_svd) return 2;
        if (tinfo.rz_svd_hits() < m_rz_svd)     return 3;
        if (fabs(tinfo.r()) > m_r_ip)           return 4;
        if (fabs(tinfo.z()) > m_z_ip)           return 5;
        if (abs(trk.lund()) != 211)
            if(tinfo.atckpi() < m_atckpi_k)     return 6;
    } else {
        int rphi, rz;
        double r, z;
        GetTrkFitInfo(trk, rphi, rz, r, z);
        if (rphi    < m_rphi_svd) return 2;
        if (rz      < m_rz_svd)   return 3;
        if (fabs(r) > m_r_ip)     return 4;
        if (fabs(z) > m_z_ip)     return 5;
        if (abs(trk.lund()) != 211){
            const double atckpi = GetKPiID(trk);
            if (atckpi < m_atckpi_k) return 6;
        }   
    }
    return 0;
}

void UISetter::GetTrkFitInfo(const Particle& trk,
                             int& rphi, int& rz, double& r, double& z) {
    const int hypo = abs(trk.lund()) == 211 ? 2 : 3;//2 -> pi, 3 -> K
    Mdst_trk_fit &trk_fit = trk.mdstCharged().trk().mhyp(hypo);
    rphi = trk_fit.nhits(3);
    rz   = trk_fit.nhits(4);

    HepPoint3D pivot(trk_fit.pivot(0),trk_fit.pivot(1),trk_fit.pivot(2));
    HepVector a(5);
    for (int i = 0; i < 5; i++) a[i] = trk_fit.helix(i);

    Helix helix(pivot, a);
    helix.pivot(IpProfile::e_position());

    r = helix.dr();
    z = helix.dz();
}

double UISetter::GetKPiID(Particle& trk) {
    atc_pid selKpi(3, 1, 5, 3, 2);
    return selKpi.prob(trk.mdstCharged());
}

double UISetter::GetKPID(Particle& trk) {
    atc_pid selKpi(3, 1, 5, 3, 4);
    return selKpi.prob(trk.mdstCharged());
}

double UISetter::GetEID(Particle& trk) {
    eid sel_e(trk.mdstCharged());
    return sel_e.prob(3, -1, 5);
}

double UISetter::GetMuID(Particle& trk) {
    Muid_mdst muID(trk.mdstCharged());
    if (!muID.Status())  return -3;
    if (muID.Chi_2() <= 0) return -4;
    return muID.Muon_likelihood();
}

void UISetter::SetTrkInfo(Particle& trk) {
    int rphi,rz;
    double r,z;
    GetTrkFitInfo(trk,rphi,rz,r,z);
    TrkUserInfo& tinfo = dynamic_cast<TrkUserInfo&>(trk.userInfo());
    tinfo.rphi_svd_hits(rphi);
    tinfo.rz_svd_hits(rz);
    tinfo.r(r);
    tinfo.z(z);
    tinfo.atckpi(GetKPiID(trk));
    tinfo.atckp(GetKPID(trk));
    tinfo.eid(GetEID(trk));
    tinfo.muid(GetMuID(trk));
}

void UISetter::SetD0lInfo(vector<Particle> &d0l) {
    for (unsigned i = 0; i < d0l.size(); i++) {
        d0l[i].userInfo(D0UserInfo());
        dynamic_cast<D0UserInfo&>(d0l[i].userInfo()).Mass(d0l[i].p().m());
        if (m_mc_flag) setMCtruth(d0l[i]);
    }
}

void UISetter::SetDlInfo(vector<Particle> &dl) {
    for (unsigned i = 0; i < dl.size(); i++) {
        dl[i].userInfo(DUserInfo());
        dynamic_cast<DUserInfo&>(dl[i].userInfo()).Mass(dl[i].p().m());
        if(m_mc_flag) setMCtruth(dl[i]);
    }
}

void UISetter::SetPhilInfo(vector<Particle> &phil) {
    for (unsigned i = 0; i < phil.size(); i++) {
        phil[i].userInfo(PhiUserInfo());
        if(m_mc_flag) setMCtruth(phil[i]);
    }
}

void UISetter::SetKst0lInfo(vector<Particle> &kst0l) {
    for (unsigned i = 0; i < kst0l.size(); i++) {
        kst0l[i].userInfo(PhiUserInfo());
        if(m_mc_flag) setMCtruth(kst0l[i]);
    }
}

void UISetter::Seth0lInfo(vector<Particle> &etal) {
    for (unsigned i = 0; i < etal.size(); i++) {
        etal[i].userInfo(EtaUserInfo());
        dynamic_cast<EtaUserInfo&>(etal[i].userInfo()).Mass(etal[i].p().m());
        if (m_mc_flag) setMCtruth(etal[i]);
    }
}

void UISetter::SetPi0lInfo(vector<Particle> &pi0l) {
    for (unsigned i = 0; i < pi0l.size(); i++) {
        pi0l[i].userInfo(Pi0UserInfo());
        dynamic_cast<Pi0UserInfo&>(pi0l[i].userInfo()).Mass(pi0l[i].p().m());
        if(m_mc_flag) setMCtruth(pi0l[i]);
    }
}

void UISetter::SetModeD0(vector<Particle> &pl, const int mode) {
    for (unsigned i = 0; i < pl.size(); i++)
        dynamic_cast<D0UserInfo&>(pl[i].userInfo()).Mode(mode);
}

void UISetter::SetModeD(vector<Particle> &pl, const int mode) {
    for (unsigned i = 0; i < pl.size(); i++)
        dynamic_cast<DUserInfo&>(pl[i].userInfo()).Mode(mode);
}

void UISetter::SetModeB(vector<Particle> &pl, const int mode) {
    for (unsigned i = 0; i < pl.size(); i++)
        dynamic_cast<BUserInfo&>(pl[i].userInfo()).Mode(mode);
}

void UISetter::SetModeEta(std::vector<Particle> &pl, const int mode) {
    for (unsigned i = 0; i < pl.size(); i++)
        dynamic_cast<EtaUserInfo&>(pl[i].userInfo()).Mode(mode);
}

void UISetter::SetBadVtx(Particle& b0) {
    dynamic_cast<B0UserInfo&>(b0.userInfo()).ChisqSig(-55);
}

void UISetter::SetBadMass(Particle& b0) {
    dynamic_cast<B0UserInfo&>(b0.userInfo()).MassChi2(-55);
}

void UISetter::SetSigVtxFitInfo(Particle& b0, kvertexfitter& kvf) {
    B0UserInfo& binfo = dynamic_cast<B0UserInfo&>(b0.userInfo());
    binfo.ZSig(kvf.vertex().z());
    binfo.SzSig(kvf.errVertex()[2][2]);
    binfo.NdfSig(kvf.dgf_tracks());
    binfo.ChisqSig(kvf.chisq_tracks());
    binfo.ClSig(kvf.cl());
    binfo.NtrkSig(3);
}

void UISetter::SetB0toDhhMassFitInfo(Particle& b0, kmassfitter& kmf) {
    if (kmf.tracks() != 3) {
        cerr << "UIS::SetB0toDhhMassFitInfo: wrong tracks number: "
             << kmf.tracks() << endl;
        return;
    }
    double hel = 0;
    double mass = 0;
    const HepLorentzVector b0lv = kmf.momentum(0) + kmf.momentum(1) 
                                                  + kmf.momentum(2);
    B0UserInfo& binfo = dynamic_cast<B0UserInfo&>(b0.userInfo());

    hel = RTools::ResHelicity(b0lv,kmf.momentum(0),kmf.momentum(1),mass);
    binfo.CosHel12(hel);
    binfo.Mass12(mass);

    hel = RTools::ResHelicity(b0lv,kmf.momentum(0),kmf.momentum(2),mass);
    binfo.CosHel13(hel);
    binfo.Mass13(mass);

    hel = RTools::ResHelicity(b0lv,kmf.momentum(1),kmf.momentum(2),mass);
    binfo.CosHel23(hel);
    binfo.Mass23(mass);

    binfo.MassChi2(kmf.chisq()/kmf.dgf());
}

void UISetter::SetB0toDhhRawInfo(Particle& b0) {
    if (b0.nChildren() != 3) {
        cerr << "UIS::SetB0toDhhRawInfo: wrong children number: "
             << b0.nChildren() << endl;
        return;
    }
    double hel = 0;
    double mass = 0;
    const HepLorentzVector& b0lv = b0.p();
    const HepLorentzVector& p1 = b0.child(0).p();
    const HepLorentzVector& p2 = b0.child(1).p();
    const HepLorentzVector& p3 = b0.child(2).p();
    B0UserInfo& binfo = dynamic_cast<B0UserInfo&>(b0.userInfo());

    hel = RTools::ResHelicity(b0lv, p1, p2, mass);
    binfo.CosHel12(hel);
    binfo.Mass12(mass);

    hel = RTools::ResHelicity(b0lv, p1, p3, mass);
    binfo.CosHel13(hel);
    binfo.Mass13(mass);

    hel = RTools::ResHelicity(b0lv, p2, p3, mass);
    binfo.CosHel23(hel);
    binfo.Mass23(mass);
}

void UISetter::SetMbc_deltaE(Particle& b0,
                             const double& mbc, const double& de) {
    BUserInfo& binfo = dynamic_cast<BUserInfo&>(b0.userInfo());
    binfo.Mbc(mbc);
    binfo.deltaE(de);
}

void UISetter::SetTagVtxFitInfo(Particle& b0, TagVK& tagv) {
    B0UserInfo& binfo = dynamic_cast<B0UserInfo&>(b0.userInfo());
    binfo.ZAsc(tagv.vtx().z());
    binfo.SzAsc(tagv.errVtx()[2][2]);
    binfo.NtrkAsc((int)tagv.ntrk());
    binfo.NdfAsc((int)tagv.ndf_tracks());
    binfo.ChisqAsc(tagv.chisq_tracks());
    binfo.ClAsc(tagv.cl());
    binfo.tagl(tagv.isTagLeptonVertex());
    binfo.taglid(tagv.isTagLeptonVertex() ?
                 tagv.VertexTagLepton().get_ID() : 0);
}

void UISetter::SetRawDtoKspipiVars(Particle &d0) {
    if (d0.nChildren() != 3) {
        cerr << "D0 children: " << d0.nChildren() << "!" << endl;
        return;
    }
    const double mp = (d0.child(0).p() + d0.child(1).p()).m2();
    const double mm = (d0.child(0).p() + d0.child(2).p()).m2();
    dynamic_cast<D0UserInfo&>(d0.userInfo()).SetRawDalitzVars(mp, mm);
}

#if defined(BELLE_NAMESPACE)
}
#endif 

