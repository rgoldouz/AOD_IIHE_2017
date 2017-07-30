#include "../interface/MiniAODHelper.h"

#include "FWCore/Utilities/interface/Exception.h"

using namespace std;

// Constructor
MiniAODHelper::MiniAODHelper() 
{
  isSetUp = false;

  vertexIsSet = false;
  rhoIsSet = false;
  factorizedjetcorrectorIsSet = false;

  // twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagging#Preliminary_working_or_operating
  // Preliminary working (or operating) points for CSVv2+IVF
  CSVLwp = 0.5426; // 0.460;//CSVv2 0.423; // 10.1716% DUSG mistag efficiency
  CSVMwp = 0.8484; // 0.800;//CSVv2 0.814; // 1.0623% DUSG mistag efficiency
  CSVTwp = 0.9535; // 0.935;//CSVv2 0.941; // 0.1144% DUSG mistag efficiency

  samplename = "blank";



}

// Destructor
MiniAODHelper::~MiniAODHelper(){

}

// Set up parameters one by one
void MiniAODHelper::SetUp(string iEra, int iSampleNumber, const analysisType::analysisType iAnalysis, bool iIsData){
  // Make sure we don't set up more than once
  if(isSetUp){ ThrowFatalError("Trying to set up 'BEANhelper' for the second time. Check your code."); }

  // Bring in the external values
  era          = iEra;
  sampleNumber = iSampleNumber;
  analysis     = iAnalysis;
  isData       = iIsData;

  // Error checking here
  if((era != "2011") && (era != "2012_52x") && (era != "2012_53x") && (era != "2015_72x") && (era != "2015_73x") && (era != "2015_74x")){ ThrowFatalError("era set to '" + era + "' but it has to be either 2011, 2012_52x, 2012_53x, 2015_72x, 2015_73x, or 2015_74x"); }
  if(sampleNumber==0){ ThrowFatalError("'sampleNumber' cannot be '0'."); }

  // Setup PU reweighing
  //SetUpPUreweighing(iCollisionDS);

  // Setup CSV reweighting
  //SetUpCSVreweighting();

  // Setup jet efficiency scale factors
  //SetUpJetSF();

  // Setup lepton efficiency scale factors
  //SetUpLeptonSF();

  // Awknowledge setup
  isSetUp = true;

}

void MiniAODHelper::SetUpPUWeights(const std::string& fileNameMCNPU,const std::string& histNameMCNPU,const std::string& fileNameDataNPUEstimated,const std::string& histNameDataNPUEstimated) {
  puWeightProducer_.initWeights(fileNameMCNPU,histNameMCNPU,fileNameDataNPUEstimated,histNameDataNPUEstimated);
}

// Set up parameters one by one
void MiniAODHelper::SetVertex(const reco::Vertex& inputVertex){

  vertex = inputVertex;

  vertexIsSet = true;
}

// Set up parameters one by one
void MiniAODHelper::SetRho(double inputRho){

  useRho = inputRho;

  rhoIsSet = true;
}


// Set up parameters one by one

namespace {
  struct ByEta {
    bool operator()(const pat::PackedCandidate *c1, const pat::PackedCandidate *c2) const {
      return c1->eta() < c2->eta();
    }
    bool operator()(float c1eta, const pat::PackedCandidate *c2) const {
      return c1eta < c2->eta();
    }
    bool operator()(const pat::PackedCandidate *c1, float c2eta) const {
      return c1->eta() < c2eta;
    }
  };
}

// Set up parameters one by one
void MiniAODHelper::SetPackedCandidates(const std::vector<pat::PackedCandidate> & all, int fromPV_thresh, float dz_thresh, bool also_leptons){

  allcands_ = &all;
  charged_.clear(); neutral_.clear(); pileup_.clear();
  for (const pat::PackedCandidate &p : all) {
    if (p.charge() == 0) {
      neutral_.push_back(&p);
    }
    else {

      if ( (abs(p.pdgId()) == 211 ) || ( also_leptons && ((abs(p.pdgId()) == 11 ) || (abs(p.pdgId()) == 13 )) ) )  {

	if (p.fromPV() > fromPV_thresh && fabs(p.dz()) < dz_thresh ) {
	  charged_.push_back(&p);
	}
	else {
	  pileup_.push_back(&p);
	}
      }
    }
  }
  //  if (weightCone_ > 0) weights_.resize(neutral_.size());
  //  std::fill(weights_.begin(), weights_.end(), -1.f);
  std::sort(charged_.begin(), charged_.end(), ByEta());
  std::sort(neutral_.begin(), neutral_.end(), ByEta());
  std::sort(pileup_.begin(),  pileup_.end(),  ByEta());
  clearVetos();
}

// Return packed cands collection
std::vector<pat::PackedCandidate> MiniAODHelper::GetPackedCandidates(void){

  std::vector<pat::PackedCandidate> packed_cands_collection = *allcands_;

  if (packed_cands_collection.size() == 0) std::cout << "MiniAODHelper WARNING: packedCandidates are NOT set!" << std::endl;

  return packed_cands_collection;
}


std::vector<pat::Muon>
MiniAODHelper::GetSelectedMuons(const std::vector<pat::Muon>& inputMuons, const float iMinPt, const muonID::muonID iMuonID, const coneSize::coneSize iconeSize, const corrType::corrType icorrType, const float iMaxEta){

  CheckSetUp();

  std::vector<pat::Muon> selectedMuons;

  for( std::vector<pat::Muon>::const_iterator it = inputMuons.begin(), ed = inputMuons.end(); it != ed; ++it ){
    if( isGoodMuon(*it,iMinPt,iMaxEta,iMuonID,iconeSize,icorrType) ) selectedMuons.push_back(*it);
  }

  return selectedMuons;
}


std::vector<pat::Electron>
MiniAODHelper::GetSelectedElectrons(const std::vector<pat::Electron>& inputElectrons, const float iMinPt, const electronID::electronID iElectronID, const float iMaxEta){

  CheckSetUp();

  std::vector<pat::Electron> selectedElectrons;

  for( std::vector<pat::Electron>::const_iterator it = inputElectrons.begin(), ed = inputElectrons.end(); it != ed; ++it ){
    if( isGoodElectron(*it,iMinPt,iMaxEta,iElectronID) ) selectedElectrons.push_back(*it);
  }

  return selectedElectrons;
}

std::vector<pat::Tau>
MiniAODHelper::GetSelectedTaus(const std::vector<pat::Tau>& inputTaus, const float iMinPt, const tauu::ID id){

  CheckSetUp();

  std::vector<pat::Tau> selectedTaus;

  for( std::vector<pat::Tau>::const_iterator it = inputTaus.begin(), ed = inputTaus.end(); it != ed; ++it ){
    if( isGoodTau(*it,iMinPt,id) ) selectedTaus.push_back(*it);
  }

  return selectedTaus;
}




bool MiniAODHelper::passesMuonPOGIdTight(const pat::Muon& iMuon){

    if( !iMuon.isGlobalMuon()) return false;
    if( !iMuon.globalTrack().isAvailable() ) return false;

    bool passesGlobalTrackID = ( (iMuon.globalTrack()->normalizedChi2() < 10.)
				 && (iMuon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0)
				 );
    if(!passesGlobalTrackID) return false;


    if(!iMuon.muonBestTrack().isAvailable() )return false;
    bool passesMuonBestTrackID = ( (fabs(iMuon.muonBestTrack()->dxy(vertex.position())) < 0.2)
				   && (fabs(iMuon.muonBestTrack()->dz(vertex.position())) < 0.5)
				   );
    if(!passesMuonBestTrackID) return false;

    if(!iMuon.innerTrack().isAvailable() ) return false;
    bool passesInnerTrackID = (iMuon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0);
    if(!passesInnerTrackID) return false;

    if(!iMuon.track().isAvailable() ) return false;
    bool passesTrackID = (iMuon.track()->hitPattern().trackerLayersWithMeasurement() > 5);
    if(!passesTrackID) return false;

    if(iMuon.numberOfMatchedStations() <= 1) return false;

    if(!iMuon.isPFMuon()) return false;

    return true;

}

// ICHEP dataset medium ID
// https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideMuonIdRun2#Short_Term_Medium_Muon_Definitio
bool MiniAODHelper::passesMuonPOGIdICHEPMedium(const pat::Muon& iMuon){
  bool isLooseMuon=false;
  isLooseMuon=(iMuon.isPFMuon() && (iMuon.isGlobalMuon() || iMuon.isTrackerMuon()));

  if(!isLooseMuon) return false;

  bool goodGlob = (iMuon.isGlobalMuon() && iMuon.globalTrack()->normalizedChi2() < 3 && iMuon.combinedQuality().chi2LocalPosition < 12 && iMuon.combinedQuality().trkKink < 20);

  bool isMedium = (isLooseMuon && iMuon.innerTrack()->validFraction() > 0.49 && muon::segmentCompatibility(iMuon) > (goodGlob ? 0.303 : 0.451));

  if(!isMedium) return false;
  return true;

}

