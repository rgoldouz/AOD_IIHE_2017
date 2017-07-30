#ifndef UserCode_IIHETree_IIHEModuleMET_h
#define UserCode_IIHETree_IIHEModuleMET_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/PatCandidates/interface/MET.h"

class IIHEMETVariableBase{
public:
  IIHEMETVariableBase(std::string, std::string, int) ;
  ~IIHEMETVariableBase(){} ;

  const std::string       Name(){ return       name_ ; }
  const std::string BranchName(){ return branchName_ ; }
  const int         branchType(){ return branchType_ ; }

  virtual void reset(){} ;
  virtual void store(IIHEAnalysis*){} ;
  bool addBranch(IIHEAnalysis*) ;

private:
  std::string       name_ ;
  std::string branchName_ ;
  int         branchType_ ;
};

class IIHEMETVariableInt: IIHEMETVariableBase{
public:
  IIHEMETVariableInt(std::string, std::string) ;
  ~IIHEMETVariableInt(){} ;
  void reset(){ value_ = -999 ; }
  void fill(int value){ value_ = value ; }
  void store(IIHEAnalysis*) ;
private:
  int value_ ;
};

class IIHEMETVariableFloat: IIHEMETVariableBase{
public:
  IIHEMETVariableFloat(std::string, std::string) ;
  ~IIHEMETVariableFloat(){} ;
  void reset(){ value_ = -999.0 ; }
  void fill(float value){ value_ = value ; }
  void store(IIHEAnalysis*) ;
private:
  float value_ ;
};

class IIHEMETWrapper{
public:
  explicit IIHEMETWrapper(std::string);
  ~IIHEMETWrapper(){};

  void addBranches(IIHEAnalysis*) ;
  void reset() ;
  void fill(pat::MET) ;
  void store(IIHEAnalysis*) ;

  private:
  int type_ ;
  std::string prefix_ ;
  IIHEMETVariableFloat*   Pt_        ;
  IIHEMETVariableFloat*   Px_        ;
  IIHEMETVariableFloat*   Py_        ;
  IIHEMETVariableFloat*   phi_        ;
  IIHEMETVariableFloat*   significance_        ;
  std::vector<IIHEMETVariableBase*> variables_ ;
};



// class decleration
class IIHEModuleMET : public IIHEModule {
public:
  explicit IIHEModuleMET(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleMET(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleMET();
  
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

 edm::EDGetTokenT<edm::View<pat::MET> > pfMETToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetCollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1CollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1JetEnDownCollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1JetEnUpCollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1SmearCollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1SmearJetEnDownCollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1SmearJetEnUpCollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1SmearJetResDownCollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1SmearJetResUpCollectionToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetT1TxyToken_;
 edm::EDGetTokenT<edm::View<pat::MET> > patPFMetFinalCollectionToken_;


  IIHEMETWrapper* metnominalWrapper_;
  IIHEMETWrapper* metWrapper_ ;
  IIHEMETWrapper* metT1Wrapper_;
  IIHEMETWrapper* metT1JetEnDownWrapper_;
  IIHEMETWrapper* metT1JetEnUpWrapper_;
  IIHEMETWrapper* metT1SmearWrapper_;
  IIHEMETWrapper* metT1SmearJetEnDownWrapper_;
  IIHEMETWrapper* metT1SmearJetEnUpWrapper_;
  IIHEMETWrapper* metT1SmearJetResDownWrapper_;
  IIHEMETWrapper* metT1SmearJetResUpWrapper_;
  IIHEMETWrapper* metT1TxyWrapper_;
  IIHEMETWrapper* metFinalWrapper_;

 bool isMC_;
};
#endif
