#ifndef UserCode_IIHETree_IIHEModuleVertex_h
#define UserCode_IIHETree_IIHEModuleVertex_h

#include "UserCode/IIHETree/interface/IIHEModule.h"

// class decleration
class IIHEModuleVertex : public IIHEModule {
private:
  edm::InputTag           primaryVertexLabel_ ;
  edm::EDGetTokenT<View<reco::Vertex>> vtxToken_;
public:
  explicit IIHEModuleVertex(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleVertex(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleVertex();
  
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
};
#endif
