# In order to run the code for MC/DATA on lxplus
#[cmsRun IIHE.py grid=False DataProcessing="mc2016" DataFormat="mc" dataset="RunIISummer16MiniAODv2" sample="ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1" address="MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000" file="0E4CD60A-0FC3-E611-BCB5-0CC47A7C3420.root"  ]
#root://eoscms//cms/store/mc/RunIISummer16MiniAODv2/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/50000/36CDAE89-B3BE-E611-B022-0025905B8604.root

# In order to run the code for DATA on lxplus
#[cmsRun IIHE.py grid=False DataProcessing="rerecodata" DataFormat="data" dataset="Run2016D" sample="DoubleEG" address="MINIAOD/03Feb2017-v1/100000" file="CC7F39AC-F5EA-E611-A125-0CC47A13CDB0.root"  ]
#root://eoscms//cms/store/data/Run2015D/DoubleEG/MINIAOD/16Dec2015-v2/00000/F6E918C9-87A6-E511-B3D3-0CC47A4D76B2.root

import sys
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as opts
import copy
import os

options = opts.VarParsing ("analysis")
options.register("sample",
                 "ST_tW_top_5f_NoFullyHadronicDecays_13TeV-powheg_TuneCUETP8M1", 
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 "Sample to analyze")
options.register("address",
                 "MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000",
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 "address of sample in eos like: MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000")
options.register("file",
                 "0E4CD60A-0FC3-E611-BCB5-0CC47A7C3420.root",
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 "file to analyze")
options.register("DataProcessing",
                 "mc2016",
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 "Data processing types. Options are:mc2016,rerecodata,promptdata")
options.register("dataset",
                 "RunIISummer16MiniAODv2",
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 "datasets to analyze: RunIISummer16MiniAODv2, Run2016B-H")
options.register("DataFormat",
                 "mc",
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.string,
                 "Data format. Options are:mc,data")
options.register('grid',
                 True,
                 opts.VarParsing.multiplicity.singleton,
                 opts.VarParsing.varType.bool,
                 'If you run on grid or localy on eos')
options.parseArguments()

###########################################################################################
#os.system('wget https://github.com/rgoldouz/miniAOD_IIHE/archive/CMSSW_8_0_26_patch1.zip')
#os.system("unzip -a " + "CMSSW_8_0_26_patch1.zip" )
#os.system("mv " + "miniAOD_IIHE-CMSSW_8_0_26_patch1/data" + " ..")
#os.system("rm -rf CMSSW_8_0_26_patch1.zip miniAOD_IIHE-CMSSW_8_0_26_patch1")
##########################################################################################
#                                      Global tags                                       #
##########################################################################################
globalTag = "80"
if options.DataProcessing == "mc2016":
  globalTag = "80X_mcRun2_asymptotic_2016_TrancheIV_v8"
if options.DataProcessing == "rerecodata":
  globalTag = "80X_dataRun2_2016SeptRepro_v7"
if options.DataProcessing == "promptdata":
  globalTag = "80X_dataRun2_Prompt_v16"
if options.DataProcessing == "data2017":
  globalTag = "92X_dataRun2_Prompt_v4"
##########################################################################################
#                                  Start the sequences                                   #
##########################################################################################

process = cms.Process("IIHEAnalysis")

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')

process.GlobalTag.globaltag = globalTag
print "Global Tag is ", process.GlobalTag.globaltag
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

##########################################################################################
#                                         Files                                          #
##########################################################################################
if options.DataFormat == "mc":
  path = "root://eoscms//eos/cms/store/"+ options.DataFormat + "/" + options.dataset  + "/" + options.sample + "/" + options.address + "/" + options.file

