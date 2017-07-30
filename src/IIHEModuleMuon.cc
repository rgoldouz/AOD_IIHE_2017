#include "UserCode/IIHETree/interface/IIHEModuleMuon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include <TRandom3.h>
#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

//////////////////////////////////////////////////////////////////////////////////////////
//                             IIHEMuonTrackVariable classes                            //
//////////////////////////////////////////////////////////////////////////////////////////
IIHEMuonTrackVariableBase::IIHEMuonTrackVariableBase(std::string prefix, std::string name, int type){
  name_       = name ;
  branchName_ = prefix + "_" + name_ ;
  branchType_ = type ;
}
bool IIHEMuonTrackVariableBase::addBranch(IIHEAnalysis* analysis){
  return analysis->addBranch(branchName_, branchType_) ;
}

IIHEMuonTrackVariableInt::IIHEMuonTrackVariableInt(std::string prefix, std::string name):
IIHEMuonTrackVariableBase(prefix, name, kVectorInt){
  reset() ;
}
void IIHEMuonTrackVariableInt::store(IIHEAnalysis* analysis){
  analysis->store(BranchName(), value_) ;
}

IIHEMuonTrackVariableFloat::IIHEMuonTrackVariableFloat(std::string prefix, std::string name):
IIHEMuonTrackVariableBase(prefix, name, kVectorFloat){
  reset() ;
}
void IIHEMuonTrackVariableFloat::store(IIHEAnalysis* analysis){
  analysis->store(BranchName(), value_ ) ;
}

//////////////////////////////////////////////////////////////////////////////////////////
//                                  IIHEMuonTrack class                                 //
//////////////////////////////////////////////////////////////////////////////////////////
IIHEMuonTrackWrapper::IIHEMuonTrackWrapper(std::string prefix){
  prefix_ = prefix ;
  
  charge_         = new IIHEMuonTrackVariableInt  (prefix_, "charge"        ) ;
  qoverp_         = new IIHEMuonTrackVariableFloat(prefix_, "qoverp"        ) ;
  pt_             = new IIHEMuonTrackVariableFloat(prefix_, "pt"            ) ;
  eta_            = new IIHEMuonTrackVariableFloat(prefix_, "eta"           ) ;
  phi_            = new IIHEMuonTrackVariableFloat(prefix_, "phi"           ) ;
  p_              = new IIHEMuonTrackVariableFloat(prefix_, "p"             ) ;
  px_             = new IIHEMuonTrackVariableFloat(prefix_, "px"            ) ;
  py_             = new IIHEMuonTrackVariableFloat(prefix_, "py"            ) ;
  pz_             = new IIHEMuonTrackVariableFloat(prefix_, "pz"            ) ;
  theta_          = new IIHEMuonTrackVariableFloat(prefix_, "theta"         ) ;
  lambda_         = new IIHEMuonTrackVariableFloat(prefix_, "lambda"        ) ;
  d0_             = new IIHEMuonTrackVariableFloat(prefix_, "d0"            ) ;
  dz_             = new IIHEMuonTrackVariableFloat(prefix_, "dz"            ) ;
  dz_beamspot_    = new IIHEMuonTrackVariableFloat(prefix_, "dz_beamspot"   ) ;
  dz_firstPVtx_   = new IIHEMuonTrackVariableFloat(prefix_, "dz_firstPVtx"  ) ;
  dxy_            = new IIHEMuonTrackVariableFloat(prefix_, "dxy"           ) ;
  dxy_beamspot_   = new IIHEMuonTrackVariableFloat(prefix_, "dxy_beamspot"  ) ;
  dxy_firstPVtx_  = new IIHEMuonTrackVariableFloat(prefix_, "dxy_firstPVtx" ) ;
  dsz_            = new IIHEMuonTrackVariableFloat(prefix_, "dsz"           ) ;
  vx_             = new IIHEMuonTrackVariableFloat(prefix_, "vx"            ) ;
  vy_             = new IIHEMuonTrackVariableFloat(prefix_, "vy"            ) ;
  vz_             = new IIHEMuonTrackVariableFloat(prefix_, "vz"            ) ;
  qoverpError_    = new IIHEMuonTrackVariableFloat(prefix_, "qoverpError"   ) ;
  ptError_        = new IIHEMuonTrackVariableFloat(prefix_, "ptError"       ) ;
  thetaError_     = new IIHEMuonTrackVariableFloat(prefix_, "thetaError"    ) ;
  lambdaError_    = new IIHEMuonTrackVariableFloat(prefix_, "lambdaError"   ) ;
  phiError_       = new IIHEMuonTrackVariableFloat(prefix_, "phiError"      ) ;
  dxyError_       = new IIHEMuonTrackVariableFloat(prefix_, "dxyError"      ) ;
  d0Error_        = new IIHEMuonTrackVariableFloat(prefix_, "d0Error"       ) ;
  dszError_       = new IIHEMuonTrackVariableFloat(prefix_, "dszError"      ) ;
  dzError_        = new IIHEMuonTrackVariableFloat(prefix_, "dzError"       ) ;
  etaError_       = new IIHEMuonTrackVariableFloat(prefix_, "etaError"      ) ;
  chi2_           = new IIHEMuonTrackVariableFloat(prefix_, "chi2"          ) ;
  ndof_           = new IIHEMuonTrackVariableFloat(prefix_, "ndof"          ) ;
  normalizedChi2_ = new IIHEMuonTrackVariableFloat(prefix_, "normalizedChi2") ;
      
  variables_.push_back((IIHEMuonTrackVariableBase*) qoverp_        ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) charge_        ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) pt_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) eta_           ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) phi_           ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) p_             ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) px_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) py_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) pz_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) theta_         ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) lambda_        ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) d0_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dz_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dz_beamspot_   ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dz_firstPVtx_  ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dxy_           ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dxy_beamspot_  ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dxy_firstPVtx_ ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dsz_           ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) vx_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) vy_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) vz_            ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) qoverpError_   ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) ptError_       ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) thetaError_    ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) lambdaError_   ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) phiError_      ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dxyError_      ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) d0Error_       ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dszError_      ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) dzError_       ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) etaError_      ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) chi2_          ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) ndof_          ) ;
  variables_.push_back((IIHEMuonTrackVariableBase*) normalizedChi2_) ;
}

