#include "binfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//     B     //
///////////////
BUserInfo::BUserInfo() :
  m_de(0.),
  m_mbc(0.),
  m_mode(-1)
{
}


BUserInfo::~BUserInfo()
{
}

BUserInfo::BUserInfo(const BUserInfo &x)
{
  *this = x;
}

BUserInfo* BUserInfo::clone(void) const
{
  BUserInfo *x = new BUserInfo( *this );
  return x;
}

BUserInfo & BUserInfo::operator = (const BUserInfo &x)
{
  m_de = x.m_de;
  m_mbc = x.m_mbc;
  m_mode = x.m_mode;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 