bool
MiniAODHelper::isGoodMuon(const pat::Muon& iMuon, const float iMinPt, const float iMaxEta, const muonID::muonID iMuonID, const coneSize::coneSize iconeSize, const corrType::corrType icorrType){

  CheckVertexSetUp();

  double minMuonPt = iMinPt;

  double maxMuonEta = iMaxEta;


  // Be skeptical about this muon making it through
  bool passesKinematics	= false;
  bool passesIso        = false;
  bool passesID         = false;
  bool isPFMuon         = false;
  bool passesTrackerID  = false;

  bool passesGlobalTrackID   = false;
  bool passesMuonBestTrackID = false;
  bool passesInnerTrackID    = false;
  bool passesTrackID         = false;


  switch(iMuonID){
  case muonID::muonPreselection:
    // see https://github.com/cms-ttH/ttH-LeptonID for adding multilepton
    // selection userFloats
    passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
    passesIso = true;
    passesID = iMuon.userFloat("idPreselection") > .5;
    break;
  case muonID::muonLooseMvaBased:
    passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
    passesIso = true;
    passesID = iMuon.userFloat("idLooseMVA") > .5;
    break;
  case muonID::muonTightMvaBased:
    passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
    passesIso = true;
    passesID = iMuon.userFloat("idTightMVA") > .5;
    break;
  case muonID::muonLooseCutBased:
    passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
    passesIso = true;
    passesID = iMuon.userFloat("idLooseCut") > .5;
    break;
  case muonID::muonTightCutBased:
    passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
    passesIso = true;
    passesID = iMuon.userFloat("idTightCut") > .5;
    break;
  case muonID::muonSide:
  case muonID::muonSideLooseMVA:
  case muonID::muonSideTightMVA:
  case muonID::muonPtOnly:
  case muonID::muonPtEtaOnly:
  case muonID::muonPtEtaIsoOnly:
  case muonID::muonPtEtaIsoTrackerOnly:
  case muonID::muonRaw:
  case muonID::muonCutBased:
  case muonID::muon2lss:
  case muonID::muonLoose:
    passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
    passesIso        = (GetMuonRelIso(iMuon,iconeSize,icorrType) < 0.200);
    isPFMuon         = iMuon.isPFMuon();

    if( iMuon.globalTrack().isAvailable() ){
      passesGlobalTrackID = ( (iMuon.globalTrack()->normalizedChi2() < 10.)
			      && (iMuon.globalTrack()->hitPattern().numberOfValidMuonHits() > 0)
			      );
    }
    if( iMuon.muonBestTrack().isAvailable() ){
      passesMuonBestTrackID = ( (fabs(iMuon.muonBestTrack()->dxy(vertex.position())) < 0.2)
				&& (fabs(iMuon.muonBestTrack()->dz(vertex.position())) < 0.5)
				);
    }
    if( iMuon.innerTrack().isAvailable() )
      passesInnerTrackID = (iMuon.innerTrack()->hitPattern().numberOfValidPixelHits() > 0);
    if( iMuon.track().isAvailable() )
      passesTrackID = (iMuon.track()->hitPattern().trackerLayersWithMeasurement() > 5);

    passesTrackerID = ( passesGlobalTrackID && passesMuonBestTrackID && passesInnerTrackID && passesTrackID && (iMuon.numberOfMatchedStations() > 1) );

    passesID        = (iMuon.isGlobalMuon() && isPFMuon && passesTrackerID);

    break;
  case muonID::muonTight:
      passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
      passesIso        = (GetMuonRelIso(iMuon,iconeSize,icorrType) < 0.15);
      passesID         = passesMuonPOGIdTight(iMuon);
      break;
  case muonID::muonTightDL:
      passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
      passesIso        = (GetMuonRelIso(iMuon,iconeSize,icorrType) < 0.25);
      passesID         = passesMuonPOGIdTight(iMuon);
      break;

  case muonID::muonMediumICHEP:
      passesKinematics = ((iMuon.pt() >= minMuonPt) && (fabs(iMuon.eta()) <= maxMuonEta));
      passesIso        = (GetMuonRelIso(iMuon,iconeSize,icorrType) < 0.15);
      passesID         = passesMuonPOGIdICHEPMedium(iMuon);
      break;

  }

  return (passesKinematics && passesIso && passesID);
}



bool
MiniAODHelper::isGoodElectron(const pat::Electron& iElectron, const float iMinPt, const float iMaxEta,const electronID::electronID iElectronID){

  CheckVertexSetUp();

  double minElectronPt = iMinPt;

  float maxElectronEta = iMaxEta;


  // Be skeptical about this electron making it through
  bool passesKinematics	= false;
  bool passesIso        = false;
  bool passesID         = false;

  double SCeta = (iElectron.superCluster().isAvailable()) ? iElectron.superCluster()->position().eta() : -99;
  double absSCeta = fabs(SCeta);

  bool inCrack = false;
  if( iElectron.superCluster().isAvailable() ) inCrack = ( absSCeta>1.4442 && absSCeta<1.5660 );


  bool myTrigPresel = true;

  //double eleID      = iElectron.electronID("eidTight");
  bool passMVAId53x = true;//( eleID>0.5 );  // For 2012_53x, tighter selection

  bool d02 = false;
  bool d04 = false;
  bool dZ  = false;
  bool no_exp_inner_trkr_hits = true; //false; // see below
  if( iElectron.gsfTrack().isAvailable() ){
    d02 = ( fabs(iElectron.gsfTrack()->dxy(vertex.position())) < 0.02 );
    d04 = ( fabs(iElectron.gsfTrack()->dxy(vertex.position())) < 0.04 );
    //no_exp_inner_trkr_hits = ( iElectron.gsfTrack()->trackerExpectedHitsInner().numberOfHits() <= 0 ); // deprecated in 7_2_0 .. replace with ..?
    dZ = ( fabs(iElectron.gsfTrack()->dz(vertex.position())) < 1. );
  }


  bool notConv = ( iElectron.passConversionVeto() );
  bool id      = ( passMVAId53x && d02 && dZ && notConv );


  switch(iElectronID){
  case electronID::electronPreselection:
    // see https://github.com/cms-ttH/ttH-LeptonID for adding multilepton
    // selection userFloats
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = true;
    passesID = iElectron.userFloat("idPreselection") > .5;
    break;
  case electronID::electronLooseCutBased:
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = true;
    passesID = iElectron.userFloat("idLooseCut") > .5;
    break;
  case electronID::electronTightCutBased:
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = true;
    passesID = iElectron.userFloat("idTightCut") > .5;
    break;
  case electronID::electronLooseMvaBased:
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = true;
    passesID = iElectron.userFloat("idLooseMVA") > .5;
    break;
  case electronID::electronTightMvaBased:
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = true;
    passesID = iElectron.userFloat("idTightMVA") > .5;
    break;
  case electronID::electronSide:
  case electronID::electronSideLooseMVA:
  case electronID::electronSideTightMVA:
  case electronID::electronLooseMinusTrigPresel:
  case electronID::electronRaw:
  case electronID::electronCutBased:
  case electronID::electron2lss:
  case electronID::electronLoose:
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso        = (GetElectronRelIso(iElectron) < 0.200);
    passesID         = ( passMVAId53x && no_exp_inner_trkr_hits && d04 && notConv && myTrigPresel );
    break;
  case electronID::electronTightMinusTrigPresel:
  case electronID::electronTight:
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso        = (GetElectronRelIso(iElectron) < 0.100);
    passesID         = ( id && no_exp_inner_trkr_hits && myTrigPresel );
    break;
  case electronID::electronPhys14L:
  case electronID::electronPhys14M:
  case electronID::electronPhys14T:
    id = PassElectronPhys14Id( iElectron, iElectronID );
    passesIso = id;
    passesID = id;
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    break;
  case electronID::electronSpring15Veto:
  case electronID::electronSpring15L:
  case electronID::electronSpring15M:
  case electronID::electronSpring15T:
    id = PassElectronSpring15Id( iElectron, iElectronID );
    passesIso = id;
    passesID = id;
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    break;
  case electronID::electronEndOf15MVA80:
    passesID = PassesMVAid80(iElectron);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = true; // TODO: what is the correct isolation here?
    break;
  case electronID::electronEndOf15MVA90:
    passesID = PassesMVAid90(iElectron);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = true; // TODO: what is the correct isolation here?
    break;
  case electronID::electronEndOf15MVA80iso0p1:
    passesID = PassesMVAid80(iElectron);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso=0.1>=GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA,effAreaType::spring15);
    break;
  case electronID::electronEndOf15MVA90iso0p1:
    passesID = PassesMVAid90(iElectron);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso=0.1>=GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA,effAreaType::spring15);
    break;
  case electronID::electronEndOf15MVA80iso0p15:
    passesID = PassesMVAid80(iElectron);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso=0.15>=GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA,effAreaType::spring15);
    break;
  case electronID::electronEndOf15MVA90iso0p15:
    passesID = PassesMVAid90(iElectron);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso=0.15>=GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA,effAreaType::spring15);
    break;

  case electronID::electron80XCutBasedL:
  case electronID::electron80XCutBasedM:
  case electronID::electron80XCutBasedT:
    passesID = PassElectron80XId(iElectron,iElectronID);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso=0.15>=GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA,effAreaType::spring16);
    break;
  case electronID::electronNonTrigMVAid80:
    passesID = PassesNonTrigMVAid80(iElectron);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = 0.15>=GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA,effAreaType::spring15);
    break;
  case electronID::electronNonTrigMVAid90:
    passesID = PassesNonTrigMVAid90(iElectron);
    passesKinematics = ((iElectron.pt() >= minElectronPt) && (fabs(iElectron.eta()) <= maxElectronEta) && !inCrack);
    passesIso = 0.15>=GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA,effAreaType::spring15);
    break;


  }

  return (passesKinematics && passesIso && passesID);
}

