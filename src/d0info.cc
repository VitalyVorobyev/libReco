#include "d0info.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

////////////////
//     D0     //
////////////////
D0UserInfo::D0UserInfo() :
  DUserInfo(),
  m_m2p(0),
  m_m2m(0),
  m_m2p_raw(0),
  m_m2m_raw(0)
{
}

D0UserInfo::~D0UserInfo()
{
}

D0UserInfo::D0UserInfo(const D0UserInfo &x) :
  DUserInfo(x)
{
  *this = x;
}

D0UserInfo* D0UserInfo::clone(void) const
{
  D0UserInfo *x = new D0UserInfo(*this);
  return x;
}

D0UserInfo & D0UserInfo::operator = (const D0UserInfo &x)
{
  DUserInfo::operator=(x);
  m_m2p     = x.m_m2p;
  m_m2m     = x.m_m2m;
  m_m2p_raw = x.m_m2p_raw;
  m_m2m_raw = x.m_m2m_raw;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