if options.DataFormat == "data":
  path = "root://eoscms//eos/cms/store/"+ options.DataFormat + "/" + options.dataset  + "/" + options.sample + "/" + options.address + "/" + options.file

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(),
#    eventsToProcess = cms.untracked.VEventRange('1:19792:3958249')
)
#process.source.fileNames.append( path )
#process.source.fileNames.append( "file:03Feb2017data.root" )
#process.source.fileNames.append( "file:TW_80_miniAOD.root" )
#process.source.fileNames.append( "file:2017data.root" )
process.source.fileNames.append( "file:AOD.root" )
###
filename_out = "outfile.root"
if options.DataFormat == "mc" and not options.grid:
#  filename_out = "file:/tmp/output_%s" % (options.sample + "_" + options.file)
  filename_out = "outfile_MC_"+options.file+".root"
if options.DataFormat == "data" and not options.grid:
  filename_out = "outfile_Data.root"

#filename_out = "outfile.root"
process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string(filename_out) )
process.TFileService = cms.Service("TFileService", fileName = cms.string(filename_out) )

##########################################################################################
#                                   IIHETree options                                     #
##########################################################################################

#Track isolation correction value for HEEP v7
#process.load("RecoEgamma.ElectronIdentification.heepIdVarValueMapProducer_cfi")
#EGamma VID for various working points
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.AOD
switchOnVIDElectronIdProducer(process, dataFormat)
my_id_modules = ["RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff",
                 "RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff"]
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)
#For fiducial study we need object at particle level
#process.load("UserCode.IIHETree.IIHEFiducialModules_cff")

##########################################################################################
#                            MY analysis input!                              #
##########################################################################################
process.load("UserCode.IIHETree.IIHETree_cfi")
process.IIHEAnalysis.globalTag = cms.string(globalTag)
process.IIHEAnalysis.isData  = cms.untracked.bool("data" in options.DataProcessing)
process.IIHEAnalysis.isMC    = cms.untracked.bool("mc" in options.DataProcessing)
#process.IIHEAnalysis.isMC    = cms.untracked.bool(False)
#****Collections added before the analysis
# VID output
process.IIHEAnalysis.eleTrkPtIsoLabel                            = cms.InputTag("heepIDVarValueMaps"    ,"eleTrkPtIso"       ,"IIHEAnalysis" )
process.IIHEAnalysis.VIDVeto                                     = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto"  )
process.IIHEAnalysis.VIDLoose                                    = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose" )
process.IIHEAnalysis.VIDMedium                                   = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium")
process.IIHEAnalysis.VIDTight                                    = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight" )
process.IIHEAnalysis.VIDmvaEleIDwp90                             = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp90" )
process.IIHEAnalysis.VIDmvaEleIDwp80                             = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp80" )
process.IIHEAnalysis.VIDHEEP7                                    = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV70"                   )
# Collections for DATA only.
process.IIHEAnalysis.particleFlowEGammaGSFixedCollection         = cms.InputTag("particleFlowEGammaGSFixed", "dupECALClusters"              )
process.IIHEAnalysis.ecalMultiAndGSGlobalRecHitEBCollection      = cms.InputTag("ecalMultiAndGSGlobalRecHitEB","dupESClusters"        ,"PAT")
process.IIHEAnalysis.METsMuEGCleanCollection                     = cms.InputTag("slimmedMETsMuEGClean"                                      )
process.IIHEAnalysis.discardedMuonCollection                     = cms.InputTag("packedPFCandidatesDiscarded"                               )

#use 80 regression + scale/smearing for electron
process.IIHEAnalysis.electronCollection80    = cms.InputTag("calibratedPatElectrons","","IIHEAnalysis")

