#include "UserCode/IIHETree/interface/IIHEModuleVertex.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleVertex::IIHEModuleVertex(const edm::ParameterSet& iConfig , edm::ConsumesCollector && iC): IIHEModule(iConfig){
  primaryVertexLabel_          = iConfig.getParameter<edm::InputTag>("primaryVertex") ;
  vtxToken_ = iC.consumes<View<reco::Vertex>>(primaryVertexLabel_);
}
IIHEModuleVertex::~IIHEModuleVertex(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleVertex::beginJob(){
  addBranch("pv_n", kUInt) ;
  setBranchType(kVectorFloat) ;
  addBranch("pv_x") ;
  addBranch("pv_y") ;
  addBranch("pv_z") ;
  addBranch("pv_ndof") ;
  addBranch("pv_normalizedChi2") ;
  addBranch("pv_isValid", kVectorBool) ;
  addBranch("pv_isFake", kVectorBool) ;
}

// ------------ method called to for each event  ------------
void IIHEModuleVertex::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  edm::Handle<View<reco::Vertex> > pvCollection_ ;
  iEvent.getByToken( vtxToken_ , pvCollection_);

  store("pv_n", (unsigned int) pvCollection_->size()) ;
  for( unsigned int i = 0 ; i < pvCollection_->size() ; i++ ) {
  edm::Ptr<reco::Vertex> pvIt = pvCollection_->ptrAt( i );
    store("pv_x"             , pvIt->x()                ) ;
    store("pv_y"             , pvIt->y()                ) ;   
    store("pv_z"             , pvIt->z()                ) ;  
    store("pv_isValid"       , pvIt->isValid()          ) ;
    store("pv_isValid"       , pvIt->isFake()          ) ;
    store("pv_ndof"          , pvIt->ndof()        ) ;
    store("pv_normalizedChi2", pvIt->normalizedChi2()   ) ;
  }
}

void IIHEModuleVertex::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleVertex::beginEvent(){}
void IIHEModuleVertex::endEvent(){}

// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleVertex::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleVertex);