void IIHEMuonTrackWrapper::addBranches(IIHEAnalysis* analysis){
  for(unsigned int i=0 ; i<variables_.size() ; ++i){
    variables_.at(i)->addBranch(analysis) ;
  }
}
void IIHEMuonTrackWrapper::reset(){
  for(unsigned int i=0 ; i<variables_.size() ; ++i){
    variables_.at(i)->reset() ;
  }
}
void IIHEMuonTrackWrapper::fill(TrackRef& track, math::XYZPoint beamspot, math::XYZPoint firstPrimaryVertex){
  float etaError = track->thetaError()/sin(track->theta()) ;

  charge_        ->fill(track->charge()                ) ;
  qoverp_        ->fill(track->qoverp()                ) ;
  pt_            ->fill(track->pt()                    ) ;
  eta_           ->fill(track->eta()                   ) ;
  phi_           ->fill(track->phi()                   ) ;
  p_             ->fill(track->p()                     ) ;
  px_            ->fill(track->px()                    ) ;
  py_            ->fill(track->py()                    ) ;
  pz_            ->fill(track->pz()                    ) ;
  theta_         ->fill(track->theta()                 ) ;
  lambda_        ->fill(track->lambda()                ) ;
  d0_            ->fill(track->d0()                    ) ;
  dz_            ->fill(track->dz()                    ) ;
  dz_beamspot_   ->fill(track->dz(beamspot)           ) ;
  dz_firstPVtx_  ->fill(track->dz(firstPrimaryVertex) ) ;
  dxy_           ->fill(track->dxy()                   ) ;
  dxy_beamspot_  ->fill(track->dxy(beamspot)          ) ;
  dxy_firstPVtx_ ->fill(track->dxy(firstPrimaryVertex)) ;
  dsz_           ->fill(track->dsz(beamspot)          ) ;
  vx_            ->fill(track->vx()                    ) ;
  vy_            ->fill(track->vy()                    ) ;
  vz_            ->fill(track->vz()                    ) ;
  ptError_       ->fill(track->ptError()               ) ;
  thetaError_    ->fill(track->thetaError()            ) ;
  lambdaError_   ->fill(track->lambdaError()           ) ;
  phiError_      ->fill(track->phiError()              ) ;
  dxyError_      ->fill(track->dxyError()              ) ;
  d0Error_       ->fill(track->d0Error()               ) ;
  dszError_      ->fill(track->dszError()              ) ;
  dzError_       ->fill(track->dzError()               ) ;
  etaError_      ->fill(etaError                       ) ;
  chi2_          ->fill(track->chi2()                  ) ;
  ndof_          ->fill(track->ndof()                  ) ;
  normalizedChi2_->fill(track->normalizedChi2()        ) ;

}
void IIHEMuonTrackWrapper::store(IIHEAnalysis* analysis){
  for(unsigned int i=0 ; i<variables_.size() ; ++i){
    variables_.at(i)->store(analysis) ;
  }
}



