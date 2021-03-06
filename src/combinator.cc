#include "combinator.h"

#include <iterator>
#include <stdexcept>

#include "uisetter.h"
#include "recotools.h"
#include "kinefitter.h"

#include "particle/PID.h"
#include "particle/utility.h"
#include "particle/combination.h"
#include "particle/ParticleUserInfo.h"
#include "mdst/findKs.h"
#include "nisKsFinder/nisKsFinder.h"
#include "mdst/mdst.h"
#include "ip/IpProfile.h"

#include MDST_H
#include HEPEVT_H

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

typedef std::vector<Particle> pvec;
typedef Combinator CMB;
typedef UISetter UIS;

using std::fabs;
using std::abs;
using std::cerr;
using std::endl;
using std::map;
using std::string;
using std::array;
using std::vector;

using std::make_move_iterator;
using std::make_pair;

using std::runtime_error;

bool CMB::m_tupfl = false;
bool CMB::m_mc_flag = false;

const double mD0    = 1.86484;
const double mDp    = 1.86961;
const double mDs    = 1.976;
const double mphi   = 1.020;
const double momega = 0.78265;
const double meta   = 0.547862;
const double mkst0  = 0.89166;
const double dmdsst = 0.1438;
const double dmdst0 = 0.14212;
const double dmdst  = 0.1454257;

const int B0ID    = 511;
const int DsID    = 431;
const int DsstID  = 433;
const int D0ID    = 421;
const int D0stID  = 423;
const int DstID   = 413;
const int GamID   = 22;
const int Pi0ID   = 111;
const int PiID    = 211;
const int EtaID   = 221;
const int OmegaID = 223;
const int DsjID   = 10411;

map<string, vector<double>> CMB::cutSym;
map<string, vector<double>> CMB::cutAsym;
map<string, double> CMB::par;

void CMB::init() {
    cutSym.insert({"m(D0)"        , {mD0   , 0.065}});
    cutSym.insert({"m(D0 -> mult)", {mD0   , 0.035}});
    cutSym.insert({"m(D+)"        , {mDp   , 0.065}});
    cutSym.insert({"m(Ds+)"       , {mDs   , 0.065}});
    cutSym.insert({"m(phi)"       , {mphi  , 0.020}});
    cutSym.insert({"m(omega)"     , {momega, 0.030}});
    cutSym.insert({"m(eta -> gg)" , {meta  , 0.035}});
    cutSym.insert({"m(eta -> ppp)", {meta  , 0.020}});
    cutSym.insert({"m(K*0)"       , {mkst0 , 0.050}});
    cutSym.insert({"dm(Ds*)"      , {dmdsst, 0.007}});
    cutSym.insert({"dm(D*0)"      , {dmdst0, 0.007}});
    cutSym.insert({"dm(D*)"       , {dmdst , 0.007}});

    cutAsym.insert({"m(K0S)", {0.48, 0.52}});
    cutAsym.insert({"m(Dsj)", {2.00, 3.00}});

    par.insert({"Emin(gamma)"         , 0.08});
    par.insert({"Emin(gamma from pi0)", 0.04});
    par.insert({"Emin(gamma from eta)", 0.08});
}

void appendMove(pvec& to, pvec& from) {
    to.insert(to.end(), make_move_iterator(from.begin()), make_move_iterator(from.end()));
    from.clear();
}

void CMB::checkPLM(PLM& plm, const string& pcl) {
    if (plm.find(pcl) == plm.end())
        plm[pcl] = pvec();
}

void CMB::checkPLM(PLM& plm, const string& pcl1, const string& pcl2) {
    if (plm.find(pcl1) == plm.end()) {
        plm[pcl1] = pvec();
        plm[pcl2] = pvec();
    }
}

void CMB::massCutSym(pvec &v, const string& key) {
    if (cutSym.find(key) == cutSym.end()) {
        cerr << "No key " << key << " in cutSym" << endl;
        throw new runtime_error("");
    }
    const auto& cut = cutSym[key];
    withMassCut(v, cut[0] - cut[1], cut[0] + cut[1]);
}

void CMB::massCutAsym(pvec &v, const string& key) {
    if (cutAsym.find(key) == cutAsym.end()) {
        cerr << "No key " << key << " in cutAsym" << endl;
        throw new runtime_error("");
    }

    const auto& cut = cutAsym[key];
    withMassCut(v, cut[0], cut[1]);
}

void CMB::dmassCutSym(pvec &v, const string& key) {
    const auto& cut = cutSym[key];
    withMassDifCut(v, cut[0] - cut[1], cut[0] + cut[1], 0);
}

void CMB::D0UIS(std::vector<Particle> &v, const int mode) {
    UIS::SetDlInfo(v);
    UIS::SetModeD(v, mode);
}

int CMB::make_kpi(PLM& plm) {
    plm["pi+"] = pvec();
    plm["pi-"] = pvec();
    plm["K+"] = pvec();
    plm["K-"] = pvec();
    return make_kpi(plm["pi+"], plm["pi-"], plm["K+"], plm["K-"]);
}

