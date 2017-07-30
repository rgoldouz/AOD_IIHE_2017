import FWCore.ParameterSet.Config as cms
import getpass, os
pwd = os.getcwd()
IIHEAnalysis = cms.EDAnalyzer("IIHEAnalysis",
    # Collections for DATA and MC.
    triggerResultsCollectionHLT                 = cms.InputTag("TriggerResults"        , ""                          ,"HLT" ),
    triggerResultsCollectionPAT                 = cms.InputTag("TriggerResults"        , ""                          ,"RECO"),
    triggerEvent                                = cms.InputTag("hltTriggerSummaryAOD"  , ""                          ,"HLT" ),
    photonCollection                            = cms.InputTag("slimmedPhotons"                                             ),
    electronCollection                          = cms.InputTag("gedGsfElectrons"       , ""                          ,"RECO"),
    muonCollection                              = cms.InputTag("muons"                                               ),
    METCollection                               = cms.InputTag("slimmedMETs"                                                ),
    JetCollection                               = cms.InputTag("slimmedJets"                                                ),
    tauCollection                               = cms.InputTag("hpsPFTauProducer"       ,""                          ,"RECO"),
    l1NonIsoCollection                          = cms.InputTag("caloStage2Digis"        ,"EGamma"                    ,"RECO"),
    superClusterCollection                      = cms.InputTag("reducedEgamma"         , "reducedSuperClusters"             ),
    eventRho                                    = cms.InputTag("fixedGridRhoFastjetAll"                                     ),
    ebReducedRecHitCollection                   = cms.InputTag("reducedEcalRecHitsEB"),
    eeReducedRecHitCollection                   = cms.InputTag("reducedEcalRecHitsEE"),
    esReducedRecHitCollection                   = cms.InputTag("reducedEcalRecHitsES"),
    PileUpSummaryInfo                           = cms.InputTag("slimmedAddPileupInfo"                                       ),
    primaryVertex                               = cms.InputTag("offlineSlimmedPrimaryVertices"                              ),
    beamSpot                                    = cms.InputTag("offlineBeamSpot"                                            ),
    generatorLabel                              = cms.InputTag("generator"                                                  ),
    genParticleSrc                              = cms.InputTag("prunedGenParticles"                                         ),
    LHELabel                                    = cms.InputTag("externalLHEProducer"                                        ),
    genJetsCollection                           = cms.InputTag("slimmedGenJets"),
    #*******************************************************************************************************************************************
    #Trigger paths that we want to save
#    triggers                                    = cms.untracked.string("singleElectron;doubleElectron;singleMuon;singlePhoton;singleElectronSingleMuon;doubleMuon"),
    triggers                                    = cms.untracked.string("singleElectron;doubleElectron;singleMuon;singlePhoton;singleElectronSingleMuon;doubleMuon;MET"),
    globalTag                                   = cms.string(""),
    
    # Trigger matching stuff.  0.5 should be sufficient.
    muon_triggerDeltaRThreshold                 = cms.untracked.double(0.5),
    HEEP_triggerDeltaRThreshold                 = cms.untracked.double(0.5),
    
    # In the absence of high ET electrons, only save events with really high Z candidates.
    ZBosonZMassAcceptLower                      = cms.untracked.double(850),
    # Don"t bother with J/psi or Upsilon, they will only weigh us down!
    ZBosonJPsiAcceptMassLower                   = cms.untracked.double(1e6),
    ZBosonJPsiAcceptMassUpper                   = cms.untracked.double(1e6),
    ZBosonUpsAcceptMassLower                    = cms.untracked.double(1e6),
    ZBosonUpsAcceptMassUpper                    = cms.untracked.double(1e6),
    
    # But make sure we save Z bosons from 50 GeV and up.
    ZBosonZMassLowerCuttoff                     = cms.untracked.double( 50),
    ZBosonDeltaRCut                             = cms.untracked.double(1e-3),
    
    # Only save Z->ee, Z->em.
    ZBosonEtThreshold                           = cms.untracked.double(15),
    ZBosonSaveZee                               = cms.untracked.bool(False ),
    ZBosonSaveZmm                               = cms.untracked.bool(False ),
    ZBosonSaveZem                               = cms.untracked.bool(False ),
    ZBosonSaveZeeg                              = cms.untracked.bool(False),
    ZBosonSaveZmmg                              = cms.untracked.bool(False),
    
    # Set pt or mass thresholds for the truth module here
    # Setting thresholds reduces the size of the output files significantly
    MCTruth_ptThreshold                         = cms.untracked.double(10.0),
    MCTruth_mThreshold                          = cms.untracked.double(20.0),
    MCTruth_DeltaROverlapThreshold              = cms.untracked.double(0.001),
    # IMPORTANT         ****SKIM OBJECT****
    electronPtThreshold                         = cms.untracked.double(15),
    muonPtThreshold                             = cms.untracked.double(15),
    photonPtThreshold                           = cms.untracked.double(15),
    jetPtThreshold                              = cms.untracked.double(20),
    tauPtTThreshold                             = cms.untracked.double(15),
    
    # IMPORTANT         ****SKIM EVENT****
    leptonsAcceptPtThreshold                    = cms.untracked.double(15),
    leptonsAccept_nEle                          = cms.untracked.int32(2),
    leptonsAccept_nEleMu                        = cms.untracked.int32(2),
    leptonsAccept_nEleTau                       = cms.untracked.int32(2),
    leptonsAccept_nMu                           = cms.untracked.int32(2),
    leptonsAccept_nMuTau                        = cms.untracked.int32(2),
    leptonsAccept_nTau                          = cms.untracked.int32(999),
    #***********************************************************************
    
    #tell the code if you are running on data or MC
    isData                                      = cms.untracked.bool(False),
    isMC                                        = cms.untracked.bool(False),
    #**********************************************************************
    
    includeLeptonsAcceptModule                  = cms.untracked.bool(False),
    includeTriggerModule                        = cms.untracked.bool(False),
    includeEventModule                          = cms.untracked.bool(False),
    includeVertexModule                         = cms.untracked.bool(False),
    includePhotonModule                         = cms.untracked.bool(False),
    includeElectronModule                       = cms.untracked.bool(False),
    includeMuonModule                           = cms.untracked.bool(False),
    includeMETModule                            = cms.untracked.bool(False),
    includeJetModule                            = cms.untracked.bool(False),
    includeTauModule                            = cms.untracked.bool(False),
    includeL1Module                             = cms.untracked.bool(False),
    includeParticleLevelObjectsModule           = cms.untracked.bool(False),
    includeZBosonModule                         = cms.untracked.bool(False),
    includeSuperClusterModule                   = cms.untracked.bool(False),
    includeTracksModule                         = cms.untracked.bool(False),
    includeMCTruthModule                        = cms.untracked.bool(False),
    includeLHEWeightModule                        = cms.untracked.bool(False),
    includeDataModule                           = cms.untracked.bool(False),
    
    #change it to true if you want to save all events
    includeAutoAcceptEventModule                = cms.untracked.bool(False),
    debug                                       = cms.bool(False)
    )
