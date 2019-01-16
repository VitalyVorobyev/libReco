// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME lreco_dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "DalitzVars.h"
#include "DeltaEMbc.h"
#include "EvtInfo.h"
#include "EvtShape.h"
#include "GammaInfo.h"
#include "GenHepEvt.h"
#include "GenParticleInfo.h"
#include "HamletTag.h"
#include "IPBoost.h"
#include "Ks0Info.h"
#include "Pi0Info.h"
#include "SFWMInfo.h"
#include "TrackInfo.h"
#include "VertexInfo.h"
#include "TrackInfo2.h"
#include "H0Info.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_DalitzVars(void *p = 0);
   static void *newArray_DalitzVars(Long_t size, void *p);
   static void delete_DalitzVars(void *p);
   static void deleteArray_DalitzVars(void *p);
   static void destruct_DalitzVars(void *p);
   static void streamer_DalitzVars(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DalitzVars*)
   {
      ::DalitzVars *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DalitzVars >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DalitzVars", ::DalitzVars::Class_Version(), "DalitzVars.h", 6,
                  typeid(::DalitzVars), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DalitzVars::Dictionary, isa_proxy, 16,
                  sizeof(::DalitzVars) );
      instance.SetNew(&new_DalitzVars);
      instance.SetNewArray(&newArray_DalitzVars);
      instance.SetDelete(&delete_DalitzVars);
      instance.SetDeleteArray(&deleteArray_DalitzVars);
      instance.SetDestructor(&destruct_DalitzVars);
      instance.SetStreamerFunc(&streamer_DalitzVars);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DalitzVars*)
   {
      return GenerateInitInstanceLocal((::DalitzVars*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DalitzVars*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DeltaEMbc(void *p = 0);
   static void *newArray_DeltaEMbc(Long_t size, void *p);
   static void delete_DeltaEMbc(void *p);
   static void deleteArray_DeltaEMbc(void *p);
   static void destruct_DeltaEMbc(void *p);
   static void streamer_DeltaEMbc(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DeltaEMbc*)
   {
      ::DeltaEMbc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DeltaEMbc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DeltaEMbc", ::DeltaEMbc::Class_Version(), "DeltaEMbc.h", 6,
                  typeid(::DeltaEMbc), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DeltaEMbc::Dictionary, isa_proxy, 16,
                  sizeof(::DeltaEMbc) );
      instance.SetNew(&new_DeltaEMbc);
      instance.SetNewArray(&newArray_DeltaEMbc);
      instance.SetDelete(&delete_DeltaEMbc);
      instance.SetDeleteArray(&deleteArray_DeltaEMbc);
      instance.SetDestructor(&destruct_DeltaEMbc);
      instance.SetStreamerFunc(&streamer_DeltaEMbc);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DeltaEMbc*)
   {
      return GenerateInitInstanceLocal((::DeltaEMbc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_EvtInfo(void *p = 0);
   static void *newArray_EvtInfo(Long_t size, void *p);
   static void delete_EvtInfo(void *p);
   static void deleteArray_EvtInfo(void *p);
   static void destruct_EvtInfo(void *p);
   static void streamer_EvtInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EvtInfo*)
   {
      ::EvtInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EvtInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EvtInfo", ::EvtInfo::Class_Version(), "EvtInfo.h", 6,
                  typeid(::EvtInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::EvtInfo::Dictionary, isa_proxy, 16,
                  sizeof(::EvtInfo) );
      instance.SetNew(&new_EvtInfo);
      instance.SetNewArray(&newArray_EvtInfo);
      instance.SetDelete(&delete_EvtInfo);
      instance.SetDeleteArray(&deleteArray_EvtInfo);
      instance.SetDestructor(&destruct_EvtInfo);
      instance.SetStreamerFunc(&streamer_EvtInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EvtInfo*)
   {
      return GenerateInitInstanceLocal((::EvtInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EvtInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_SFWMInfo(void *p = 0);
   static void *newArray_SFWMInfo(Long_t size, void *p);
   static void delete_SFWMInfo(void *p);
   static void deleteArray_SFWMInfo(void *p);
   static void destruct_SFWMInfo(void *p);
   static void streamer_SFWMInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SFWMInfo*)
   {
      ::SFWMInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SFWMInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SFWMInfo", ::SFWMInfo::Class_Version(), "SFWMInfo.h", 6,
                  typeid(::SFWMInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SFWMInfo::Dictionary, isa_proxy, 16,
                  sizeof(::SFWMInfo) );
      instance.SetNew(&new_SFWMInfo);
      instance.SetNewArray(&newArray_SFWMInfo);
      instance.SetDelete(&delete_SFWMInfo);
      instance.SetDeleteArray(&deleteArray_SFWMInfo);
      instance.SetDestructor(&destruct_SFWMInfo);
      instance.SetStreamerFunc(&streamer_SFWMInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SFWMInfo*)
   {
      return GenerateInitInstanceLocal((::SFWMInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SFWMInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_EvtShape(void *p = 0);
   static void *newArray_EvtShape(Long_t size, void *p);
   static void delete_EvtShape(void *p);
   static void deleteArray_EvtShape(void *p);
   static void destruct_EvtShape(void *p);
   static void streamer_EvtShape(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EvtShape*)
   {
      ::EvtShape *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EvtShape >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EvtShape", ::EvtShape::Class_Version(), "EvtShape.h", 7,
                  typeid(::EvtShape), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::EvtShape::Dictionary, isa_proxy, 16,
                  sizeof(::EvtShape) );
      instance.SetNew(&new_EvtShape);
      instance.SetNewArray(&newArray_EvtShape);
      instance.SetDelete(&delete_EvtShape);
      instance.SetDeleteArray(&deleteArray_EvtShape);
      instance.SetDestructor(&destruct_EvtShape);
      instance.SetStreamerFunc(&streamer_EvtShape);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EvtShape*)
   {
      return GenerateInitInstanceLocal((::EvtShape*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::EvtShape*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GammaInfo(void *p = 0);
   static void *newArray_GammaInfo(Long_t size, void *p);
   static void delete_GammaInfo(void *p);
   static void deleteArray_GammaInfo(void *p);
   static void destruct_GammaInfo(void *p);
   static void streamer_GammaInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GammaInfo*)
   {
      ::GammaInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GammaInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GammaInfo", ::GammaInfo::Class_Version(), "GammaInfo.h", 6,
                  typeid(::GammaInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::GammaInfo::Dictionary, isa_proxy, 16,
                  sizeof(::GammaInfo) );
      instance.SetNew(&new_GammaInfo);
      instance.SetNewArray(&newArray_GammaInfo);
      instance.SetDelete(&delete_GammaInfo);
      instance.SetDeleteArray(&deleteArray_GammaInfo);
      instance.SetDestructor(&destruct_GammaInfo);
      instance.SetStreamerFunc(&streamer_GammaInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GammaInfo*)
   {
      return GenerateInitInstanceLocal((::GammaInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::GammaInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GenHepEvt(void *p = 0);
   static void *newArray_GenHepEvt(Long_t size, void *p);
   static void delete_GenHepEvt(void *p);
   static void deleteArray_GenHepEvt(void *p);
   static void destruct_GenHepEvt(void *p);
   static void streamer_GenHepEvt(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenHepEvt*)
   {
      ::GenHepEvt *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GenHepEvt >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GenHepEvt", ::GenHepEvt::Class_Version(), "GenHepEvt.h", 7,
                  typeid(::GenHepEvt), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::GenHepEvt::Dictionary, isa_proxy, 16,
                  sizeof(::GenHepEvt) );
      instance.SetNew(&new_GenHepEvt);
      instance.SetNewArray(&newArray_GenHepEvt);
      instance.SetDelete(&delete_GenHepEvt);
      instance.SetDeleteArray(&deleteArray_GenHepEvt);
      instance.SetDestructor(&destruct_GenHepEvt);
      instance.SetStreamerFunc(&streamer_GenHepEvt);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenHepEvt*)
   {
      return GenerateInitInstanceLocal((::GenHepEvt*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::GenHepEvt*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GenParticleInfo(void *p = 0);
   static void *newArray_GenParticleInfo(Long_t size, void *p);
   static void delete_GenParticleInfo(void *p);
   static void deleteArray_GenParticleInfo(void *p);
   static void destruct_GenParticleInfo(void *p);
   static void streamer_GenParticleInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenParticleInfo*)
   {
      ::GenParticleInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GenParticleInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GenParticleInfo", ::GenParticleInfo::Class_Version(), "GenParticleInfo.h", 6,
                  typeid(::GenParticleInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::GenParticleInfo::Dictionary, isa_proxy, 16,
                  sizeof(::GenParticleInfo) );
      instance.SetNew(&new_GenParticleInfo);
      instance.SetNewArray(&newArray_GenParticleInfo);
      instance.SetDelete(&delete_GenParticleInfo);
      instance.SetDeleteArray(&deleteArray_GenParticleInfo);
      instance.SetDestructor(&destruct_GenParticleInfo);
      instance.SetStreamerFunc(&streamer_GenParticleInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenParticleInfo*)
   {
      return GenerateInitInstanceLocal((::GenParticleInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_HamletTag(void *p = 0);
   static void *newArray_HamletTag(Long_t size, void *p);
   static void delete_HamletTag(void *p);
   static void deleteArray_HamletTag(void *p);
   static void destruct_HamletTag(void *p);
   static void streamer_HamletTag(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::HamletTag*)
   {
      ::HamletTag *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HamletTag >(0);
      static ::ROOT::TGenericClassInfo 
         instance("HamletTag", ::HamletTag::Class_Version(), "HamletTag.h", 6,
                  typeid(::HamletTag), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::HamletTag::Dictionary, isa_proxy, 16,
                  sizeof(::HamletTag) );
      instance.SetNew(&new_HamletTag);
      instance.SetNewArray(&newArray_HamletTag);
      instance.SetDelete(&delete_HamletTag);
      instance.SetDeleteArray(&deleteArray_HamletTag);
      instance.SetDestructor(&destruct_HamletTag);
      instance.SetStreamerFunc(&streamer_HamletTag);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::HamletTag*)
   {
      return GenerateInitInstanceLocal((::HamletTag*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::HamletTag*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_IPBoost(void *p = 0);
   static void *newArray_IPBoost(Long_t size, void *p);
   static void delete_IPBoost(void *p);
   static void deleteArray_IPBoost(void *p);
   static void destruct_IPBoost(void *p);
   static void streamer_IPBoost(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::IPBoost*)
   {
      ::IPBoost *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::IPBoost >(0);
      static ::ROOT::TGenericClassInfo 
         instance("IPBoost", ::IPBoost::Class_Version(), "IPBoost.h", 6,
                  typeid(::IPBoost), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::IPBoost::Dictionary, isa_proxy, 16,
                  sizeof(::IPBoost) );
      instance.SetNew(&new_IPBoost);
      instance.SetNewArray(&newArray_IPBoost);
      instance.SetDelete(&delete_IPBoost);
      instance.SetDeleteArray(&deleteArray_IPBoost);
      instance.SetDestructor(&destruct_IPBoost);
      instance.SetStreamerFunc(&streamer_IPBoost);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::IPBoost*)
   {
      return GenerateInitInstanceLocal((::IPBoost*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::IPBoost*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Ks0Info(void *p = 0);
   static void *newArray_Ks0Info(Long_t size, void *p);
   static void delete_Ks0Info(void *p);
   static void deleteArray_Ks0Info(void *p);
   static void destruct_Ks0Info(void *p);
   static void streamer_Ks0Info(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Ks0Info*)
   {
      ::Ks0Info *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Ks0Info >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Ks0Info", ::Ks0Info::Class_Version(), "Ks0Info.h", 6,
                  typeid(::Ks0Info), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Ks0Info::Dictionary, isa_proxy, 16,
                  sizeof(::Ks0Info) );
      instance.SetNew(&new_Ks0Info);
      instance.SetNewArray(&newArray_Ks0Info);
      instance.SetDelete(&delete_Ks0Info);
      instance.SetDeleteArray(&deleteArray_Ks0Info);
      instance.SetDestructor(&destruct_Ks0Info);
      instance.SetStreamerFunc(&streamer_Ks0Info);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Ks0Info*)
   {
      return GenerateInitInstanceLocal((::Ks0Info*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Ks0Info*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_Pi0Info(void *p = 0);
   static void *newArray_Pi0Info(Long_t size, void *p);
   static void delete_Pi0Info(void *p);
   static void deleteArray_Pi0Info(void *p);
   static void destruct_Pi0Info(void *p);
   static void streamer_Pi0Info(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Pi0Info*)
   {
      ::Pi0Info *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Pi0Info >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Pi0Info", ::Pi0Info::Class_Version(), "Pi0Info.h", 6,
                  typeid(::Pi0Info), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Pi0Info::Dictionary, isa_proxy, 16,
                  sizeof(::Pi0Info) );
      instance.SetNew(&new_Pi0Info);
      instance.SetNewArray(&newArray_Pi0Info);
      instance.SetDelete(&delete_Pi0Info);
      instance.SetDeleteArray(&deleteArray_Pi0Info);
      instance.SetDestructor(&destruct_Pi0Info);
      instance.SetStreamerFunc(&streamer_Pi0Info);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Pi0Info*)
   {
      return GenerateInitInstanceLocal((::Pi0Info*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Pi0Info*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TrackInfo(void *p = 0);
   static void *newArray_TrackInfo(Long_t size, void *p);
   static void delete_TrackInfo(void *p);
   static void deleteArray_TrackInfo(void *p);
   static void destruct_TrackInfo(void *p);
   static void streamer_TrackInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TrackInfo*)
   {
      ::TrackInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TrackInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TrackInfo", ::TrackInfo::Class_Version(), "TrackInfo.h", 6,
                  typeid(::TrackInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TrackInfo::Dictionary, isa_proxy, 16,
                  sizeof(::TrackInfo) );
      instance.SetNew(&new_TrackInfo);
      instance.SetNewArray(&newArray_TrackInfo);
      instance.SetDelete(&delete_TrackInfo);
      instance.SetDeleteArray(&deleteArray_TrackInfo);
      instance.SetDestructor(&destruct_TrackInfo);
      instance.SetStreamerFunc(&streamer_TrackInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TrackInfo*)
   {
      return GenerateInitInstanceLocal((::TrackInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TrackInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_VertexInfo(void *p = 0);
   static void *newArray_VertexInfo(Long_t size, void *p);
   static void delete_VertexInfo(void *p);
   static void deleteArray_VertexInfo(void *p);
   static void destruct_VertexInfo(void *p);
   static void streamer_VertexInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VertexInfo*)
   {
      ::VertexInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::VertexInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("VertexInfo", ::VertexInfo::Class_Version(), "VertexInfo.h", 6,
                  typeid(::VertexInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::VertexInfo::Dictionary, isa_proxy, 16,
                  sizeof(::VertexInfo) );
      instance.SetNew(&new_VertexInfo);
      instance.SetNewArray(&newArray_VertexInfo);
      instance.SetDelete(&delete_VertexInfo);
      instance.SetDeleteArray(&deleteArray_VertexInfo);
      instance.SetDestructor(&destruct_VertexInfo);
      instance.SetStreamerFunc(&streamer_VertexInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VertexInfo*)
   {
      return GenerateInitInstanceLocal((::VertexInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::VertexInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_TrackInfo2(void *p = 0);
   static void *newArray_TrackInfo2(Long_t size, void *p);
   static void delete_TrackInfo2(void *p);
   static void deleteArray_TrackInfo2(void *p);
   static void destruct_TrackInfo2(void *p);
   static void streamer_TrackInfo2(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TrackInfo2*)
   {
      ::TrackInfo2 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TrackInfo2 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TrackInfo2", ::TrackInfo2::Class_Version(), "TrackInfo2.h", 9,
                  typeid(::TrackInfo2), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::TrackInfo2::Dictionary, isa_proxy, 16,
                  sizeof(::TrackInfo2) );
      instance.SetNew(&new_TrackInfo2);
      instance.SetNewArray(&newArray_TrackInfo2);
      instance.SetDelete(&delete_TrackInfo2);
      instance.SetDeleteArray(&deleteArray_TrackInfo2);
      instance.SetDestructor(&destruct_TrackInfo2);
      instance.SetStreamerFunc(&streamer_TrackInfo2);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TrackInfo2*)
   {
      return GenerateInitInstanceLocal((::TrackInfo2*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TrackInfo2*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_H0Info(void *p = 0);
   static void *newArray_H0Info(Long_t size, void *p);
   static void delete_H0Info(void *p);
   static void deleteArray_H0Info(void *p);
   static void destruct_H0Info(void *p);
   static void streamer_H0Info(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::H0Info*)
   {
      ::H0Info *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::H0Info >(0);
      static ::ROOT::TGenericClassInfo 
         instance("H0Info", ::H0Info::Class_Version(), "H0Info.h", 9,
                  typeid(::H0Info), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::H0Info::Dictionary, isa_proxy, 16,
                  sizeof(::H0Info) );
      instance.SetNew(&new_H0Info);
      instance.SetNewArray(&newArray_H0Info);
      instance.SetDelete(&delete_H0Info);
      instance.SetDeleteArray(&deleteArray_H0Info);
      instance.SetDestructor(&destruct_H0Info);
      instance.SetStreamerFunc(&streamer_H0Info);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::H0Info*)
   {
      return GenerateInitInstanceLocal((::H0Info*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::H0Info*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DalitzVars::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DalitzVars::Class_Name()
{
   return "DalitzVars";
}

//______________________________________________________________________________
const char *DalitzVars::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DalitzVars*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DalitzVars::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DalitzVars*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DalitzVars::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DalitzVars*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DalitzVars::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DalitzVars*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DeltaEMbc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DeltaEMbc::Class_Name()
{
   return "DeltaEMbc";
}

//______________________________________________________________________________
const char *DeltaEMbc::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DeltaEMbc::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DeltaEMbc::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DeltaEMbc::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EvtInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *EvtInfo::Class_Name()
{
   return "EvtInfo";
}

//______________________________________________________________________________
const char *EvtInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EvtInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EvtInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EvtInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EvtInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvtInfo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EvtInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvtInfo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SFWMInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SFWMInfo::Class_Name()
{
   return "SFWMInfo";
}

//______________________________________________________________________________
const char *SFWMInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SFWMInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SFWMInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SFWMInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SFWMInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SFWMInfo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SFWMInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SFWMInfo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EvtShape::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *EvtShape::Class_Name()
{
   return "EvtShape";
}

//______________________________________________________________________________
const char *EvtShape::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EvtShape*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EvtShape::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::EvtShape*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *EvtShape::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvtShape*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *EvtShape::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::EvtShape*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GammaInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GammaInfo::Class_Name()
{
   return "GammaInfo";
}

//______________________________________________________________________________
const char *GammaInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GammaInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GammaInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GammaInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GammaInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GammaInfo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GammaInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GammaInfo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GenHepEvt::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GenHepEvt::Class_Name()
{
   return "GenHepEvt";
}

//______________________________________________________________________________
const char *GenHepEvt::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenHepEvt*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GenHepEvt::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenHepEvt*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GenHepEvt::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenHepEvt*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GenHepEvt::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenHepEvt*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GenParticleInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GenParticleInfo::Class_Name()
{
   return "GenParticleInfo";
}

//______________________________________________________________________________
const char *GenParticleInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GenParticleInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GenParticleInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GenParticleInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr HamletTag::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *HamletTag::Class_Name()
{
   return "HamletTag";
}

//______________________________________________________________________________
const char *HamletTag::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HamletTag*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int HamletTag::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::HamletTag*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HamletTag::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HamletTag*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HamletTag::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::HamletTag*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr IPBoost::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *IPBoost::Class_Name()
{
   return "IPBoost";
}

//______________________________________________________________________________
const char *IPBoost::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::IPBoost*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int IPBoost::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::IPBoost*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *IPBoost::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::IPBoost*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *IPBoost::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::IPBoost*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Ks0Info::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Ks0Info::Class_Name()
{
   return "Ks0Info";
}

//______________________________________________________________________________
const char *Ks0Info::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ks0Info*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Ks0Info::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Ks0Info*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Ks0Info::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ks0Info*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Ks0Info::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Ks0Info*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Pi0Info::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Pi0Info::Class_Name()
{
   return "Pi0Info";
}

//______________________________________________________________________________
const char *Pi0Info::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Pi0Info*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Pi0Info::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Pi0Info*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Pi0Info::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Pi0Info*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Pi0Info::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Pi0Info*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TrackInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackInfo::Class_Name()
{
   return "TrackInfo";
}

//______________________________________________________________________________
const char *TrackInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrackInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TrackInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrackInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TrackInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrackInfo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TrackInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrackInfo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr VertexInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *VertexInfo::Class_Name()
{
   return "VertexInfo";
}

//______________________________________________________________________________
const char *VertexInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::VertexInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int VertexInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::VertexInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *VertexInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::VertexInfo*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *VertexInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::VertexInfo*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TrackInfo2::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackInfo2::Class_Name()
{
   return "TrackInfo2";
}

//______________________________________________________________________________
const char *TrackInfo2::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrackInfo2*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TrackInfo2::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TrackInfo2*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TrackInfo2::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrackInfo2*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TrackInfo2::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TrackInfo2*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr H0Info::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *H0Info::Class_Name()
{
   return "H0Info";
}

//______________________________________________________________________________
const char *H0Info::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::H0Info*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int H0Info::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::H0Info*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *H0Info::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::H0Info*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *H0Info::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::H0Info*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DalitzVars::Streamer(TBuffer &R__b)
{
   // Stream an object of class DalitzVars.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((double*)msq);
      R__b.ReadStaticArray((double*)cosh);
      R__b.CheckByteCount(R__s, R__c, DalitzVars::IsA());
   } else {
      R__c = R__b.WriteVersion(DalitzVars::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(msq, 3);
      R__b.WriteArray(cosh, 3);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DalitzVars(void *p) {
      return  p ? new(p) ::DalitzVars : new ::DalitzVars;
   }
   static void *newArray_DalitzVars(Long_t nElements, void *p) {
      return p ? new(p) ::DalitzVars[nElements] : new ::DalitzVars[nElements];
   }
   // Wrapper around operator delete
   static void delete_DalitzVars(void *p) {
      delete ((::DalitzVars*)p);
   }
   static void deleteArray_DalitzVars(void *p) {
      delete [] ((::DalitzVars*)p);
   }
   static void destruct_DalitzVars(void *p) {
      typedef ::DalitzVars current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DalitzVars(TBuffer &buf, void *obj) {
      ((::DalitzVars*)obj)->::DalitzVars::Streamer(buf);
   }
} // end of namespace ROOT for class ::DalitzVars

//______________________________________________________________________________
void DeltaEMbc::Streamer(TBuffer &R__b)
{
   // Stream an object of class DeltaEMbc.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> de;
      R__b >> mbc;
      R__b.CheckByteCount(R__s, R__c, DeltaEMbc::IsA());
   } else {
      R__c = R__b.WriteVersion(DeltaEMbc::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << de;
      R__b << mbc;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DeltaEMbc(void *p) {
      return  p ? new(p) ::DeltaEMbc : new ::DeltaEMbc;
   }
   static void *newArray_DeltaEMbc(Long_t nElements, void *p) {
      return p ? new(p) ::DeltaEMbc[nElements] : new ::DeltaEMbc[nElements];
   }
   // Wrapper around operator delete
   static void delete_DeltaEMbc(void *p) {
      delete ((::DeltaEMbc*)p);
   }
   static void deleteArray_DeltaEMbc(void *p) {
      delete [] ((::DeltaEMbc*)p);
   }
   static void destruct_DeltaEMbc(void *p) {
      typedef ::DeltaEMbc current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DeltaEMbc(TBuffer &buf, void *obj) {
      ((::DeltaEMbc*)obj)->::DeltaEMbc::Streamer(buf);
   }
} // end of namespace ROOT for class ::DeltaEMbc

//______________________________________________________________________________
void EvtInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class EvtInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> exp;
      R__b >> run;
      R__b >> evtn;
      R__b.CheckByteCount(R__s, R__c, EvtInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(EvtInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << exp;
      R__b << run;
      R__b << evtn;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EvtInfo(void *p) {
      return  p ? new(p) ::EvtInfo : new ::EvtInfo;
   }
   static void *newArray_EvtInfo(Long_t nElements, void *p) {
      return p ? new(p) ::EvtInfo[nElements] : new ::EvtInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_EvtInfo(void *p) {
      delete ((::EvtInfo*)p);
   }
   static void deleteArray_EvtInfo(void *p) {
      delete [] ((::EvtInfo*)p);
   }
   static void destruct_EvtInfo(void *p) {
      typedef ::EvtInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_EvtInfo(TBuffer &buf, void *obj) {
      ((::EvtInfo*)obj)->::EvtInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::EvtInfo

//______________________________________________________________________________
void SFWMInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class SFWMInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> kmm2;
      R__b >> ket;
      R__b >> khso00;
      R__b >> khso01;
      R__b >> khso02;
      R__b >> khso03;
      R__b >> khso04;
      R__b >> khso10;
      R__b >> khso12;
      R__b >> khso14;
      R__b >> khso20;
      R__b >> khso22;
      R__b >> khso24;
      R__b >> khoo0;
      R__b >> khoo1;
      R__b >> khoo2;
      R__b >> khoo3;
      R__b >> khoo4;
      R__b.CheckByteCount(R__s, R__c, SFWMInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(SFWMInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << kmm2;
      R__b << ket;
      R__b << khso00;
      R__b << khso01;
      R__b << khso02;
      R__b << khso03;
      R__b << khso04;
      R__b << khso10;
      R__b << khso12;
      R__b << khso14;
      R__b << khso20;
      R__b << khso22;
      R__b << khso24;
      R__b << khoo0;
      R__b << khoo1;
      R__b << khoo2;
      R__b << khoo3;
      R__b << khoo4;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SFWMInfo(void *p) {
      return  p ? new(p) ::SFWMInfo : new ::SFWMInfo;
   }
   static void *newArray_SFWMInfo(Long_t nElements, void *p) {
      return p ? new(p) ::SFWMInfo[nElements] : new ::SFWMInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_SFWMInfo(void *p) {
      delete ((::SFWMInfo*)p);
   }
   static void deleteArray_SFWMInfo(void *p) {
      delete [] ((::SFWMInfo*)p);
   }
   static void destruct_SFWMInfo(void *p) {
      typedef ::SFWMInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SFWMInfo(TBuffer &buf, void *obj) {
      ((::SFWMInfo*)obj)->::SFWMInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::SFWMInfo

//______________________________________________________________________________
void EvtShape::Streamer(TBuffer &R__b)
{
   // Stream an object of class EvtShape.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> cos_thr;
      R__b >> thr_sig;
      R__b >> thr_oth;
      k0.Streamer(R__b);
      k1.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, EvtShape::IsA());
   } else {
      R__c = R__b.WriteVersion(EvtShape::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << cos_thr;
      R__b << thr_sig;
      R__b << thr_oth;
      k0.Streamer(R__b);
      k1.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_EvtShape(void *p) {
      return  p ? new(p) ::EvtShape : new ::EvtShape;
   }
   static void *newArray_EvtShape(Long_t nElements, void *p) {
      return p ? new(p) ::EvtShape[nElements] : new ::EvtShape[nElements];
   }
   // Wrapper around operator delete
   static void delete_EvtShape(void *p) {
      delete ((::EvtShape*)p);
   }
   static void deleteArray_EvtShape(void *p) {
      delete [] ((::EvtShape*)p);
   }
   static void destruct_EvtShape(void *p) {
      typedef ::EvtShape current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_EvtShape(TBuffer &buf, void *obj) {
      ((::EvtShape*)obj)->::EvtShape::Streamer(buf);
   }
} // end of namespace ROOT for class ::EvtShape

//______________________________________________________________________________
void GammaInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class GammaInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> e;
      R__b >> costh;
      R__b.CheckByteCount(R__s, R__c, GammaInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(GammaInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << e;
      R__b << costh;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GammaInfo(void *p) {
      return  p ? new(p) ::GammaInfo : new ::GammaInfo;
   }
   static void *newArray_GammaInfo(Long_t nElements, void *p) {
      return p ? new(p) ::GammaInfo[nElements] : new ::GammaInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_GammaInfo(void *p) {
      delete ((::GammaInfo*)p);
   }
   static void deleteArray_GammaInfo(void *p) {
      delete [] ((::GammaInfo*)p);
   }
   static void destruct_GammaInfo(void *p) {
      typedef ::GammaInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GammaInfo(TBuffer &buf, void *obj) {
      ((::GammaInfo*)obj)->::GammaInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::GammaInfo

//______________________________________________________________________________
void GenHepEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class GenHepEvt.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         vector<int> &R__stl =  idhep;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<int> &R__stl =  daF;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<int> &R__stl =  daL;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, GenHepEvt::IsA());
   } else {
      R__c = R__b.WriteVersion(GenHepEvt::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         vector<int> &R__stl =  idhep;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<int> &R__stl =  daF;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<int> &R__stl =  daL;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GenHepEvt(void *p) {
      return  p ? new(p) ::GenHepEvt : new ::GenHepEvt;
   }
   static void *newArray_GenHepEvt(Long_t nElements, void *p) {
      return p ? new(p) ::GenHepEvt[nElements] : new ::GenHepEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenHepEvt(void *p) {
      delete ((::GenHepEvt*)p);
   }
   static void deleteArray_GenHepEvt(void *p) {
      delete [] ((::GenHepEvt*)p);
   }
   static void destruct_GenHepEvt(void *p) {
      typedef ::GenHepEvt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GenHepEvt(TBuffer &buf, void *obj) {
      ((::GenHepEvt*)obj)->::GenHepEvt::Streamer(buf);
   }
} // end of namespace ROOT for class ::GenHepEvt

//______________________________________________________________________________
void GenParticleInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class GenParticleInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> id;
      R__b >> flag;
      R__b.ReadStaticArray((int*)chain);
      R__b.CheckByteCount(R__s, R__c, GenParticleInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(GenParticleInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << id;
      R__b << flag;
      R__b.WriteArray(chain, 9);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GenParticleInfo(void *p) {
      return  p ? new(p) ::GenParticleInfo : new ::GenParticleInfo;
   }
   static void *newArray_GenParticleInfo(Long_t nElements, void *p) {
      return p ? new(p) ::GenParticleInfo[nElements] : new ::GenParticleInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenParticleInfo(void *p) {
      delete ((::GenParticleInfo*)p);
   }
   static void deleteArray_GenParticleInfo(void *p) {
      delete [] ((::GenParticleInfo*)p);
   }
   static void destruct_GenParticleInfo(void *p) {
      typedef ::GenParticleInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GenParticleInfo(TBuffer &buf, void *obj) {
      ((::GenParticleInfo*)obj)->::GenParticleInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::GenParticleInfo

//______________________________________________________________________________
void HamletTag::Streamer(TBuffer &R__b)
{
   // Stream an object of class HamletTag.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> tag_LH;
      R__b >> tag_LH_err;
      R__b >> tagl;
      R__b >> taglid;
      R__b >> flv;
      R__b.CheckByteCount(R__s, R__c, HamletTag::IsA());
   } else {
      R__c = R__b.WriteVersion(HamletTag::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << tag_LH;
      R__b << tag_LH_err;
      R__b << tagl;
      R__b << taglid;
      R__b << flv;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_HamletTag(void *p) {
      return  p ? new(p) ::HamletTag : new ::HamletTag;
   }
   static void *newArray_HamletTag(Long_t nElements, void *p) {
      return p ? new(p) ::HamletTag[nElements] : new ::HamletTag[nElements];
   }
   // Wrapper around operator delete
   static void delete_HamletTag(void *p) {
      delete ((::HamletTag*)p);
   }
   static void deleteArray_HamletTag(void *p) {
      delete [] ((::HamletTag*)p);
   }
   static void destruct_HamletTag(void *p) {
      typedef ::HamletTag current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_HamletTag(TBuffer &buf, void *obj) {
      ((::HamletTag*)obj)->::HamletTag::Streamer(buf);
   }
} // end of namespace ROOT for class ::HamletTag

//______________________________________________________________________________
void IPBoost::Streamer(TBuffer &R__b)
{
   // Stream an object of class IPBoost.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((double*)ip);
      R__b.ReadStaticArray((double*)boost);
      R__b.CheckByteCount(R__s, R__c, IPBoost::IsA());
   } else {
      R__c = R__b.WriteVersion(IPBoost::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(ip, 3);
      R__b.WriteArray(boost, 3);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_IPBoost(void *p) {
      return  p ? new(p) ::IPBoost : new ::IPBoost;
   }
   static void *newArray_IPBoost(Long_t nElements, void *p) {
      return p ? new(p) ::IPBoost[nElements] : new ::IPBoost[nElements];
   }
   // Wrapper around operator delete
   static void delete_IPBoost(void *p) {
      delete ((::IPBoost*)p);
   }
   static void deleteArray_IPBoost(void *p) {
      delete [] ((::IPBoost*)p);
   }
   static void destruct_IPBoost(void *p) {
      typedef ::IPBoost current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_IPBoost(TBuffer &buf, void *obj) {
      ((::IPBoost*)obj)->::IPBoost::Streamer(buf);
   }
} // end of namespace ROOT for class ::IPBoost

//______________________________________________________________________________
void Ks0Info::Streamer(TBuffer &R__b)
{
   // Stream an object of class Ks0Info.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m;
      R__b >> m_raw;
      R__b.ReadStaticArray((double*)p);
      R__b >> dr;
      R__b >> dz;
      R__b >> dphi;
      R__b >> fl;
      R__b.CheckByteCount(R__s, R__c, Ks0Info::IsA());
   } else {
      R__c = R__b.WriteVersion(Ks0Info::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m;
      R__b << m_raw;
      R__b.WriteArray(p, 3);
      R__b << dr;
      R__b << dz;
      R__b << dphi;
      R__b << fl;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Ks0Info(void *p) {
      return  p ? new(p) ::Ks0Info : new ::Ks0Info;
   }
   static void *newArray_Ks0Info(Long_t nElements, void *p) {
      return p ? new(p) ::Ks0Info[nElements] : new ::Ks0Info[nElements];
   }
   // Wrapper around operator delete
   static void delete_Ks0Info(void *p) {
      delete ((::Ks0Info*)p);
   }
   static void deleteArray_Ks0Info(void *p) {
      delete [] ((::Ks0Info*)p);
   }
   static void destruct_Ks0Info(void *p) {
      typedef ::Ks0Info current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Ks0Info(TBuffer &buf, void *obj) {
      ((::Ks0Info*)obj)->::Ks0Info::Streamer(buf);
   }
} // end of namespace ROOT for class ::Ks0Info

//______________________________________________________________________________
void Pi0Info::Streamer(TBuffer &R__b)
{
   // Stream an object of class Pi0Info.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m;
      R__b >> m_raw;
      R__b.ReadStaticArray((double*)p);
      R__b >> eg1;
      R__b >> eg2;
      R__b >> chisq;
      R__b.CheckByteCount(R__s, R__c, Pi0Info::IsA());
   } else {
      R__c = R__b.WriteVersion(Pi0Info::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m;
      R__b << m_raw;
      R__b.WriteArray(p, 3);
      R__b << eg1;
      R__b << eg2;
      R__b << chisq;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_Pi0Info(void *p) {
      return  p ? new(p) ::Pi0Info : new ::Pi0Info;
   }
   static void *newArray_Pi0Info(Long_t nElements, void *p) {
      return p ? new(p) ::Pi0Info[nElements] : new ::Pi0Info[nElements];
   }
   // Wrapper around operator delete
   static void delete_Pi0Info(void *p) {
      delete ((::Pi0Info*)p);
   }
   static void deleteArray_Pi0Info(void *p) {
      delete [] ((::Pi0Info*)p);
   }
   static void destruct_Pi0Info(void *p) {
      typedef ::Pi0Info current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Pi0Info(TBuffer &buf, void *obj) {
      ((::Pi0Info*)obj)->::Pi0Info::Streamer(buf);
   }
} // end of namespace ROOT for class ::Pi0Info

//______________________________________________________________________________
void TrackInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class TrackInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((double*)p);
      R__b >> r;
      R__b >> z;
      R__b >> rz_svd;
      R__b >> rphi_svd;
      R__b >> atckpi;
      R__b >> atckp;
      R__b >> eid;
      R__b >> muid;
      R__b.CheckByteCount(R__s, R__c, TrackInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(TrackInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(p, 3);
      R__b << r;
      R__b << z;
      R__b << rz_svd;
      R__b << rphi_svd;
      R__b << atckpi;
      R__b << atckp;
      R__b << eid;
      R__b << muid;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TrackInfo(void *p) {
      return  p ? new(p) ::TrackInfo : new ::TrackInfo;
   }
   static void *newArray_TrackInfo(Long_t nElements, void *p) {
      return p ? new(p) ::TrackInfo[nElements] : new ::TrackInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_TrackInfo(void *p) {
      delete ((::TrackInfo*)p);
   }
   static void deleteArray_TrackInfo(void *p) {
      delete [] ((::TrackInfo*)p);
   }
   static void destruct_TrackInfo(void *p) {
      typedef ::TrackInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TrackInfo(TBuffer &buf, void *obj) {
      ((::TrackInfo*)obj)->::TrackInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::TrackInfo

//______________________________________________________________________________
void VertexInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class VertexInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> z;
      R__b >> sz;
      R__b >> ntrk;
      R__b >> ndf;
      R__b >> chisq;
      R__b >> cl;
      R__b.CheckByteCount(R__s, R__c, VertexInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(VertexInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << z;
      R__b << sz;
      R__b << ntrk;
      R__b << ndf;
      R__b << chisq;
      R__b << cl;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_VertexInfo(void *p) {
      return  p ? new(p) ::VertexInfo : new ::VertexInfo;
   }
   static void *newArray_VertexInfo(Long_t nElements, void *p) {
      return p ? new(p) ::VertexInfo[nElements] : new ::VertexInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_VertexInfo(void *p) {
      delete ((::VertexInfo*)p);
   }
   static void deleteArray_VertexInfo(void *p) {
      delete [] ((::VertexInfo*)p);
   }
   static void destruct_VertexInfo(void *p) {
      typedef ::VertexInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_VertexInfo(TBuffer &buf, void *obj) {
      ((::VertexInfo*)obj)->::VertexInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::VertexInfo

//______________________________________________________________________________
void TrackInfo2::Streamer(TBuffer &R__b)
{
   // Stream an object of class TrackInfo2.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> p;
      R__b >> costh;
      R__b >> phi;
      R__b >> r;
      R__b >> z;
      R__b >> rz_svd;
      R__b >> rphi_svd;
      R__b >> atckpi;
      R__b >> atckp;
      R__b >> eid;
      R__b >> muid;
      R__b.CheckByteCount(R__s, R__c, TrackInfo2::IsA());
   } else {
      R__c = R__b.WriteVersion(TrackInfo2::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << p;
      R__b << costh;
      R__b << phi;
      R__b << r;
      R__b << z;
      R__b << rz_svd;
      R__b << rphi_svd;
      R__b << atckpi;
      R__b << atckp;
      R__b << eid;
      R__b << muid;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TrackInfo2(void *p) {
      return  p ? new(p) ::TrackInfo2 : new ::TrackInfo2;
   }
   static void *newArray_TrackInfo2(Long_t nElements, void *p) {
      return p ? new(p) ::TrackInfo2[nElements] : new ::TrackInfo2[nElements];
   }
   // Wrapper around operator delete
   static void delete_TrackInfo2(void *p) {
      delete ((::TrackInfo2*)p);
   }
   static void deleteArray_TrackInfo2(void *p) {
      delete [] ((::TrackInfo2*)p);
   }
   static void destruct_TrackInfo2(void *p) {
      typedef ::TrackInfo2 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TrackInfo2(TBuffer &buf, void *obj) {
      ((::TrackInfo2*)obj)->::TrackInfo2::Streamer(buf);
   }
} // end of namespace ROOT for class ::TrackInfo2

//______________________________________________________________________________
void H0Info::Streamer(TBuffer &R__b)
{
   // Stream an object of class H0Info.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m;
      R__b >> p;
      R__b >> costh;
      R__b >> phi;
      R__b >> mchi2;
      R__b.CheckByteCount(R__s, R__c, H0Info::IsA());
   } else {
      R__c = R__b.WriteVersion(H0Info::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m;
      R__b << p;
      R__b << costh;
      R__b << phi;
      R__b << mchi2;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_H0Info(void *p) {
      return  p ? new(p) ::H0Info : new ::H0Info;
   }
   static void *newArray_H0Info(Long_t nElements, void *p) {
      return p ? new(p) ::H0Info[nElements] : new ::H0Info[nElements];
   }
   // Wrapper around operator delete
   static void delete_H0Info(void *p) {
      delete ((::H0Info*)p);
   }
   static void deleteArray_H0Info(void *p) {
      delete [] ((::H0Info*)p);
   }
   static void destruct_H0Info(void *p) {
      typedef ::H0Info current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_H0Info(TBuffer &buf, void *obj) {
      ((::H0Info*)obj)->::H0Info::Streamer(buf);
   }
} // end of namespace ROOT for class ::H0Info

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 216,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace {
  void TriggerDictionaryInitialization_lreco_dict_Impl() {
    static const char* headers[] = {
"DalitzVars.h",
"DeltaEMbc.h",
"EvtInfo.h",
"EvtShape.h",
"GammaInfo.h",
"GenHepEvt.h",
"GenParticleInfo.h",
"HamletTag.h",
"IPBoost.h",
"Ks0Info.h",
"Pi0Info.h",
"SFWMInfo.h",
"TrackInfo.h",
"VertexInfo.h",
"TrackInfo2.h",
"H0Info.h",
0
    };
    static const char* includePaths[] = {
"/usr/local/include",
"/home/vitaly/work/MyLibs/libReco/include/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "lreco_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$DalitzVars.h")))  DalitzVars;
class __attribute__((annotate("$clingAutoload$DeltaEMbc.h")))  DeltaEMbc;
class __attribute__((annotate("$clingAutoload$EvtInfo.h")))  EvtInfo;
class __attribute__((annotate("$clingAutoload$SFWMInfo.h")))  __attribute__((annotate("$clingAutoload$EvtShape.h")))  SFWMInfo;
class __attribute__((annotate("$clingAutoload$EvtShape.h")))  EvtShape;
class __attribute__((annotate("$clingAutoload$GammaInfo.h")))  GammaInfo;
class __attribute__((annotate("$clingAutoload$GenHepEvt.h")))  GenHepEvt;
class __attribute__((annotate("$clingAutoload$GenParticleInfo.h")))  GenParticleInfo;
class __attribute__((annotate("$clingAutoload$HamletTag.h")))  HamletTag;
class __attribute__((annotate("$clingAutoload$IPBoost.h")))  IPBoost;
class __attribute__((annotate("$clingAutoload$Ks0Info.h")))  Ks0Info;
class __attribute__((annotate("$clingAutoload$Pi0Info.h")))  Pi0Info;
class __attribute__((annotate("$clingAutoload$TrackInfo.h")))  TrackInfo;
class __attribute__((annotate("$clingAutoload$VertexInfo.h")))  VertexInfo;
class __attribute__((annotate("$clingAutoload$TrackInfo2.h")))  TrackInfo2;
class __attribute__((annotate("$clingAutoload$H0Info.h")))  H0Info;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "lreco_dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "DalitzVars.h"
#include "DeltaEMbc.h"
#include "EvtInfo.h"
#include "EvtShape.h"
#include "GammaInfo.h"
#include "GenHepEvt.h"
#include "GenParticleInfo.h"
#include "HamletTag.h"
#include "IPBoost.h"
#include "Ks0Info.h"
#include "Pi0Info.h"
#include "SFWMInfo.h"
#include "TrackInfo.h"
#include "VertexInfo.h"
#include "TrackInfo2.h"
#include "H0Info.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"DalitzVars", payloadCode, "@",
"DeltaEMbc", payloadCode, "@",
"EvtInfo", payloadCode, "@",
"EvtShape", payloadCode, "@",
"GammaInfo", payloadCode, "@",
"GenHepEvt", payloadCode, "@",
"GenParticleInfo", payloadCode, "@",
"H0Info", payloadCode, "@",
"HamletTag", payloadCode, "@",
"IPBoost", payloadCode, "@",
"Ks0Info", payloadCode, "@",
"Pi0Info", payloadCode, "@",
"SFWMInfo", payloadCode, "@",
"TrackInfo", payloadCode, "@",
"TrackInfo2", payloadCode, "@",
"VertexInfo", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("lreco_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_lreco_dict_Impl, {}, classesHeaders, /*has no C++ module*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_lreco_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_lreco_dict() {
  TriggerDictionaryInitialization_lreco_dict_Impl();
}