bool
MiniAODHelper::isGoodTau(const pat::Tau& tau, const float min_pt, const tauu::ID id)
{
  CheckVertexSetUp();

  bool passesIsolation = false;
  bool passesID = tau.tauID("decayModeFindingNewDMs") >= .5;

  if (!tau.leadChargedHadrCand().isAvailable())
     return false;

  auto track = tau.leadChargedHadrCand()->bestTrack();
  if (!track)
     return false;

  // systematics are only defined for p_T > 20
  bool passesKinematics = \
                          (tau.pt() >= std::max(20.f, min_pt)) and \
                          (fabs(tau.eta()) <= 2.3) and \
                          (track->pt() >= 5.) and \
                          (fabs(track->dxy(vertex.position())) < 1000.) and \
                          (fabs(track->dz(vertex.position())) <= 0.2);

  switch (id) {
     case tauu::nonIso:
        passesID = passesID and \
                   tau.tauID("againstMuonLoose3") >= .5 and \
                   tau.tauID("againstElectronVLooseMVA6") >= .5;
        passesIsolation = true;
        break;
     case tauu::loose:
        passesID = passesID and \
                   tau.tauID("againstMuonLoose3") >= .5 and \
                   tau.tauID("againstElectronVLooseMVA6") >= .5;
        passesIsolation = tau.tauID("byLooseCombinedIsolationDeltaBetaCorr3Hits") >= .5;
        break;
     case tauu::medium:
        passesID = passesID and \
                   tau.tauID("againstMuonLoose3") >= .5 and \
                   tau.tauID("againstElectronLooseMVA6") >= .5;
        passesIsolation = tau.tauID("byMediumCombinedIsolationDeltaBetaCorr3Hits") >= .5;
        break;
     case tauu::tight:
        passesID = passesID and \
                   tau.tauID("againstMuonTight3") >= .5 and \
                   tau.tauID("againstElectronMediumMVA6") >= .5;
        passesIsolation = tau.tauID("byTightCombinedIsolationDeltaBetaCorr3Hits") >= .5;
        break;
  }

  return passesKinematics && passesIsolation && passesID;
}



float MiniAODHelper::GetMuonRelIso(const pat::Muon& iMuon) const
{
  float result = 9999;

  double pfIsoCharged = iMuon.pfIsolationR03().sumChargedHadronPt;
  double pfIsoNeutral = iMuon.pfIsolationR03().sumNeutralHadronEt + iMuon.pfIsolationR03().sumPhotonEt;

  double pfIsoPUSubtracted = std::max( 0.0, pfIsoNeutral - 0.5*iMuon.pfIsolationR03().sumPUPt );

  result = (pfIsoCharged + pfIsoPUSubtracted)/iMuon.pt();

  return result;
}

//overloaded
float MiniAODHelper::GetMuonRelIso(const pat::Muon& iMuon,const coneSize::coneSize iconeSize, const corrType::corrType icorrType, std::map<std::string,double> *miniIso_calculation_params) const
{

  // !!! NOTE !!! rho used with Phys14 should be: fixedGridRhoFastjetAll
  // !!! NOTE !!! rho used with Spring15 should be: fixedGridRhoFastjetCentralNeutral

  float result = 9999;

  double correction = 9999.;
  double EffArea = 9999.;
  double Eta = abs(iMuon.eta());

  double pfIsoCharged;
  double pfIsoNeutral;
  double pfIsoPUSubtracted;

  switch(iconeSize)
    {
    case coneSize::R04:
      pfIsoCharged = iMuon.pfIsolationR04().sumChargedHadronPt;
      pfIsoNeutral = iMuon.pfIsolationR04().sumNeutralHadronEt + iMuon.pfIsolationR04().sumPhotonEt;

      switch(icorrType)
	{
	case corrType::rhoEA:
	  //based on R04 Phys14_25ns_v1
	  // if (Eta >= 0. && Eta < 0.8) EffArea = 0.1546;
	  // else if (Eta >= 0.8 && Eta < 1.3) EffArea = 0.1325;
	  // else if (Eta >= 1.3 && Eta < 2.0) EffArea = 0.0913;
	  // else if (Eta >= 2.0 && Eta < 2.2) EffArea = 0.1212;
	  // else if (Eta >= 2.2 && Eta <= 2.5) EffArea = 0.2085;
	  EffArea = -9999.;
	  correction = useRho*EffArea;
	  break;
	case corrType::deltaBeta:
	  correction =  0.5*iMuon.pfIsolationR04().sumPUPt;
	  break;
	}

      pfIsoPUSubtracted = std::max( 0.0, pfIsoNeutral - correction );
      result = (pfIsoCharged + pfIsoPUSubtracted)/iMuon.pt();
      break;

    case coneSize::R03:
      pfIsoCharged = iMuon.pfIsolationR03().sumChargedHadronPt;
      pfIsoNeutral = iMuon.pfIsolationR03().sumNeutralHadronEt + iMuon.pfIsolationR03().sumPhotonEt;

      switch(icorrType)
	{
	case corrType::rhoEA:
	  //effective area based on R03 Phys14_25ns_v1
	  // if (Eta >= 0. && Eta < 0.8) EffArea = 0.0913;
	  // else if (Eta >= 0.8 && Eta < 1.3) EffArea = 0.0765;
	  // else if (Eta >= 1.3 && Eta < 2.0) EffArea = 0.0546;
	  // else if (Eta >= 2.0 && Eta < 2.2) EffArea = 0.0728;
	  // else if (Eta >= 2.2 && Eta <= 2.5) EffArea = 0.1177;

	  //effective area based on R03 Spring15
	  if (abs(Eta) < 0.8) EffArea = 0.0735;
	  else if (abs(Eta) < 1.3) EffArea = 0.0619;
	  else if (abs(Eta) < 2.0) EffArea = 0.0465;
	  else if (abs(Eta) < 2.2) EffArea = 0.0433;
	  else EffArea = 0.0577;

	  correction = useRho*EffArea;
	  break;
	case corrType::deltaBeta:
	  correction = 0.5*iMuon.pfIsolationR03().sumPUPt;
	  break;
	}

      pfIsoPUSubtracted = std::max( 0.0, pfIsoNeutral - correction );
      result = (pfIsoCharged + pfIsoPUSubtracted)/iMuon.pt();
      break;

    case coneSize::miniIso:
      double miniIsoR = 10.0/min(max(float(iMuon.pt()), float(50.)),float(200.));
      pfIsoCharged = isoSumRaw(charged_, iMuon, miniIsoR, 0.0001, 0.0, SelfVetoPolicy::selfVetoAll);
      pfIsoNeutral = isoSumRaw(neutral_, iMuon, miniIsoR, 0.01, 0.5, SelfVetoPolicy::selfVetoAll);
      switch(icorrType)
	{
	case corrType::rhoEA:
	  //effective area based on R03 Phys14_25ns_v1
	  // if (abs(Eta) < 0.8) EffArea = 0.0913;
	  // else if (abs(Eta) < 1.3) EffArea = 0.0765;
	  // else if (abs(Eta) < 2.0) EffArea = 0.0546;
	  // else if (abs(Eta) < 2.2) EffArea = 0.0728;
	  // else EffArea = 0.1177;

	  //effective area based on R03 Spring15
	  if (abs(Eta) < 0.8) EffArea = 0.0735;
	  else if (abs(Eta) < 1.3) EffArea = 0.0619;
	  else if (abs(Eta) < 2.0) EffArea = 0.0465;
	  else if (abs(Eta) < 2.2) EffArea = 0.0433;
	  else EffArea = 0.0577;

	  correction = useRho*EffArea*(miniIsoR/0.3)*(miniIsoR/0.3);
	  break;
	case corrType::deltaBeta:
	  double miniAbsIsoPU = isoSumRaw(pileup_, iMuon, miniIsoR, 0.01, 0.5, SelfVetoPolicy::selfVetoAll);
	  correction = 0.5*miniAbsIsoPU;
	  break;
	}

      pfIsoPUSubtracted = std::max( 0.0, pfIsoNeutral - correction);
      result = (pfIsoCharged + pfIsoPUSubtracted)/iMuon.pt();

      if (miniIso_calculation_params) {
         miniIso_calculation_params->clear();
         (*miniIso_calculation_params)["miniAbsIsoCharged"] = pfIsoCharged;
         (*miniIso_calculation_params)["miniAbsIsoNeutral"] = pfIsoNeutral;
         (*miniIso_calculation_params)["rho"] = useRho;
         (*miniIso_calculation_params)["effArea"] = EffArea;
         (*miniIso_calculation_params)["miniIsoR"] = miniIsoR;
         (*miniIso_calculation_params)["miniAbsIsoNeutralcorr"] = pfIsoPUSubtracted;
      }
      break;
    }
  return result;
}

