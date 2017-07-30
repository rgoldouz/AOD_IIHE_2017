#include "UserCode/IIHETree/interface/IIHEModuleData.h"
#include "UserCode/IIHETree/interface/TriggerObject.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleData::IIHEModuleData(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  METCollectionLabel_                       = iConfig.getParameter<edm::InputTag>("METsMuEGCleanCollection") ; 
  METCollectionToken_                       = iC.consumes<View<pat::MET> > (METCollectionLabel_);

  particleFlowEGammaGSFixedCollectionLabel_ = iConfig.getParameter<edm::InputTag>("particleFlowEGammaGSFixedCollection") ;
  particleFlowEGammaGSFixedCollectionToken_ = iC.consumes<bool> (particleFlowEGammaGSFixedCollectionLabel_);

  pfcandidateCollectionLabel_               = iConfig.getParameter<edm::InputTag>("discardedMuonCollection") ;
  pfcandidateCollectionToken_               = iC.consumes<View<pat::PackedCandidate> > (pfcandidateCollectionLabel_);

  ecalMultiAndGSGlobalRecHitEBLabel_        = iConfig.getParameter<edm::InputTag>("ecalMultiAndGSGlobalRecHitEBCollection") ;
  ecalMultiAndGSGlobalRecHitEBToken_        = iC.consumes<edm::EDCollection<DetId>>(ecalMultiAndGSGlobalRecHitEBLabel_);
}
IIHEModuleData::~IIHEModuleData(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleData::beginJob(){
  setBranchType(kVectorFloat) ;
  setBranchType(kVectorBool) ;
  addBranch("gsf_bGSfix_ecaldrivenSeed"   ) ;

  setBranchType(kVectorInt) ;
  addBranch("gsf_bGSfix_nLostInnerHits"   ) ;

  setBranchType(kFloat) ;
  addBranch("MET_pfMetMuEGClean_et"   ) ;
  addBranch("MET_pfMetMuEGClean_phi"  ) ;

  addBranch("ev_particleFlowEGammaGSFixed", kBool) ;
  addBranch("ev_ecalMultiAndGSGlobalRecHitEB", kBool) ;

}

// ------------ method called to for each event  ------------
void IIHEModuleData::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){



  edm::Handle<edm::View<pat::MET> > METCollection_;
  iEvent.getByToken(METCollectionToken_, METCollection_);
  edm::Handle<bool> particleFlowEGammaGSFixedCollection_ ;
  iEvent.getByToken(particleFlowEGammaGSFixedCollectionToken_, particleFlowEGammaGSFixedCollection_) ;
  edm::Handle<edm::EDCollection<DetId>> ecalMultiAndGSGlobalRecHitEB_;
  iEvent.getByToken(ecalMultiAndGSGlobalRecHitEBToken_, ecalMultiAndGSGlobalRecHitEB_) ;



  store("ev_ecalMultiAndGSGlobalRecHitEB", ecalMultiAndGSGlobalRecHitEB_.isValid()) ;

  bool particleFlowEGammaGSFixed = *particleFlowEGammaGSFixedCollection_ ;
  store("ev_particleFlowEGammaGSFixed", particleFlowEGammaGSFixed) ;

  if (METCollection_.isValid()) {
    const pat::MET *pfMET = 0;
    pfMET = &(METCollection_->front());
    store("MET_pfMetMuEGClean_et"   , pfMET->et()    ) ;
    store("MET_pfMetMuEGClean_phi"  , pfMET->phi()   ) ;
  }

}

void IIHEModuleData::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleData::beginEvent(){}
void IIHEModuleData::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleData::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleData);
