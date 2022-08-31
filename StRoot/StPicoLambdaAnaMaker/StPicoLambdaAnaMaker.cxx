#include "StPicoLambdaAnaMaker.h"
//#include "StPicoHFMaker/StHFCuts.h"
#include <iostream>
#include <cmath>

ClassImp(StPicoLambdaAnaMaker)

using namespace std;

// _________________________________________________________
StPicoLambdaAnaMaker::StPicoLambdaAnaMaker(char const* name, StPicoDstMaker* picoMaker, char const* outputBaseFileName,
               char const* inputHFListHFtree = "") :
  StPicoHFMaker(name, picoMaker, outputBaseFileName, inputHFListHFtree),
  mDecayChannel(kChannel1), mOutFileBaseName(outputBaseFileName){


  // constructor
}

// _________________________________________________________
StPicoLambdaAnaMaker::~StPicoLambdaAnaMaker() {
  // destructor
}

// _________________________________________________________
int StPicoLambdaAnaMaker::InitHF() {
  // -- INITIALIZE USER HISTOGRAMS ETC HERE -------------------
  //    add them to the output list mOutList which is automatically written

  // EXAMPLE //  mOutList->Add(new TH1F(...));
  // EXAMPLE //  TH1F* hist = static_cast<TH1F*>(mOutList->Last());

  //QA histograms and TOF matching histograms
  //mOutList->Add(new TH1F("h_time_per_event","h_time_per_event", 2000., 0., 20.));

  mOutList->Add(new TH1F("h_piTPC","h_piTPC",100,0,10));
  mOutList->Add(new TH1F("h_kTPC","h_kTPC",100,0,10));
  mOutList->Add(new TH1F("h_pTPC","h_pTPC",100,0,10));

  mOutList->Add(new TH1F("h_piTPC_prim","h_piTPC_prim",100,0,10));
  mOutList->Add(new TH1F("h_kTPC_prim","h_kTPC_prim",100,0,10));
  mOutList->Add(new TH1F("h_pTPC_prim","h_pTPC_prim",100,0,10));

  mOutList->Add(new TH2F("h_piTOF","h_piTOF",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF","h_kTOF",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF","h_pTOF",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOF_20","h_piTOF_20",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF_20","h_kTOF_20",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF_20","h_pTOF_20",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOF_1sig","h_piTOF_1sig",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_kTOF_1sig","h_kTOF_1sig",100,0,10, 2, 0, 2));
  mOutList->Add(new TH2F("h_pTOF_1sig","h_pTOF_1sig",100,0,10, 2, 0, 2));

  mOutList->Add(new TH2F("h_piTOFbeta","h_piTOFbeta",500,0,10, 500, 0, 5));
  mOutList->Add(new TH2F("h_kTOFbeta","h_kTOFbeta",500,0,10, 500, 0, 5));
  mOutList->Add(new TH2F("h_pTOFbeta","h_pTOFbeta",500,0,10, 500, 0, 5));

  mOutList->Add(new TH2F("h_pinsigma","h_pinsigma",100,0,10, 99, -5, 5));
  mOutList->Add(new TH2F("h_knsigma","h_knsigma",100,0,10, 99, -5, 5));
  mOutList->Add(new TH2F("h_pnsigma","h_pnsigma",100,0,10, 99, -5, 5));

  mOutList->Add(new TH2F("h_dedx","h_dedx", 100, 0, 10, 100, 0, 10));
  mOutList->Add(new TH2F("h_OneOverBeta", "h_OneOverBeta", 100, 0, 10, 100, 0, 4));


  mOutFileBaseName = mOutFileBaseName.ReplaceAll(".root", "");


  //create TTree to store D+- candidates
  ntp_Lambda = new TTree("ntp_Lambda", "Lambda TTree"); //create TTree


//---Set TTree branches------------------------------------------------------------------------------
  //commented branches were used for QA and in older versions of the analysis
  //commented in order to reduce output file size as much as possible

  //event
  ntp_Lambda->Branch("runId", &runId, "runId/I");            //Int_t runId
  ntp_Lambda->Branch("eventId", &eventId, "eventId/I");       //Int_t eventId

  //Pion
  ntp_Lambda->Branch("pi_pt", &pi_pt, "pi_pt/F");               //Float_t pi_pt
  ntp_Lambda->Branch("pi_phi", &pi_phi, "pi_phi/F");             //Float_t pi_phi
  ntp_Lambda->Branch("pi_eta", &pi_eta, "pi_eta/F");             //Float_t pi_eta
  ntp_Lambda->Branch("pi_dca", &pi_dca, "pi_dca/F");               //Float_t pi_dca
  ntp_Lambda->Branch("pi_ch", &pi_ch, "pi_ch/F");               //Float_t pi_ch
  ntp_Lambda->Branch("hasTOFinfo_pion", &hasTOFinfo_pion, "hasTOFinfo_pion/I");   //Float_t hasTOFinfo_pion

  //Proton
  ntp_Lambda->Branch("p_pt", &p_pt, "p_pt/F");               //Float_t p_pt
  ntp_Lambda->Branch("p_phi", &p_phi, "p_phi/F");             //Float_t p_phi
  ntp_Lambda->Branch("p_eta", &p_eta, "p_eta/F");             //Float_t p_eta
  ntp_Lambda->Branch("p_dca", &p_dca, "p_dca/F");               //Float_t p_dca
  ntp_Lambda->Branch("p_ch", &p_ch, "p_ch/F");               //Float_t p_ch
  ntp_Lambda->Branch("hasTOFinfo_proton", &hasTOFinfo_proton, "hasTOFinfo_proton/I");   //Float_t hasTOFinfo_proton

  //dca, flag, prim. vertex
  ntp_Lambda->Branch("charge", &charge, "charge/I");      //Int_t charge
  ntp_Lambda->Branch("pairDCA", &pairDCA, "pairDCA/F");      //Int_t pairDCA

  //Lambda baryon
  ntp_Lambda->Branch("L_theta", &L_theta, "L_theta/F");   //Float_t L_theta
  ntp_Lambda->Branch("L_cosThetaStar", &L_cosThetaStar, "L_cosThetaStar/F");   //Float_t L_cosThetaStar
  ntp_Lambda->Branch("L_decayL", &L_decayL, "L_decayL/F");  //Float_t L_decayL
  ntp_Lambda->Branch("L_phi", &L_phi, "L_phi/F");      //Float_t L_phi
  ntp_Lambda->Branch("L_eta", &L_eta, "L_eta/F");      //Float_t L_eta
  ntp_Lambda->Branch("L_pt", &L_pt, "L_pt/F");        //Float_t L_pt
  ntp_Lambda->Branch("L_mass", &L_mass, "L_mass/F");    //Float_t L_mass

//-----------------------Set branches END------------------------------------------------------------------------------

  mRunNumber = 0;
  return kStOK;
}

// _________________________________________________________
void StPicoLambdaAnaMaker::ClearHF(Option_t *opt="") {
  return;
}

// _________________________________________________________
int StPicoLambdaAnaMaker::FinishHF() {
   if( isMakerMode() != StPicoHFMaker::kWrite )
    ntp_Lambda->Write(); //for candidates

  return kStOK;
}

// _________________________________________________________
int StPicoLambdaAnaMaker::MakeHF() {

  //create and analyze D+- candidates

  //createCandidates() makes triplets of K and pi and checks that they pass cuts
  //analyzeCandidates() saves relevant information about the triplets to TTree

  //std::clock_t start1 = std::clock();//kvapil

  if (isMakerMode() == StPicoHFMaker::kWrite) {
    createCandidates();
  }
  else if (isMakerMode() == StPicoHFMaker::kRead) {
    // -- the reading back of the perviously written trees happens in the background
    analyzeCandidates();
  }
  else if (isMakerMode() == StPicoHFMaker::kAnalyze) {
    createCandidates();
    analyzeCandidates();
  }


  //QA and TOF matching histograms
  TH1F *h_piTPC = static_cast<TH1F*>(mOutList->FindObject("h_piTPC"));
  TH1F *h_kTPC = static_cast<TH1F*>(mOutList->FindObject("h_kTPC"));
  TH1F *h_pTPC = static_cast<TH1F*>(mOutList->FindObject("h_pTPC"));

  TH1F *h_piTPC_prim = static_cast<TH1F*>(mOutList->FindObject("h_piTPC_prim"));
  TH1F *h_kTPC_prim = static_cast<TH1F*>(mOutList->FindObject("h_kTPC_prim"));
  TH1F *h_pTPC_prim = static_cast<TH1F*>(mOutList->FindObject("h_pTPC_prim"));

  TH2F *h_piTOF = static_cast<TH2F*>(mOutList->FindObject("h_piTOF"));
  TH2F *h_kTOF = static_cast<TH2F*>(mOutList->FindObject("h_kTOF"));
  TH2F *h_pTOF = static_cast<TH2F*>(mOutList->FindObject("h_pTOF"));

  TH2F *h_piTOF_20 = static_cast<TH2F*>(mOutList->FindObject("h_piTOF_20"));
  TH2F *h_kTOF_20 = static_cast<TH2F*>(mOutList->FindObject("h_kTOF_20"));
  TH2F *h_pTOF_20 = static_cast<TH2F*>(mOutList->FindObject("h_pTOF_20"));

  TH2F *h_piTOF_1sig = static_cast<TH2F*>(mOutList->FindObject("h_piTOF_1sig"));
  TH2F *h_kTOF_1sig = static_cast<TH2F*>(mOutList->FindObject("h_kTOF_1sig"));
  TH2F *h_pTOF_1sig = static_cast<TH2F*>(mOutList->FindObject("h_pTOF_1sig"));

  TH2F *h_piTOFbeta = static_cast<TH2F*>(mOutList->FindObject("h_piTOFbeta"));
  TH2F *h_kTOFbeta = static_cast<TH2F*>(mOutList->FindObject("h_kTOFbeta"));
  TH2F *h_pTOFbeta = static_cast<TH2F*>(mOutList->FindObject("h_pTOFbeta"));

  TH2F *h_pinsigma = static_cast<TH2F*>(mOutList->FindObject("h_pinsigma"));
  TH2F *h_knsigma = static_cast<TH2F*>(mOutList->FindObject("h_knsigma"));
  TH2F *h_pnsigma = static_cast<TH2F*>(mOutList->FindObject("h_pnsigma"));

  TH2F *h_dedx = static_cast<TH2F*>(mOutList->FindObject("h_dedx"));
  TH2F *h_OneOverBeta = static_cast<TH2F*>(mOutList->FindObject("h_OneOverBeta"));


  //primary vertex
  TVector3 pVtx = mPicoDst->event()->primaryVertex();



  //loop over PicoDst tracks
  UInt_t nTracks = mPicoDst->numberOfTracks();
  for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
  {
      StPicoTrack const* trk = mPicoDst->track(iTrack);
      if (!trk) continue;
      
      StPicoPhysicalHelix helix = trk->helix(mPicoDst->event()->bField()); //SL16j, Vanek
      TVector3 momentum = trk->gMom(pVtx, mPicoDst->event()->bField()); //bFiled NOT in kilogauss - properly computed inside gMom(...) function in StPicoTrack

      //check nHits and eta of track
      if(!(mHFCuts->hasGoodNHitsFitMinHist(trk))) continue;
      if (!(mHFCuts->hasGoodEta(momentum))) continue;

      //PID of pions for QA histograms and 
      if (mHFCuts->hasGoodTPCnSigmaPion(trk))
      {
        
        //calculate beta of track from its momentum
        float piBeta = mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()); //SL16j, Vanek

        //get beta of track from TOF, if TOF information is available          
        Int_t piTofAvailable = 0;
        if(!isnan(piBeta) && piBeta > 0)
        {
           piTofAvailable = 1;
           float tofPion = fabs(1. / piBeta - sqrt(1+M_PION_PLUS*M_PION_PLUS/(momentum.Mag()*momentum.Mag())));
           h_piTOFbeta->Fill(trk->gPt(),tofPion);
        }

        //fill QA and TOF matching histograms
        h_piTOF->Fill(trk->gPt(),piTofAvailable);

        if (mHFCuts->hasGoodNHitsFitMinHist(trk))
        {
          h_piTPC->Fill(trk->gPt());

          if(trk->isPrimary())
          {
            h_piTPC_prim->Fill(trk->pPt());
          }

          h_piTOF_20->Fill(trk->gPt(),piTofAvailable);

        }
  
        if (fabs(mHFCuts->hasGoodNSigmaHist(trk, 1))) h_piTOF_1sig->Fill(trk->gPt(),piTofAvailable); //hasGoodNSigmaHist(trk, 1) => check nSigma of pion
      }

      if (mHFCuts->hasGoodTPCnSigmaKaon(trk))
      { //new, hasGoodTPCnSigmaKaon defined in StPicoCutsBase

      
        //float kBeta = mHFCuts->getTofBetaBase(trk);  //SL16d
        float kBeta = mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()); //SL16j, Vanek
        Int_t kTofAvailable = 0;
        if(!isnan(kBeta) && kBeta > 0)
        {
          kTofAvailable = 1;
          float tofKaon = fabs(1. / kBeta - sqrt(1+M_KAON_PLUS*M_KAON_PLUS/(momentum.Mag()*momentum.Mag())));
          h_kTOFbeta->Fill(trk->gPt(),tofKaon);
        }

        h_kTOF->Fill(trk->gPt(),kTofAvailable);
        if (mHFCuts->hasGoodNHitsFitMinHist(trk))
        {
          h_kTPC->Fill(trk->gPt());

          if(trk->isPrimary())
          {
            h_kTPC_prim->Fill(trk->pPt());
          }

          h_kTOF_20->Fill(trk->gPt(),kTofAvailable);   //see line 303
        }
        
        if (fabs(mHFCuts->hasGoodNSigmaHist(trk, 2))) h_kTOF_1sig->Fill(trk->gPt(),kTofAvailable); //hasGoodNSigmaHist(trk, 2) => check nSigma of kaon

      }

       
      if (mHFCuts->hasGoodTPCnSigmaProton(trk))
      { //new, hasGoodTPCnSigmaProton defined in StPicoCutsBase
        //float pBeta = mHFCuts->getTofBetaBase(trk); //SL16d
        float pBeta = mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()); //SL16j, Vanek
        Int_t pTofAvailable = 0;
        if(!isnan(pBeta) && pBeta > 0)
        {
          pTofAvailable = 1;
          float tofProton = fabs(1. / pBeta - sqrt(1+M_PROTON*M_PROTON/(momentum.Mag()*momentum.Mag())));
          h_pTOFbeta->Fill(trk->gPt(),tofProton);
        }

        h_pTOF->Fill(trk->gPt(),pTofAvailable);

        if (mHFCuts->hasGoodNHitsFitMinHist(trk))
        {
          h_pTPC->Fill(trk->gPt());

          if(trk->isPrimary())
          {
            h_pTPC_prim->Fill(trk->pPt());
          }
        
          h_pTOF_20->Fill(trk->gPt(),pTofAvailable);
        }
            
        if (fabs(mHFCuts->hasGoodNSigmaHist(trk, 3))) h_pTOF_1sig->Fill(trk->gPt(),pTofAvailable); //hasGoodNSigmaHist(trk, 3) => check nSigma of proton


      }

      h_pinsigma->Fill(momentum.Mag(),trk->nSigmaPion());
      h_knsigma->Fill(momentum.Mag(),trk->nSigmaKaon());
      h_pnsigma->Fill(momentum.Mag(),trk->nSigmaProton());

      h_dedx->Fill(momentum.Mag(),trk->dEdx());

      float globBeta = mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()); //SL16j, Vanek
        
      if(!isnan(globBeta) && globBeta > 0)
      {
        h_OneOverBeta->Fill(momentum.Mag(),1./globBeta);
      }


  } // .. end tracks loop


