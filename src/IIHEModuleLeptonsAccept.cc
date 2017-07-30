#include "UserCode/IIHETree/interface/IIHEModuleLeptonsAccept.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleLeptonsAccept::IIHEModuleLeptonsAccept(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  ptThreshold_         = iConfig.getUntrackedParameter<double>("leptonsAcceptPtThreshold") ;
  nEleThreshold_       = iConfig.getUntrackedParameter<int>("leptonsAccept_nEle"     ) ;
  nEleMuThreshold_     = iConfig.getUntrackedParameter<int>("leptonsAccept_nEleMu"   ) ;
  nEleTauThreshold_    = iConfig.getUntrackedParameter<int>("leptonsAccept_nEleTau"  ) ;
  nMuThreshold_        = iConfig.getUntrackedParameter<int>("leptonsAccept_nMu"      ) ;
  nMuTauThreshold_     = iConfig.getUntrackedParameter<int>("leptonsAccept_nMuTau"   ) ;
  nTauThreshold_       = iConfig.getUntrackedParameter<int>("leptonsAccept_nTau"     ) ;

  electronCollectionLabel_     = iConfig.getParameter<edm::InputTag>("electronCollection"      ) ;
  muonCollectionLabel_         = iConfig.getParameter<edm::InputTag>("muonCollection"          ) ;
  tauCollectionLabel_          = iConfig.getParameter<edm::InputTag>("tauCollection");

  electronCollectionToken_ =  iC.consumes<View<reco::GsfElectron> > (electronCollectionLabel_);
  muonCollectionToken_     =  iC.consumes<View<reco::Muon> > (muonCollectionLabel_);
  tauCollectionToken_      =  iC.consumes<View<reco::PFTau>> (tauCollectionLabel_);
}
IIHEModuleLeptonsAccept::~IIHEModuleLeptonsAccept(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleLeptonsAccept::beginJob(){
  nAcceptAll_ = 0 ;
}

// ------------ method called to for each event  ------------
void IIHEModuleLeptonsAccept::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<edm::View<reco::GsfElectron> > electronCollection_;
  iEvent.getByToken( electronCollectionToken_, electronCollection_) ;

  edm::Handle<edm::View<reco::Muon> > muonCollection_;
  iEvent.getByToken( muonCollectionToken_, muonCollection_) ;

  Handle<View<reco::PFTau> > tauCollection_ ;
  iEvent.getByToken( tauCollectionToken_, tauCollection_ );

  int nEl = 0 ;
  int nMu = 0 ;
  int nTau = 0 ;
  
  for( unsigned int i = 0 ; i < electronCollection_->size() ; i++ ) {
    Ptr<reco::GsfElectron> gsfiter = electronCollection_->ptrAt( i );
    float pt = gsfiter->pt() ;
    float HEEP_ET  = gsfiter->caloEnergy()*sin(gsfiter->p4().theta()) ;
    if(pt>ptThreshold_ || HEEP_ET>ptThreshold_) nEl++ ;
  }

  for( unsigned int i = 0 ; i < muonCollection_->size() ; i++ ) {
    Ptr<reco::Muon> muiter = muonCollection_->ptrAt( i );
    float pt = muiter->pt() ;
    if(pt>ptThreshold_) nMu++ ;
  }

  for ( unsigned int i = 0; i <tauCollection_->size(); ++i) {
    Ptr<reco::PFTau> tauni = tauCollection_->ptrAt( i );
    float pt = tauni->pt();
    if(pt>ptThreshold_) nTau++ ;
  }

  
  bool acceptEle         = (nEl           >= nEleThreshold_     ) ;
  bool acceptElemu       = (nEl + nMu     >= nEleMuThreshold_ && nEl >0 &&  nMu>0 ) ;
  bool acceptEleTau      = (nEl + nTau    >= nEleTauThreshold_  && nEl >0 && nTau>0 ) ;
  bool acceptMu          = (nMu           >= nMuThreshold_      ) ;
  bool acceptMuTau       = (nMu + nTau    >= nMuTauThreshold_  && nMu>0 && nTau>0 ) ;
  bool acceptTau         = (nTau          >= nTauThreshold_     ) ;

  bool acceptThisEvent = (acceptEle || acceptElemu || acceptEleTau || acceptMu || acceptMuTau || acceptTau) ;
  // Save the event if we see something we like
  if(acceptThisEvent){
    acceptEvent() ;
    nAcceptAll_++ ;
  }
  else{
    //rejectEvent() ;
  }
}
void IIHEModuleLeptonsAccept::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleLeptonsAccept::beginEvent(){}
void IIHEModuleLeptonsAccept::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleLeptonsAccept::endJob(){
  std::cout << std::endl << "IIHEModuleLeptonsAccept report:" << std::endl ;
  std::cout << "  nAcceptAll  = " << nAcceptAll_   << std::endl ;
}

DEFINE_FWK_MODULE(IIHEModuleLeptonsAccept);
