#ifndef UserCode_IIHETree_IIHEModuleSuperCluster_h
#define UserCode_IIHETree_IIHEModuleSuperCluster_h

#include "UserCode/IIHETree/interface/IIHEModule.h"

// class decleration
class IIHEModuleSuperCluster : public IIHEModule {
private:
  inline float etacorr(float eta, float pvz, float scz){ return asinh(sinh(eta)*(1.0-pvz/scz)) ; }
  edm::InputTag  superClusterCollectionLabel_ ;
  edm::EDGetTokenT<edm::View<reco::SuperCluster>> superClusterCollectionToken_ ;

  edm::EDGetTokenT<View<reco::Vertex>> vtxToken_;
  edm::InputTag           primaryVertexLabel_ ;
public:
  explicit IIHEModuleSuperCluster(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleSuperCluster(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleSuperCluster();
  
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
