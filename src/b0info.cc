#include "b0info.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

////////////////
//     B0     //
////////////////
B0UserInfo::B0UserInfo() :
  BUserInfo(),
  m_tag_chi2(-1),
  m_tag_ndf(0),
  m_vtx_chi2(-1),
  m_z_sig_d0(-1.),
  m_sz_sig_d0(-1.),
  m_chisq_sig_d0(-1.),
  m_ndf_sig_d0(0),
  m_ntrk_sig_d0(0),
  m_z_sig_pipi(-1.),
  m_sz_sig_pipi(-1.),
  m_chisq_sig_pipi(-1.),
  m_ndf_sig_pipi(0),
  m_ntrk_sig_pipi(0),
  m_z_sig_noip(-1.),
  m_sz_sig_noip(-1.),
  m_chisq_sig_noip(-1.),
  m_ndf_sig_noip(0),
  m_ntrk_sig_noip(0),
  m_z_sig(-1.),
  m_z_asc(-1.),
  m_sz_sig(-1.),
  m_sz_asc(-1.),
  m_st_sig(-1.),
  m_st_asc(-1.),
  m_ntrk_sig(0),
  m_ntrk_asc(0),
  m_ndf_z_sig(0),
  m_ndf_z_asc(0),
  m_chisq_z_sig(-1.),
  m_chisq_z_asc(-1.),
  m_cl_z_asc(-1.),
  m_cl_z_sig(-1.),
  m_fq_LH(-1.),
  m_fq_err_LH(-1.),
  m_q_LH(-1.),
  m_flv_mc(0),
  m_mismass(0),
  m_cos_theta(-2.),
  m_cos_theta_cms(-2.),
  m_e_cms(-1.),
  m_tagl(-1),
  m_taglid(-1),
  m_cos_hel_dpip(-99.),
  m_cos_hel_dpim(-99.),
  m_cos_hel_pipi(-99.),
  m_dpip_mass(-99.),
  m_dpim_mass(-99.),
  m_pipi_mass(-99.),
  m_mass_chisq(-99.)
{
}

B0UserInfo::~B0UserInfo()
{
}

B0UserInfo::B0UserInfo(const B0UserInfo &x)
{
  *this = x;
}

B0UserInfo* B0UserInfo::clone(void) const
{
  B0UserInfo *x = new B0UserInfo( *this );
  return x;
}

B0UserInfo & B0UserInfo::operator = (const B0UserInfo &x)
{
  BUserInfo::operator=(x);
  m_z_sig_d0 = x.m_z_sig_d0;
  m_sz_sig_d0 = x.m_sz_sig_d0;
  m_chisq_sig_d0 = x.m_chisq_sig_d0;
  m_ndf_sig_d0 = x.m_ndf_sig_d0;
  m_ntrk_sig_d0 = x.m_ntrk_sig_d0;
  m_z_sig_pipi = x.m_z_sig_pipi;
  m_sz_sig_pipi = x.m_sz_sig_pipi;
  m_chisq_sig_pipi = x.m_chisq_sig_pipi;
  m_ndf_sig_pipi = x.m_ndf_sig_pipi;
  m_ntrk_sig_pipi = x.m_ntrk_sig_pipi;
  m_z_sig_noip = x.m_z_sig_noip;
  m_sz_sig_noip = x.m_sz_sig_noip;
  m_chisq_sig_noip = x.m_chisq_sig_noip;
  m_ndf_sig_noip = x.m_ndf_sig_noip;
  m_ntrk_sig_noip = x.m_ntrk_sig_noip;
  m_tag_chi2 = x.m_tag_chi2;
  m_tag_ndf = x.m_tag_ndf;
  m_vtx_chi2 = x.m_vtx_chi2;
  m_z_sig = x.m_z_sig;
  m_z_asc = x.m_z_asc;
  m_sz_sig = x.m_sz_sig;
  m_sz_asc = x.m_sz_asc;
  m_st_sig = x.m_st_sig;
  m_st_asc = x.m_st_asc;
  m_ntrk_sig = x.m_ntrk_sig;
  m_ntrk_asc = x.m_ntrk_asc;
  m_ndf_z_sig = x.m_ndf_z_sig;
  m_ndf_z_asc = x.m_ndf_z_asc;
  m_chisq_z_sig = x.m_chisq_z_sig;
  m_chisq_z_asc = x.m_chisq_z_asc;
  m_cl_z_sig = x.m_cl_z_sig;
  m_cl_z_asc = x.m_cl_z_asc;
  m_fq_NN = x.m_fq_NN;
  m_fq_err_NN = x.m_fq_err_NN;
  m_q_NN = x.m_q_NN;
  m_fq_LH = x.m_fq_LH;
  m_fq_err_LH = x.m_fq_err_LH;
  m_q_LH = x.m_q_LH;
  m_flv_mc = x.m_flv_mc;
  m_mismass = x.m_mismass;
  m_cos_theta = x.m_cos_theta;
  m_cos_theta_cms = x.m_cos_theta_cms;
  m_e_cms = x.m_e_cms;
  m_tagl = x.m_tagl;
  m_taglid = x.m_taglid;
  m_cos_hel_dpip = x.m_cos_hel_dpip;
  m_cos_hel_dpim = x.m_cos_hel_dpim;
  m_cos_hel_pipi = x.m_cos_hel_pipi;
  m_dpip_mass = x.m_dpip_mass;
  m_dpim_mass = x.m_dpim_mass;
  m_pipi_mass = x.m_pipi_mass;
  m_mass_chisq = x.m_mass_chisq;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
