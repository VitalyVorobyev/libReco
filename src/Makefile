Sources=DalitzVars.cc DeltaEMbc.cc EvtInfo.cc EvtShape.cc GammaInfo.cc GenHepEvt.cc GenParticleInfo.cc HamletTag.cc IPBoost.cc Ks0Info.cc Pi0Info.cc SFWMInfo.cc TrackInfo.cc VertexInfo.cc

INC=-I../include

data_str_dict.cc: $(Sources)
	rootcint -f $@ -c $(INC) -p $^