/*
    double duration = (double) (std::clock() - start1) / (double) CLOCKS_PER_SEC;
  TH1F *h_time_per_event = static_cast<TH1F*>(mOutList->FindObject("h_time_per_event"));
  h_time_per_event->Fill(duration);
*/
  return kStOK;
}


int StPicoLambdaAnaMaker::createQA(){
       
   return 0;
}

// _________________________________________________________
int StPicoLambdaAnaMaker::createCandidates() {
  // Creating candidates for Lambda decay
  // Lambda -> p pi- decay

  for (unsigned short idxPion = 0; idxPion < mIdxPicoPions.size(); ++idxPion)
  {
    StPicoTrack const *pion = mPicoDst->track(mIdxPicoPions[idxPion]);
    // -- Pion selection

    for (unsigned short idxProton = 0; idxProton < mIdxPicoProtons.size(); ++idxProton)
    {
      StPicoTrack const *proton = mPicoDst->track(mIdxPicoProtons[idxProton]);
      // -- Kaon selection
      // -- TOF
      //if( !mHFCuts->isHybridTOFProton(proton, mHFCuts->getTofBetaBase(proton, mPicoDst->event()->bField())) ) continue; //SL16j, Vanek, hybrid TOF kaons, probably not needed here
      if (mIdxPicoProtons[idxProton] == mIdxPicoPions[idxPion]) continue;

      // -- Making pair, want proton first for cosThetaStar calculation
      StHFPair pair(proton, pion, mHFCuts->getHypotheticalMass(StHFCuts::kProton), mHFCuts->getHypotheticalMass(StHFCuts::kPion), mIdxPicoProtons[idxProton], mIdxPicoPions[idxPion], mPrimVtx, mBField);

      if(!mHFCuts->isGoodSecondaryVertexPair(pair)) continue;

      mPicoHFEvent->addHFSecondaryVertexPair(&pair);
        
    } //end for (unsigned short idxPion2 = idxPion1+1; idxPion2 < mIdxPicoPions.size(); ++idxPion2)
  } //end for (unsigned short idxPion1 = 0; idxPion1 < mIdxPicoPions.size(); ++idxPion1)

  return kStOK;
}

