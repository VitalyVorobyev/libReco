Sources=DalitzVars.h DeltaEMbc.h EvtInfo.h EvtShape.h GammaInfo.h GenHepEvt.h GenParticleInfo.h HamletTag.h IPBoost.h Ks0Info.h Pi0Info.h SFWMInfo.h TrackInfo.h VertexInfo.h TrackInfo2.h H0Info.h

#Sources=$(wildcard *.h)

lreco_dict.cc: $(Sources)
	rootcint -f $@ -c -p $^