void MiniAODHelper::AddMuonRelIso(pat::Muon& iMuon,const coneSize::coneSize iconeSize, const corrType::corrType icorrType, std::string userFloatName) const{
  float iso=GetMuonRelIso(iMuon,iconeSize,icorrType);
  iMuon.addUserFloat(userFloatName,iso);
}

void MiniAODHelper::AddMuonRelIso(std::vector<pat::Muon>& muons,const coneSize::coneSize iconeSize, const corrType::corrType icorrType, std::string userFloatName) const{
  for(auto mu=muons.begin(); mu!=muons.end(); mu++){
    AddMuonRelIso(*mu,iconeSize,icorrType,userFloatName);
  }
}


float MiniAODHelper::GetElectronRelIso(const pat::Electron& iElectron) const
{
  float result = 9999;

  double pfIsoCharged = iElectron.pfIsolationVariables().sumChargedHadronPt;
  double pfIsoNeutral = iElectron.pfIsolationVariables().sumNeutralHadronEt + iElectron.pfIsolationVariables().sumPhotonEt;

  double pfIsoPUSubtracted = std::max( 0.0, pfIsoNeutral - 0.5*iElectron.pfIsolationVariables().sumPUPt );

  result = (pfIsoCharged + pfIsoPUSubtracted)/iElectron.pt();

  return result;
}

//overloaded
float MiniAODHelper::GetElectronRelIso(const pat::Electron& iElectron,const coneSize::coneSize iconeSize, const corrType::corrType icorrType,const effAreaType::effAreaType ieffAreaType, std::map<std::string,double>* miniIso_calculation_params) const
{
   //rho*EA corrections based on phys14
   //details here: https://www.dropbox.com/s/66lzhbro09diksa/effectiveareas-pog-121214.pdf?dl=0
  // !!! NOTE !!! rho used should be: fixedGridRhoFastjetAll
  float result = 9999;

  double correction = 9999.;
  double EffArea = 9999.;
  double Eta = abs(iElectron.eta());

  double pfIsoCharged;
  double pfIsoNeutral;
  double pfIsoPUSubtracted;

  switch(iconeSize)
    {
    case coneSize::R04:
    case coneSize::R03:
      pfIsoCharged = iElectron.pfIsolationVariables().sumChargedHadronPt;
      pfIsoNeutral = iElectron.pfIsolationVariables().sumNeutralHadronEt + iElectron.pfIsolationVariables().sumPhotonEt;

      switch(icorrType)
	{
	case corrType::rhoEA:
	  if(ieffAreaType==effAreaType::phys14){
	    if (Eta >= 0. && Eta < 0.8) EffArea = 0.1013;
	    else if (Eta >= 0.8 && Eta < 1.3) EffArea = 0.0988;
	    else if (Eta >= 1.3 && Eta < 2.0) EffArea = 0.0572;
	    else if (Eta >= 2.0 && Eta < 2.2) EffArea = 0.0842;
	    else if (Eta >= 2.2 && Eta <= 2.5) EffArea = 0.1530;
	  }
	  else if (ieffAreaType==effAreaType::spring15){
	    if (Eta >= 0. && Eta < 1.0) EffArea = 0.1752;
	    else if (Eta >= 1.0 && Eta < 1.479) EffArea = 0.1862;
	    else if (Eta >= 1.479 && Eta < 2.0) EffArea = 0.1411;
	    else if (Eta >= 2.0 && Eta < 2.2) EffArea = 0.1534;
	    else if (Eta >= 2.2 && Eta < 2.3) EffArea = 0.1903;
	    else if (Eta >= 2.3 && Eta < 2.4) EffArea = 0.2243;
	    else if (Eta >= 2.4 && Eta < 2.5) EffArea = 0.2687;
	  }
	  else if (ieffAreaType==effAreaType::spring16){
	    if (Eta >= 0. && Eta < 1.0) EffArea = 0.1703;
	    else if (Eta >= 1.0 && Eta < 1.479) EffArea = 0.1715;
	    else if (Eta >= 1.479 && Eta < 2.0) EffArea = 0.1213;
	    else if (Eta >= 2.0 && Eta < 2.2) EffArea = 0.1230;
	    else if (Eta >= 2.2 && Eta < 2.3) EffArea = 0.1635;
	    else if (Eta >= 2.3 && Eta < 2.4) EffArea = 0.1937;
	    else if (Eta >= 2.4 && Eta < 5) EffArea = 0.2393;
	  }

	  if(!rhoIsSet) std::cout << " !! ERROR !! Trying to get rhoEffArea correction without setting rho" << std::endl;
	  correction = useRho*EffArea;
	  break;
	case corrType::deltaBeta:
	  correction = 0.5*iElectron.pfIsolationVariables().sumPUPt;
	  break;
	}
      pfIsoPUSubtracted = std::max( 0.0, pfIsoNeutral - correction );
      result = (pfIsoCharged + pfIsoPUSubtracted)/iElectron.pt();
      break;
    case coneSize::miniIso:
      double innerR_ch;
      double innerR_nu;
      double miniIsoR = 10.0/min(max(float(iElectron.pt()), float(50.)),float(200.));
      if (iElectron.isEB())
	{
	  innerR_ch = 0.0;
	  innerR_nu = 0.0;
	}
      else
	{
	  innerR_ch = 0.015;
	  innerR_nu = 0.08;
	}

      pfIsoCharged = isoSumRaw(charged_, iElectron, miniIsoR, innerR_ch, 0.0, SelfVetoPolicy::selfVetoNone);
      pfIsoNeutral = isoSumRaw(neutral_, iElectron, miniIsoR, innerR_nu, 0.0, SelfVetoPolicy::selfVetoNone, 22)+isoSumRaw(neutral_, iElectron, miniIsoR, 0.0, 0.0, SelfVetoPolicy::selfVetoNone, 130);
      switch(icorrType)
	{
	case corrType::rhoEA:
	  //effective area based on R03

	  if(ieffAreaType==effAreaType::phys14)
	    {
	      if (Eta >= 0. && Eta < 0.8) EffArea = 0.1013;
	      else if (Eta >= 0.8 && Eta < 1.3) EffArea = 0.0988;
	      else if (Eta >= 1.3 && Eta < 2.0) EffArea = 0.0572;
	      else if (Eta >= 2.0 && Eta < 2.2) EffArea = 0.0842;
	      else if (Eta >= 2.2 && Eta <= 2.5) EffArea = 0.1530;
	    }
	  else if (ieffAreaType==effAreaType::spring15)
	    {
	      if (Eta >= 0. && Eta < 1.0) EffArea = 0.1752;
	      else if (Eta >= 1.0 && Eta < 1.479) EffArea = 0.1862;
	      else if (Eta >= 1.479 && Eta < 2.0) EffArea = 0.1411;
	      else if (Eta >= 2.0 && Eta < 2.2) EffArea = 0.1534;
	      else if (Eta >= 2.2 && Eta < 2.3) EffArea = 0.1903;
	      else if (Eta >= 2.3 && Eta < 2.4) EffArea = 0.2243;
	      else if (Eta >= 2.4 && Eta < 2.5) EffArea = 0.2687;
	    }

	  if(!rhoIsSet) std::cout << " !! ERROR !! Trying to get rhoEffArea correction without setting rho" << std::endl;
	  correction = useRho*EffArea*(miniIsoR/0.3)*(miniIsoR/0.3);
	  break;
	case corrType::deltaBeta:
	  double miniAbsIsoPU = isoSumRaw(pileup_, iElectron, miniIsoR, innerR_ch, 0.0, SelfVetoPolicy::selfVetoNone);
	  correction = 0.5*miniAbsIsoPU;
	  break;
	}
      pfIsoPUSubtracted = std::max( 0.0, pfIsoNeutral - correction);
      result = (pfIsoCharged + pfIsoPUSubtracted)/iElectron.pt();

      if (miniIso_calculation_params) {
         miniIso_calculation_params->clear();
         (*miniIso_calculation_params)["miniAbsIsoCharged"] = pfIsoCharged;
         (*miniIso_calculation_params)["miniAbsIsoNeutral"] = pfIsoNeutral;
         (*miniIso_calculation_params)["rho"] = useRho;
         (*miniIso_calculation_params)["effArea"] = EffArea;
         (*miniIso_calculation_params)["miniIsoR"] = miniIsoR;
         (*miniIso_calculation_params)["miniAbsIsoNeutralcorr"] = pfIsoPUSubtracted;
      }
      break;
    }
  return result;
}

