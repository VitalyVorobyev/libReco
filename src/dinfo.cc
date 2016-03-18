#include "dinfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

////////////////////
// D0, D+ and Ds+ //
////////////////////
DUserInfo::DUserInfo() :
  m_mass(0.),
  m_vfit_done(false),
  m_vfit_good(false),
  m_vtx_chi2(-1.),
  m_mfit_done(false),
  m_mfit_good(false),
  m_mass_chi2(-1.),
  m_mode(-1)
{
}

DUserInfo::~DUserInfo()
{
}

DUserInfo::DUserInfo(const DUserInfo &x)
{
  *this = x;
}

DUserInfo* DUserInfo::clone(void) const
{
  DUserInfo *x = new DUserInfo( *this );
  return x;
}

DUserInfo& DUserInfo::operator=(const DUserInfo &x){
  m_mass      = x.m_mass;
  m_vfit_done = x.m_vfit_done;
  m_vfit_good = x.m_vfit_good;
  m_vtx_chi2  = x.m_vtx_chi2;
  m_mfit_done = x.m_mfit_done;
  m_mfit_good = x.m_mfit_good;
  m_mass_chi2 = x.m_mass_chi2;
  m_mode      = x.m_mode;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
