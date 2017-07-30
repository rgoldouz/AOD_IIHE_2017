#include "UserCode/IIHETree/interface/IIHEModuleTau.h"

#include <iostream>
#include <TMath.h>
#include <vector>

using namespace std ;
using namespace reco;
using namespace edm ;

IIHEModuleTau::IIHEModuleTau(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC): IIHEModule(iConfig){
  ETThreshold_ = iConfig.getUntrackedParameter<double>("tauPtTThreshold" ) ;
  tauCollectionLabel_     = iConfig.getParameter<edm::InputTag>("tauCollection");
  tauCollectionToken_     = iC.consumes<View<pat::Tau>> (tauCollectionLabel_);
}
IIHEModuleTau::~IIHEModuleTau(){}

// ------------ method called once each job just before starting event loop  ------------
void IIHEModuleTau::beginJob(){

  addBranch("tau_n", kUInt);

  setBranchType(kVectorFloat);
  addBranch("tau_px");
  addBranch("tau_py");
  addBranch("tau_pz");
  addBranch("tau_pt");
  addBranch("tau_eta");
  addBranch("tau_theta");
  addBranch("tau_phi");
  addBranch("tau_energy");
  addBranch("tau_mass");
  addBranch("tau_dxy");
  addBranch("tau_dxy_error");
  addBranch("tau_ptLeadChargedCand");
  addBranch("tau_decayModeFinding");
  addBranch("tau_decayModeFindingNewDMs");
  addBranch("tau_againstMuonLoose3");
  addBranch("tau_againstMuonTight3");
  addBranch("tau_byLooseCombinedIsolationDeltaBetaCorr3Hits");
  addBranch("tau_byMediumCombinedIsolationDeltaBetaCorr3Hits");
  addBranch("tau_byTightCombinedIsolationDeltaBetaCorr3Hits");
  addBranch("tau_byCombinedIsolationDeltaBetaCorrRaw3Hits");
  addBranch("tau_byIsolationMVArun2v1DBoldDMwLTraw");
  addBranch("tau_byVLooseIsolationMVArun2v1DBoldDMwLT");
  addBranch("tau_byLooseIsolationMVArun2v1DBoldDMwLT");
  addBranch("tau_byMediumIsolationMVArun2v1DBoldDMwLT");
  addBranch("tau_byTightIsolationMVArun2v1DBoldDMwLT");
  addBranch("tau_byVTightIsolationMVArun2v1DBoldDMwLT");
  addBranch("tau_byVVTightIsolationMVArun2v1DBoldDMwLT");
  addBranch("tau_byIsolationMVArun2v1DBnewDMwLTraw");
  addBranch("tau_byVLooseIsolationMVArun2v1DBnewDMwLT");
  addBranch("tau_byLooseIsolationMVArun2v1DBnewDMwLT");
  addBranch("tau_byMediumIsolationMVArun2v1DBnewDMwLT");
  addBranch("tau_byTightIsolationMVArun2v1DBnewDMwLT");
  addBranch("tau_byVTightIsolationMVArun2v1DBnewDMwLT");
  addBranch("tau_byVVTightIsolationMVArun2v1DBnewDMwLT");
  addBranch("tau_byIsolationMVArun2v1PWoldDMwLTraw");
  addBranch("tau_byVLooseIsolationMVArun2v1PWoldDMwLT");
  addBranch("tau_byLooseIsolationMVArun2v1PWoldDMwLT");
  addBranch("tau_byMediumIsolationMVArun2v1PWoldDMwLT");
  addBranch("tau_byTightIsolationMVArun2v1PWoldDMwLT");
  addBranch("tau_byVTightIsolationMVArun2v1PWoldDMwLT");
  addBranch("tau_byVVTightIsolationMVArun2v1PWoldDMwLT");
  addBranch("tau_byIsolationMVArun2v1PWnewDMwLTraw");
  addBranch("tau_byVLooseIsolationMVArun2v1PWnewDMwLT");
  addBranch("tau_byLooseIsolationMVArun2v1PWnewDMwLT");
  addBranch("tau_byMediumIsolationMVArun2v1PWnewDMwLT");
  addBranch("tau_byTightIsolationMVArun2v1PWnewDMwLT");
  addBranch("tau_byVTightIsolationMVArun2v1PWnewDMwLT");
  addBranch("tau_byVVTightIsolationMVArun2v1PWnewDMwLT");
  addBranch("tau_byIsolationMVArun2v1DBdR03oldDMwLTraw");
  addBranch("tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT");
  addBranch("tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT");
  addBranch("tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT");
  addBranch("tau_byTightIsolationMVArun2v1DBdR03oldDMwLT");
  addBranch("tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT");
  addBranch("tau_byVVTightIsolationMVArun2v1DBdR03oldDMwLT");
  addBranch("tau_byIsolationMVArun2v1PWdR03oldDMwLTraw");
  addBranch("tau_byVLooseIsolationMVArun2v1PWdR03oldDMwLT");
  addBranch("tau_byLooseIsolationMVArun2v1PWdR03oldDMwLT");
  addBranch("tau_byMediumIsolationMVArun2v1PWdR03oldDMwLT");
  addBranch("tau_byTightIsolationMVArun2v1PWdR03oldDMwLT");
  addBranch("tau_byVTightIsolationMVArun2v1PWdR03oldDMwLT");
  addBranch("tau_byVVTightIsolationMVArun2v1PWdR03oldDMwLT");
  addBranch("tau_againstElectronMVA6Raw");
  addBranch("tau_againstElectronMVA6category");
  addBranch("tau_againstElectronVLooseMVA6");
  addBranch("tau_againstElectronLooseMVA6");
  addBranch("tau_againstElectronMediumMVA6");
  addBranch("tau_againstElectronTightMVA6");
  addBranch("tau_againstElectronVTightMVA6");
  addBranch("tau_mc_bestDR");
  addBranch("tau_mc_ERatio");

  setBranchType(kVectorUInt);
  addBranch("tau_numberOfIsolationChargedHadrCands");
  addBranch("tau_numberOfSignalChargedHadrCands");

  setBranchType(kVectorInt);
  addBranch("tau_mc_index");
  addBranch("tau_decayMode");
  addBranch("tau_charge");

  setBranchType(kVectorBool);
  addBranch("tau_isPFTau");
  addBranch("tau_hasSecondaryVertex");
}