// _________________________________________________________
int StPicoLambdaAnaMaker::analyzeCandidates() {

  // --- Analyze previously constructed triplets and save to ntuple
  // -- Decay channel1
  TClonesArray const * aCandidates= mPicoHFEvent->aHFSecondaryVertices();

  float ZDCx = mPicoDst->event()->ZDCx();
  float BBCx = mPicoDst->event()->BBCx();

  if( mPicoHFEvent->nHFSecondaryVertices() > 0 )
  {
    for (unsigned int idx = 0; idx <  mPicoHFEvent->nHFSecondaryVertices(); ++idx)
    {
      StHFPair const* pair = static_cast<StHFPair*>(aCandidates->At(idx));

      if(pair->eta() == 0) continue;

      StPicoTrack const* pion = mPicoDst->track(pair->particle2Idx()); //pion is second particle in the pair
      StPicoTrack const* proton = mPicoDst->track(pair->particle1Idx()); //proton is the firs particle in the pair
      

      //TOF ---
      float pionBetaBase = -1;

      pionBetaBase = mHFCuts->getTofBetaBase(pion, mPicoDst->event()->bField());

      //get beta of track from TOF, if TOF information is available          
      hasTOFinfo_pion = 0;

      if(!isnan(pionBetaBase) && pionBetaBase > 0)
      {
        hasTOFinfo_pion = 1;
        //float tofPion = fabs(1. / piBeta - sqrt(1+M_PION_PLUS*M_PION_PLUS/(momentum.Mag()*momentum.Mag())));        
      }


      float protonBetaBase = -1;
      protonBetaBase = mHFCuts->getTofBetaBase(proton, mPicoDst->event()->bField()); 

      hasTOFinfo_proton = 0;
      if(!isnan(protonBetaBase) && protonBetaBase > 0)
      {
        hasTOFinfo_proton = 1;
        //float tofPion = fabs(1. / piBeta - sqrt(1+M_PION_PLUS*M_PION_PLUS/(momentum.Mag()*momentum.Mag())));        
      }
      
      // ---
      // Saving to TTree
      runId = mPicoHFEvent->runId();
      eventId = mPicoHFEvent->eventId();

      pi_pt  = pion->gPt();
      pi_phi = pion->gMom().Phi();
      pi_eta = pion->gMom(mPrimVtx,mBField).PseudoRapidity();
      pi_dca = pion->gDCAxy(mPrimVtx.x(), mPrimVtx.y());
      pi_ch = pion->charge();
      
      
      //eventId and runId common for all particles in the triplet
      p_pt = proton->gPt();
      p_phi = proton->gMom().Phi();
      p_eta = proton->gMom(mPrimVtx,mBField).PseudoRapidity();      
      p_dca = proton->gDCAxy(mPrimVtx.x(), mPrimVtx.y());
      p_ch = proton->charge();

      // -- Flag for like-sign or unlike-sign pairs
      charge = -99;
      if( (pion->charge() + proton->charge()) == 0 ) charge = 0; // unlike-sign - from decay of Lambda
      else if( fabs(pion->charge() + proton->charge()) == 2 ) charge = 1; // unlike-sign - combinatorial background

      pairDCA = pair->dcaDaughters();

      L_theta = pair->pointingAngle();
      L_cosThetaStar = pair->cosThetaStar();
      L_decayL = pair->decayLength();
      L_phi = pair->phi();
      L_eta = pair->eta();
      L_pt = pair->pt();
      L_mass = pair->m();

      ntp_Lambda->Fill();

    } // for (unsigned int idx = 0; idx <  mPicoHFEvent->nHFSecondaryVertices(); ++idx) {
  }

  return kStOK;
}