void MiniAODHelper::AddElectronRelIso(pat::Electron& iElectron,const coneSize::coneSize iconeSize, const corrType::corrType icorrType,const effAreaType::effAreaType ieffAreaType, std::string userFloatName) const{
    float iso=GetElectronRelIso(iElectron,iconeSize,icorrType,ieffAreaType);
    iElectron.addUserFloat(userFloatName,iso);
}

void MiniAODHelper::AddElectronRelIso(std::vector<pat::Electron>& electrons,const coneSize::coneSize iconeSize, const corrType::corrType icorrType,const effAreaType::effAreaType ieffAreaType, std::string userFloatName) const{
    for(auto el=electrons.begin(); el!=electrons.end(); el++){
	AddElectronRelIso(*el,iconeSize,icorrType,ieffAreaType,userFloatName);
    }
}




bool MiniAODHelper::PassElectron80XId(const pat::Electron& iElectron, const electronID::electronID iElectronID) const{

  double SCeta = (iElectron.superCluster().isAvailable()) ? iElectron.superCluster()->position().eta() : -99;
  double absSCeta = fabs(SCeta);
  double relIso = GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA, effAreaType::spring16);

  bool isEB = ( absSCeta < 1.479 );

  double full5x5_sigmaIetaIeta = iElectron.full5x5_sigmaIetaIeta();
//   double dEtaIn = fabs( iElectron.deltaEtaSuperClusterTrackAtVtx() );
  double dEtaInSeed = iElectron.superCluster().isNonnull() && iElectron.superCluster()->seed().isNonnull() ? iElectron.deltaEtaSuperClusterTrackAtVtx() - iElectron.superCluster()->eta() + iElectron.superCluster()->seed()->eta() : std::numeric_limits<float>::max();
  double fabsdEtaInSeed=fabs(dEtaInSeed);
  double dPhiIn = fabs( iElectron.deltaPhiSuperClusterTrackAtVtx() );
  double hOverE = iElectron.hcalOverEcal();

  double ooEmooP = -999;
  if( iElectron.ecalEnergy() == 0 ) ooEmooP = 1e30;
  else if( !std::isfinite(iElectron.ecalEnergy()) ) ooEmooP = 1e30;
  else ooEmooP = fabs(1.0/iElectron.ecalEnergy() - iElectron.eSuperClusterOverP()/iElectron.ecalEnergy() );

  double d0 = -999;
  double dZ = -999;
  double expectedMissingInnerHits = 999;
  if( iElectron.gsfTrack().isAvailable() ){
    d0 = fabs(iElectron.gsfTrack()->dxy(vertex.position()));
    dZ = fabs(iElectron.gsfTrack()->dz(vertex.position()));
    expectedMissingInnerHits = iElectron.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
  }

  bool passConversionVeto = ( iElectron.passConversionVeto() );

  bool pass = false;
  switch(iElectronID){
  case electronID::electron80XCutBasedL:
    if( isEB ){
      pass = ( full5x5_sigmaIetaIeta < 0.011 &&
	       fabsdEtaInSeed < 0.00477 &&
	       dPhiIn < 0.222 &&
	       hOverE < 0.298 &&
	       ooEmooP < 0.241 &&
// 	       d0 < 0.035904 &&
// 	       dZ < 0.075496 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.0994
	       );
    }
    else{
      pass = ( full5x5_sigmaIetaIeta < 0.0314 &&
	       fabsdEtaInSeed < 0.00868 &&
	       dPhiIn < 0.213 &&
	       hOverE < 0.101 &&
	       ooEmooP < 0.14 &&
// 	       d0 < 0.035904 &&
// 	       dZ < 0.075496 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.107
	       );
    }
    break;
  case electronID::electron80XCutBasedM:
    if( isEB ){
      pass = ( full5x5_sigmaIetaIeta < 0.00998 &&
	       fabsdEtaInSeed < 0.00311 &&
	       dPhiIn < 0.103 &&
	       hOverE < 0.253 &&
	       ooEmooP < 0.134 &&
	       d0 < 0.05 &&
	       dZ < 0.10 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.0695
	       );
    }
    else{
      pass = ( full5x5_sigmaIetaIeta < 0.0298 &&
	       fabsdEtaInSeed < 0.00609 &&
	       dPhiIn < 0.045 &&
	       hOverE < 0.0878 &&
	       ooEmooP < 0.13 &&
	       d0 < 0.10 &&
	       dZ < 0.20 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.0821
	       );
    }
    break;
    case electronID::electron80XCutBasedT:
    if( isEB ){
      pass = ( full5x5_sigmaIetaIeta < 0.00998 &&
	       fabsdEtaInSeed < 0.00308 &&
	       dPhiIn < 0.0816 &&
	       hOverE < 0.0414 &&
	       ooEmooP < 0.0129 &&
// 	       d0 < 0.035904 &&
// 	       dZ < 0.075496 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.0588
	       );
    }
    else{
      pass = ( full5x5_sigmaIetaIeta < 0.0292 &&
	       fabsdEtaInSeed < 0.00605 &&
	       dPhiIn < 0.0394 &&
	       hOverE < 0.0641 &&
	       ooEmooP < 0.0129 &&
// 	       d0 < 0.035904 &&
// 	       dZ < 0.075496 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.0571
	       );
    }
    break;
  default:
    break;
  }

  return pass;
}


bool MiniAODHelper::PassElectronPhys14Id(const pat::Electron& iElectron, const electronID::electronID iElectronID) const{

  double SCeta = (iElectron.superCluster().isAvailable()) ? iElectron.superCluster()->position().eta() : -99;
  double absSCeta = fabs(SCeta);

  bool isEB = ( absSCeta < 1.479 );

  // double pfIsoCharged = iElectron.pfIsolationVariables().sumChargedHadronPt;
  // double pfIsoNeutralHadron = iElectron.pfIsolationVariables().sumNeutralHadronEt;
  // double pfIsoNeutralPhoton = iElectron.pfIsolationVariables().sumPhotonEt;
  // double pfIsoSumPUPt = iElectron.pfIsolationVariables().sumPUPt;

  // double relIso = (pfIsoCharged + std::max( pfIsoNeutralHadron + pfIsoNeutralPhoton - 0.5*pfIsoSumPUPt, 0.0 ))/iElectron.pt();
  double relIso = GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA);

  double full5x5_sigmaIetaIeta = iElectron.full5x5_sigmaIetaIeta();
  double dEtaIn = fabs( iElectron.deltaEtaSuperClusterTrackAtVtx() );
  double dPhiIn = fabs( iElectron.deltaPhiSuperClusterTrackAtVtx() );
  double hOverE = iElectron.hcalOverEcal();

  double ooEmooP = -999;
  if( iElectron.ecalEnergy() == 0 ) ooEmooP = 1e30;
  else if( !std::isfinite(iElectron.ecalEnergy()) ) ooEmooP = 1e30;
  else ooEmooP = fabs(1.0/iElectron.ecalEnergy() - iElectron.eSuperClusterOverP()/iElectron.ecalEnergy() );

  double d0 = -999;
  double dZ = -999;
  double expectedMissingInnerHits = -999;
  if( iElectron.gsfTrack().isAvailable() ){
    d0 = fabs(iElectron.gsfTrack()->dxy(vertex.position()));
    dZ = fabs(iElectron.gsfTrack()->dz(vertex.position()));
    expectedMissingInnerHits = iElectron.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
  }

  bool passConversionVeto = ( iElectron.passConversionVeto() );

  bool pass = false;
  switch(iElectronID){
  case electronID::electronPhys14L:
    if( isEB ){
      pass = ( full5x5_sigmaIetaIeta < 0.010331 &&
	       dEtaIn < 0.009277 &&
	       dPhiIn < 0.094739 &&
	       hOverE < 0.093068 &&
	       ooEmooP < 0.189968 &&
	       d0 < 0.035904 &&
	       dZ < 0.075496 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.130136
	       );
    }
    else{
      pass = ( full5x5_sigmaIetaIeta < 0.031838 &&
	       dEtaIn < 0.009833 &&
	       dPhiIn < 0.149934 &&
	       hOverE < 0.115754 &&
	       ooEmooP < 0.140662 &&
	       d0 < 0.099266 &&
	       dZ < 0.197897 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.163368
	       );
    }
    break;
  case electronID::electronPhys14M:
    if( isEB ){
      pass = ( full5x5_sigmaIetaIeta < 0.009996 &&
	       dEtaIn < 0.008925 &&
	       dPhiIn < 0.035973 &&
	       hOverE < 0.050537  &&
	       ooEmooP < 0.091942 &&
	       d0 < 0.012235 &&
	       dZ < 0.042020 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.107587
	       );
    }
    else{
      pass = ( full5x5_sigmaIetaIeta < 0.030135 &&
	       dEtaIn < 0.007429 &&
	       dPhiIn < 0.067879 &&
	       hOverE < 0.086782 &&
	       ooEmooP < 0.100683 &&
	       d0 < 0.036719 &&
	       dZ < 0.138142 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.113254
	       );
    }
    break;
  case electronID::electronPhys14T:
    if( isEB ){
      pass = ( full5x5_sigmaIetaIeta < 0.009947 &&
	       dEtaIn < 0.006046 &&
	       dPhiIn < 0.028092 &&
	       hOverE < 0.045772 &&
	       ooEmooP < 0.020118 &&
	       d0 < 0.008790 &&
	       dZ < 0.021226 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.069537
	       );
    }
    else{
      pass = ( full5x5_sigmaIetaIeta < 0.028237 &&
	       dEtaIn < 0.007057 &&
	       dPhiIn < 0.030159 &&
	       hOverE < 0.067778 &&
	       ooEmooP < 0.098919 &&
	       d0 < 0.027984 &&
	       dZ < 0.133431 &&
	       expectedMissingInnerHits <= 1 &&
	       passConversionVeto &&
	       relIso < 0.078265
	       );
    }
    break;
  default:
    break;
  }

  return pass;
}