int CMB::make_kpi(pvec &pipl, pvec &piml, pvec &kpl, pvec &kml) {
    pipl.clear(); piml.clear(); kpl.clear(); kml.clear();
    const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();

    Mdst_charged_Manager &chg_mgr = Mdst_charged_Manager::get_manager();
    for (std::vector<Mdst_charged>::const_iterator i = chg_mgr.begin();
         i != chg_mgr.end(); i++) {
        const Mdst_charged& track = *i;
        Particle pi(track, Ptype(track.charge() > 0 ? "PI+" : "PI-"));
        Particle K(track, Ptype(track.charge( ) > 0 ? "K+"  : "K-" ));

        if (m_tupfl) {
            pi.userInfo(TrkUserInfo());
            K.userInfo(TrkUserInfo());
            UIS::SetTrkInfo(pi);
            UIS::SetTrkInfo(K);

            const int flag = UIS::CheckTrk(K, true);
            if (flag > 0 && flag != 6) continue;
            const double rip = fabs(static_cast<TrkUserInfo&>(K.userInfo()).r());
            const Gen_hepevt &h = m_mc_flag ? get_hepevt(track) : null;
            if (h) {
                pi.relation().genHepevt(h);
                K.relation().genHepevt(h);
            }
            if( track.charge() > 0) {
            pipl.push_back(pi);
            if (flag != 6) kpl.push_back(K);
            } else {
                piml.push_back(pi);
                if (flag != 6) kml.push_back(K);
            }
        } else { // skim
            const int flag = UIS::CheckTrk(K, false);
            if (flag > 0 && flag != 6) continue;
            if (track.charge() > 0) {
                pipl.push_back(pi);
                if (flag != 6) kpl.push_back(K);
            } else {
                piml.push_back(pi);
                if (flag != 6) kml.push_back(K);
            }
       }
    }
    return pipl.size() + piml.size();
}

int CMB::make_ks(PLM& plm) {
    plm["K0S"] = pvec();
    return make_ks(plm["K0S"]);
}

int CMB::make_ks(pvec &ksl) {
    ksl.clear();
    const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();
    nisKsFinder ksnb;

    Mdst_vee2_Manager &vee2_mgr = Mdst_vee2_Manager::get_manager();
    const HepPoint3D IP = IpProfile::usable() ? IpProfile::e_position() :
                                                HepPoint3D(0);
    for (std::vector<Mdst_vee2>::iterator i = vee2_mgr.begin();
         i != vee2_mgr.end(); i++) {
        Mdst_vee2 &vee2 = *i;
        int kind = vee2.kind();
        if (kind != 1) continue;
        ksnb.candidates(vee2, IP);
        if (!ksnb.standard()) continue;
        Particle p(vee2);
        if (p.p().m() > cutAsym["m(K0S)"][1] || p.p().m() < cutAsym["m(K0S)"][0]) continue;
        const Gen_hepevt &h = m_mc_flag ? get_hepevt(vee2) : null;
        if (h) p.relation().genHepevt(h);
        if (m_tupfl) {
            p.userInfo(KsUserInfo());
            KsUserInfo& info = static_cast<KsUserInfo&>(p.userInfo());
            info.dr(ksnb.drp() > ksnb.drn() ? ksnb.drp() : ksnb.drn());
            info.dz(ksnb.zdist());
            info.dphi(ksnb.dphi());
            info.fl(ksnb.fl());
        }
        ksl.push_back(p);
    }
    return ksl.size();
}

int CMB::make_d0tok0shh(pvec &d0l, pvec &ksl, pvec &hpl, pvec &hml, int mode) {
    d0l.clear();
    combination(d0l, Ptype("D0"), ksl, hpl, hml);
    massCutSym(d0l, "m(D0)");
    if (m_tupfl) D0UIS(d0l, mode);
    return d0l.size();
}

int CMB::make_d0tohh(pvec &d0l, pvec &h1l, pvec &h2l, int mode) {
    d0l.clear();
    combination(d0l, Ptype("D0"), h1l, h2l);
    massCutSym(d0l, "m(D0 -> mult)");
    if (m_tupfl) D0UIS(d0l, mode);
    return d0l.size();
}

int CMB::make_d0tokpi(pvec &d0l, pvec &kml, pvec &pipl, pvec &kpl, pvec &piml,
                      int mode) {
    pvec d0bl;
    make_d0tokpi(d0l, d0bl, kml, pipl, kpl, piml, mode);
    appendMove(d0l, d0bl);
    return d0l.size();
}

int CMB::make_d0tokpi(pvec &d0l, pvec &d0bl, pvec &kml, pvec &pipl,
                      pvec &kpl, pvec &piml, int mode) {
    d0l.clear(); d0bl.clear();
    combination(d0l, Ptype("D0"), kml, pipl);
    combination(d0bl, Ptype("D0B"), kpl, piml);
    massCutSym(d0l, "m(D0)");
    massCutSym(d0bl, "m(D0)");
    if(m_tupfl){
        D0UIS(d0l, mode);
        D0UIS(d0bl, mode);
    }
    return d0l.size() + d0bl.size();
}

int CMB::make_kstar0(PLM& plm, bool separate) {
    plm["K*0"] = pvec();
    plm["K*0bar"] = pvec();
    if (separate)
        return make_kstar0(plm["K*0"], plm["K*0bar"], plm["K+"], plm["pi-"], plm["K-"], plm["pi+"]);
    return make_kstar0(plm["K*0"], plm["K+"], plm["pi-"], plm["K-"], plm["pi+"]);
}

int CMB::make_kstar0(pvec &kst0l, pvec &kpl, pvec &piml,
                     pvec &kml, pvec &pipl) {
    pvec kst0bl;
    make_kstar0(kst0l, kst0bl, kpl, piml, kml, pipl);
    appendMove(kst0l, kst0bl);
    return kst0l.size();
}