//////////////////////////////////////////////////////////////////////////////////////////
//                                  Main IIHEMuonModule                                 //
//////////////////////////////////////////////////////////////////////////////////////////
IIHEModuleMuon::IIHEModuleMuon(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC):
  IIHEModule(iConfig),
  globalTrackWrapper_(new IIHEMuonTrackWrapper("mu_gt")),
  outerTrackWrapper_ (new IIHEMuonTrackWrapper("mu_ot")),
  innerTrackWrapper_ (new IIHEMuonTrackWrapper("mu_it")),
  improvedMuonBestTrackWrapper_ (new IIHEMuonTrackWrapper("mu_ibt")){
  
  storeGlobalTrackMuons_ = iConfig.getUntrackedParameter<bool>("storeGlobalTrackMuons", true ) ;
  storeStandAloneMuons_  = iConfig.getUntrackedParameter<bool>("storeStandAloneMuons" , true ) ;
  storeInnerTrackMuons_  = iConfig.getUntrackedParameter<bool>("storeInnerTrackMuons" , true ) ;
  storeImprovedMuonBestTrackMuons_  = iConfig.getUntrackedParameter<bool>("storeImprovedMuonBestTrackMuons" , true ) ;
  ETThreshold_          = iConfig.getUntrackedParameter<double>("muonPtThreshold") ;

  primaryVertexLabel_          = iConfig.getParameter<edm::InputTag>("primaryVertex") ;
  vtxToken_ = iC.consumes<View<reco::Vertex>>(primaryVertexLabel_);
  beamSpotToken_      = iC.consumes<reco::BeamSpot>(iConfig.getParameter<InputTag>("beamSpot")) ;
  muonCollectionLabel_         = iConfig.getParameter<edm::InputTag>("muonCollection"          ) ;
  muonCollectionToken_ =  iC.consumes<View<reco::Muon> > (muonCollectionLabel_);
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC") ;
}
IIHEModuleMuon::~IIHEModuleMuon(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleMuon::beginJob(){
  setBranchType(kUInt) ;
  addBranch("mu_n"   ) ;
  IIHEAnalysis* analysis = parent_ ;
  if(storeGlobalTrackMuons_) globalTrackWrapper_->addBranches(analysis) ;
  if(storeStandAloneMuons_ )  outerTrackWrapper_->addBranches(analysis) ;
  if(storeInnerTrackMuons_ )  innerTrackWrapper_->addBranches(analysis) ;
  if(storeImprovedMuonBestTrackMuons_ )  improvedMuonBestTrackWrapper_->addBranches(analysis) ;
  
  // Muon type block
  setBranchType(kVectorBool) ;
  addBranch("mu_isGlobalMuon"      ) ;
  addBranch("mu_isStandAloneMuon"  ) ;
  addBranch("mu_isTrackerMuon"     ) ;
  addBranch("mu_isPFMuon"          ) ;
  addBranch("mu_isPFIsolationValid") ;
  addBranch("mu_isGoodMuonTMLastStationLoose" ) ;
  addBranch("mu_isGoodMuonTMLastStationTight" ) ;
  addBranch("mu_isGoodMuonTM2DComrecoibilityLoose") ;
  addBranch("mu_isGoodMuonTM2DComrecoibilityTight" ) ;
  addBranch("mu_isGoodMuonTMOneStationLoose"  ) ;
  addBranch("mu_isGoodMuonTMOneStationTight"  ) ;
  addBranch("mu_isGoodMuonTMLastStationOptimizedLowPtLoose" ) ;
  addBranch("mu_isGoodMuonTMLastStationOptimizedLowPtTight" ) ;
  addBranch("mu_isTightMuon"     ) ;
  addBranch("mu_isMediumMuon"     ) ;
  addBranch("mu_isLooseMuon"     ) ;
  addBranch("mu_isSoftMuon"     ) ;
  addBranch("mu_isHighPtMuon"     ) ;

  
  // Hits block
  setBranchType(kVectorInt) ;
  addBranch("mu_numberOfMatchedStations" ) ;
  addBranch("mu_numberOfValidPixelHits"  ) ;
  addBranch("mu_trackerLayersWithMeasurement") ;
  addBranch("mu_numberOfValidMuonHits"   ) ;
  addBranch("mu_pixelLayersWithMeasurement") ;

  // Isolation block
  setBranchType(kVectorFloat) ;
  addBranch("mu_innerTrack_validFraction"        ) ;
  addBranch("mu_combinedQuality_trkKink"        ) ;
  addBranch("mu_combinedQuality_chi2LocalPosition"        ) ;
  addBranch("mu_segmentComrecoibility"        ) ;
  addBranch("mu_dB"        ) ;

  addBranch("mu_isolationR03_sumPt"        ) ;
  addBranch("mu_isolationR03_trackerVetoPt") ;
  addBranch("mu_isolationR03_emEt"         ) ;
  addBranch("mu_isolationR03_emVetoEt"     ) ;
  addBranch("mu_isolationR03_hadEt"        ) ;
  addBranch("mu_isolationR03_hadVetoEt"    ) ;
  
  addBranch("mu_isolationR05_sumPt"        ) ;
  addBranch("mu_isolationR05_trackerVetoPt") ;
  addBranch("mu_isolationR05_emEt"         ) ;
  addBranch("mu_isolationR05_emVetoEt"     ) ;
  addBranch("mu_isolationR05_hadEt"        ) ;
  addBranch("mu_isolationR05_hadVetoEt"    ) ;
  
  addBranch("mu_pfIsolationR03_sumChargedHadronPt"             ) ;
  addBranch("mu_pfIsolationR03_sumChargedParticlePt"           ) ;
  addBranch("mu_pfIsolationR03_sumPhotonEt"                    ) ;
  addBranch("mu_pfIsolationR03_sumNeutralHadronEtHighThreshold") ;
  addBranch("mu_pfIsolationR03_sumPhotonEtHighThreshold"       ) ;
  addBranch("mu_pfIsolationR03_sumPUPt"                        ) ;
  
  addBranch("mu_pfIsolationR04_sumChargedHadronPt"             ) ;
  addBranch("mu_pfIsolationR04_sumChargedParticlePt"           ) ;
  addBranch("mu_pfIsolationR04_sumPhotonEt"                    ) ;
  addBranch("mu_pfIsolationR04_sumNeutralHadronEtHighThreshold") ;
  addBranch("mu_pfIsolationR04_sumPhotonEtHighThreshold"       ) ;
  addBranch("mu_pfIsolationR04_sumPUPt"                        ) ;

/*  
  addBranch("mu_pfMeanDRIsoProfileR03_sumChargedHadronPt"             ) ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumChargedParticlePt"           ) ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumPhotonEt"                    ) ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumNeutralHadronEtHighThreshold") ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumPhotonEtHighThreshold"       ) ;
  addBranch("mu_pfMeanDRIsoProfileR03_sumPUPt"                        ) ;
  
  addBranch("mu_pfMeanDRIsoProfileR04_sumChargedHadronPt"             ) ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumChargedParticlePt"           ) ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumPhotonEt"                    ) ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumNeutralHadronEtHighThreshold") ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumPhotonEtHighThreshold"       ) ;
  addBranch("mu_pfMeanDRIsoProfileR04_sumPUPt"                        ) ;
*/
  addBranch("mu_pfIsoDbCorrected03"             ) ;
  addBranch("mu_pfIsoDbCorrected04"             ) ;
  addBranch("mu_isoTrackerBased03"             ) ;
 
  if (isMC_){ 
  addBranch("mu_mc_bestDR", kVectorFloat) ;
  addBranch("mu_mc_index" , kVectorInt  ) ;
  addBranch("mu_mc_ERatio", kVectorFloat) ;
  }
}

// ------------ method called to for each event  ------------
void IIHEModuleMuon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  edm::Handle<View<reco::Vertex> > pvCollection_ ;
  iEvent.getByToken( vtxToken_ , pvCollection_);
  edm::Ptr<reco::Vertex> firstpvertex = pvCollection_->ptrAt( 0 );

  edm::Handle<edm::View<reco::Muon> > muonCollection_;
  iEvent.getByToken( muonCollectionToken_, muonCollection_) ;

  edm::Handle<reco::BeamSpot> beamspotHandle_ ;
  iEvent.getByToken(beamSpotToken_, beamspotHandle_) ;

  store("mu_n", (unsigned int)(muonCollection_->size())) ;
  // Muons come with four tracks:
  //   Standalone track.  This is made from the outer detector
  //   Inner track.  This is made from the tracking system
  //   Global track.  This is made from a combination of the inner and outer tracks.
  //   muonBestTrack. This is the best reconstruction of the muon track parameters for high-pt muons
  // So we need to be a little careful when we get the variables.
  
  IIHEAnalysis* analysis = parent_ ;
  
  for( unsigned int i = 0 ; i < muonCollection_->size() ; i++ ) {
    Ptr<reco::Muon> muIt = muonCollection_->ptrAt( i );

    bool isGlobalMuon     = muIt->isGlobalMuon()     ;
    bool isStandAloneMuon = muIt->isStandAloneMuon() ;
    bool isTrackerMuon    = muIt->isTrackerMuon()    ;
    
    // Try to save some disk space and CPU time
    bool storeThisMuon = false ;
    if(isGlobalMuon     && storeGlobalTrackMuons_) storeThisMuon = true ;
    if(isStandAloneMuon && storeStandAloneMuons_ ) storeThisMuon = true ;
    if(isTrackerMuon    && storeInnerTrackMuons_ ) storeThisMuon = true ;
    if(storeThisMuon==false) continue ;

    reco::TrackRef globalTrack = muIt->globalTrack() ;
    reco::TrackRef  outerTrack = muIt->outerTrack()  ;
    reco::TrackRef  innerTrack = muIt->innerTrack()  ;
    reco::TrackRef  improvedMuonBestTrack = muIt->tunePMuonBestTrack();

    bool saveMuon = false ;
    if(storeInnerTrackMuons_){
      if( innerTrack.isNonnull() && muIt->   isTrackerMuon()){
        if(innerTrack->pt() > ETThreshold_) saveMuon = true ;
      }
    }
    if(storeStandAloneMuons_){
      if( outerTrack.isNonnull() && muIt->isStandAloneMuon()){
        if(outerTrack->pt() > ETThreshold_) saveMuon = true ;
      }
    }
    if(storeGlobalTrackMuons_){
      if(globalTrack.isNonnull() && muIt->    isGlobalMuon()){
        if(globalTrack->pt() > ETThreshold_ || improvedMuonBestTrack->pt() > ETThreshold_) saveMuon = true ;
      }
    }
    if(saveMuon==false) continue ;

 
    store("mu_isGlobalMuon"      , isGlobalMuon              ) ;
    store("mu_isStandAloneMuon"  , isStandAloneMuon          ) ;
    store("mu_isTrackerMuon"     , isTrackerMuon             ) ;
    store("mu_isPFMuon"          , muIt->isPFMuon()          ) ;
    store("mu_isPFIsolationValid", muIt->isPFIsolationValid()) ;  
    store("mu_isGoodMuonTMLastStationLoose"        , muon::isGoodMuon(*muIt,muon::TMLastStationLoose  )) ;
    store("mu_isGoodMuonTMLastStationTight"        , muon::isGoodMuon(*muIt,muon::TMLastStationTight  )) ;
//    store("mu_isGoodMuonTM2DComrecoibilityLoose"        , muon::isGoodMuon(*muIt,muon::TM2DComrecoibilityLoose  )) ;
//    store("mu_isGoodMuonTM2DComrecoibilityTight"        , muon::isGoodMuon(*muIt,muon::TM2DComrecoibilityTight  )) ;
    store("mu_isGoodMuonTMOneStationLoose"        , muon::isGoodMuon(*muIt,muon::TMOneStationLoose  )) ;
    store("mu_isGoodMuonTMOneStationTight"        , muon::isGoodMuon(*muIt,muon::TMOneStationTight  )) ;
    store("mu_isGoodMuonTMLastStationOptimizedLowPtLoose"        , muon::isGoodMuon(*muIt,muon::TMLastStationOptimizedLowPtLoose  )) ;
    store("mu_isGoodMuonTMLastStationOptimizedLowPtTight"        , muon::isGoodMuon(*muIt,muon::TMLastStationOptimizedLowPtTight  )) ;
    store("mu_isTightMuon"       , muon::isTightMuon(*muIt,*pvCollection_->begin())    ) ;
    store("mu_isMediumMuon"      , muon::isMediumMuon(*muIt)    ) ;
    store("mu_isLooseMuon"       , muon::isLooseMuon(*muIt)    ) ;
    store("mu_isSoftMuon"        , muon::isSoftMuon(*muIt,*pvCollection_->begin())  ) ;
    store("mu_isHighPtMuon"      , muon::isHighPtMuon(*muIt,*pvCollection_->begin())    ) ;

    int numberOfMatchStations    = 0 ;
    int numberOfValidPixelHits   = 0 ;
    int trackerLayersWithMeasurement = 0 ;
    int pixelLayersWithMeasurement = 0;
    int numberOfValidMuonHits    = 0 ;
    
    numberOfMatchStations = muIt->numberOfMatchedStations() ;
    if(isTrackerMuon){
      numberOfValidPixelHits   = muIt->innerTrack()->hitPattern().numberOfValidPixelHits() ;
      trackerLayersWithMeasurement = muIt->innerTrack()->hitPattern().trackerLayersWithMeasurement() ;
      pixelLayersWithMeasurement = muIt->innerTrack()->hitPattern().pixelLayersWithMeasurement();
    }
    
    if(isGlobalMuon){
      numberOfValidMuonHits = muIt->globalTrack()->hitPattern().numberOfValidMuonHits() ;
    }
    
    store("mu_numberOfMatchedStations"           , numberOfMatchStations   ) ;
    store("mu_numberOfValidPixelHits"            , numberOfValidPixelHits  ) ;
    store("mu_trackerLayersWithMeasurement"      , trackerLayersWithMeasurement) ;
    store("mu_pixelLayersWithMeasurement"        , pixelLayersWithMeasurement);
    store("mu_numberOfValidMuonHits"             , numberOfValidMuonHits   ) ;

    if (innerTrack.isNonnull())    store("mu_innerTrack_validFraction",muIt->innerTrack()->validFraction()        ) ;
    else store("mu_innerTrack_validFraction", -999.0        ) ;
    store("mu_combinedQuality_trkKink" ,muIt->combinedQuality().trkKink        ) ;
    store("mu_combinedQuality_chi2LocalPosition",muIt->combinedQuality().chi2LocalPosition        ) ;
//    store("mu_segmentComrecoibility" ,muon::segmentComrecoibility(*muIt)       ) ;
//    store("mu_dB" , muIt->dB()       ) ;


    
    globalTrackWrapper_->reset() ;
    outerTrackWrapper_ ->reset() ;
    innerTrackWrapper_ ->reset() ;
    improvedMuonBestTrackWrapper_ ->reset() ;
    
    if(storeInnerTrackMuons_){
      if( innerTrack.isNonnull() && muIt->   isTrackerMuon()){
        innerTrackWrapper_->fill( innerTrack, beamspotHandle_->position(), firstpvertex->position()) ;
      }
      innerTrackWrapper_ ->store(analysis) ;
    }
    if(storeStandAloneMuons_){
      if( outerTrack.isNonnull() && muIt->isStandAloneMuon()){
        outerTrackWrapper_->fill( outerTrack, beamspotHandle_->position(), firstpvertex->position()) ; 
      }
      outerTrackWrapper_ ->store(analysis) ;
    }
    if(storeGlobalTrackMuons_){
      if(globalTrack.isNonnull() && muIt->    isGlobalMuon()){
        globalTrackWrapper_->fill(globalTrack, beamspotHandle_->position(), firstpvertex->position()) ;
      }
      globalTrackWrapper_->store(analysis) ;
    }
    if(storeImprovedMuonBestTrackMuons_){
      if( globalTrack.isNonnull() && muIt->    isGlobalMuon()){
        improvedMuonBestTrackWrapper_->fill( improvedMuonBestTrack, beamspotHandle_->position(), firstpvertex->position()) ;
      }
      improvedMuonBestTrackWrapper_ ->store(analysis) ;
    }
   
    // Isolation variables
    const MuonIsolation   iso30       = muIt->isolationR03() ;
    const MuonIsolation   iso50       = muIt->isolationR05() ;
    const MuonPFIsolation pfIso30     = muIt->pfIsolationR03() ;
    const MuonPFIsolation pfIso40     = muIt->pfIsolationR04() ;
    
    store("mu_isolationR03_sumPt"        , iso30.sumPt        ) ;
    store("mu_isolationR03_trackerVetoPt", iso30.trackerVetoPt) ;
    store("mu_isolationR03_emEt"         , iso30.emEt         ) ;
    store("mu_isolationR03_emVetoEt"     , iso30.emVetoEt     ) ;
    store("mu_isolationR03_hadEt"        , iso30.hadEt        ) ;
    store("mu_isolationR03_hadVetoEt"    , iso30.hadVetoEt    ) ;
    
    store("mu_isolationR05_sumPt"        , iso50.sumPt        ) ;
    store("mu_isolationR05_trackerVetoPt", iso50.trackerVetoPt) ;
    store("mu_isolationR05_emEt"         , iso50.emEt         ) ;
    store("mu_isolationR05_emVetoEt"     , iso50.emVetoEt     ) ;
    store("mu_isolationR05_hadEt"        , iso50.hadEt        ) ;
    store("mu_isolationR05_hadVetoEt"    , iso50.hadVetoEt    ) ;
    
    store("mu_pfIsolationR03_sumChargedHadronPt"             , pfIso30.sumChargedHadronPt             ) ;
    store("mu_pfIsolationR03_sumChargedParticlePt"           , pfIso30.sumChargedParticlePt           ) ;
    store("mu_pfIsolationR03_sumPhotonEt"                    , pfIso30.sumPhotonEt                    ) ;
    store("mu_pfIsolationR03_sumNeutralHadronEtHighThreshold", pfIso30.sumNeutralHadronEtHighThreshold) ;
    store("mu_pfIsolationR03_sumPhotonEtHighThreshold"       , pfIso30.sumPhotonEtHighThreshold       ) ;
    store("mu_pfIsolationR03_sumPUPt"                        , pfIso30.sumPUPt                        ) ;
    
    store("mu_pfIsolationR04_sumChargedHadronPt"             , pfIso40.sumChargedHadronPt             ) ;
    store("mu_pfIsolationR04_sumChargedParticlePt"           , pfIso40.sumChargedParticlePt           ) ;
    store("mu_pfIsolationR04_sumPhotonEt"                    , pfIso40.sumPhotonEt                    ) ;
    store("mu_pfIsolationR04_sumNeutralHadronEtHighThreshold", pfIso40.sumNeutralHadronEtHighThreshold) ;
    store("mu_pfIsolationR04_sumPhotonEtHighThreshold"       , pfIso40.sumPhotonEtHighThreshold       ) ;
    store("mu_pfIsolationR04_sumPUPt"                        , pfIso40.sumPUPt                        ) ;
/*
    const MuonPFIsolation pfMeanIso30 = muIt->pfMeanDRIsoProfileR03() ;
    const MuonPFIsolation pfMeanIso40 = muIt->pfMeanDRIsoProfileR04() ;

    store("mu_pfMeanDRIsoProfileR03_sumChargedHadronPt"             , pfMeanIso30.sumChargedHadronPt             ) ;
    store("mu_pfMeanDRIsoProfileR03_sumChargedParticlePt"           , pfMeanIso30.sumChargedParticlePt           ) ;
    store("mu_pfMeanDRIsoProfileR03_sumPhotonEt"                    , pfMeanIso30.sumPhotonEt                    ) ;
    store("mu_pfMeanDRIsoProfileR03_sumNeutralHadronEtHighThreshold", pfMeanIso30.sumNeutralHadronEtHighThreshold) ;
    store("mu_pfMeanDRIsoProfileR03_sumPhotonEtHighThreshold"       , pfMeanIso30.sumPhotonEtHighThreshold       ) ;
    store("mu_pfMeanDRIsoProfileR03_sumPUPt"                        , pfMeanIso30.sumPUPt                        ) ;
    
    store("mu_pfMeanDRIsoProfileR04_sumChargedHadronPt"             , pfMeanIso40.sumChargedHadronPt             ) ;
    store("mu_pfMeanDRIsoProfileR04_sumChargedParticlePt"           , pfMeanIso40.sumChargedParticlePt           ) ;
    store("mu_pfMeanDRIsoProfileR04_sumPhotonEt"                    , pfMeanIso40.sumPhotonEt                    ) ;
    store("mu_pfMeanDRIsoProfileR04_sumNeutralHadronEtHighThreshold", pfMeanIso40.sumNeutralHadronEtHighThreshold) ;
    store("mu_pfMeanDRIsoProfileR04_sumPhotonEtHighThreshold"       , pfMeanIso40.sumPhotonEtHighThreshold       ) ;
    store("mu_pfMeanDRIsoProfileR04_sumPUPt"                        , pfMeanIso40.sumPUPt                        ) ;
*/   
    store("mu_pfIsoDbCorrected03" , (muIt->pfIsolationR03().sumChargedHadronPt + max(0., muIt->pfIsolationR03().sumNeutralHadronEt + muIt->pfIsolationR03().sumPhotonEt - 0.5*muIt->pfIsolationR03().sumPUPt))/muIt->pt()          ) ;
    store("mu_pfIsoDbCorrected04" , (muIt->pfIsolationR04().sumChargedHadronPt + max(0., muIt->pfIsolationR04().sumNeutralHadronEt + muIt->pfIsolationR04().sumPhotonEt - 0.5*muIt->pfIsolationR04().sumPUPt))/muIt->pt()         ) ;
    store("mu_isoTrackerBased03"  , muIt->isolationR03().sumPt/muIt->pt()          ) ;

    if (isMC_){ 
    // Now apply truth matching.
      int index = MCTruth_matchEtaPhi_getIndex(muIt->eta(), muIt->phi()) ;
      if(index>=0){
        const MCTruthObject* MCTruth = MCTruth_getRecordByIndex(index) ;
        store("mu_mc_bestDR", deltaR(muIt->eta(), muIt->phi(), MCTruth->eta(), MCTruth->phi())) ;
        store("mu_mc_index" , index) ;
        store("mu_mc_ERatio", muIt->energy()/MCTruth->energy()) ;
      } 
      if(index<=0){
        store("mu_mc_bestDR", 999.0) ;
        store("mu_mc_index" ,    -1) ;
        store("mu_mc_ERatio", 999.0) ;
      }
    }
  }
}

void IIHEModuleMuon::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleMuon::beginEvent(){}
void IIHEModuleMuon::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleMuon::endJob(){}

DEFINE_FWK_MODULE(IIHEModuleMuon);
