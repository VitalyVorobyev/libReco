#ifndef B0INFO_H
#define B0INFO_H

#include "particle/ParticleUserInfo.h"
#include "binfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

class B0UserInfo : public BUserInfo {
 public:
    B0UserInfo();
    B0UserInfo(const B0UserInfo &);
    virtual ~B0UserInfo();
    B0UserInfo * clone(void) const;
    B0UserInfo & operator = (const B0UserInfo &);

 public:
    void CosTheta(const double& v) {m_cos_theta = v;}
    double CosTheta(void) const {return m_cos_theta;}

    void CosThetaCMS(const double& v) {m_cos_theta_cms = v;}
    double CosThetaCMS(void) const {return m_cos_theta_cms;}

    void Ecms(const double& v) {m_e_cms = v;}
    double Ecms(void) const {return m_e_cms;}

//////////////
//  TagVtx  //
//////////////
    void TagChi2(const double& v) {m_tag_chi2 = v;}
    double TagChi2(void) const {return m_tag_chi2;}

    void TagNDF(const int v) {m_tag_ndf = v;}
    int TagNDF(void) const {return m_tag_ndf;}

/////////
// Vtx //
/////////
    void VtxChi2(const double& v) {m_vtx_chi2 = v;}
    double VtxChi2(void) const {return m_vtx_chi2;}

// CP fit
    void ZSig(const double& v) {m_z_sig = v;}
    double ZSig(void) const {return m_z_sig;}

    void ZAsc(const double& v) {m_z_asc = v;}
    double ZAsc(void) const {return m_z_asc;}

    void SzSig(const double& v) {m_sz_sig = v;}
    double SzSig(void) const {return m_sz_sig;}

    void SzAsc(const double& v) {m_sz_asc = v;}
    double SzAsc(void) const {return m_sz_asc;}

    void StSig(const double& v) {m_st_sig = v;}
    double StSig(void) const {return m_st_sig;}

    void StAsc(const double& v) {m_st_asc = v;}
    double StAsc(void) const {return m_st_asc;}

    void NtrkSig(const int& v) {m_ntrk_sig = v;}
    int NtrkSig(void) const {return m_ntrk_sig;}

    void NtrkAsc(const int& v) {m_ntrk_asc = v;}
    int NtrkAsc(void) const {return m_ntrk_asc;}

    void NdfSig(const int& v) {m_ndf_z_sig = v;}
    int NdfSig(void) const {return m_ndf_z_sig;}

    void NdfAsc(const int& v) {m_ndf_z_asc = v;}
    int NdfAsc(void) const {return m_ndf_z_asc;}

    void ChisqSig(const double& v) {m_chisq_z_sig = v;}
    double ChisqSig(void) const {return m_chisq_z_sig;}

    void ChisqAsc(const double& v) {m_chisq_z_asc = v;}
    double ChisqAsc(void) const {return m_chisq_z_asc;}

    void ClSig(const double& v) {m_cl_z_sig = v;}
    double ClSig(void) const {return m_cl_z_sig;}

    void ClAsc(const double& v) {m_cl_z_asc = v;}
    double ClAsc(void) const {return m_cl_z_asc;}

//////////
//  MC  //
//////////
    void FlvMC(const int v) {m_flv_mc = v;}
    int FlvMC(void) const {return m_flv_mc;}

//////////////
//  TagFlv  //
//////////////
    void fqLH(const double& v) {m_fq_LH = v;}
    double fqLH(void) const {return m_fq_LH;}

    void fqErrLH(const double& v) {m_fq_err_LH = v;}
    double fqErrLH(void) const {return m_fq_err_LH;}

    void qLH(const double& v) {m_q_LH = v;}
    double qLH(void) const {return m_q_LH;}

    void tagl(const int v){m_tagl = v;}
    int tagl(void) const {return m_tagl;}

    void taglid(const int v){m_taglid = v;}
    int taglid(void) const {return m_taglid;}

/////////////////
// 3 body vars //
/////////////////
    void CosHel12(const double& v) {m_cos_hel_dpip = v;}
    void CosHel13(const double& v) {m_cos_hel_dpim = v;}
    void CosHel23(const double& v) {m_cos_hel_pipi = v;}

    void Mass12(const double& v) {m_dpip_mass = v;}
    void Mass13(const double& v) {m_dpim_mass = v;}
    void Mass23(const double& v) {m_pipi_mass = v;}

    double CosHel12(void) const {return m_cos_hel_dpip;}
    double CosHel13(void) const {return m_cos_hel_dpim;}
    double CosHel23(void) const {return m_cos_hel_pipi;}

    double Mass12(void) const {return m_dpip_mass;}
    double Mass13(void) const {return m_dpim_mass;}
    double Mass23(void) const {return m_pipi_mass;}

    void MassChi2(const double& x) {m_mass_chisq = x;}
    double MassChi2(void) const {return m_mass_chisq;}

 private:
    int m_tagl, m_taglid;

    int m_mode;
    double m_tag_chi2;
    int m_tag_ndf;

    double m_vtx_chi2;
    double m_z_sig_d0, m_sz_sig_d0, m_chisq_sig_d0;
    int m_ndf_sig_d0, m_ntrk_sig_d0;
    double m_z_sig_pipi, m_sz_sig_pipi, m_chisq_sig_pipi;
    int m_ndf_sig_pipi, m_ntrk_sig_pipi;
    double m_z_sig_noip, m_sz_sig_noip, m_chisq_sig_noip;
    int m_ndf_sig_noip, m_ntrk_sig_noip;

    double m_z_sig;
    double m_z_asc;
    double m_sz_sig;
    double m_sz_asc;
    double m_st_sig;
    double m_st_asc;
    int m_ntrk_sig;
    int m_ntrk_asc;
    int m_ndf_z_sig;
    int m_ndf_z_asc;
    double m_chisq_z_sig;
    double m_chisq_z_asc;
    double m_cl_z_asc;
    double m_cl_z_sig;

    double m_fq_NN;
    double m_fq_err_NN;
    double m_q_NN;
    double m_fq_LH;
    double m_fq_err_LH;
    double m_q_LH;

    int m_flv_mc;
    double m_mismass;
    double m_cos_theta;
    double m_cos_theta_cms;
    double m_e_cms;

    double m_cos_hel_dpip;
    double m_cos_hel_dpim;
    double m_cos_hel_pipi;

    double m_dpip_mass;
    double m_dpim_mass;
    double m_pipi_mass;

    double m_mass_chisq;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