int CMB::make_kstar0(pvec &kst0l, pvec &kst0bl, pvec &kpl, pvec &piml,
                     pvec &kml, pvec &pipl) {
    kst0l.clear(); kst0bl.clear();
    combination(kst0bl, Ptype(-313), kml, pipl);
    combination(kst0l, Ptype(313), kpl, piml);
    massCutSym(kst0l, "m(K*0)");
    massCutSym(kst0bl, "m(K*0)");
    if (m_tupfl) {
        UIS::SetKst0lInfo(kst0l);
        UIS::SetKst0lInfo(kst0bl);
    }
    return kst0l.size() + kst0bl.size();
}

int CMB::make_d0tokpipi0(pvec &d0l, pvec &kml, pvec &pipl, pvec &kpl,
                         pvec &piml, pvec &pi0l, int mode, bool narrow) {
    pvec d0bl;
    make_d0tokpipi0(d0l, d0bl, kml, pipl, kpl, piml, pi0l, mode, narrow);
    appendMove(d0l, d0bl);
    return d0l.size();
}

int CMB::make_d0tokpipi0(pvec &d0l, pvec &d0bl, pvec &kml, pvec &pipl,
                         pvec &kpl, pvec &piml, pvec &pi0l,
                         int mode, bool narrow) {
    d0l.clear(); d0bl.clear();
    if (!pi0l.size()) return 0;
    combination(d0l, Ptype("D0"), kml, pipl, pi0l);
    combination(d0bl, Ptype("D0B"), kpl, piml, pi0l);
    massCutSym(d0l, "m(D0 -> mult)");
    massCutSym(d0bl, "m(D0 -> mult)");
    if (m_tupfl) {
        D0UIS(d0l, mode);
        D0UIS(d0bl, mode);
    }
    return d0l.size() + d0bl.size();
}

int CMB::make_d0tokpipipi(pvec &d0l, pvec &kml, pvec &pipl, pvec &kpl,
                          pvec &piml, int mode, bool narrow) {
    pvec d0bl;
    make_d0tokpipipi(d0l, d0bl, kml, pipl, kpl, piml, mode, narrow);
    appendMove(d0l, d0bl);
    return d0l.size();
}

int CMB::make_d0tokpipipi(pvec &d0l, pvec &d0bl, pvec &kml, pvec &pipl,
                          pvec &kpl, pvec &piml,
                          int mode, bool narrow) {
    d0l.clear(); d0bl.clear();
    combination(d0l, Ptype("D0"), pipl, piml, pipl, kml);
    combination(d0bl, Ptype("D0B"), piml, pipl, piml, kpl);
    massCutSym(d0l, "m(D0 -> mult)");
    massCutSym(d0bl, "m(D0 -> mult)");
    if (m_tupfl) {
        D0UIS(d0l, mode);
        D0UIS(d0bl, mode);
    }
    return d0l.size() + d0bl.size();
}

int CMB::make_dptokpipi(pvec &dpl, pvec &kml, pvec &pipl,
                        pvec &kpl, pvec &piml, int mode) {
    pvec dml;
    make_dptokpipi(dpl, dml, kml, pipl, kpl, piml, mode);
    appendMove(dpl, dml);
    return dpl.size();
}

int CMB::make_dptokpipi(pvec &dpl, pvec &dml, pvec &kml, pvec &pipl,
                        pvec &kpl, pvec &piml, int mode) {
    dpl.clear(); dml.clear();
    combination(dpl, Ptype("D+"), kml, pipl, pipl);
    combination(dml, Ptype("D-"), kpl, piml, piml);
    massCutSym(dpl, "m(D+)");
    massCutSym(dml, "m(D+)");
    if (m_tupfl) {
        UIS::SetDlInfo(dpl);
        UIS::SetDlInfo(dml);
        UIS::SetModeD(dpl, mode);
        UIS::SetModeD(dml, mode);
    }
    return dpl.size() + dml.size();
}

int CMB::make_pi0(PLM& plm) {
    plm["pi0"] = pvec();
    return make_pi0(plm["pi0"]);
}

int CMB::make_pi0(pvec &pi0l) {
    pi0l.clear();
    const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();
// pi0 > gamma gamma
    Mdst_pi0_Manager &pi0_mgr = Mdst_pi0_Manager::get_manager();
    for (vector<Mdst_pi0>::const_iterator i = pi0_mgr.begin();
         i != pi0_mgr.end(); i++) {
        const Mdst_pi0& pi0 = *i;
        Particle p(pi0);
        if (p.child(0).p().t() < par["Emin(gamma from pi0)"] ||
            p.child(1).p().t() < par["Emin(gamma from pi0)"]) continue;
        if (m_tupfl) {
            const Gen_hepevt &h = m_mc_flag ? get_hepevt(pi0) : null;
            if (h) p.relation().genHepevt(h);
            const Gen_hepevt &hg1 = m_mc_flag ?
                  get_hepevt(p.child(0).mdstGamma()) : null;
            if (hg1) p.child(0).relation().genHepevt(hg1);
            const Gen_hepevt &hg2 = m_mc_flag ?
                  get_hepevt(p.child(1).mdstGamma()) : null;
            if (hg2) p.child(1).relation().genHepevt(hg2);
            p.userInfo(Pi0UserInfo());
            static_cast<Pi0UserInfo&>(p.userInfo()).Mass(pi0.mass());
            static_cast<Pi0UserInfo&>(p.userInfo()).Chi2(pi0.chisq());
            setGammasError(p, HepPoint3D(0), 1.*HepSymMatrix(3, 1));  // Necessary for kinematic fitting
        }
        pi0l.push_back(p);
    }
    return pi0l.size();
}

