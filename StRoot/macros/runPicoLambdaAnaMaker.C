
#ifndef __CINT__
#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"

#include "StMaker.h"
#include "StChain.h"

#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoHFMaker/StPicoHFEvent.h"
#include "StPicoHFMaker/StHFCuts.h"

#include "macros/loadSharedHFLibraries.C"

#include <iostream>
#include <ctime>
#include <cstdio>

#include "StPicoLambdaAnaMaker/StPicoLambdaAnaMaker.h" //kvapil

using namespace std;

#else
class StChain;
#endif

StChain *chain;

void runPicoLambdaAnaMaker(const Char_t *inputFile="test.list", const Char_t *outputFile="outputBaseName",  
    const unsigned int makerMode = 0 /*kAnalyze*/,
    const Char_t *badRunListFileName = "picoList_bad_MB.list", const Char_t *treeName = "picoHFtree",
    const Char_t *productionBasePath = "/star/data100/reco/AuAu_200_production_2016/ReversedFullField/P16ij/2016",
    const unsigned int decayChannel = 0 /* kChannel0 */) 
{ 
  // -- Check STAR Library. Please set SL_version to the original star library used in the production 
  //    from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  string SL_version = "SL21d"; //originally SL16j, not available at RCF any more
  string env_SL = getenv ("STAR");
  if (env_SL.find(SL_version)==string::npos) {
    cout<<"Environment Star Library does not match the requested library in runPicoHFMyAnaMaker.C. Exiting..."<<endl;
    exit(1);
  }

#ifdef __CINT__
  gROOT->LoadMacro("loadSharedHFLibraries.C");
  loadSharedHFLibraries();
#endif


  chain = new StChain();

  // ========================================================================================
  //makerMode    = StPicoHFMaker::kAnalyze;
  // ========================================================================================

  cout << "Maker Mode    " << makerMode << endl;
  cout << "TreeName      " << treeName << endl; 
  cout << "Decay Channel " << decayChannel << endl; 

  TString sInputFile(inputFile);
  TString sInputListHF("");  
  TString sProductionBasePath(productionBasePath);
  TString sTreeName(treeName);

  if (makerMode == StPicoHFMaker::kAnalyze) {
    if (!sInputFile.Contains(".list") && !sInputFile.Contains("picoDst.root")) {
      cout << "No input list or picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFMaker::kWrite) {
    if (!sInputFile.Contains("picoDst.root")) {
      cout << "No input picoDst root file provided! Exiting..." << endl;
      exit(1);
    }
  }
  else if (makerMode == StPicoHFMaker::kRead) {
    if (!sInputFile.Contains(".list")) {
      cout << "No input list provided! Exiting..." << endl;
      exit(1);
    }

    // -- prepare filelist for picoDst from trees
    sInputListHF = sInputFile;
    sInputFile = "tmpPico.list";

    TString command = "sed 's|" + sTreeName + ".root|picoDst.root|g' " + sInputListHF + " > " + sInputFile;
    cout << "COMMAND : " << command << endl; 
    gSystem->Exec(command.Data());

    command = "sed -i 's|^.*" + sTreeName + "|" + sProductionBasePath + "|g' " + sInputFile; // + " > " + sInputFile;
    cout << "COMMAND : " << command << endl; 
    gSystem->Exec(command.Data());
  }
  else {
    cout << "Unknown makerMode! Exiting..." << endl;
    exit(1);
  }
  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(StPicoDstMaker::IoRead, sInputFile, "picoDstMaker"); //for local testing only
  StPicoLambdaAnaMaker* picoLambdaAnaMaker = new StPicoLambdaAnaMaker("picoLambdaAnaMaker", picoDstMaker, outputFile, sInputListHF);
  picoLambdaAnaMaker->setMakerMode(makerMode);
  picoLambdaAnaMaker->setDecayChannel(StPicoLambdaAnaMaker::kChannel1);//not needed?
  picoLambdaAnaMaker->setTreeName(treeName);


  StHFCuts* hfCuts = new StHFCuts("hfBaseCuts");
  picoLambdaAnaMaker->setHFBaseCuts(hfCuts);

  // ---------------------------------------------------
  // -- Set Base cuts for HF analysis

  // -- File name of bad run list
  hfCuts->setBadRunListFileName(badRunListFileName);

  // -- ADD USER CUTS HERE ----------------------------
  hfCuts->setCutVzMax(30.);
  hfCuts->setCutVzVpdVzMax(3.);

  //Run17 SL21d triggers
  hfCuts->addTriggerId(570001);    // VPDMB-30 (st_physics)

  hfCuts->setCutNHitsFitMin(20); //for analysis (TTree)
  hfCuts->setCutNHitsFitMinHist(20); //for histograms

  hfCuts->setCutRequireHFT(false);

  hfCuts->setCutNHitsFitnHitsMax(0.52); 

  // ---------------------------------------------------

  picoLambdaAnaMaker->setDecayMode(StPicoHFEvent::kTwoParticleDecay); //to setup secondary vertices as StHFPair


  hfCuts->setCutEta(1.);

  hfCuts->setCutDcaMin(0.7,StHFCuts::kPion);
  //hfCuts->setCutDcaMin(0.01,StHFCuts::kKaon); 
  hfCuts->setCutDcaMin(0.1,StHFCuts::kProton); 

  //-----------SECONDARY PAIR CUTS----------------------------
  float dcaDaughters12Max;
  float decayLengthMin, decayLengthMax;
  float cosThetaMin, massMin, massMax;

  dcaDaughters12Max = 1.;

  decayLengthMin = 2.;
  decayLengthMax = 10.;

  cosThetaMin = 0.95;

  massMin = 0.9;
  massMax = 1.3;

  hfCuts->setCutSecondaryPair(dcaDaughters12Max, decayLengthMin, decayLengthMax, cosThetaMin, massMin, massMax);
  // --- Lomnitz cuts to remove noise from ghosting
  //------------------------------------------------------------

  //Single track pt
  hfCuts->setCutPtRange(0.15,20.0,StHFCuts::kPion);
  hfCuts->setCutPtRange(0.15,20.0,StHFCuts::kKaon); //for QA histograms
  hfCuts->setCutPtRange(0.15,20.0,StHFCuts::kProton);
  //___________________________________________________________________________________________________________

  
  //TPC setters
  hfCuts->setCutTPCNSigmaPion(3.0);
  hfCuts->setCutTPCNSigmaKaon(2.0); //for QA histograms
  hfCuts->setCutTPCNSigmaProton(2.0);

  //for QA histograms
  hfCuts->setCutTPCNSigmaHadronHist(1.0, 1); //1 = pion
  hfCuts->setCutTPCNSigmaHadronHist(1.0, 2); //2 = kaon - for QA histograms
  hfCuts->setCutTPCNSigmaHadronHist(1.0, 3); //3 = proton

  //TOF setters, need to set pt range as well
  hfCuts->setCutTOFDeltaOneOverBeta(0.03, StHFCuts::kProton);
  hfCuts->setCutPtotRangeHybridTOF(0.15,20.0,StHFCuts::kProton); //for TMVA

  hfCuts->setCutTOFDeltaOneOverBeta(0.03, StHFCuts::kPion);
  hfCuts->setCutPtotRangeHybridTOF(0.15,20.0,StHFCuts::kPion); //for TMVA

  // ========================================================================================

  chain->Init();
  cout << "chain->Init();" << endl;
  int nEvents = picoDstMaker->chain()->GetEntries();
  cout << " Total entries = " << nEvents << endl;

  for (Int_t i=0; i<nEvents; i++) {
    if(i%10000==0)
      cout << "Working on eventNumber " << i << endl;

    chain->Clear();
    int iret = chain->Make(i);

    if (iret) { cout << "Bad return code!" << iret << endl; break;}

    //total++;
  }

  cout << "****************************************** " << endl;
  cout << "Work done... now its time to close up shop!"<< endl;
  cout << "****************************************** " << endl;
  chain->Finish();

  cout << "****************************************** " << endl;
  cout << "total number of events  " << nEvents << endl;
  cout << "****************************************** " << endl;

  cout << "****************************************** " << endl;

  delete chain;

  // -- clean up if in read mode
  if (makerMode == StPicoHFMaker::kRead)
    gSystem->Exec(Form("rm -f %s", sInputFile.Data()));
}