bool MiniAODHelper::PassElectronSpring15Id(const pat::Electron& iElectron, const electronID::electronID iElectronID) const{

    double SCeta = (iElectron.superCluster().isAvailable()) ? iElectron.superCluster()->position().eta() : -99;
    double absSCeta = fabs(SCeta);

    bool isEB = ( absSCeta < 1.479 );
    double relIso = GetElectronRelIso(iElectron, coneSize::R03, corrType::rhoEA,effAreaType::spring15);

    double full5x5_sigmaIetaIeta = iElectron.full5x5_sigmaIetaIeta();
    double dEtaIn = fabs( iElectron.deltaEtaSuperClusterTrackAtVtx() );
    double dPhiIn = fabs( iElectron.deltaPhiSuperClusterTrackAtVtx() );
    double hOverE = iElectron.hcalOverEcal();

    double ooEmooP = -999;
    if( iElectron.ecalEnergy() == 0 ) ooEmooP = 1e30;
    else if( !std::isfinite(iElectron.ecalEnergy()) ) ooEmooP = 1e30;
    else ooEmooP = fabs(1.0/iElectron.ecalEnergy() - iElectron.eSuperClusterOverP()/iElectron.ecalEnergy() );

    double d0 = -999;
    double dZ = -999;
    double expectedMissingInnerHits = -999;
    if( iElectron.gsfTrack().isAvailable() ){
	d0 = fabs(iElectron.gsfTrack()->dxy(vertex.position()));
	dZ = fabs(iElectron.gsfTrack()->dz(vertex.position()));
	expectedMissingInnerHits = iElectron.gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
    }

    bool passConversionVeto = ( iElectron.passConversionVeto() );

    bool pass = false;
    switch(iElectronID){
    case electronID::electronSpring15Veto:
	if( isEB ){
	    pass=(
		  full5x5_sigmaIetaIeta < 0.0114  &&
		  dEtaIn < 0.0152  &&
		  dPhiIn < 0.216  &&
		  hOverE < 0.181  &&
		  relIso < 0.126  &&
		  ooEmooP < 0.207  &&
		  d0 < 0.0564  &&
		  dZ < 0.472  &&
		  expectedMissingInnerHits <= 2  &&
		  passConversionVeto
		  );
	}
	else{
	    pass=(
		  full5x5_sigmaIetaIeta < 0.0352  &&
		  dEtaIn < 0.0113  &&
		  dPhiIn < 0.237  &&
		  hOverE < 0.116  &&
		  relIso < 0.144  &&
		  ooEmooP < 0.174  &&
		  d0 < 0.222  &&
		  dZ < 0.921  &&
		  expectedMissingInnerHits <= 3  &&
		  passConversionVeto
		  );
	}
	break;

    case electronID::electronSpring15L:
	if( isEB ){
	    pass=(
		  full5x5_sigmaIetaIeta <= 0.0103  &&
		  dEtaIn < 0.0105  &&
		  dPhiIn < 0.115  &&
		  hOverE < 0.104  &&
		  relIso < 0.0893  &&
		  ooEmooP < 0.102  &&
		  d0 < 0.0261  &&
		  dZ < 0.41  &&
		  expectedMissingInnerHits <= 2  &&
		  passConversionVeto
		  );
	}
	else{
	    pass=(
		  full5x5_sigmaIetaIeta < 0.0301  &&
		  dEtaIn < 0.00814  &&
		  dPhiIn < 0.182  &&
		  hOverE < 0.0897  &&
		  relIso < 0.121  &&
		  ooEmooP < 0.126  &&
		  d0 < 0.118  &&
		  dZ < 0.822  &&
		  expectedMissingInnerHits <= 1  &&
		  passConversionVeto
		  );
	}
	break;

    case electronID::electronSpring15M:
	if( isEB ){
	    pass=(
		  full5x5_sigmaIetaIeta < 0.0101  &&
		  dEtaIn < 0.0103  &&
		  dPhiIn < 0.0336  &&
		  hOverE < 0.0876  &&
		  relIso < 0.0766  &&
		  ooEmooP < 0.0174  &&
		  d0 < 0.0118  &&
		  dZ < 0.373  &&
		  expectedMissingInnerHits <= 2  &&
		  passConversionVeto
		  );
	}
	else{
	    pass=(
		  full5x5_sigmaIetaIeta < 0.0283  &&
		  dEtaIn < 0.00733  &&
		  dPhiIn < 0.114  &&
		  hOverE < 0.0678  &&
		  relIso < 0.0678  &&
		  ooEmooP < 0.0898  &&
		  d0 < 0.0739  &&
		  dZ < 0.602  &&
		  expectedMissingInnerHits <= 1  &&
		  passConversionVeto
		  );
	}
	break;

    case electronID::electronSpring15T:
	if( isEB ){
	    pass=(
		  full5x5_sigmaIetaIeta < 0.0101 &&
		  dEtaIn < 0.00926 &&
		  dPhiIn < 0.0336 &&
		  hOverE < 0.0597 &&
		  relIso < 0.0354 &&
		  ooEmooP < 0.012 &&
		  d0 < 0.0111 &&
		  dZ < 0.0466 &&
		  expectedMissingInnerHits <= 2 &&
		  passConversionVeto
		  );
	}
	else{
	    pass=(
		  full5x5_sigmaIetaIeta < 0.0279 &&
		  dEtaIn < 0.00724 &&
		  dPhiIn < 0.0918 &&
		  hOverE < 0.0615 &&
		  relIso < 0.0646 &&
		  ooEmooP < 0.00999 &&
		  d0 < 0.0351 &&
		  dZ < 0.417 &&
		  expectedMissingInnerHits <= 1 &&
		  passConversionVeto
		  );
	}
	break;

    default:
	break;
    }

    return pass;
}
// adds electron mva output as user float to electrons
// you have to run the mva id producer to get the value maps
// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MultivariateElectronIdentificationRun2#MVA_producer_based_recipe_AN1
vector<pat::Electron> MiniAODHelper::GetElectronsWithMVAid(edm::Handle<edm::View<pat::Electron> > electrons, edm::Handle<edm::ValueMap<float> > mvaValues, edm::Handle<edm::ValueMap<int> > mvaCategories) const {
    // Loop over electrons
    vector<pat::Electron> electrons_with_id;
    for (size_t i = 0; i < electrons->size(); ++i){
	// Look up MVA id and category
	float mvaValue  = (*mvaValues)[electrons->ptrAt(i)]; //  MVA values
	int mvaCategory = (*mvaCategories)[electrons->ptrAt(i)]; // category of electron (barrel <0.8, barrel >0, endcap)
	// add mva id to output collections
	electrons_with_id.push_back(electrons->at(i));
	electrons_with_id.back().addUserFloat("mvaValue",mvaValue);
	electrons_with_id.back().addUserInt("mvaCategory",mvaCategory);
    }
    return electrons_with_id;
}
bool MiniAODHelper::InECALbarrel(const pat::Electron& iElectron) const{
    return abs(iElectron.superCluster()->position().eta()) < 1.4442;
}

