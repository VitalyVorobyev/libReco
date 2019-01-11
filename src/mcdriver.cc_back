int b2d0pipi::BDecayTime_MC(const Gen_hepevt& b0,double& sigDT,double& tagDT,double& sigZ,double& tagZ,double& UpsZ,double& zups){
  if(!b0){
    sigDT = -999; sigZ = -998;
    tagDT = -999; tagZ = -998;
    return -1;
  }
  Gen_hepevt btag;
  if(TagB_MC(b0,btag)){
    sigDT = -999; sigZ = -999;
    tagDT = -999; tagZ = -999;
    return -1;
  }
  Gen_hepevt btagch = mc_list[btag.da(0)-1];
  if(!btagch){
    sigDT = -998; sigZ = -998;
    tagDT = -998; tagZ = -998;
    return -2;
  }
  Gen_hepevt bsigch = mc_list[b0.da(0)-1];
  if(!bsigch){
    sigDT = -997; sigZ = -997;
    tagDT = -997; tagZ = -997;
    return -3;
  }
  if(dump){
    cout << "Btag ch id = " << btagch.idhep();
    cout << ", Btag id = " << btag.idhep();
    cout << ", Bsig ch id = " << bsigch.idhep();
    cout << ", Bsig id = " << b0.idhep() << endl;
  }
  HepLorentzVector btagchLV = mc_4vert_b0(btagch);
  HepLorentzVector btagLV   = mc_4vert_b0(btag);
  HepLorentzVector tagLV = btagchLV - btagLV;
  tagZ = tagLV.z();
  HepLorentzVector bsigLV = mc_4vert_b0(b0);
  UpsZ = bsigLV.z();
  zups = UpsZ;
  if(abs(btagLV.z()-bsigLV.z())>1.e-4) cout << "tagZ = " << btagLV.z() << ", sigZ = " << bsigLV.z() << endl;
  HepLorentzVector sigLV = mc_4vert_d0(b0) - bsigLV;
  sigZ = sigLV.z();
  HepLorentzVector tagLM = mc_4mom_b0(btag);
  HepLorentzVector sigLM = mc_4mom_b0(b0);
  HepLorentzVector sumLM = tagLM+sigLM;
  Hep3Vector bv = -sumLM.boostVector();
  Hep3Vector BEbv = -BeamEnergy::CMBoost();
if(dump){
    cout << "My boost: (" << bv.x() << "," << bv.y() << "," << bv.z() << ")" << endl;
    cout << "BE boost: (" << BEbv.x() << "," << BEbv.y() << "," << BEbv.z() << ")" << endl;
  }
  tagLV.boost(bv);
  sigLV.boost(bv);
  sigDT = sigLV.t();
  tagDT = tagLV.t();
  return 0;
}

int b2d0pipi::TagB_MC(const Gen_hepevt& b0, Gen_hepevt& btag){
  const int b0id = b0.idhep();
  for(int i=0; i<mc_list.size(); i++){
    if(mc_list[i].idhep() == -b0id){
      btag = mc_list[i];
      return 0;
    }
  }
  return -1;
}

Gen_hepevt b2d0pipi::mc_get_d0(const Gen_hepevt& b0){
  if(abs(mc_list[b0.da(0)-1].idhep()) == 423){
    Gen_hepevt& dst0 = mc_list[b0.da(0)-1];
    if(abs(mc_list[dst0.da(0)-1].idhep()) == 421) return mc_list[dst0.da(0)-1];
    else return mc_list[dst0.da(0)];
  } else if(abs(mc_list[b0.da(0)].idhep()) == 423){
    Gen_hepevt& dst0 = mc_list[b0.da(0)];
    if(abs(mc_list[dst0.da(0)-1].idhep()) == 421) return mc_list[dst0.da(0)-1];
    else return mc_list[dst0.da(0)];
  } else{
    if(abs(mc_list[b0.da(0)-1].idhep()) == 421) return mc_list[b0.da(0)-1];
    else                                        return mc_list[b0.da(0)];
  }
}

