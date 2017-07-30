import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.GenJetParticles_cff import *

#Use this only to produce the jet and lepton collections
genParticlesForFiducial = cms.EDProducer(
    "IIHEParticleLevelMCProducer",
    genParticlesSource = cms.InputTag("prunedGenParticles"),
    genJetsSource = cms.InputTag("slimmedGenJets"),
    ignoreParticleIDs=genParticlesForJets.ignoreParticleIDs,
#    excludeFromResonancePids=genParticlesForJets.excludeFromResonancePids
    excludeFromResonancePids=cms.vuint32(11,12,13,14,16),
    #now only doing genJets selection:
    useJetsNoNu = cms.untracked.bool(True),
    doBaseCollections = cms.untracked.bool(True),
    doDressedCollections = cms.untracked.bool(True),
    doRescaledBHadronJetSelection  = cms.untracked.bool(True),
    doBDescendentJetSelection = cms.untracked.bool(False),
)


#############################################

######### EdmNtuples production ##############
from RecoJets.JetProducers.ak5GenJets_cfi import *

ak1DressedElectrons = ak5GenJets.clone(
    src= cms.InputTag("genParticlesForFiducial","genEGammaForDressing"), 
    rParam       = cms.double(0.1)
    )
ak1DressedMuons = ak5GenJets.clone(
    src= cms.InputTag("genParticlesForFiducial","genMuGammaForDressing"), 
    rParam       = cms.double(0.1)
    )

ak1DressedLeptons = ak5GenJets.clone(
    src= cms.InputTag("genParticlesForFiducial","genLeptonsForDressing"), 
    rParam       = cms.double(0.1)
    )

#from TopQuarkAnalysis.IIHE.IIHEProducers_cff import PDFInfo

FiducialSeq = cms.Sequence (
    genParticlesForFiducial *
    ak1DressedLeptons       *
    ak1DressedMuons         *
    ak1DressedElectrons 
    )
