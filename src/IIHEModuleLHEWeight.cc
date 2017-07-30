#include "UserCode/IIHETree/interface/IIHEModuleLHEWeight.h"
#include <iostream>
#include <TMath.h>
#include <vector>
#include <typeinfo>
#include <sstream>
#include <string>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleLHEWeight::IIHEModuleLHEWeight(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  lheEventLabel_ = iC.consumes<LHEEventProduct> (iConfig.getParameter<InputTag>("LHELabel"));
}
IIHEModuleLHEWeight::~IIHEModuleLHEWeight(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleLHEWeight::beginJob(){
  setBranchType(kFloat) ;
  addBranch("LHE_weight_nominal");
  setBranchType(kVectorFloat) ;
  addBranch("LHE_weight_sys");
  setBranchType(kVectorChar) ;
  addBranch("LHE_id_sys");
}

// ------------ method called to for each event  ------------
void IIHEModuleLHEWeight::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<LHEEventProduct> lhe_handle;
  iEvent.getByToken(lheEventLabel_, lhe_handle);
  if (lhe_handle.isValid()){
    store("LHE_weight_nominal",(float) lhe_handle->weights().at(0).wgt);
    for (unsigned i = 0; i < lhe_handle->weights().size(); ++i) {
string target(lhe_handle->weights().at(i).id.data());
    store("LHE_weight_sys",(float) lhe_handle->weights().at(i).wgt);
    store("LHE_id_sys",target );
    }
  }
}

void IIHEModuleLHEWeight::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleLHEWeight::beginEvent(){}
void IIHEModuleLHEWeight::endEvent(){}
void IIHEModuleLHEWeight::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleLHEWeight);