bool MiniAODHelper::InECALendcap(const pat::Electron& iElectron) const{
    return abs(iElectron.superCluster()->position().eta()) > 1.5660;
}

bool MiniAODHelper::PassesMVAidPreselection(const pat::Electron& iElectron) const{
    if (iElectron.pt()<15) return false;
    if(InECALbarrel(iElectron)){
	return (iElectron.full5x5_sigmaIetaIeta() < 0.012
		&& iElectron.hcalOverEcal() < 0.09
		&& (iElectron.ecalPFClusterIso() / iElectron.pt()) < 0.37
		&& (iElectron.hcalPFClusterIso() / iElectron.pt()) < 0.25
		&& (iElectron.dr03TkSumPt() / iElectron.pt()) < 0.18
		&& fabs(iElectron.deltaEtaSuperClusterTrackAtVtx()) < 0.0095
		&& fabs(iElectron.deltaPhiSuperClusterTrackAtVtx()) < 0.065);
    }
    else if(InECALendcap(iElectron)){
	return (iElectron.full5x5_sigmaIetaIeta() < 0.033
		&& iElectron.hcalOverEcal() <0.09
		&& (iElectron.ecalPFClusterIso() / iElectron.pt()) < 0.45
		&& (iElectron.hcalPFClusterIso() / iElectron.pt()) < 0.28
		&& (iElectron.dr03TkSumPt() / iElectron.pt()) < 0.18);
    }
    else return false;
}
// returns true if electron passes above preselection and the cut corresponding to the electron-category (0: eta<0.8, 1: 0.8<eta<1.4442, 2: 1.5560<eta)
bool MiniAODHelper::PassesMVAidCuts(const pat::Electron& el, float cut0, float cut1, float cut2, bool b_requirePreselection ) const{
    if(!el.hasUserFloat("mvaValue") || !el.hasUserInt("mvaCategory")) {
	std::cout << "mvaValue or category not set, run MiniAODHelper::AddMVAidToElectrons first" << std::endl;
	return false;
    }
    if( b_requirePreselection && !PassesMVAidPreselection(el)) return false;
    bool pass=false;
    int category =el.userInt("mvaCategory");
    float value= el.userFloat("mvaValue");
//     std::cout<<el.pt()<<" "<<el.eta()<<" "<<category<<" "<<value<<std::endl;
    // the categories 0 1 and 2 are for low pT electrons.
    switch(category){
	case 0: pass=false; break;
	case 1: pass=false; break;
	case 2: pass=false; break;
        case 3: pass=value>cut0; break;
        case 4: pass=value>cut1; break;
        case 5: pass=value>cut2; break;
	
        default: std::cout << "unknown electron mva category pT eta "<< el.pt()<<" "<<el.eta() << std::endl;
    }
    return pass;
}


bool MiniAODHelper::PassesMVAid80(const pat::Electron& el) const{
    return PassesMVAidCuts(el,0.988153,0.967910,0.841729);
}

bool MiniAODHelper::PassesMVAid90(const pat::Electron& el) const{
    return PassesMVAidCuts(el,0.972153,0.922126,0.610764);
}



bool MiniAODHelper::PassesGeneralPurposeMVA2016WP80(const pat::Electron& el) const{
  const bool DO_NOT_REQUIRE_PRESELECTION = false;
  return PassesMVAidCuts(el, 0.941 , 0.899 , 0.758 , DO_NOT_REQUIRE_PRESELECTION );// Values from : https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2?rev=26
}

bool MiniAODHelper::PassesGeneralPurposeMVA2016WP90(const pat::Electron& el) const{
  const bool DO_NOT_REQUIRE_PRESELECTION = false;
  return PassesMVAidCuts(el, 0.837, 0.715, 0.357 , DO_NOT_REQUIRE_PRESELECTION );// Values from : https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2?rev=26
}


bool MiniAODHelper::PassesNonTrigMVAid80(const pat::Electron& el) const{
  const bool DO_NOT_REQUIRE_PRESELECTION = false;
  return PassesMVAidCuts(el,0.967083,0.929117,0.726311, DO_NOT_REQUIRE_PRESELECTION );// Values from : https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2?rev=26
}

bool MiniAODHelper::PassesNonTrigMVAid90(const pat::Electron& el) const{
  const bool DO_NOT_REQUIRE_PRESELECTION = false;
  return PassesMVAidCuts(el,0.913286, 0.805013, 0.358969, DO_NOT_REQUIRE_PRESELECTION ); // Values from : https://twiki.cern.ch/twiki/bin/view/CMS/MultivariateElectronIdentificationRun2?rev=26
}

void MiniAODHelper::addVetos(const reco::Candidate &cand) {
  for (unsigned int i = 0, n = cand.numberOfSourceCandidatePtrs(); i < n; ++i) {
    const reco::CandidatePtr &cp = cand.sourceCandidatePtr(i);
    if (cp.isNonnull() && cp.isAvailable()) vetos_.push_back(&*cp);
  }
}

void MiniAODHelper::clearVetos() {
  vetos_.clear();
}

float MiniAODHelper::isoSumRaw(const std::vector<const pat::PackedCandidate *> & cands, const reco::Candidate &cand, float dR, float innerR, float threshold, SelfVetoPolicy::SelfVetoPolicy selfVeto, int pdgId) const
{
  float dR2 = dR*dR, innerR2 = innerR*innerR;

  std::vector<const reco::Candidate *> vetos(vetos_);
  for (unsigned int i = 0, n = cand.numberOfSourceCandidatePtrs(); i < n; ++i) {
    if (selfVeto == SelfVetoPolicy::selfVetoNone) break;
    const reco::CandidatePtr &cp = cand.sourceCandidatePtr(i);
    if (cp.isNonnull() && cp.isAvailable()) {
      vetos.push_back(&*cp);
      if (selfVeto == SelfVetoPolicy::selfVetoFirst) break;
    }
  }

  typedef std::vector<const pat::PackedCandidate *>::const_iterator IT;
  IT candsbegin = std::lower_bound(cands.begin(), cands.end(), cand.eta() - dR, ByEta());
  IT candsend = std::upper_bound(candsbegin, cands.end(), cand.eta() + dR, ByEta());

  double isosum = 0;
  for (IT icharged = candsbegin; icharged < candsend; ++icharged) {
    // pdgId
    if (pdgId > 0 && abs((*icharged)->pdgId()) != pdgId) continue;
    // threshold
    if (threshold > 0 && (*icharged)->pt() < threshold) continue;
    // cone
    float mydr2 = reco::deltaR2(**icharged, cand);
    if (mydr2 >= dR2 || mydr2 < innerR2) continue;
    // veto
    if (std::find(vetos.begin(), vetos.end(), *icharged) != vetos.end()) {
      continue;
    }
    // add to sum
    isosum += (*icharged)->pt();
  }
  return isosum;
}



bool MiniAODHelper::checkIfRegisterd( const reco::Candidate * candidate , std::vector< const reco::Candidate * > list ){

  for ( std::vector< const reco::Candidate * >::iterator it = list.begin() ;
	it != list.end() ;
	it ++ ){
    if( candidate == * it  ) return true  ;
  }

  return false ;

}


const reco::Candidate * MiniAODHelper::GetObjectJustBeforeDecay( const reco::Candidate * particle ){

  for ( unsigned int i = 0 ; i <  particle -> numberOfDaughters(); i++ ){
    if( particle -> daughter( i ) -> pdgId()  ==  particle -> pdgId() ){

      return GetObjectJustBeforeDecay( particle -> daughter (i) );

    } // end if
  } // end for

  return particle ;

}