Gen_hepevt b2d0pipi::mc_get_pip(const Gen_hepevt& b0){
  if(mc_list.at(mc_get_d0(b0).da(0)).idhep() == 211) return mc_list.at(mc_get_d0(b0).da(0));
  if(mc_list.at(mc_get_d0(b0).da(0)+1).idhep() == 211) return mc_list.at(mc_get_d0(b0).da(0)+1);
  return mc_list.at(mc_get_d0(b0).da(0)-1);
}
Gen_hepevt b2d0pipi::mc_get_pim(const Gen_hepevt& b0){
  if(mc_list.at(mc_get_d0(b0).da(0)).idhep() == -211) return mc_list.at(mc_get_d0(b0).da(0));
  if(mc_list.at(mc_get_d0(b0).da(0)+1).idhep() == -211) return mc_list.at(mc_get_d0(b0).da(0)+1);
  return mc_list.at(mc_get_d0(b0).da(0)-1);
}
Gen_hepevt b2d0pipi::mc_get_k0s(const Gen_hepevt& b0){
  if(abs(mc_list.at(mc_get_d0(b0).da(0)).idhep()) == 310) return mc_list.at(mc_get_d0(b0).da(0));
  if(abs(mc_list.at(mc_get_d0(b0).da(0)+1).idhep()) == 310) return mc_list.at(mc_get_d0(b0).da(0)+1);
  return mc_list.at(mc_get_d0(b0).da(0)-1);
}
double b2d0pipi::DDecayTime_MC(const Gen_hepevt& b0){
  HepLorentzVector d4r = mc_4vert_k0s(b0) - mc_4vert_d0(b0);
  Hep3Vector bv = -mc_4mom_d0(b0).boostVector();
  d4r.boost(bv);
  return d4r.t();
}

HepLorentzVector b2d0pipi::mc_4vert_b0(const Gen_hepevt& b0){
  if(dump){
    cout << b0 << " (";
    cout << b0.idhep() << ") : (";
    cout << b0.V(0) << ",";
    cout << b0.V(1) << ",";
    cout << b0.V(2) << ",";
    cout << b0.V(3) << ")" << endl;
  }
  return HepLorentzVector(b0.V(0),b0.V(1),b0.V(2),b0.V(3));
}
HepLorentzVector b2d0pipi::mc_4vert_d0(const Gen_hepevt& b0){
  const Gen_hepevt& d0 = mc_get_d0(b0);
  return HepLorentzVector(d0.V(0),d0.V(1),d0.V(2),d0.V(3));
}
HepLorentzVector b2d0pipi::mc_4vert_k0s(const Gen_hepevt& b0){
  const Gen_hepevt& k0s = mc_get_k0s(b0);
  return HepLorentzVector(k0s.V(0),k0s.V(1),k0s.V(2),k0s.V(3));
}
HepLorentzVector b2d0pipi::mc_4vert_pip(const Gen_hepevt& b0){
  const Gen_hepevt& pip = mc_get_pip(b0);
  return HepLorentzVector(pip.V(0),pip.V(1),pip.V(2),pip.V(3));
}
HepLorentzVector b2d0pipi::mc_4mom_b0(const Gen_hepevt& b0){
  return HepLorentzVector(b0.P(0),b0.P(1),b0.P(2),b0.P(3));
}
HepLorentzVector b2d0pipi::mc_4mom_d0(const Gen_hepevt& b0){
  const Gen_hepevt& d0 = mc_get_d0(b0);
  return HepLorentzVector(d0.P(0),d0.P(1),d0.P(2),d0.P(3));
}
HepLorentzVector b2d0pipi::mc_4mom_pip(const Gen_hepevt& b0){
  const Gen_hepevt& pip = mc_get_pip(b0);
  return HepLorentzVector(pip.P(0),pip.P(1),pip.P(2),pip.P(3));
}
HepLorentzVector b2d0pipi::mc_4mom_pim(const Gen_hepevt& b0){
  const Gen_hepevt& pim = mc_get_pim(b0);
  return HepLorentzVector(pim.P(0),pim.P(1),pim.P(2),pim.P(3));
}
HepLorentzVector b2d0pipi::mc_4mom_k0s(const Gen_hepevt& b0){
  const Gen_hepevt& k0s = mc_get_k0s(b0);
  return HepLorentzVector(k0s.P(0),k0s.P(1),k0s.P(2),k0s.P(3));
}

double b2d0pipi::z_asc_mc(const Particle& _b0){
  if(!_b0.relation().genHepevt()) return -999;
  const int b0id = _b0.relation().genHepevt().idhep();
  for(int i=0; i<mc_list.size(); i++){
    if(mc_list[i].idhep() == -b0id){
      return mc_list[mc_list[i].da(0)-1].V(3);
    }
  }
  return 999;
}

