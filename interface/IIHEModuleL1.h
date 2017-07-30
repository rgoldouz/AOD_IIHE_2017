#ifndef UserCode_IIHETree_IIHEModuleL1_h
#define UserCode_IIHETree_IIHEModuleL1_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/L1Candidate.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"

#include "CondFormats/L1TObjects/interface/L1TUtmTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1TUtmTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1TGlobalParameters.h"

// class decleration
class IIHEModuleL1 : public IIHEModule {
 public:
  explicit IIHEModuleL1(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleL1(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleL1();
  
  void   pubBeginJob(){   beginJob() ; } ;
  void pubBeginEvent(){ beginEvent() ; } ;
  void   pubEndEvent(){   endEvent() ; } ;
  virtual void pubAnalyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){ analyze(iEvent, iSetup) ; } ;
  
  virtual void beginEvent() ;
  virtual void endEvent() ;
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);

 private:
  edm::EDGetTokenT<BXVector<l1t::EGamma>> l1noniso_;
};
#endif