// _________________________________________________________
bool StPicoLambdaAnaMaker::isHadron(StPicoTrack const * const trk, int pidFlag) const {
  // -- good hadron
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, pidFlag));
}

// _________________________________________________________
bool StPicoLambdaAnaMaker::isPion(StPicoTrack const * const trk) const {
  // -- good pion

   TVector3 t = trk->gMom();
   if (!mHFCuts->hasGoodEta(t)) return false; 
   if (!mHFCuts->isHybridTOFHadron(trk, mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()), StHFCuts::kPion) ) return false;
   if (!mHFCuts->cutMinDcaToPrimVertex(trk, StPicoCutsBase::kPion)) return false;
   return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, StPicoCutsBase::kPion));
}

// _________________________________________________________
bool StPicoLambdaAnaMaker::isKaon(StPicoTrack const * const trk) const {
  // -- good kaon

  TVector3 t = trk->gMom();
  if (!mHFCuts->hasGoodEta(t)) return false;
  if (!mHFCuts->isHybridTOFHadron(trk, mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()), StHFCuts::kKaon) ) return false;
  if (!mHFCuts->cutMinDcaToPrimVertex(trk, StPicoCutsBase::kKaon)) return false;
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, StPicoCutsBase::kKaon));
}

// _________________________________________________________
bool StPicoLambdaAnaMaker::isProton(StPicoTrack const * const trk) const {
  // -- good proton
  TVector3 t = trk->gMom();
  if (!mHFCuts->hasGoodEta(t)) return false; 
  if (!mHFCuts->isHybridTOFHadron(trk, mHFCuts->getTofBetaBase(trk, mPicoDst->event()->bField()), StHFCuts::kProton) ) return false;
  if (!mHFCuts->cutMinDcaToPrimVertex(trk, StPicoCutsBase::kProton)) return false;
  return (mHFCuts->isGoodTrack(trk) && mHFCuts->isTPCHadron(trk, StPicoCutsBase::kProton));
}

