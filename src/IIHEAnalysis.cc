// System includes
#include <iostream>
#include <TMath.h>
#include <vector>

#include <boost/algorithm/string.hpp>

// IIHE includes
#include "UserCode/IIHETree/interface/IIHEAnalysis.h"

#include "UserCode/IIHETree/interface/EtSort.h"
#include "UserCode/IIHETree/interface/BranchWrapper.h"
#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "UserCode/IIHETree/interface/IIHEModuleEvent.h"
#include "UserCode/IIHETree/interface/IIHEModuleVertex.h"
#include "UserCode/IIHETree/interface/IIHEModuleSuperCluster.h"
#include "UserCode/IIHETree/interface/IIHEModulePhoton.h"
#include "UserCode/IIHETree/interface/IIHEModuleGedGsfElectron.h"
#include "UserCode/IIHETree/interface/IIHEModuleMuon.h"
#include "UserCode/IIHETree/interface/IIHEModuleMCTruth.h"
#include "UserCode/IIHETree/interface/IIHEModuleLHEWeight.h"
#include "UserCode/IIHETree/interface/IIHEModuleMET.h"
#include "UserCode/IIHETree/interface/IIHEModuleJet.h"
#include "UserCode/IIHETree/interface/IIHEModuleTau.h"
#include "UserCode/IIHETree/interface/IIHEModuleL1.h"
#include "UserCode/IIHETree/interface/IIHEModuleData.h"
#include "UserCode/IIHETree/interface/IIHEModuleTrigger.h"
#include "UserCode/IIHETree/interface/IIHEModuleZBoson.h"
#include "UserCode/IIHETree/interface/IIHEModuleLeptonsAccept.h"
#include "UserCode/IIHETree/interface/IIHEModuleAutoAcceptEvent.h"

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEAnalysis::IIHEAnalysis(const edm::ParameterSet& iConfig):
hltPrescaleProvider_(iConfig, consumesCollector(), *this)
{
  currentVarType_ = -1 ;
  debug_     = iConfig.getParameter<bool  >("debug"    ) ;
  globalTag_ = iConfig.getParameter<string>("globalTag") ;
  nEvents_ = 0 ;
  nEventsStored_ = 0 ;
  acceptEvent_ = false ;
  
  edm::Service<TFileService> fs ;
  fs->file().cd() ;
  
  //mainFile_ = TFile("outfile.root", "RECREATE") ;
  dataTree_ = new TTree("IIHEAnalysis", "IIHEAnalysis") ;
  metaTree_ = new TTree("meta", "Information about globalTag etc") ;
  metaTree_->Branch("globalTag", &globalTag_) ;

  trigEventTag_ = iConfig.getParameter<InputTag>("triggerEvent");  
  MCTruthModule_ = 0 ;
  
  includeLeptonsAcceptModule_   = iConfig.getUntrackedParameter<bool>("includeLeptonsAcceptModule" ) ;
  includeTriggerModule_         = iConfig.getUntrackedParameter<bool>("includeTriggerModule"       ) ;
  includeEventModule_           = iConfig.getUntrackedParameter<bool>("includeEventModule"         ) ;
  includeVertexModule_          = iConfig.getUntrackedParameter<bool>("includeVertexModule"        ) ;
  includeSuperClusterModule_    = iConfig.getUntrackedParameter<bool>("includeSuperClusterModule"  ) ;
  includePhotonModule_          = iConfig.getUntrackedParameter<bool>("includePhotonModule"        ) ;
  includeElectronModule_        = iConfig.getUntrackedParameter<bool>("includeElectronModule"      ) ;
  includeMuonModule_            = iConfig.getUntrackedParameter<bool>("includeMuonModule"          ) ;
  includeMETModule_             = iConfig.getUntrackedParameter<bool>("includeMETModule"           ) ;
  includeJetModule_             = iConfig.getUntrackedParameter<bool>("includeJetModule"           ) ;
  includeTauModule_             = iConfig.getUntrackedParameter<bool>("includeTauModule"           ) ;
  includeL1Module_              = iConfig.getUntrackedParameter<bool>("includeL1Module"            ) ;
  includeDataModule_            = iConfig.getUntrackedParameter<bool>("includeDataModule"          ) ;
  includeMCTruthModule_         = iConfig.getUntrackedParameter<bool>("includeMCTruthModule"       ) ;
  includeLHEWeightModule_         = iConfig.getUntrackedParameter<bool>("includeLHEWeightModule"       ) ;
  includeZBosonModule_          = iConfig.getUntrackedParameter<bool>("includeZBosonModule"        ) ;
  includeAutoAcceptEventModule_ = iConfig.getUntrackedParameter<bool>("includeAutoAcceptEventModule") ;
  
  if(includeLeptonsAcceptModule_  ) childModules_.push_back(new IIHEModuleLeptonsAccept(iConfig ,consumesCollector())  ) ;   
  if(includeEventModule_          ) childModules_.push_back(new IIHEModuleEvent(iConfig   ,consumesCollector()       )) ;
  if(includeLHEWeightModule_         ) childModules_.push_back(new IIHEModuleLHEWeight(iConfig ,consumesCollector())         ) ;
  if(includeMCTruthModule_        ){
    MCTruthModule_ = new IIHEModuleMCTruth(iConfig ,consumesCollector()) ;
    childModules_.push_back(MCTruthModule_) ;
  }
  if(includeVertexModule_         ) childModules_.push_back(new IIHEModuleVertex(iConfig ,consumesCollector())         ) ;
  if(includeSuperClusterModule_   ) childModules_.push_back(new IIHEModuleSuperCluster(iConfig ,consumesCollector())   ) ;
  if(includePhotonModule_         ) childModules_.push_back(new IIHEModulePhoton(iConfig ,consumesCollector())         ) ;
  if(includeElectronModule_       ) childModules_.push_back(new IIHEModuleGedGsfElectron(iConfig ,consumesCollector()) ) ;
  if(includeMuonModule_           ) childModules_.push_back(new IIHEModuleMuon(iConfig ,consumesCollector())           ) ;
  if(includeJetModule_            ) childModules_.push_back(new IIHEModuleJet(iConfig ,consumesCollector())            ) ;
  if(includeMETModule_            ) childModules_.push_back(new IIHEModuleMET(iConfig ,consumesCollector())            ) ;
  if(includeTauModule_            ) childModules_.push_back(new IIHEModuleTau(iConfig ,consumesCollector())            ) ;
  if(includeL1Module_             ) childModules_.push_back(new IIHEModuleL1(iConfig ,consumesCollector())             ) ;
  if(includeDataModule_           ) childModules_.push_back(new IIHEModuleData(iConfig ,consumesCollector())           ) ;
  if(includeZBosonModule_         ) childModules_.push_back(new IIHEModuleZBoson(iConfig ,consumesCollector())         ) ;  
  if(includeAutoAcceptEventModule_) childModules_.push_back(new IIHEModuleAutoAcceptEvent(iConfig ,consumesCollector())) ; 
  if(includeTriggerModule_        ) childModules_.push_back(new IIHEModuleTrigger(iConfig,consumesCollector())        ) ; 
}

