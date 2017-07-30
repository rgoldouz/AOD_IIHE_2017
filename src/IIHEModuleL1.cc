#include "UserCode/IIHETree/interface/IIHEModuleL1.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleL1::IIHEModuleL1(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  l1noniso_ = iC.consumes<BXVector<l1t::EGamma>>(iConfig.getParameter<edm::InputTag>("l1NonIsoCollection"));
}
IIHEModuleL1::~IIHEModuleL1(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleL1::beginJob(){

  setBranchType(kVectorInt);
  addBranch("L1_pt");
  addBranch("L1_eta");
  addBranch("L1_phi");
  addBranch("L1_Iso");
}

// ------------ method called to for each event  ------------
void IIHEModuleL1::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<BXVector<l1t::EGamma>> egammas;
  iEvent.getByToken( l1noniso_, egammas );
  for(int i = egammas->getFirstBX(); i <= egammas->getLastBX(); ++i) {
    for(std::vector<l1t::EGamma>::const_iterator eg = egammas->begin(i); eg != egammas->end(i); ++eg) {
      if (eg->hwPt() <10) continue;
        store("L1_pt" , eg->hwPt());
        store("L1_eta", eg->hwEta());
        store("L1_phi", eg->hwPhi());
        store("L1_Iso", eg->hwIso());
    }
  }
}
void IIHEModuleL1::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleL1::beginEvent(){}
void IIHEModuleL1::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleL1::endJob(){
}

DEFINE_FWK_MODULE(IIHEModuleL1);