void CMB::setH0toGGErrors(pvec& etal) {
    for (unsigned i = 0; i < etal.size(); i++)
        setGammasError(etal[i], HepPoint3D(0), 1.*HepSymMatrix(3, 1));
}

int CMB::make_gamma(PLM& plm) {
    plm["gamma"] = pvec();
    return make_gamma(plm["gamma"]);
}

int CMB::make_gamma(pvec& gl) {
    gl.clear(); makeGamma(gl); withPCut(gl, par["Emin(gamma)"]);
    return gl.size();
}

int CMB::make_etato2g(pvec& etal, pvec& gammas, int mode) {
    etal.clear();
// eta -> gamma gamma
    gammas.clear();
    makeGamma(gammas); withPCut(gammas, par["Emin(gamma from eta)"]);
    combination(etal, Ptype(EtaID), gammas, gammas);
    massCutSym(etal, "m(eta -> gg)");
    if (m_tupfl) {
        UIS::Seth0lInfo(etal);
        UIS::SetModeEta(etal, mode);
        CMB::setH0toGGErrors(etal);
    }
    return etal.size();
}

int CMB::make_etato3pi(pvec& etal, pvec& pipl, pvec& piml, pvec& pi0l, int mode) {
    etal.clear();
    combination(etal, Ptype(EtaID), pipl, piml, pi0l);
    massCutSym(etal, "m(eta -> ppp)");
    if (m_tupfl) {
        UIS::Seth0lInfo(etal);
        UIS::SetModeEta(etal, mode);
    }
    return etal.size();
}

int CMB::make_omega(pvec& omegal, pvec& pipl, pvec& piml, pvec& pi0l) {
    omegal.clear();
// omega -> pi+ pi- pi0
    combination(omegal, Ptype(OmegaID), pipl, piml, pi0l);
    massCutSym(omegal, "m(omega)");
    if (m_tupfl) UIS::Seth0lInfo(omegal);
    return omegal.size();
}

int CMB::make_phitokk(PLM& plm) {
    if (plm.find("K+") == plm.end()) make_kpi(plm);
    return make_phitokk(plm["phi"], plm["K+"], plm["K-"]);
}

int CMB::make_phitokk(pvec &phil, pvec &kpl, pvec &kml) {
    phil.clear();
    combination(phil, Ptype("PHI"), kpl, kml);
    massCutSym(phil, "m(phi)");
    if (m_tupfl) UIS::SetPhilInfo(phil);
    return phil.size();
}

int CMB::make_dstophipi(PLM& plm, bool separate, int mode) {
    if (plm.find("Ds+") == plm.end()) {
        plm["Ds+"] = pvec();
        plm["Ds-"] = pvec();
    }
    if (plm.find("phi") == plm.end()) make_phitokk(plm);
    if (plm.find("pi+") == plm.end()) make_kpi(plm);
    pvec dsp, dsm;
    make_dstoh0hp(dsp, dsm, plm["phi"], plm["pi+"], plm["pi-"], mode);
    appendMove(plm["Ds+"], dsp);
    appendMove(plm[(separate ? "Ds-" : "Ds+")], dsm);
    return plm["Ds+"].size() + plm["Ds-"].size();
}

int CMB::make_dstokstk(PLM& plm, bool separate, int mode) {
    if (plm.find("Ds+") == plm.end()) {
        plm["Ds+"] = pvec();
        plm["Ds-"] = pvec();
    }
    if (plm.find("K*0") == plm.end()) make_kstar0(plm, separate);
    if (plm.find("K+") == plm.end()) make_kpi(plm);
    pvec dsp, dsm;
    make_dstoKK(dsp, dsm, plm["K*0bar"], plm["K+"], plm["K*0"], plm["K-"], mode);
    appendMove(plm["Ds+"], dsp);
    appendMove(plm[(separate ? "Ds-" : "Ds+")], dsm);
    return plm["Ds+"].size() + plm["Ds-"].size();
}

int CMB::make_dstoks0k(PLM& plm, bool separate, int mode) {
    if (plm.find("Ds+") == plm.end()) {
        plm["Ds+"] = pvec();
        plm["Ds-"] = pvec();
    }
    if (plm.find("K0S") == plm.end()) make_ks(plm);
    if (plm.find("K+") == plm.end()) make_kpi(plm);
    pvec dsp, dsm;
    make_dstoh0hp(dsp, dsm, plm["K0S"], plm["K+"], plm["K-"], mode);
    appendMove(plm["Ds+"], dsp);
    appendMove(plm[(separate ? "Ds-" : "Ds+")], dsm);
    return plm["Ds+"].size() + plm["Ds-"].size();
}

int CMB::make_dstoh0hp(pvec &dsl, pvec &h0l, pvec &hpl, pvec &hml, int mode) {
    pvec dsml;
    make_dstoh0hp(dsl, dsml, h0l, hpl, hml, mode);
    appendMove(dsl, dsml);
    return dsl.size();
}