IIHEAnalysis::~IIHEAnalysis(){}

const MCTruthObject* IIHEAnalysis::MCTruth_getRecordByIndex(int index){
  if(MCTruthModule_){
    return MCTruthModule_->getRecordByIndex(index) ;
  }
  return 0 ;
}
const MCTruthObject* IIHEAnalysis::MCTruth_matchEtaPhi(float eta, float phi){
  if(MCTruthModule_){
    return MCTruthModule_->matchEtaPhi(eta, phi) ;
  }
  return 0 ;
}
int IIHEAnalysis::MCTruth_matchEtaPhi_getIndex(float eta, float phi){
  if(MCTruthModule_){
    return MCTruthModule_->matchEtaPhi_getIndex(eta, phi) ;
  }
  return -1 ;
}

bool IIHEAnalysis::addValueToMetaTree(std::string parName, float value){
  BranchWrapperF* bw = new BranchWrapperF(parName) ;
  metaTreePars_.push_back(bw) ;
  bw->config(metaTree_) ;
  bw->set(value) ;
  return true ;
}

bool IIHEAnalysis::addFVValueToMetaTree(std::string parName, std::vector<float> value){
  BranchWrapperFV* bw = new BranchWrapperFV(parName) ;
  for (unsigned int i=0 ; i<value.size() ; ++i){
    bw->push(value[i]);
  }
  bw->config(metaTree_) ;
  return true ;
}

bool IIHEAnalysis::branchExists(std::string name){
  for(unsigned int i=0 ; i<allVars_.size() ; ++i){
    if(allVars_.at(i)->name()==name) return true ;
  }
  return false ;
}

