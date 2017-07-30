#ifndef UserCode_IIHETree_IIHEModuleTau_h
#define UserCode_IIHETree_IIHEModuleTau_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

// class decleration
class IIHEModuleTau : public IIHEModule {
 public:
  explicit IIHEModuleTau(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleTau(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleTau();
  
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
  edm::EDGetTokenT<edm::View<pat::Tau> > tauCollectionToken_;
  edm::InputTag                          tauCollectionLabel_ ;
  float ETThreshold_ ;
};
#endif
