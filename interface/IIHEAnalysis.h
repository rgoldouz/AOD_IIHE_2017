#ifndef UserCode_IIHETree_IIHEAnalysis_h
#define UserCode_IIHETree_IIHEAnalysis_h

// System includes
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// user includes
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "UserCode/IIHETree/interface/BranchWrapper.h"
#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "UserCode/IIHETree/interface/TriggerObject.h"
#include "UserCode/IIHETree/interface/MCTruthObject.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "DataFormats/Math/interface/Point3D.h"
// ROOT includes
#include "TFile.h"
#include "TTree.h"

// Local includes
#include "UserCode/IIHETree/interface/Types.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

// Forward declarations
class IIHEModule ;
class IIHEModuleMCTruth ;

// class decleration
class IIHEAnalysis : public edm::EDAnalyzer {

friend class IIHEModuleVertex ;
friend class IIHEModuleMuon ;
friend class IIHEModuleTracks ;


public:
  explicit IIHEAnalysis(const edm::ParameterSet& iConfig);

  ~IIHEAnalysis();
  
  bool store(std::string, bool    );
  bool store(std::string, double  );
  bool store(std::string, float   );
  bool store(std::string, int     );
  bool store(std::string, std::string     );
  bool store(std::string, unsigned int);
  bool store(std::string, unsigned long int);
  bool store(std::string, std::vector<bool        >);
  bool store(std::string, std::vector<double      >);
  bool store(std::string, std::vector<float       >);
  bool store(std::string, std::vector<int         >);
  bool store(std::string, std::vector<unsigned int>);
  
  bool addBranch(std::string) ;
  bool addBranch(std::string,int) ;
  bool branchExists(std::string) ;
  
  void setBranchType(int) ;
  int  getBranchType() ;
  int  saveToFile(TObject*) ;
  void listBranches() ;
  
  bool addValueToMetaTree(std::string, float) ;
  bool addFVValueToMetaTree(std::string, std::vector<float>) ; 
  // MC truth
  void addToMCTruthWhitelist(std::vector<int>) ;
  std::vector<int> getMCTruthWhitelist(){ return MCTruthWhitelist_ ; }
  
  void configureBranches();
  std::vector<std::string> splitString(const std::string&, const char*) ;

  int getPreScaleIndex(){ return preScaleIndex_ ; }
  
  bool getAcceptStatus(){ return acceptEvent_ ; }
  void   vetoEvent(){ acceptEvent_ = false ; }
  void acceptEvent(){ acceptEvent_ =  true ; }
  void rejectEvent(){ rejectEvent_ =  true ; }
  
  const MCTruthObject* MCTruth_getRecordByIndex(int) ;
  const MCTruthObject* MCTruth_matchEtaPhi(float, float) ;
  int MCTruth_matchEtaPhi_getIndex(float, float) ;
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  
  void beginEvent() ;
  void endEvent() ;
  
  // ----------member data ---------------------------
  std::vector<BranchWrapperBase*> allVars_ ;
  std::vector<BranchWrapperBVV* > vars_BVV_;
  std::vector<BranchWrapperDVV* > vars_DVV_;
  std::vector<BranchWrapperFVV* > vars_FVV_;
  std::vector<BranchWrapperIVV* > vars_IVV_;
  std::vector<BranchWrapperUVV* > vars_UVV_;
  std::vector<BranchWrapperBV*  > vars_BV_ ;
  std::vector<BranchWrapperDV*  > vars_DV_ ;
  std::vector<BranchWrapperFV*  > vars_FV_ ;
  std::vector<BranchWrapperIV*  > vars_IV_ ;
  std::vector<BranchWrapperCV*  > vars_CV_ ;
  std::vector<BranchWrapperUV*  > vars_UV_ ;
  std::vector<BranchWrapperULV* > vars_ULV_ ;
  std::vector<BranchWrapperB*   > vars_B_  ;
  std::vector<BranchWrapperD*   > vars_D_  ;
  std::vector<BranchWrapperF*   > vars_F_  ;
  std::vector<BranchWrapperI*   > vars_I_  ;
  std::vector<BranchWrapperC*   > vars_C_  ;
  std::vector<BranchWrapperU*   > vars_U_  ;
  std::vector<BranchWrapperUL*   > vars_UL_  ;
  
  int currentVarType_ ;
  std::vector< std::pair<std::string, int> > listOfBranches_  ;
  std::vector< std::pair<std::string, int> > missingBranches_ ;
  
  // Bools for including each module so they can be turned on/off without recompilation
  bool includeEventModule_           ;
  bool includeVertexModule_          ;
  bool includeSuperClusterModule_    ;
  bool includePhotonModule_          ;
  bool includeElectronModule_        ;
  bool includeMuonModule_            ;
  bool includeMETModule_             ;
  bool includeJetModule_             ;
  bool includeTauModule_             ;
  bool includeL1Module_             ;
  bool includeParticleLevelObjectsModule_;
  bool includeDataModule_             ;
  bool includeMCTruthModule_         ;
  bool includeLHEWeightModule_         ;
  bool includeTriggerModule_         ;
  bool includeZBosonModule_          ;
  bool includeLeptonsAcceptModule_   ;
  bool includeAutoAcceptEventModule_ ;
  bool includeTracksModule_           ;

  int preScaleIndex_; 
  HLTPrescaleProvider hltPrescaleProvider_;
  edm::InputTag trigEventTag_;  
  // The event only gets saved if acceptEvent_ == true
  bool acceptEvent_ ;
  // This variable is used to reject an event early on.  This prevents the analyser
  // running over the rest of the modules if it's not going to save the event anyway.
  bool rejectEvent_ ;
 
  std::vector<float> nRuns_; 
  int nEvents_ ;
  int nEventsStored_ ;
  bool debug_;
  std::string globalTag_ ;
  
  // MC truth module
  IIHEModuleMCTruth* MCTruthModule_ ;
  std::vector<int> MCTruthWhitelist_ ;
  std::vector<IIHEModule*> childModules_;
  std::vector<BranchWrapperF*> metaTreePars_ ;
  
  TTree* dataTree_ ;
  TTree* metaTree_ ;
};
#endif
//define this as a plug-in