void IIHEAnalysis::setBranchType(int type){ currentVarType_ = type ; }
int  IIHEAnalysis::getBranchType(){ return currentVarType_ ; }

bool IIHEAnalysis::addBranch(std::string name){ return addBranch(name, currentVarType_) ; }
bool IIHEAnalysis::addBranch(std::string name, int type){
  // First check to see if this branch name has already been used
  bool success = !(branchExists(name)) ;
  if(debug_) std::cout << "Adding a branch named " << name << " " << success << endl ;
  if(success==false){
    return false ;
  }
  listOfBranches_.push_back(std::pair<std::string,int>(name,type)) ;
  switch(type){
    case kBool:{
      BranchWrapperB*  bw = new BranchWrapperB(name) ;
      vars_B_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kDouble:{
      BranchWrapperD*  bw = new BranchWrapperD(name) ;
      vars_D_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
    }
    case kFloat:{
      BranchWrapperF*  bw = new BranchWrapperF(name) ;
      vars_F_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kInt:{
      BranchWrapperI*  bw = new BranchWrapperI(name) ;
      vars_I_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kChar:{
      BranchWrapperC*  bw = new BranchWrapperC(name) ;
      vars_C_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kUInt:{
      BranchWrapperU*  bw = new BranchWrapperU(name) ;
      vars_U_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kULInt:{
      BranchWrapperUL*  bw = new BranchWrapperUL(name) ;
      vars_UL_  .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorBool:{
      BranchWrapperBV* bw = new BranchWrapperBV(name) ;
      vars_BV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorDouble:{
      BranchWrapperDV* bw = new BranchWrapperDV(name) ;
      vars_DV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorFloat:{
      BranchWrapperFV* bw = new BranchWrapperFV(name) ;
      vars_FV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorInt:{
      BranchWrapperIV* bw = new BranchWrapperIV(name) ;
      vars_IV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorChar:{
      BranchWrapperCV* bw = new BranchWrapperCV(name) ;
      vars_CV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorUInt:{
      BranchWrapperUV* bw = new BranchWrapperUV(name) ;
      vars_UV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorULInt:{
      BranchWrapperULV* bw = new BranchWrapperULV(name) ;
      vars_ULV_ .push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorBool:{
      BranchWrapperBVV* bw = new BranchWrapperBVV(name) ;
      vars_BVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorDouble:{
      BranchWrapperDVV* bw = new BranchWrapperDVV(name) ;
      vars_DVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorFloat:{
      BranchWrapperFVV* bw = new BranchWrapperFVV(name) ;
      vars_FVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorInt:{
      BranchWrapperIVV* bw = new BranchWrapperIVV(name) ;
      vars_IVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    case kVectorVectorUInt:{
      BranchWrapperUVV* bw = new BranchWrapperUVV(name) ;
      vars_UVV_.push_back(bw) ;
      allVars_.push_back((BranchWrapperBase*)bw) ;
      break ;
    }
    default :
      std::cout << "Failed to make a branch" << std::endl ;
      return false ; // Bail out if we don't know the type of branch
  }
  return true ;
}

// ------------ method called once each job just before starting event loop  -------------
void IIHEAnalysis::beginJob(){
  for(unsigned int i=0 ; i<childModules_.size() ; ++i){
    childModules_.at(i)->config(this) ;
    childModules_.at(i)->pubBeginJob() ;
  }
  
  // We have to call this last because other modules can add particles to the whitelist.
  // However the other modules must follow MCTruthModule to use the whitelist.  Here is
  // where we break the chicken and egg problem.
  if(MCTruthModule_) MCTruthModule_->setWhitelist() ;
  
  configureBranches() ;
}

void IIHEAnalysis::listBranches(){
  dataTree_->GetListOfLeaves()->ls() ;
}

int IIHEAnalysis::saveToFile(TObject* obj){
  edm::Service<TFileService> fs ;
  fs->file().cd() ;
  return obj->Write() ;
}

void IIHEAnalysis::configureBranches(){
  for(unsigned int i=0 ; i<allVars_.size() ; ++i){
    allVars_.at(i)->config(dataTree_) ;
  }
  return ;
}

void IIHEAnalysis::addToMCTruthWhitelist(std::vector<int> pdgIds){
  for(unsigned int i=0 ; i<pdgIds.size() ; ++i){
    bool add = true ;
    for(unsigned int j=0 ; j<MCTruthWhitelist_.size() ; j++){
      if(abs(MCTruthWhitelist_.at(j))==abs(pdgIds.at(i))){
        add = false ;
        break ;
      }
    }
    if(add){
      MCTruthWhitelist_.push_back(pdgIds.at(i)) ;
    }
  }
}

// ------------ method called to for each event  -----------------------------------------

void IIHEAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  preScaleIndex_ = hltPrescaleProvider_.prescaleSet(iEvent,iSetup);
  beginEvent() ;
  for(unsigned i=0 ; i<childModules_.size() ; ++i){
    childModules_.at(i)->pubAnalyze(iEvent, iSetup) ;
    if(rejectEvent_) break ;
  }
  endEvent() ;
}

void IIHEAnalysis::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){
  bool changed = true ;
  hltPrescaleProvider_.init(iRun, iSetup, trigEventTag_.process(), changed);

  nRuns_.push_back(iRun.run());
  for(unsigned int i=0 ; i<childModules_.size() ; ++i){
    childModules_.at(i)->pubBeginRun(iRun, iSetup) ;
  }
}

void IIHEAnalysis::beginEvent(){
  acceptEvent_ = false ;
  rejectEvent_ = false ;
  for(unsigned int i=0 ; i<childModules_.size() ; ++i){ childModules_.at(i)->pubBeginEvent() ; }
  for(unsigned int i=0 ; i<allVars_.size()      ; ++i){ allVars_.at(i)->beginEvent()         ; }
}
void IIHEAnalysis::endEvent(){
  for(unsigned int i=0 ; i<childModules_.size() ; ++i){ childModules_.at(i)->pubEndEvent() ; }
  for(unsigned int i=0 ; i<allVars_.size()      ; ++i){      allVars_.at(i)->endEvent()    ; }
  if(true==acceptEvent_ && false==rejectEvent_){
    dataTree_->Fill() ;
    nEventsStored_++ ;
  }
  nEvents_++ ;
}

// ------------ method called once each job just after ending the event loop  ------------
void IIHEAnalysis::endJob(){

  for(unsigned int i=0 ; i<childModules_.size() ; ++i){ childModules_.at(i)->pubEndJob() ; }
  std::vector<std::string> untouchedBranchNames ;
  for(unsigned int i=0 ; i<allVars_.size() ; ++i){
    if(allVars_.at(i)->is_touched()==false) untouchedBranchNames.push_back(allVars_.at(i)->name()) ;
  }
  if(debug_==true){
    if(untouchedBranchNames.size()>0){
      std::cout << "The following branches were never touched:" << std::endl ;
      for(unsigned int i=0 ; i<untouchedBranchNames.size() ; ++i){
        std::cout << "  " << untouchedBranchNames.at(i) << std::endl ;
      }
    }
  }
  
  addValueToMetaTree("nEventsRaw"   , nEvents_      ) ;
  addValueToMetaTree("nEventsStored", nEventsStored_) ;
  addFVValueToMetaTree("nRuns", nRuns_) ;
  metaTree_->Fill() ;
  
  std::cout << "There were " << nEvents_ << " total events of which " << nEventsStored_ << " were stored to file." << std::endl ;

}

// ------------ method for storing information into the TTree  ------------
bool IIHEAnalysis::store(std::string name, bool value){
  for(unsigned int i=0 ; i<vars_B_.size() ; ++i){
    if(vars_B_.at(i)->name()==name){
      vars_B_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_BV_.size() ; ++i){
    if(vars_BV_.at(i)->name()==name){
      vars_BV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (bool) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, double value){
  // Try to fill doubles, then floats
  for(unsigned int i=0 ; i<vars_D_.size() ; ++i){
    if(vars_D_.at(i)->name()==name){
      vars_D_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_DV_.size() ; ++i){
    if(vars_DV_.at(i)->name()==name){
      vars_DV_ .at(i)->push(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_F_.size() ; ++i){
    if(vars_F_.at(i)->name()==name){
      vars_F_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_FV_.size() ; ++i){
    if(vars_FV_.at(i)->name()==name){
      vars_FV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (double) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, float value){
  // Try to fill floats, then doubles
  for(unsigned int i=0 ; i<vars_F_.size() ; ++i){
    if(vars_F_.at(i)->name()==name){
      vars_F_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_FV_.size() ; ++i){
    if(vars_FV_.at(i)->name()==name){
      vars_FV_ .at(i)->push(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_D_.size() ; ++i){
    if(vars_D_.at(i)->name()==name){
      vars_D_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_DV_.size() ; ++i){
    if(vars_DV_.at(i)->name()==name){
      vars_DV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (float) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, int value){
  for(unsigned int i=0 ; i<vars_I_.size() ; ++i){
    if(vars_I_.at(i)->name()==name){
      vars_I_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_IV_.size() ; ++i){
    if(vars_IV_.at(i)->name()==name){
      vars_IV_ .at(i)->push(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_U_.size() ; ++i){
    if(vars_U_.at(i)->name()==name){
      vars_U_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_UV_.size() ; ++i){
    if(vars_UV_.at(i)->name()==name){
      vars_UV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (int) branch named " << name << std::endl ;
  return false ;
}


bool IIHEAnalysis::store(std::string name, std::string value){
  for(unsigned int i=0 ; i<vars_C_.size() ; ++i){
    if(vars_C_.at(i)->name()==name){
      vars_C_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_CV_.size() ; ++i){
    if(vars_CV_.at(i)->name()==name){
      vars_CV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (char) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, unsigned int value){
  for(unsigned int i=0 ; i<vars_U_.size() ; ++i){
    if(vars_U_.at(i)->name()==name){
      vars_U_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_UV_.size() ; ++i){
    if(vars_UV_.at(i)->name()==name){
      vars_UV_ .at(i)->push(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_I_.size() ; ++i){
    if(vars_I_.at(i)->name()==name){
      vars_I_ .at(i)->set(value) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_IV_.size() ; ++i){
    if(vars_IV_.at(i)->name()==name){
      vars_IV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (uint) branch named " << name << std::endl ;
  return false ;
}

bool IIHEAnalysis::store(std::string name, unsigned long int value){ 
  for(unsigned int i=0 ; i<vars_UL_.size() ; ++i){
    if(vars_UL_.at(i)->name()==name){
      vars_UL_ .at(i)->set(value) ;
      return true ;
    }
  } 
  for(unsigned int i=0 ; i<vars_ULV_.size() ; ++i){
    if(vars_ULV_.at(i)->name()==name){
      vars_ULV_ .at(i)->push(value) ;
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (ulint) branch named " << name << std::endl ;
  return false ;
}

bool IIHEAnalysis::store(std::string name, std::vector<bool> values){
  for(unsigned int i=0 ; i<vars_BVV_.size() ; ++i){
    if(vars_BVV_.at(i)->name()==name){
      vars_BVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_BV_.size() ; ++i){
    if(vars_BV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_BV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector bool) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, std::vector<float> values){
  for(unsigned int i=0 ; i<vars_FVV_.size() ; ++i){
    if(vars_FVV_.at(i)->name()==name){
      vars_FVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_FV_.size() ; ++i){
    if(vars_FV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_FV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector float) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, std::vector<double> values){
  for(unsigned int i=0 ; i<vars_DVV_.size() ; ++i){
    if(vars_DVV_.at(i)->name()==name){
      vars_DVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_DV_.size() ; ++i){
    if(vars_DV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_DV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector double) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, std::vector<int> values){
  for(unsigned int i=0 ; i<vars_IVV_.size() ; ++i){
    if(vars_IVV_.at(i)->name()==name){
      vars_IVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_IV_.size() ; ++i){
    if(vars_IV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_IV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector int) branch named " << name << std::endl ;
  return false ;
}
bool IIHEAnalysis::store(std::string name, std::vector<unsigned int> values){
  for(unsigned int i=0 ; i<vars_UVV_.size() ; ++i){
    if(vars_UVV_.at(i)->name()==name){
      vars_UVV_ .at(i)->push(values) ;
      return true ;
    }
  }
  for(unsigned int i=0 ; i<vars_UV_.size() ; ++i){
    if(vars_UV_.at(i)->name()==name){
      for(unsigned j=0 ; j<values.size() ; ++j){
        vars_UV_ .at(i)->push(values.at(j)) ;
      }
      return true ;
    }
  }
  if(debug_) std::cout << "Could not find a (vector uint) branch named " << name << std::endl ;
  return false ;
}

// Function to split strings.  Required for passing comma separated arguments via the pset
std::vector<std::string> IIHEAnalysis::splitString(const string &text, const char* sep){
  vector<string> results ;
  boost::split(results, text, boost::is_any_of(","));
  return results ;
}

DEFINE_FWK_MODULE(IIHEAnalysis);
