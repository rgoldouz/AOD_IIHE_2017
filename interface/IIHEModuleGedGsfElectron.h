#ifndef UserCode_IIHETree_IIHEModuleGedGsfElectron_h
#define UserCode_IIHETree_IIHEModuleGedGsfElectron_h

#include "UserCode/IIHETree/interface/IIHEModule.h"
#include "UserCode/IIHETree/interface/MiniAODHelper.h"
// class decleration
class IIHEModuleGedGsfElectron : public IIHEModule {
private:
  inline float etacorr(float eta, float pvz, float scz){ return asinh(sinh(eta)*(1.0-pvz/scz)) ; }
  edm::EDGetTokenT<EcalRecHitCollection>        ebReducedRecHitCollection_;
  edm::EDGetTokenT<EcalRecHitCollection>        eeReducedRecHitCollection_;
  edm::EDGetTokenT<EcalRecHitCollection>        esReducedRecHitCollection_;
  edm::EDGetTokenT<reco::TrackCollection>       generalTracksToken_;
  edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_ ;
  edm::EDGetTokenT<double> rhoTokenAll_; 
  edm::EDGetTokenT<edm::ValueMap<float> > eleTrkPtIso_;
  edm::EDGetTokenT<edm::ValueMap<bool> > VIDVeto_;
  edm::EDGetTokenT<edm::ValueMap<bool> > VIDLoose_;
  edm::EDGetTokenT<edm::ValueMap<bool> > VIDMedium_;
  edm::EDGetTokenT<edm::ValueMap<bool> > VIDTight_;
  edm::EDGetTokenT<edm::ValueMap<bool> > VIDmvaEleIDwp90_;
  edm::EDGetTokenT<edm::ValueMap<bool> > VIDmvaEleIDwp80_;
  edm::EDGetTokenT<edm::ValueMap<bool> > VIDHEEP7_;

  edm::EDGetTokenT<edm::View<reco::GsfElectron>> electronCollectionToken_;
  edm::EDGetTokenT<edm::View<reco::GsfElectron>> electronCollectionToken80_;
  edm::InputTag      electronCollectionLabel_ ;
  edm::EDGetTokenT<View<reco::Vertex>> vtxToken_;
  edm::InputTag           primaryVertexLabel_ ;
  float ETThreshold_ ;

public:
  explicit IIHEModuleGedGsfElectron(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
  explicit IIHEModuleGedGsfElectron(const edm::ParameterSet& iConfig): IIHEModule(iConfig){};
  ~IIHEModuleGedGsfElectron() ;
  
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
