#ifndef UserCode_IIHETree_IIHEModuleData_h
#define UserCode_IIHETree_IIHEModuleData_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/Common/interface/EDCollection.h"
#include "UserCode/IIHETree/interface/TriggerObject.h"

// class decleration
class IIHEModuleData : public IIHEModule {
public:
  explicit IIHEModuleData(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleData(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleData();
  
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

  edm::EDGetTokenT<edm::View<pat::MET> > METCollectionToken_;
  edm::InputTag      METCollectionLabel_ ;

  edm::EDGetTokenT<bool> particleFlowEGammaGSFixedCollectionToken_;
  edm::InputTag      particleFlowEGammaGSFixedCollectionLabel_ ;

  edm::EDGetTokenT<View<pat::PackedCandidate> > pfcandidateCollectionToken_;
  edm::InputTag   pfcandidateCollectionLabel_ ;

  edm::EDGetTokenT<edm::EDCollection<DetId>> ecalMultiAndGSGlobalRecHitEBToken_;
  edm::InputTag   ecalMultiAndGSGlobalRecHitEBLabel_ ;

};
#endif