void MiniAODHelper::FillTopQuarkDecayInfomration ( const reco::Candidate * c ,
						   struct _topquarkdecayobjects * topdecayobjects) {

  topdecayobjects -> top  = c ;
  topdecayobjects -> isWChild_tau = false ;

  c = GetObjectJustBeforeDecay( c );

  for ( unsigned int i = 0 ; i <  c -> numberOfDaughters(); i++ ){
    if( abs( c -> daughter( i ) -> pdgId() ) == 5 ||
	abs( c -> daughter( i ) -> pdgId() ) == 3 ||
	abs( c -> daughter( i ) -> pdgId() ) == 1 ){
      topdecayobjects -> bottom = c -> daughter( i );
    }
    if( abs( c -> daughter( i ) -> pdgId() ) == 24 ){
      topdecayobjects -> W = c -> daughter( i );
    }
  }


  const reco::Candidate * W = topdecayobjects -> W ;

  // (case-1) In some MC, W boson decays but stays (example : W -> u+d+W)
  // (case-2) In other MC, W boson decays after some step (example : W->W->u+d)
  //  In order to handle both case,
  //   - check if the W boson has fermion in ites daughter.
  //      -> if so (=case 1), this is the W boson to see.
  //      -> if not(=case 2), trackdown the W boson
  bool W_boson_decays  = false ;
  for ( unsigned int i = 0 ; i <  W -> numberOfDaughters(); i++ ){
    if(       abs( W -> daughter( i ) -> pdgId() ) == 6
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 4
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 2
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 12
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 14
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 16
	      || abs( W -> daughter( i ) -> pdgId() ) == 5
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 3
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 1
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 11
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 13
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 15 ){
      W_boson_decays = true ;
    } // end if
  }// end for-loop
  if( ! W_boson_decays ){
    W = GetObjectJustBeforeDecay( W ) ;
  }


  for ( unsigned int i = 0 ; i <  W -> numberOfDaughters(); i++ ){

    if(       abs( W -> daughter( i ) -> pdgId() ) == 6
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 4
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 2
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 12
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 14
	      ||  abs( W -> daughter( i ) -> pdgId() ) == 16 ){
      // --- up type
      topdecayobjects -> WChild_up   =  W -> daughter( i );
    }else if ( abs( W -> daughter( i ) -> pdgId() ) == 5
	       ||  abs( W -> daughter( i ) -> pdgId() ) == 3
	       ||  abs( W -> daughter( i ) -> pdgId() ) == 1
	       ||  abs( W -> daughter( i ) -> pdgId() ) == 11
	       ||  abs( W -> daughter( i ) -> pdgId() ) == 13
	       ||  abs( W -> daughter( i ) -> pdgId() ) == 15 ){
      // --- down type
      topdecayobjects -> WChild_down =   W -> daughter( i );
    }

    if( abs( W -> daughter( i ) -> pdgId() ) == 15 ){
      topdecayobjects ->  isWChild_tau = true ;
    }

  }// W boson loop


  if( ! ( topdecayobjects ->  isWChild_tau ) ) return ;

  const reco::Candidate * tau = topdecayobjects -> WChild_down ;
  // - - - -
  // track down until the Tau lepton decays
  // - - - -
  for( bool ready = false; ! ready ; ){
    ready = true ;
    for ( unsigned int i = 0 ; i <  tau -> numberOfDaughters(); i++ ){
      if( abs( tau -> daughter( i ) -> pdgId() ) == 15 ){
	ready = false;
	tau = tau -> daughter( i ) ;
	break ;
      }
    }

  }

  for ( unsigned int i = 0 ; i <  tau -> numberOfDaughters(); i++ ){

    if ( abs( tau -> daughter( i ) -> pdgId() ) == 16 ){
      topdecayobjects -> Tau_Neu =  GetObjectJustBeforeDecay( tau -> daughter( i ) ) ;
    }else{
      topdecayobjects -> TauChildren.push_back( GetObjectJustBeforeDecay ( tau -> daughter( i ) ) );
    }

  }// Tau loop

}




MiniAODHelper::TTbarDecayMode MiniAODHelper::GetTTbarDecay(edm::Handle<std::vector<reco::GenParticle> >& mcparticles,
							   TLorentzVector * topquark ,
							   TLorentzVector * antitopquark ){

  struct _topquarkdecayobjects topPosDecay = { };
  struct _topquarkdecayobjects topNegDecay = { };

  std::vector<const reco::Candidate * > idx_top_pos ;
  std::vector<const reco::Candidate * > idx_top_neg ;

  for(size_t i=0; i<mcparticles->size();i++){

    if( abs( (*mcparticles)[i].pdgId()  ) == 6 ){
      const reco::Candidate * cand =  & (*mcparticles)[i] ;
      cand = GetObjectJustBeforeDecay ( cand );

      if ( cand -> pdgId() == 6  && !  checkIfRegisterd( cand , idx_top_pos ) ){
	idx_top_pos  . push_back( cand );
	FillTopQuarkDecayInfomration ( cand ,
				       & topPosDecay ) ;
      }

      if ( cand -> pdgId() == - 6 && !  checkIfRegisterd( cand , idx_top_neg ) ){
	idx_top_neg  . push_back( cand );
	FillTopQuarkDecayInfomration ( cand ,
				       & topNegDecay ) ;
      }

    } // end if : |PDGID|==6

  }// end mcparticles-Loop.

  if( idx_top_pos.size() != 1 || idx_top_neg.size() != 1 ) return ChNotDefined ;

  if( topquark !=0 ){
    topquark -> SetPtEtaPhiM( topPosDecay.top->pt(),
			      topPosDecay.top->eta(),
			      topPosDecay.top->phi(),
			      topPosDecay.top->mass());
  }
  if(antitopquark != 0 ){
    antitopquark -> SetPtEtaPhiM( topNegDecay.top->pt(),
				  topNegDecay.top->eta(),
				  topNegDecay.top->phi(),
				  topNegDecay.top->mass());
  }
  if( (   topPosDecay . isLeptonicDecay() ) && ( ! topNegDecay . isLeptonicDecay() ) ) return SingleLepCh ;
  if( ( ! topPosDecay . isLeptonicDecay() ) && (   topNegDecay . isLeptonicDecay() ) ) return SingleLepCh ;
  if( (   topPosDecay . isLeptonicDecay() ) && (   topNegDecay . isLeptonicDecay() ) ) return DiLepCh ;
  if( ( ! topPosDecay . isLeptonicDecay() ) && ( ! topNegDecay . isLeptonicDecay() ) ) return FullHadCh ;

  return ChNotDefined ;

}


    ///////////////////
    /// Higgs Decay ///
    ///////////////////

int MiniAODHelper::GetHiggsDecay(edm::Handle<std::vector<reco::GenParticle> >& mcparticles){

  int Hdecay = -1;

  if( mcparticles.isValid() ){

    Hdecay=0;

    for( size_t k = 0; k < mcparticles->size(); k++ ){
      const reco::Candidate & mcParticle = (*mcparticles)[k];

      int status = mcParticle.status();
      int pdgId  = mcParticle.pdgId();
      int absId  = abs( pdgId );
      int numdgt = mcParticle.numberOfDaughters();

      //// must be a Higgs and status 62(pythia 8)
      if( absId!=25 || status!=62  ) continue;
      if (!(numdgt>1))continue;

      int d0=-99, d1=-99;

      int ind0 = 0;
      int ind1 = 1;

      if( numdgt>2 ){
	if( mcParticle.daughter(0)->pdgId()==pdgId ){
	  ind0 = 1;
	  ind1 = 2;
	}
	if( mcParticle.daughter(1)->pdgId()==pdgId ){
	  ind0 = 0;
	  ind1 = 2;
	}
      }

      d0 = mcParticle.daughter(ind0)->pdgId();
      d1 = mcParticle.daughter(ind1)->pdgId();

      d0 = abs(d0);
      d1 = abs(d1);

      if( d0==5 && d1==5 ) Hdecay = 1;  //bb
      if( d0==24 && d1==24) Hdecay = 2;   //WW
      if( d0==15 && d1==15) Hdecay = 3;  //TauTau
      if( d0==21 && d1==21) Hdecay = 4;  //glueglue
      if( d0==4 && d1==4 ) Hdecay = 5;  //cc
      if( d0==23 && d1==23) Hdecay = 6;  //ZZ

      if( d0==22 && d1==23) Hdecay = 7;  //Zy
      if( d0==23 && d1==22) Hdecay = 7;  //Zy

      if( d0==22 && d1==22) Hdecay = 8;  //yy

      if( d0==21 && d1==22) Hdecay = 9; //gy
      if( d0==22 && d1==21) Hdecay = 9; //gy
      if( d0==3 && d1==3) Hdecay = 10; //ss

      if( d0==13 && d1==13) Hdecay = 11; //mumu

      if( (Hdecay==0) && (d0==22 || d1==22)) Hdecay =12; //?y
      if( (Hdecay==0) && (d0==21 || d1==21)) Hdecay = 13; //?g
      if( (Hdecay==0) && (d0>100 || d1>100)) Hdecay = 14; //?Hadron

      if( d0==1 && d1==1) Hdecay = 15; //uu
      if( d0==2 && d1==2) Hdecay = 16; //dd
      if( d0==6 && d1==6) Hdecay = 17; //tt
      if( d0==11 && d1==11) Hdecay = 18; //ee

    }

  }


  return Hdecay;
}