double StPicoLambdaAnaMaker::DCA(StPicoTrack const * const trk, TVector3 const & vtx) const {
  // -- particle DCA

  return ((trk->origin() - vtx).Mag());
}


bool StPicoLambdaAnaMaker::isCloseTracks(StPicoTrack const * const trk1, StPicoTrack const * const trk2, TVector3 const & vtx, float bField) const {
  // -- good pair

  if( ( trk1->origin()-vtx ).Mag()>1.5 || ( trk2->origin()-vtx ).Mag()>1.5 ) return false;

  TVector3 const p1Mom = trk1->gMom();
  TVector3 const p2Mom = trk2->gMom();
  StPicoPhysicalHelix const p1StraightLine(p1Mom, trk1->origin(), 0, trk1->charge());
  StPicoPhysicalHelix const p2StraightLine(p2Mom, trk2->origin(), 0, trk2->charge());

  pair<double, double> const ss = p1StraightLine.pathLengths(p2StraightLine);
  TVector3 const p1AtDcaToP2 = p1StraightLine.at(ss.first);
  TVector3 const p2AtDcaToP1 = p2StraightLine.at(ss.second);
  float const dca = (p1AtDcaToP2-p2AtDcaToP1).Mag();

  if(dca > mHFCuts->cutSecondaryTripletDcaDaughters12Max()) return false; 

  return true;
}

//-----------------------------------------------------------------------------