//double b2d0pipi::t_asc_mc(const Particle& _b0){
//  if(!_b0.relation().genHepevt()) return -999;
//  const int b0id = _b0.relation().genHepevt().idhep();
//  for(int i=0; i<mc_list.size(); i++){
//    if(mc_list[i].idhep() == -b0id){
//      return mc_list[mc_list[i].da(0)-1].V(0);
//    }
//  }
//  return 999;
//}

int b2d0pipi::GetMpMm_MC(const Gen_hepevt& b0,double& mp, double& mm){
  const HepLorentzVector& mom_pip = mc_4mom_pip(b0);
  const HepLorentzVector& mom_pim = mc_4mom_pim(b0);
  const HepLorentzVector& mom_k0s = mc_4mom_k0s(b0);

  HepLorentzVector s = mom_pip+mom_k0s;
  mp = s.m2();
  s = mom_pim+mom_k0s;
  mm = s.m2();

  return 0;
}

int b2d0pipi::NPTag(Particle& _b0){
  if(!_b0.relation().genHepevt()) return 0;
  const Gen_hepevt& b0 = _b0.relation().genHepevt();
  Gen_hepevt b0tag;
  const int b0id = _b0.relation().genHepevt().idhep();
//  cout << "b0id = " << b0id << ", size = " << mc_list.size() << endl;
  int i=0;
  for(i=0; i<mc_list.size(); i++){
    if(mc_list[i].idhep() == -b0id){
      b0tag = mc_list[i];
      break;
    }
  }
  if(i == mc_list.size()){
    cout << "Can't find tag B0" << endl;
    return -1;
  }
  for(i = b0tag.daFirst(); i <= b0tag.daLast(); i++){
    if(!i) continue;
    const Gen_hepevt& da = mc_list[i-1];
    int did = abs(da.idhep());
//    cout << "da" << i << ": " << did << endl;
    if(!did) continue;
    if(did == 411 || did == 421) return 1;
    for(int j = da.daFirst(); j <= da.daLast(); j++){
      if(!j) continue;
      const Gen_hepevt& dda = mc_list[j-1];
      int ddid = abs(dda.idhep());
//      cout << " dda" << j << ": " << ddid << endl;
      if(ddid == 411 || ddid == 421) return 1;
      for(int k = dda.daFirst(); k <= dda.daLast(); k++){
        if(!k) continue;
        const Gen_hepevt& ddda = mc_list[k-1];
        int dddid = abs(ddda.idhep());
//        cout << "  ddda" << k << ": " << dddid << endl;
        if(dddid == 411 || dddid == 421) return 1;
      }
    }
  }
  return 0;
}

/////////////////////
//   MC Functions  //
/////////////////////
int b2d0pipi::IsSignalD(const Particle& _d0,int& mode, int& h0mode, int& flv, double& z, double& t, double& td0, double& mp, double& mm, double& zups){
  mode = 0; h0mode = 0; flv = 0;
  if(!_d0.relation().genHepevt()) return -1;
  const Gen_hepevt& d0 = _d0.relation().genHepevt();
  if(!d0.moFirst()) return -5;
  const Gen_hepevt& b0 = mc_list[d0.moFirst()-1];
  if(abs(b0.idhep()) != 511) return -4;
  if((b0.daLast()-b0.daFirst())>1) return -2;
  const Gen_hepevt& h0 = (mc_list[b0.daFirst()-1].idhep() ==  d0.idhep()) ? mc_list[b0.daLast()-1] : mc_list[b0.daFirst()-1];
  switch(h0.idhep()){
  case 111:// pi0
    mode = 1;
    h0mode = 10;
    break;
  case 113:// rho(770)
    mode = 4;
    h0mode = 40;
    break;
  case 221:// eta
    mode = 2;
    h0mode = (h0.daLast()-h0.daFirst()) == 1 ? 10 : 20;
    break;
  case 223:// omega
    mode = 3;
    h0mode = 20;
    break;
  default:
    return -3;
  }
  b0.idhep() == 511 ? flv = 1 : flv = -1;
  double tasc,zasc,zup;
  BDecayTime_MC(b0,t,tasc,z,zasc,zup,zups);
  td0 = DDecayTime_MC(b0);
  GetMpMm_MC(b0,mp,mm);
  if(b0.idhep() == -511) flv = -1;
  else                   flv = 1;
  return 0;
}

