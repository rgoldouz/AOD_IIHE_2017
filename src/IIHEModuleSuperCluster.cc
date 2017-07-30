#include "UserCode/IIHETree/interface/IIHEModuleSuperCluster.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleSuperCluster::IIHEModuleSuperCluster(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  superClusterCollectionLabel_ = iConfig.getParameter<edm::InputTag>("superClusterCollection"  ) ;
  superClusterCollectionToken_ =  iC.consumes<View<reco::SuperCluster>> (superClusterCollectionLabel_);

  primaryVertexLabel_          = iConfig.getParameter<edm::InputTag>("primaryVertex") ;
  vtxToken_ = iC.consumes<View<reco::Vertex>>(primaryVertexLabel_);
}
IIHEModuleSuperCluster::~IIHEModuleSuperCluster(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleSuperCluster::beginJob(){
  addBranch("sc_n",kUInt) ;
  setBranchType(kVectorFloat) ;
  addBranch("sc_energy") ;
  addBranch("sc_eta") ;
  addBranch("sc_etacorr") ;
  addBranch("sc_theta") ;
  addBranch("sc_thetacorr") ;
  addBranch("sc_et") ;
  addBranch("sc_phi") ;
  addBranch("sc_px") ;
  addBranch("sc_py") ;
  addBranch("sc_pz") ;
  addBranch("sc_x") ;
  addBranch("sc_y") ;
  addBranch("sc_z") ;
}

// ------------ method called to for each event  ------------
void IIHEModuleSuperCluster::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<View<reco::SuperCluster>> superClusterCollection_ ;
  iEvent.getByToken(superClusterCollectionToken_, superClusterCollection_) ;
  // Retrieve primary vertex collection
  edm::Handle<View<reco::Vertex> > pvCollection_ ;
  iEvent.getByToken( vtxToken_ , pvCollection_);
  edm::Ptr<reco::Vertex> firstpvertex = pvCollection_->ptrAt( 0 );;

  float pv_z = firstpvertex->z() ;
  
  store("sc_n", (unsigned int) superClusterCollection_->size()) ;

  for( unsigned int i = 0 ; i < superClusterCollection_->size() ; i++ ) {
    Ptr<reco::SuperCluster> scIt = superClusterCollection_->ptrAt( i );
    float sc_energy = scIt->rawEnergy()+scIt->preshowerEnergy() ;
    float sc_et     = sc_energy/cosh(scIt->eta()) ;
    float etaCorr = etacorr( scIt->eta(), pv_z, scIt->position().z()) ;
      
    store("sc_eta"      , scIt->eta()) ;
    store("sc_etacorr"  , etaCorr) ;
    store("sc_theta"    , 2.*atan(exp(-1.*scIt->eta()))) ;
    store("sc_thetacorr", 2.*atan(exp(-1.*etaCorr))) ;
    store("sc_phi"      , scIt->phi()) ;
    store("sc_energy"   , sc_energy) ;
    store("sc_et"       , sc_et    );
    store("sc_px"       , sc_et*cos(scIt->phi())) ;
    store("sc_py"       , sc_et*sin(scIt->phi()));
    store("sc_pz"       , sc_energy*tanh(scIt->eta())) ;
    store("sc_x"        , scIt->position().x()) ;
  edm::InputTag          muonCollectionLabel_ ;
    store("sc_y"        , scIt->position().y()) ;
    store("sc_z"        , scIt->position().z()) ;
  }
}

void IIHEModuleSuperCluster::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleSuperCluster::beginEvent(){}
void IIHEModuleSuperCluster::endEvent(){}

// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleSuperCluster::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleSuperCluster);