#jet smeared collection
process.IIHEAnalysis.JetCollection                   = cms.InputTag("basicJetsForMet" ,"","IIHEAnalysis")
process.IIHEAnalysis.JetCollectionSmeared            = cms.InputTag("patSmearedJets"               ,"","IIHEAnalysis")
process.IIHEAnalysis.JetCollectionEnUp               = cms.InputTag("shiftedPatJetEnUp"            ,"","IIHEAnalysis")
process.IIHEAnalysis.JetCollectionEnDown             = cms.InputTag("shiftedPatJetEnDown"          ,"","IIHEAnalysis")
process.IIHEAnalysis.JetCollectionSmearedJetResUp    = cms.InputTag("shiftedPatSmearedJetResUp"    ,"","IIHEAnalysis")
process.IIHEAnalysis.JetCollectionSmearedJetResDown  = cms.InputTag("shiftedPatSmearedJetResDown"  ,"","IIHEAnalysis")

#MET collections
process.IIHEAnalysis.patPFMetCollection                        = cms.InputTag("patPFMet"                  , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1Collection                      = cms.InputTag("patPFMetT1"                , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1JetEnDownCollection             = cms.InputTag("patPFMetT1JetEnDown"       , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1JetEnUpCollection               = cms.InputTag("patPFMetT1JetEnUp"         , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1SmearCollection                 = cms.InputTag("patPFMetT1Smear"           , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1SmearJetEnDownCollection        = cms.InputTag("patPFMetT1SmearJetEnDown"  , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1SmearJetEnUpCollection          = cms.InputTag("patPFMetT1SmearJetEnUp"    , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1SmearJetResDownCollection       = cms.InputTag("patPFMetT1SmearJetResDown" , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1SmearJetResUpCollection         = cms.InputTag("patPFMetT1SmearJetResUp"   , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetT1TxyCollection                   = cms.InputTag("patPFMetT1Txy"             , ""                ,"IIHEAnalysis"  )
process.IIHEAnalysis.patPFMetFinalCollection                   = cms.InputTag("slimmedMETs"               , ""                ,"IIHEAnalysis"  )

#particle level fiducial collections
process.IIHEAnalysis.particleLevelJetsCollection               = cms.InputTag("pseudoTop"       , "jets"     ,"IIHEAnalysis"  )
process.IIHEAnalysis.particleLevelak1DressedLeptonCollection   = cms.InputTag("pseudoTop"       , "leptons"  ,"IIHEAnalysis"  )
process.IIHEAnalysis.particleLevelMETCollection                = cms.InputTag("pseudoTop"       , "mets","IIHEAnalysis"  )



process.IIHEAnalysis.includeLeptonsAcceptModule  = cms.untracked.bool(True)
process.IIHEAnalysis.includeTriggerModule        = cms.untracked.bool(True)
process.IIHEAnalysis.includeEventModule          = cms.untracked.bool(True)
#process.IIHEAnalysis.includeVertexModule         = cms.untracked.bool(True)
process.IIHEAnalysis.includeElectronModule       = cms.untracked.bool(True)
process.IIHEAnalysis.includeMuonModule           = cms.untracked.bool(True)
#process.IIHEAnalysis.includeMETModule            = cms.untracked.bool(True)
#process.IIHEAnalysis.includeJetModule            = cms.untracked.bool(True)
#process.IIHEAnalysis.includeTauModule            = cms.untracked.bool(True)
#process.IIHEAnalysis.includeMCTruthModule        = cms.untracked.bool("mc" in options.DataProcessing)
#process.IIHEAnalysis.includeLHEWeightModule        = cms.untracked.bool(True)
#process.IIHEAnalysis.includeDataModule            = cms.untracked.bool("data" in options.DataProcessing)


#process.IIHEAnalysis.includeAutoAcceptEventModule                = cms.untracked.bool(True)
##########################################################################################
#                            Woohoo!  We"re ready to start!                              #
##########################################################################################
#process.p1 = cms.Path(process.kt6PFJetsForIsolation+process.IIHEAnalysis)
#process.out = cms.OutputModule(
#    "PoolOutputModule",
#    fileName = cms.untracked.string("EDM.root")
#    )

process.p1 = cms.Path(
    process.egmGsfElectronIDSequence * 
    process.IIHEAnalysis
    )

#process.outpath = cms.EndPath(process.out)

