#ifndef UserCode_IIHETree_IIHEModuleLHEWeight_h
#define UserCode_IIHETree_IIHEModuleLHEWeight_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"

// class decleration
class IIHEModuleLHEWeight : public IIHEModule {
public:
  explicit IIHEModuleLHEWeight(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleLHEWeight(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleLHEWeight();
  
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
  edm::EDGetTokenT<LHEEventProduct> lheEventLabel_;
};
#endif
