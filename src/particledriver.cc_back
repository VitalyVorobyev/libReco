#include "particledriver.h"

const Particle& b2d0pipi::get_d0(const Particle& b0){
  if(b0.child(0).lund() != 423) return b0.child(0);
  else return b0.child(0).child(0);
}
const Particle& b2d0pipi::get_pip(const Particle& b0){
  const Particle& d0 = get_d0(b0);
  return d0.child(1);
}
const Particle& b2d0pipi::get_pim(const Particle& b0){
  const Particle& d0 = get_d0(b0);
  return d0.child(2);
}
const Particle& b2d0pipi::get_k0s(const Particle& b0){
  const Particle& d0 = get_d0(b0);
  return d0.child(0);
}