int CMB::make_dstoh0hp(pvec &dspl, pvec &dsml, pvec &h0l,
                       pvec &hpl, pvec &hml, int mode) {
    dspl.clear(); dsml.clear();
    combination(dspl, Ptype( DsID), h0l, hpl);
    combination(dsml, Ptype(-DsID), h0l, hml);
    massCutSym(dspl, "m(Ds+)");
    massCutSym(dsml, "m(Ds+)");
    if (m_tupfl) {
        UIS::SetDlInfo(dspl);
        UIS::SetDlInfo(dsml);
        UIS::SetModeD(dspl, mode);
        UIS::SetModeD(dsml, mode);
    }
    return dspl.size() + dsml.size();
}

int CMB::make_dstoKK(pvec &dsl, pvec &kst0bl, pvec &kpl,
                     pvec &kst0l, pvec &kml, int mode) {
    pvec dsml;
    make_dstoKK(dsl, dsml, kst0bl, kpl, kst0l, kml, mode);
    appendMove(dsl, dsml);
    return dsl.size();
}

int CMB::make_dstoKK(pvec &dspl, pvec &dsml, pvec &kst0bl, pvec &kpl,
                     pvec &kst0l, pvec &kml, int mode) {
    dspl.clear(); dsml.clear();
    combination(dspl, Ptype(DsID), kst0bl, kpl);
    combination(dsml, Ptype(-DsID), kst0l, kml);
    massCutSym(dspl, "m(Ds+)");
    massCutSym(dsml, "m(Ds+)");
    if (m_tupfl) {
        UIS::SetDlInfo(dspl);
        UIS::SetDlInfo(dsml);
        UIS::SetModeD(dspl, mode);
        UIS::SetModeD(dsml, mode);
    }
    return dspl.size() + dsml.size();
}

int CMB::make_dsstar(PLM& plm, bool separate) {
    plm["Ds*+"] = pvec();
    plm["Ds*-"] = pvec();
    make_dsstar(plm["Ds*+"], plm["Ds+"], plm["gamma"]);
    make_dsstar(plm["Ds*-"], plm["Ds-"], plm["gamma"]);
    if (!separate)
        appendMove(plm["Ds*+"], plm["Ds*-"]);
    return plm["Ds*+"].size() + plm["Ds*-"].size();
}

int CMB::make_dsstar(pvec &dsstl, pvec &dsl, pvec &gammal) {
    dsstl.clear();
    if (!dsl.size() || !gammal.size()) return 0;
    combination(dsstl, dsl[0].pType().charge() > 0 ? Ptype(DsstID) :
                                                    Ptype(-DsstID), dsl, gammal);
    dmassCutSym(dsstl, "dm(Ds*)");
    return dsstl.size();
}

int CMB::make_dstar(pvec &dstl, pvec &dl, pvec &xl) {
    dstl.clear();
    if (!dl.size() || !xl.size()) return 0;
    int dstlund = 0;
    string key;
    if (dl[0].lund() == DsID && xl[0].lund() == GamID) {
        dstlund = DsstID;  // D_s*+ -> Ds+ gamma
        key = "dm(Ds*)";
    } else if (dl[0].lund() == -DsID && xl[0].lund() == GamID) {
        dstlund = -DsstID;  // D_s*- -> Ds- gamma
        key = "dm(Ds*)";
    } else if (dl[0].lund() ==  D0ID && xl[0].lund() == Pi0ID) {
        dstlund = D0stID;  // D*0 -> D0 pi0
        key = "dm(D*0)";
    } else if (dl[0].lund() == -D0ID && xl[0].lund() == Pi0ID) {
        dstlund = -D0stID;  // anti-D*0 -> anti-D0 pi0
        key = "dm(D*0)";;
    } else if (abs(dl[0].lund()) == D0ID && xl[0].lund() ==  PiID) {
        dstlund = DstID;  // D*+ -> D0 pi+
        key = "dm(D*)";
    } else if (abs(dl[0].lund()) == D0ID && xl[0].lund() == -PiID) {
        dstlund = -DstID;  // D*- -> anti-D0 pi-
        key = "dm(D*)";
    } else {
        cerr << "CMB::make_dstar: wrong particles "
             << dl[0].lund() << " " << xl[0].lund() << endl;
        return 0;
    }
    const Ptype dsttype(dstlund);
    combination(dstl, dsttype, dl, xl);
    dmassCutSym(dstl, key);
    return dstl.size();
}

