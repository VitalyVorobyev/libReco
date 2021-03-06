#include "b0info.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//    B0     //
///////////////
B0UserInfo::B0UserInfo() :
    m_tagl(-1), m_taglid(-1),
    m_mode(-1),
    m_tag_chi2(-1),
    m_tag_ndf(-1),
    m_vtx_chi2(-1),

    m_z_sig(-99),
    m_z_asc(-99),
    m_sz_sig(0),
    m_sz_asc(0),
    m_st_sig(0),
    m_st_asc(0),
    m_ntrk_sig(-1),
    m_ntrk_asc(-1),
    m_ndf_z_sig(-1),
    m_ndf_z_asc(-1),
    m_chisq_z_sig(0),
    m_chisq_z_asc(0),
    m_cl_z_asc(-1),
    m_cl_z_sig(-1),

    m_fq_NN(-2),
    m_fq_err_NN(-2),
    m_q_NN(-2),
    m_fq_LH(-2),
    m_fq_err_LH(-2),
    m_q_LH(-2),

    m_flv_mc(0),
    m_mismass(0),
    m_cos_theta(-2),
    m_cos_theta_cms(-2),
    m_e_cms(0),

    m_cos_hel_dpip(-2),
    m_cos_hel_dpim(-2),
    m_cos_hel_pipi(-2),

    m_dpip_mass(0),
    m_dpim_mass(0),
    m_pipi_mass(0),

    m_mass_chisq(-1.) {}

B0UserInfo* B0UserInfo::clone(void) const {
    B0UserInfo *x = new B0UserInfo(*this);
    return x;
}

#if defined(BELLE_NAMESPACE)
}
#endif 

