#ifndef UserCode_IIHETree_IIHEModuleJet_h
#define UserCode_IIHETree_IIHEModuleJet_h
#include "UserCode/IIHETree/interface/btag_weighter.h"
#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

// class decleration

class IIHEModuleJet : public IIHEModule {
public:
  explicit IIHEModuleJet(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleJet(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleJet();
  
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
  edm::InputTag pfJetLabel_;
  edm::EDGetTokenT<edm::View<pat::Jet> > pfJetToken_;
  edm::InputTag pfJetLabelSmeared_;
  edm::EDGetTokenT<edm::View<pat::Jet> > pfJetTokenSmeared_;
  edm::InputTag pfJetLabelEnUp_;
  edm::EDGetTokenT<edm::View<pat::Jet> > pfJetTokenEnUp_;
  edm::InputTag pfJetLabelEnDown_;
  edm::EDGetTokenT<edm::View<pat::Jet> > pfJetTokenEnDown_;
  edm::InputTag pfJetLabelSmearedJetResUp_;
  edm::EDGetTokenT<edm::View<pat::Jet> > pfJetTokenSmearedJetResUp_;
  edm::InputTag pfJetLabelSmearedJetResDown_;
  edm::EDGetTokenT<edm::View<pat::Jet> > pfJetTokenSmearedJetResDown_;

  float ETThreshold_ ;
  bool isMC_;
  BTagWeighter *btw;
};
#endif
