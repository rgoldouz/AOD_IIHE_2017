#ifndef UserCode_IIHETree_IIHEModuleParticleLevelObjects_h
#define UserCode_IIHETree_IIHEModuleParticleLevelObjects_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

// class decleration
class IIHEModuleParticleLevelObjects : public IIHEModule {
 public:
  explicit IIHEModuleParticleLevelObjects(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleParticleLevelObjects(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleParticleLevelObjects();
  
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
    edm::EDGetTokenT<std::vector<reco::GenJet> > particleLevelJetsToken_;
    edm::EDGetTokenT<std::vector<reco::GenJet> > particleLevelak1DressedLeptonToken_;
    edm::EDGetTokenT<std::vector<reco::MET> > particleLevelMETToken_;

};
#endif