// D*sj -> Ds gamma
int CMB::make_dsjtodsgamma(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("gamma") == plm.end()) make_gamma(plm);
    pvec dsjp, dsjm;
    make_dsjtodsx(dsjp, dsjm, plm["Ds+"], plm["Ds-"], plm["gamma"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds gamma gamma
int CMB::make_dsjtods2gamma(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("gamma") == plm.end()) make_gamma(plm);
    pvec dsjp, dsjm;
    make_dsjtodsxy(dsjp, dsjm, plm["Ds+"], plm["Ds-"], plm["gamma"], plm["gamma"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds pi0 gamma
int CMB::make_dsjtodspi0gamma(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("gamma") == plm.end()) make_gamma(plm);
    if (plm.find("pi0") == plm.end())   make_pi0(plm);
    pvec dsjp, dsjm;
    make_dsjtodsxy(dsjp, dsjm, plm["Ds+"], plm["Ds-"], plm["pi0"], plm["gamma"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds pi0
int CMB::make_dsjtodspi0(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("pi0") == plm.end()) make_pi0(plm);
    pvec dsjp, dsjm;
    make_dsjtodsx(dsjp, dsjm, plm["Ds+"], plm["Ds-"], plm["pi0"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds pi+ pi-
int CMB::make_dsjtodspipi(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("pi+") == plm.end()) make_kpi(plm);
    pvec dsjp, dsjm;
    make_dsjtodsxy(dsjp, dsjm, plm["Ds+"], plm["Ds-"], plm["pi+"], plm["pi-"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds pi+ pi- gamma
int CMB::make_dsjtodspipigamma(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("pi+") == plm.end()) make_kpi(plm);
    if (plm.find("gamma") == plm.end()) make_gamma(plm);
    pvec dsjp, dsjm;
    make_dsjtodsxyz(dsjp, dsjm, plm["Ds+"], plm["Ds-"], plm["pi+"], plm["pi-"], plm["gamma"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds pi0 pi0
int CMB::make_dsjtods2pi0(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("pi0") == plm.end()) make_pi0(plm);
    pvec dsjp, dsjm;
    make_dsjtodsxy(dsjp, dsjm, plm["Ds+"], plm["Ds-"], plm["pi0"], plm["pi0"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds pi0 pi0 gamma
int CMB::make_dsjtods2pi0gamma(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("gamma") == plm.end()) make_gamma(plm);
    if (plm.find("pi0") == plm.end()) make_pi0(plm);
    pvec dsjp, dsjm;
    make_dsjtodsxyz(dsjp, dsjm, plm["Ds+"], plm["Ds-"], plm["pi0"], plm["pi0"], plm["gamma"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds* gamma
int CMB::make_dsjtodsstgamma(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("gamma") == plm.end()) make_gamma(plm);
    if (plm.find("Ds*+")  == plm.end()) make_dsstar(plm, separate);

    pvec dsjp, dsjm;
    make_dsjtodsx(dsjp, dsjm, plm["Ds*+"], plm["Ds*-"], plm["gamma"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds* pi0
int CMB::make_dsjtodsstpi0(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("pi0")  == plm.end()) make_pi0(plm);
    if (plm.find("Ds*+") == plm.end()) make_dsstar(plm, separate);

    pvec dsjp, dsjm;
    make_dsjtodsx(dsjp, dsjm, plm["Ds*+"], plm["Ds*-"], plm["pi0"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds* pi+ pi-
int CMB::make_dsjtodsstpipi(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("pi+")  == plm.end()) make_kpi(plm);
    if (plm.find("Ds*+") == plm.end()) make_dsstar(plm, separate);

    pvec dsjp, dsjm;
    make_dsjtodsxy(dsjp, dsjm, plm["Ds*+"], plm["Ds*-"], plm["pi+"], plm["pi-"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

// D*sj -> Ds* pi0 pi0
int CMB::make_dsjtodsst2pi0(PLM& plm, bool separate, int mode) {
    checkPLM(plm, "Dsj+", "Dsj-");
    if (plm.find("pi0")  == plm.end()) make_pi0(plm);
    if (plm.find("Ds*+") == plm.end()) make_dsstar(plm, separate);

    pvec dsjp, dsjm;
    make_dsjtodsxy(dsjp, dsjm, plm["Ds*+"], plm["Ds*-"], plm["pi0"], plm["pi0"], mode);
    appendMove(plm["Dsj+"], dsjp);
    appendMove(plm[(separate ? "Dsj-" : "Dsj+")], dsjm);
    return plm["Dsj+"].size() + plm["Dsj-"].size();
}

int CMB::make_dsjtodsx(pvec &dsjl, pvec &dspl, pvec &dsml, pvec &xl, int mode) {
    pvec dsjml;
    make_dsjtodsx(dsjl, dsjml, dspl, dsml, xl, mode);
    appendMove(dsjl, dsjml);
    return dsjl.size();
}

int CMB::make_dsjtodsx(pvec &dsjpl, pvec &dsjml, pvec &dspl, pvec &dsml, pvec &xl, int mode) {
    dsjpl.clear(); dsjml.clear();
    if (!xl.size()) return 0;
    if (dspl.size()) {
        combination(dsjpl, Ptype(DsID), dspl, xl);
        massCutAsym(dsjpl, "m(Dsj)");
    }
    if (dsml.size()) {
        combination(dsjml, Ptype(-DsID), dsml, xl);
        massCutAsym(dsjml, "m(Dsj)");
    }
    if (m_tupfl) {
        UIS::SetDlInfo(dsjpl);
        UIS::SetDlInfo(dsjml);
        UIS::SetModeD(dsjpl, mode);
        UIS::SetModeD(dsjml, mode);
    }
    return dsjpl.size() + dsjml.size();
}

int CMB::make_dsjtodsxy(pvec &dsjl, pvec &dspl, pvec &dsml,
                        pvec &xl, pvec &yl, int mode) {
    pvec dsjml;
    make_dsjtodsxy(dsjl, dsjml, dspl, dsml, xl, yl, mode);
    appendMove(dsjl, dsjml);
    return dsjl.size();
}

int CMB::make_dsjtodsxy(pvec &dsjpl, pvec &dsjml, pvec &dspl,
                        pvec &dsml, pvec &xl, pvec &yl, int mode) {
    dsjpl.clear(); dsjml.clear();
    if (!xl.size() || !yl.size()) return 0;
    if (dspl.size()) {
        combination(dsjpl, Ptype(DsID), dspl, xl, yl);
        massCutAsym(dsjpl, "m(Dsj)");
    }
    if (dsml.size()) {
        combination(dsjml, Ptype(-DsID), dsml, xl, yl);
        massCutAsym(dsjml, "m(Dsj)");
    }
    if (m_tupfl) {
        UIS::SetDlInfo(dsjpl);
        UIS::SetDlInfo(dsjml);
        UIS::SetModeD(dsjpl, mode);
        UIS::SetModeD(dsjml, mode);
    }
    return dsjpl.size() + dsjml.size();
}

int CMB::make_dsjtodsxyz(pvec &dsjl, pvec &dspl, pvec &dsml,
                        pvec &xl, pvec &yl, pvec &zl, int mode) {
    pvec dsjml;
    make_dsjtodsxyz(dsjl, dsjml, dspl, dsml, xl, yl, zl, mode);
    appendMove(dsjl, dsjml);
    return dsjl.size();
}

int CMB::make_dsjtodsxyz(pvec &dsjpl, pvec &dsjml, pvec &dspl,
                        pvec &dsml, pvec &xl, pvec &yl, pvec& zl, int mode) {
    dsjpl.clear(); dsjml.clear();
    if (!xl.size() || !yl.size() || !zl.size()) return 0;
    if (dspl.size()) {
        combination(dsjpl, Ptype(DsID), dspl, xl, yl, zl);
        massCutAsym(dsjpl, "m(Dsj)");
    }
    if (dsml.size()) {
        combination(dsjml, Ptype(-DsID), dsml, xl, yl, zl);
        massCutAsym(dsjml, "m(Dsj)");
    }
    if (m_tupfl) {
        UIS::SetDlInfo(dsjpl);
        UIS::SetDlInfo(dsjml);
        UIS::SetModeD(dsjpl, mode);
        UIS::SetModeD(dsjml, mode);
    }
    return dsjpl.size() + dsjml.size();
}

int CMB::make_b0toxy(pvec& b0l, pvec& xpl, pvec& yml, pvec& xml, pvec& ypl, int mode) {
    b0l.clear();
    const bool b0flag  = xpl.size() && yml.size();
    const bool b0bflag = xml.size() && ypl.size();
    if (!b0flag && !b0bflag) return 0;
    if (b0flag) combination(b0l, Ptype("B0"), xpl, yml);
    if (b0bflag) {
        pvec b0bl;
        combination(b0bl, Ptype(-B0ID), xml, ypl);
        appendMove(b0l, b0bl);
    }
    double mbc, de;
    for (unsigned i = 0; i < b0l.size(); i++) {
        Particle& B0 = b0l[i];
        RTools::Mbc_deltaE(B0,mbc,de);
        if (!m_tupfl) {  // skim
            if (fabs(de) < 0.3 && mbc > 5.22 && mbc < 5.30) {
                if (!RTools::IsDuplicated(B0)) return 1;
            }
        } else {  // ntuple
            if (de > 0.3 || de < -0.3 || mbc < 5.22 || mbc > 5.30) {
                b0l.erase(b0l.begin()+i); i--; continue;
            }
            if (RTools::IsDuplicated(B0)) {
                b0l.erase(b0l.begin()+i); i--; continue;
            }
            B0.userInfo(BUserInfo());
            UIS::SetMbc_deltaE(B0,mbc,de);
        }
    }
    if (m_tupfl) {
        UIS::SetModeB(b0l, mode);
        return b0l.size();
    }
    return 0;
}

int CMB::make_bptoxy(pvec& bpl, pvec& x0l, pvec& ypl, int mode) {
    bpl.clear();
    if (!x0l.size() || !ypl.size()) return 0;
    combination(bpl, ypl[0].pType().charge() > 0 ? Ptype("B+") : Ptype("B-"), x0l, ypl);
    double mbc, de;
    for (unsigned i = 0; i < bpl.size(); i++) {
        Particle& Bp = bpl[i];
        RTools::Mbc_deltaE(Bp,mbc,de);
        if (!m_tupfl) {  // skim
            if (fabs(de) < 0.3 && mbc > 5.22 && mbc < 5.30) {
                if(!RTools::IsDuplicated(Bp)) return 1;
            }
        } else {
            if (de > 0.3 || de < -0.3 || mbc < 5.22 || mbc > 5.30) {
                bpl.erase(bpl.begin() + i); i--; continue;
            }
            if (RTools::IsDuplicated(Bp)){
                bpl.erase(bpl.begin() + i); i--; continue;
            }
            Bp.userInfo(BUserInfo());
            UIS::SetMbc_deltaE(Bp, mbc, de);
        }
    }
    if (m_tupfl) {
        UIS::SetModeB(bpl, mode);
        return bpl.size();
    }
    return 0;
}

int CMB::make_b0tod0pipi(pvec& b0l, pvec &d0l, pvec &pipl, pvec &piml) {
    b0l.clear();
    combination(b0l,Ptype("B0"),d0l,pipl,piml);
    double mbc, de;
    for (unsigned i = 0; i < b0l.size(); i++) {
        Particle& B0 = b0l[i];
        RTools::Mbc_deltaE(B0, mbc, de);
        if (!m_tupfl) {
            if (fabs(de) < 0.35 && mbc > 5.18 && mbc < 5.31) {
                if (!RTools::IsDuplicated(B0)) return 1;
            }
        } else {  // tuple
            B0.userInfo(B0UserInfo());
            const Particle& D0 = B0.child(0);
            if (de > 0.3 || de < -0.3 || mbc < 5.25 || mbc > 5.30) {
                b0l.erase(b0l.begin()+i); i--; continue;
            }
            if (RTools::IsDuplicated(B0)) {
                b0l.erase(b0l.begin()+i); i--; continue;
            }
            UIS::SetB0toDhhRawInfo(B0);

            if (make_vtx_for_b0tod0pipi(B0)) {
                UIS::SetBadVtx(B0);
                UIS::SetMbc_deltaE(B0, mbc, de);
                continue;
             }
             if (make_mass_for_b0tod0pipi(B0)) {
                 UIS::SetBadMass(B0);
             }      
             UIS::SetMbc_deltaE(B0,mbc,de);
         }
    }
    return m_tupfl ? b0l.size() : 0;
}

int CMB::make_vtx_for_b0tod0pipi(Particle& b0) {
    const int d0index = RTools::ChldIndex(b0, 421, true);
    if (d0index < 0) return -4;  // There is no D0 among children
    Particle& D0 = b0.child(d0index);
    // * Vertex fitting * //
    DUserInfo& dinfo = dynamic_cast<DUserInfo&>(D0.userInfo());
    if (!dinfo.IsVtxFitDone()) {
        double d0_vtx_chisq = -1;
        const int d0_vtx_fit_flag = KineFitter::vtx_fit_r(D0, d0_vtx_chisq);
        dinfo.VtxChi2(d0_vtx_chisq);
        dinfo.VtxFitDone();
        dinfo.VtxFitGood(!d0_vtx_fit_flag);
    }
    if (!dinfo.IsVtxFitGood()) {
        cerr << "Bad D0 vtx fit for mode " << dinfo.Mode() << endl;
        return -1;
    }
    int fit_flag = 0; 
    kvertexfitter kvf = KineFitter::vtx_fit(b0, true, fit_flag);
    if (fit_flag) return -2;
    UIS::SetSigVtxFitInfo(b0, kvf);
    if (RTools::TagVertex(b0)) return -3;
    return 0;
}

int CMB::make_mass_for_b0tod0pipi(Particle& b0) {
    const int d0index = RTools::ChldIndex(b0, 421, true);
    if (d0index < 0) return -4;  // There is no D0 among children
    Particle& D0 = b0.child(d0index);
    DUserInfo& dinfo = dynamic_cast<DUserInfo&>(D0.userInfo());
    if (!dinfo.IsMassFitDone()) {
        double d0_mass_chisq = -1;
        const int d0_mass_fit_flag = KineFitter::mass_fit_r(D0, d0_mass_chisq);
        dinfo.MassChi2(d0_mass_chisq);
        dinfo.MassFitDone();
        dinfo.MassFitGood(!d0_mass_fit_flag);
    }
    if(!dinfo.IsMassFitGood()){
        cerr << "Bad D0 mass fit for mode " << dinfo.Mode() << endl;
        return -1;
    }
    kmassfitter kmf = KineFitter::mass_fit(b0, true);
    UIS::SetB0toDhhMassFitInfo(b0, kmf);
    return 0;
}

int CMB::make_b0tod0pi0pi0(pvec& b0l, pvec &d0l, pvec &pi0l) {
    b0l.clear();
    combination(b0l, Ptype("B0"), d0l, pi0l, pi0l);
    double mbc, de;
    for (unsigned i = 0; i < b0l.size(); i++) {
        Particle& B0 = b0l[i];
        RTools::Mbc_deltaE(B0,mbc,de);
        if (!m_tupfl) {
            if (fabs(de) < 0.32 && mbc > 5.18 && mbc < 5.31) {
                if (!RTools::IsDuplicated(B0)) return 1;
            }
        } else {
            B0.userInfo(B0UserInfo());
            if (de > 0.3 || de < -0.3 || mbc < 5.2 || mbc > 5.3) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            if (RTools::IsDuplicated(B0)) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            int d0index = 0;
            if(abs(B0.child(0).lund()) == 421) d0index = 0;
            else if(abs(B0.child(1).lund()) == 421) d0index = 1;
            else if(abs(B0.child(2).lund()) == 421) d0index = 2;
            else continue;
            Particle& D0 = B0.child(d0index);
            DUserInfo& dinfo = dynamic_cast<DUserInfo&>(D0.userInfo());
            if (!dinfo.IsVtxFitDone()) {
                double d0_vtx_chisq = -1;
                const int d0_vtx_fit_flag = KineFitter::vtx_fit_r(D0,d0_vtx_chisq);
                dinfo.VtxChi2(d0_vtx_chisq);
                dinfo.VtxFitDone();
                dinfo.VtxFitGood(!d0_vtx_fit_flag);
            }
            const bool d0_vtx_fit_good = ((const DUserInfo&)D0.userInfo()).IsVtxFitGood();
            if (!d0_vtx_fit_good) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            if (KineFitter::b0tod0pipi_d0vtx_fit(B0)) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            if (RTools::TagVertex(B0)) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            UIS::SetMbc_deltaE(B0,mbc,de);
        }
     }
     return m_tupfl ? b0l.size() : 0;
}

int CMB::make_dststtoxyz(pvec& dststl, pvec& xl, pvec& yl, pvec& zl,
                         const int mode) {
    dststl.clear();
    if (!xl.size()) return 0;
    if (!yl.size()) return 0;
    if (!zl.size()) return 0;
    combination(dststl, Ptype(DsjID), xl, yl, zl);
    massCutAsym(dststl, "m(Dsj)");
    if (m_tupfl) {
        UIS::SetDlInfo(dststl);
        UIS::SetModeD(dststl, mode);
    }
    return dststl.size();
}

#if defined(BELLE_NAMESPACE)
}
#endif