// ------------ method called to for each event  ------------
void IIHEModuleTau::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
  Handle<View<pat::Tau> > tauCollection_ ;
  iEvent.getByToken( tauCollectionToken_, tauCollection_ );

  store("tau_n", (unsigned int) tauCollection_ -> size() );
  for ( unsigned int i = 0; i <tauCollection_->size(); ++i) {
    Ptr<pat::Tau> tauni = tauCollection_->ptrAt( i );
    if(tauni->pt() < ETThreshold_) continue ;

    store("tau_px"    , tauni->px()) ;
    store("tau_py"    , tauni->py()) ;
    store("tau_pz"    , tauni->pz()) ;
    store("tau_pt"    , tauni->pt()) ;
    store("tau_eta"   , tauni->eta()) ;
    store("tau_theta" , tauni->theta()) ;
    store("tau_phi"   , tauni->phi()) ;
    store("tau_energy", tauni->energy()) ;
    store("tau_mass"  , tauni->mass()) ;
    store("tau_dxy"   , tauni->dxy()) ;
    store("tau_dxy_error"         , tauni->dxy_error()) ;
    store("tau_ptLeadChargedCand" , tauni->ptLeadChargedCand()) ;
    store("tau_decayModeFinding"                           , tauni->tauID("decayModeFinding") );
    store("tau_decayModeFindingNewDMs"                     , tauni->tauID("decayModeFindingNewDMs") );
    store("tau_byLooseCombinedIsolationDeltaBetaCorr3Hits" , tauni->tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits")) ;
    store("tau_byMediumCombinedIsolationDeltaBetaCorr3Hits", tauni->tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") );
    store("tau_byTightCombinedIsolationDeltaBetaCorr3Hits" , tauni->tauID("byTightCombinedIsolationDeltaBetaCorr3Hits") );
    store("tau_byCombinedIsolationDeltaBetaCorrRaw3Hits"   , tauni->tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") );
    store("tau_byIsolationMVArun2v1DBoldDMwLTraw"          , tauni->tauID("byIsolationMVArun2v1DBoldDMwLTraw") );
    store("tau_byVLooseIsolationMVArun2v1DBoldDMwLT"       , tauni->tauID("byVLooseIsolationMVArun2v1DBoldDMwLT") );
    store("tau_byLooseIsolationMVArun2v1DBoldDMwLT"        , tauni->tauID("byLooseIsolationMVArun2v1DBoldDMwLT") );
    store("tau_byMediumIsolationMVArun2v1DBoldDMwLT"       , tauni->tauID("byMediumIsolationMVArun2v1DBoldDMwLT") );
    store("tau_byTightIsolationMVArun2v1DBoldDMwLT"        , tauni->tauID("byTightIsolationMVArun2v1DBoldDMwLT") );
    store("tau_byVTightIsolationMVArun2v1DBoldDMwLT"       , tauni->tauID("byVTightIsolationMVArun2v1DBoldDMwLT") );
    store("tau_byVVTightIsolationMVArun2v1DBoldDMwLT"      , tauni->tauID("byVVTightIsolationMVArun2v1DBoldDMwLT") );
    store("tau_byIsolationMVArun2v1DBnewDMwLTraw"          , tauni->tauID("byIsolationMVArun2v1DBnewDMwLTraw") );
    store("tau_byVLooseIsolationMVArun2v1DBnewDMwLT"       , tauni->tauID("byVLooseIsolationMVArun2v1DBnewDMwLT") );
    store("tau_byLooseIsolationMVArun2v1DBnewDMwLT"        , tauni->tauID("byLooseIsolationMVArun2v1DBnewDMwLT") );
    store("tau_byMediumIsolationMVArun2v1DBnewDMwLT"       , tauni->tauID("byMediumIsolationMVArun2v1DBnewDMwLT") );
    store("tau_byTightIsolationMVArun2v1DBnewDMwLT"        , tauni->tauID("byTightIsolationMVArun2v1DBnewDMwLT") );
    store("tau_byVTightIsolationMVArun2v1DBnewDMwLT"       , tauni->tauID("byVTightIsolationMVArun2v1DBnewDMwLT") );
    store("tau_byVVTightIsolationMVArun2v1DBnewDMwLT"      , tauni->tauID("byVVTightIsolationMVArun2v1DBnewDMwLT") );
    store("tau_byIsolationMVArun2v1PWoldDMwLTraw"          , tauni->tauID("byIsolationMVArun2v1PWoldDMwLTraw") );
    store("tau_byVLooseIsolationMVArun2v1PWoldDMwLT"       , tauni->tauID("byVLooseIsolationMVArun2v1PWoldDMwLT") );
    store("tau_byLooseIsolationMVArun2v1PWoldDMwLT"        , tauni->tauID("byLooseIsolationMVArun2v1PWoldDMwLT") );
    store("tau_byMediumIsolationMVArun2v1PWoldDMwLT"       , tauni->tauID("byMediumIsolationMVArun2v1PWoldDMwLT") );
    store("tau_byTightIsolationMVArun2v1PWoldDMwLT"        , tauni->tauID("byTightIsolationMVArun2v1PWoldDMwLT") );
    store("tau_byVTightIsolationMVArun2v1PWoldDMwLT"       , tauni->tauID("byVTightIsolationMVArun2v1PWoldDMwLT") );
    store("tau_byVVTightIsolationMVArun2v1PWoldDMwLT"      , tauni->tauID("byVVTightIsolationMVArun2v1PWoldDMwLT") );
    store("tau_byIsolationMVArun2v1PWnewDMwLTraw"          , tauni->tauID("byIsolationMVArun2v1PWnewDMwLTraw") );
    store("tau_byVLooseIsolationMVArun2v1PWnewDMwLT"       , tauni->tauID("byVLooseIsolationMVArun2v1PWnewDMwLT") );
    store("tau_byLooseIsolationMVArun2v1PWnewDMwLT"        , tauni->tauID("byLooseIsolationMVArun2v1PWnewDMwLT") );
    store("tau_byMediumIsolationMVArun2v1PWnewDMwLT"       , tauni->tauID("byMediumIsolationMVArun2v1PWnewDMwLT") );
    store("tau_byTightIsolationMVArun2v1PWnewDMwLT"        , tauni->tauID("byTightIsolationMVArun2v1PWnewDMwLT") );
    store("tau_byVTightIsolationMVArun2v1PWnewDMwLT"       , tauni->tauID("byVTightIsolationMVArun2v1PWnewDMwLT") );
    store("tau_byVVTightIsolationMVArun2v1PWnewDMwLT"      , tauni->tauID("byVVTightIsolationMVArun2v1PWnewDMwLT") );
    store("tau_byIsolationMVArun2v1DBdR03oldDMwLTraw"      , tauni->tauID("byIsolationMVArun2v1DBdR03oldDMwLTraw") );
    store("tau_byVLooseIsolationMVArun2v1DBdR03oldDMwLT"   , tauni->tauID("byVLooseIsolationMVArun2v1DBdR03oldDMwLT") );
    store("tau_byLooseIsolationMVArun2v1DBdR03oldDMwLT"    , tauni->tauID("byLooseIsolationMVArun2v1DBdR03oldDMwLT") );
    store("tau_byMediumIsolationMVArun2v1DBdR03oldDMwLT"   , tauni->tauID("byMediumIsolationMVArun2v1DBdR03oldDMwLT") );
    store("tau_byTightIsolationMVArun2v1DBdR03oldDMwLT"    , tauni->tauID("byTightIsolationMVArun2v1DBdR03oldDMwLT") );
    store("tau_byVTightIsolationMVArun2v1DBdR03oldDMwLT"   , tauni->tauID("byVTightIsolationMVArun2v1DBdR03oldDMwLT") );
    store("tau_byVVTightIsolationMVArun2v1DBdR03oldDMwLT"  , tauni->tauID("byVVTightIsolationMVArun2v1DBdR03oldDMwLT") );
    store("tau_byIsolationMVArun2v1PWdR03oldDMwLTraw"      , tauni->tauID("byIsolationMVArun2v1PWdR03oldDMwLTraw") );
    store("tau_byVLooseIsolationMVArun2v1PWdR03oldDMwLT"   , tauni->tauID("byVLooseIsolationMVArun2v1PWdR03oldDMwLT") );
    store("tau_byLooseIsolationMVArun2v1PWdR03oldDMwLT"    , tauni->tauID("byLooseIsolationMVArun2v1PWdR03oldDMwLT") );
    store("tau_byMediumIsolationMVArun2v1PWdR03oldDMwLT"   , tauni->tauID("byMediumIsolationMVArun2v1PWdR03oldDMwLT") );
    store("tau_byTightIsolationMVArun2v1PWdR03oldDMwLT"    , tauni->tauID("byTightIsolationMVArun2v1PWdR03oldDMwLT") );
    store("tau_byVTightIsolationMVArun2v1PWdR03oldDMwLT"   , tauni->tauID("byVTightIsolationMVArun2v1PWdR03oldDMwLT") );
    store("tau_byVVTightIsolationMVArun2v1PWdR03oldDMwLT"  , tauni->tauID("byVVTightIsolationMVArun2v1PWdR03oldDMwLT") );
    store("tau_againstMuonLoose3"          , tauni->tauID("againstMuonLoose3") );
    store("tau_againstMuonTight3"          , tauni->tauID("againstMuonTight3") );
    store("tau_againstElectronMVA6Raw"     , tauni->tauID("againstElectronMVA6Raw") );
    store("tau_againstElectronMVA6category", tauni->tauID("againstElectronMVA6category") );
    store("tau_againstElectronVLooseMVA6"  , tauni->tauID("againstElectronVLooseMVA6") );
    store("tau_againstElectronLooseMVA6"   , tauni->tauID("againstElectronLooseMVA6") );
    store("tau_againstElectronMediumMVA6"  , tauni->tauID("againstElectronMediumMVA6") );
    store("tau_againstElectronTightMVA6"   , tauni->tauID("againstElectronTightMVA6") );
    store("tau_againstElectronVTightMVA6"  , tauni->tauID("againstElectronVTightMVA6") );

    store("tau_decayMode"         , tauni->decayMode()) ;
    store("tau_charge"            , tauni->charge()) ;
    store("tau_isPFTau"           , tauni->isPFTau()) ;
    store("tau_hasSecondaryVertex", tauni->hasSecondaryVertex()) ;

    store("tau_numberOfIsolationChargedHadrCands"  , tauni->isolationChargedHadrCands().size());
    store("tau_numberOfSignalChargedHadrCands"     , tauni->signalChargedHadrCands().size());

    // Now apply truth matching.
    int index = MCTruth_matchEtaPhi_getIndex(tauni->eta(), tauni->phi()) ;
    if(index>=0){
      const MCTruthObject* MCTruth = MCTruth_getRecordByIndex(index) ;
      store("tau_mc_bestDR", deltaR(tauni->eta(), tauni->phi(), MCTruth->eta(), MCTruth->phi())) ;
      store("tau_mc_index" , index) ;
      store("tau_mc_ERatio", tauni->energy()/MCTruth->energy()) ;
    }
    else{
      store("tau_mc_bestDR", 999.0) ;
      store("tau_mc_index" ,    -1) ;
      store("tau_mc_ERatio", 999.0) ;
    }

  }
}
void IIHEModuleTau::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup){}
void IIHEModuleTau::beginEvent(){}
void IIHEModuleTau::endEvent(){}


// ------------ method called once each job just after ending the event loop  ------------
void IIHEModuleTau::endJob(){
}

DEFINE_FWK_MODULE(IIHEModuleTau);
