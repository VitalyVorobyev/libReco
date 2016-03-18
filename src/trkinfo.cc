#include "trkinfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

//////////////
//  Tracks  //
//////////////
TrkUserInfo::TrkUserInfo() :
  m_rz_svd_hits(0),
  m_rphi_svd_hits(0),
  m_atckpi(0.),
  m_atckp(0.),
  m_eid(0.),
  m_muid(0.),
  m_r(0.),
  m_z(0.)
{
}

TrkUserInfo::~TrkUserInfo()
{
}

TrkUserInfo::TrkUserInfo(const TrkUserInfo &x)
{
  *this = x;
}

TrkUserInfo* TrkUserInfo::clone(void) const
{
  TrkUserInfo *x = new TrkUserInfo( *this );
  return x;
}

TrkUserInfo & TrkUserInfo::operator = (const TrkUserInfo &x)
{
  m_rz_svd_hits   = x.m_rz_svd_hits;
  m_rphi_svd_hits = x.m_rphi_svd_hits;
  m_atckpi        = x.m_atckpi;
  m_atckp         = x.m_atckp;
  m_eid           = x.m_eid;
  m_muid          = x.m_muid;
  m_r             = x.m_r;
  m_z             = x.m_z;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
