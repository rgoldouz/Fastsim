#define Analysis_cxx
#include "Analysis.h"
#include "Gflash.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPad.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TLine.h"
#include "TStopwatch.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TGraphAsymmErrors.h"
#include <cmath>
#include "TLorentzVector.h"
#include "TPaveText.h"
#include "TPie.h"
#include "TF1.h"
#include "TVector3.h"
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include "TRandom3.h"
#define nGammaPar 3 //#pars for Gamma fn

Double_t GammaProfile(Double_t *x,Double_t *par) {
double Gamma1=0;
Gamma1 = par[2]*((par[1]*pow((x[0])*(par[1]),par[0]-1.0)*exp(-(x[0])*(par[1])))/tgamma(par[0]));
return Gamma1;
}

void displayProgress(long current, long max){
  using std::cerr;
  if (max<1000) return;
  if (current%(max/1000)!=0 && current<max-1) return;

  int width = 52; // Hope the terminal is at least that wide.
  int barWidth = width - 2;
  cerr << "\x1B[2K"; // Clear line
  cerr << "\x1B[2000D"; // Cursor left
  cerr << '[';
  for(int i=0 ; i<barWidth ; ++i){ if(i<barWidth*current/max){ cerr << '=' ; }else{ cerr << ' ' ; } }
  cerr << ']';
  cerr << " " << Form("%8d/%8d (%5.2f%%)", (int)current, (int)max, 500.0*current/max) ;
  cerr.flush();
}

TH1F* RandomGammaProfile(double alpha, double beta, double range, int nbin, const char* name, double weight){
  int n=1000;
  double rand;
  TH1F *Eprofile;
  TF1 *funcH1;
  Eprofile = new TH1F(name,name,nbin,0,range);
  funcH1 = new TF1("GammaH", GammaProfile, 0, range, nGammaPar);
  funcH1->SetParameters(alpha ,beta,1);
  for (int j = 0; j < n; ++j){
    rand = funcH1->GetRandom();
    Eprofile->Fill(rand, weight);
  }
  delete funcH1;
  return Eprofile;
}


void Analysis::Loop(TString fname)
{
  gStyle->SetOptStat(0);
  cout<<fname<<endl;

  TFile *f = new TFile("hists/Hist"+fname+".root","RECREATE");
  f->cd();

  double EcalRadLen =89;
  double HcalRadLen =143;
  double HcalIntLen =150.5;
  double EcalIntLen =300;   
  double rand;
  Gflash gflash;

  TH1F *HadEPHcal, *HadEPEcal, *HHcal, *HEcal, *HHcal2;
  TH1F *Eprofile, *EPHCAL, *EPECAL, *hClone;

  TF1 *funcH;
  TH1F *fDp       = new TH1F( "fDp", "fDp" ,           50, 0  , 1    );
  TH1F *fPi0      = new TH1F( "fPi0", "fPi0" ,         50, 0  , 1    );
  TH1F *fPi0L     = new TH1F( "fPi0L", "fPi0L" ,       50, 0  , 1    );
  TH1F *fHadECAL  = new TH1F( "fHadECAL", "fHadECAL" , 50, 0  , 1    );
  TH1F *Class     = new TH1F( "Class", "Class" ,        4, 0  , 4    );
  TH1F *fSP       = new TH1F( "fSP", "fSP" ,            4, 0  , 4    );
  TH1F *SP        = new TH1F( "SP", "SP" ,            400, 0  , 4000 );
  TH1F *HadEPFullsim_spHCAL      = new TH1F( "HadEPFullsim_spHCAL","HadEPFullsim_spHCAL" ,             30, 0  , 15    );
  TH1F *HadEPFullsim_spHCALFit   = new TH1F( "HadEPFullsim_spHCALFit","HadEPFullsim_spHCALFit" ,       30, 0  , 15    );
  TH1F *HadEPFullsim_spECAL_HCAL   = new TH1F( "HadEPFullsim_spECAL_HCAL","HadEPFullsim_spECAL_HCAL" ,       30, 0  , 15    );
  TH1F *HadEPFullsim_spECAL_HCALFit   = new TH1F( "HadEPFullsim_spECAL_HCALFit","HadEPFullsim_spECAL_HCALFit" ,       30, 0  , 15    );
  TH1F *HadEPFullsim_spECAL_ECAL   = new TH1F( "HadEPFullsim_spECAL_ECAL","HadEPFullsim_spECAL_ECAL" ,       30, 0  , 15    );
  TH1F *HadEPFullsim_spECAL_ECALFit   = new TH1F( "HadEPFullsim_spECAL_ECALFit","HadEPFullsim_spECAL_ECALFit" ,       30, 0  , 15    );

  TH1F *FirstPi0EPFullsim_spHCAL     = new TH1F( "FirstPi0EPFullsim_spHCAL","FirstPi0EPFullsim_spHCAL" ,           30, 0  , 15    );
  TH1F *FirstPi0EPFullsim_spECAL_HCAL     = new TH1F( "FirstPi0EPFullsim_spECAL_HCAL","FirstPi0EPFullsim_spECAL_HCAL" ,  30, 0  , 15    );
  TH1F *FirstPi0EPFullsim_spECAL_ECAL     = new TH1F( "FirstPi0EPFullsim_spECAL_ECAL","FirstPi0EPFullsim_spECAL_ECAL" ,  30, 0  , 15    );
  TH1F *FirstPi0EPFullsim_spHCALFit     = new TH1F( "FirstPi0EPFullsim_spHCALFit","FirstPi0EPFullsim_spHCALFit" ,           30, 0  , 15    );
  TH1F *FirstPi0EPFullsim_spECAL_HCALFit     = new TH1F( "FirstPi0EPFullsim_spECAL_HCALFit","FirstPi0EPFullsim_spECAL_HCALFit" ,  30, 0  , 15    );
  TH1F *FirstPi0EPFullsim_spECAL_ECALFit     = new TH1F( "FirstPi0EPFullsim_spECAL_ECALFit","FirstPi0EPFullsim_spECAL_ECALFit" ,  30, 0  , 15    );

  TH1F *LatePi0EPFullsim_spHCAL     = new TH1F( "LatePi0EPFullsim_spHCAL","LatePi0EPFullsim_spHCAL" ,           30, 0  , 15    );
  TH1F *LatePi0EPFullsim_spECAL_HCAL     = new TH1F( "LatePi0EPFullsim_spECAL_HCAL","LatePi0EPFullsim_spECAL_HCAL" ,  30, 0  , 15    );
  TH1F *LatePi0EPFullsim_spECAL_ECAL     = new TH1F( "LatePi0EPFullsim_spECAL_ECAL","LatePi0EPFullsim_spECAL_ECAL" ,  30, 0  , 15    );
  TH1F *LatePi0EPFullsim_spHCALFit     = new TH1F( "LatePi0EPFullsim_spHCALFit","LatePi0EPFullsim_spHCALFit" ,           30, 0  , 15    );
  TH1F *LatePi0EPFullsim_spECAL_HCALFit     = new TH1F( "LatePi0EPFullsim_spECAL_HCALFit","LatePi0EPFullsim_spECAL_HCALFit" ,  30, 0  , 15    );
  TH1F *LatePi0EPFullsim_spECAL_ECALFit     = new TH1F( "LatePi0EPFullsim_spECAL_ECALFit","LatePi0EPFullsim_spECAL_ECALFit" ,  30, 0  , 15    );

  TH1F *Alpha_HadEPFullsim_spHCAL = new TH1F( "Alpha_HadEPFullsim_spHCAL", "Alpha_HadEPFullsim_spHCAL" ,   50, -5  , 5    );
  TH1F *Alpha_HadEPFullsim_spECAL_ECAL = new TH1F( "Alpha_HadEPFullsim_spECAL_ECAL", "Alpha_HadEPFullsim_spECAL_ECAL" ,   50, -5  , 5    );
  TH1F *Alpha_HadEPFullsim_spECAL_HCAL = new TH1F( "Alpha_HadEPFullsim_spECAL_HCAL", "Alpha_HadEPFullsim_spECAL_HCAL" ,   50, -5  , 5    );
  TH1F *Beta_HadEPFullsim_spHCAL = new TH1F( "Beta_HadEPFullsim_spHCAL", "Beta_HadEPFullsim_spHCAL" ,   50, -5  , 5    );
  TH1F *Beta_HadEPFullsim_spECAL_ECAL = new TH1F( "Beta_HadEPFullsim_spECAL_ECAL", "Beta_HadEPFullsim_spECAL_ECAL" ,   50, -5  , 5    );
  TH1F *Beta_HadEPFullsim_spECAL_HCAL = new TH1F( "Beta_HadEPFullsim_spECAL_HCAL", "Beta_HadEPFullsim_spECAL_HCAL" ,   50, -5  , 5    );
  TH2F *AlphaBetaCorr_HadEPFullsim_spHCAL = new TH2F( "AlphaBetaCorr_HadEPFullsim_spHCAL", "AlphaBetaCorr_HadEPFullsim_spHCAL" ,   200,-10,10,200,-10,10    );
  TH2F *AlphaBetaCorr_HadEPFullsim_spECAL_ECAL = new TH2F( "AlphaBetaCorr_HadEPFullsim_spECAL_ECAL", "AlphaBetaCorr_HadEPFullsim_spECAL_ECAL" ,   200,-10,10,200,-10,10    );
  TH2F *AlphaBetaCorr_HadEPFullsim_spECAL_HCAL = new TH2F( "AlphaBetaCorr_HadEPFullsim_spECAL_HCAL", "AlphaBetaCorr_HadEPFullsim_spECAL_HCAL" ,   200,-10,10,200,-10,10    );
  TH1F *Alpha_HadEPFastsim_spHCAL = new TH1F( "Alpha_HadEPFastsim_spHCAL", "Alpha_HadEPFastsim_spHCAL" ,   50, -5  , 5    );
  TH1F *Alpha_HadEPFastsim_spECAL_ECAL = new TH1F( "Alpha_HadEPFastsim_spECAL_ECAL", "Alpha_HadEPFastsim_spECAL_ECAL" ,   50, -5  , 5    );
  TH1F *Alpha_HadEPFastsim_spECAL_HCAL = new TH1F( "Alpha_HadEPFastsim_spECAL_HCAL", "Alpha_HadEPFastsim_spECAL_HCAL" ,   50, -5  , 5    );
  TH1F *Beta_HadEPFastsim_spHCAL = new TH1F( "Beta_HadEPFastsim_spHCAL", "Beta_HadEPFastsim_spHCAL" ,   50, -5  , 5    );
  TH1F *Beta_HadEPFastsim_spECAL_ECAL = new TH1F( "Beta_HadEPFastsim_spECAL_ECAL", "Beta_HadEPFastsim_spECAL_ECAL" ,   50, -5  , 5    );
  TH1F *Beta_HadEPFastsim_spECAL_HCAL = new TH1F( "Beta_HadEPFastsim_spECAL_HCAL", "Beta_HadEPFastsim_spECAL_HCAL" ,   50, -5  , 5    );

  TH2D *h2_SpVsECALeOverHCALe   = new TH2D("h2_SpVsECALeOverHCALe","h2_SpVsECALeOverHCALe", 100 , 0,4000, 50 ,0,1);
  TH2D *h2_classVsSP   = new TH2D("h2_classVsSP","h2_classVsSP", 4 , 0,4, 100 , 0,4000);
//   HadEPFullsim_spECAL_HCALFit = new TH1F( "HadEPFullsim_spECAL_HCALFit","HadEPFullsim_spECAL_HCALFit" ,   30, 0  , 10    );
//   HadEPFullsim_spECAL_ECALFit = new TH1F( "HadEPFullsim_spECAL_ECALFit","HadEPFullsim_spECAL_ECALFit" ,   30, 0  , 10    );

  Double_t mean, var;
  if (fChain == 0) return;
  Int_t nentries = (Int_t) fChain->GetEntries();
  float N = float(nentries);
  int C = 0;
  Long64_t nbytes = 0, nb = 0;
  TCanvas *c1;
  stringstream ss;
  stringstream Intgral;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    displayProgress(jentry, nentries) ;
//initialize the GFlash class
    gflash.initialize(    p_E, //  double energy,
                            sim_pvIneInt_z, //  double showerStartingPoint,
                            sim_eTotal, //  double depositEnergy,
                            sim_ePi0First, //  double energyPi0First,
                            sim_ePi0Late, //  double energypi0Late
                            h_eHadronic_ECAL, //HistEnergyHadronicEcal,
                            h_eHadronic_HCAL, //HistEnergyHadronicHcal,
                            h_ePi0First_ECAL, //HistEnergyPiFirst
                            h_ePi0First_HCAL, //HistEnergyPiFirst
                            h_ePi0Late_ECAL, //HistEnergyPiLate
                            h_ePi0Late_HCAL //HistEnergyPiLate
    );

//    if (jentry<100 && gflash.theHistEnergyPi0FirstEcal->Integral()==0){
//      c1 = new TCanvas("c1","A Simple Graph Example",800,100,800,500);
//      c1->cd();
//      gflash.theHistEnergyPi0FirstHcal->SetLineColor(1);
//      gflash.theHistEnergyPi0FirstHcal->Draw("hist");
//      gflash.getRandomProfile(gflash.alphaEnergyPi0FirstHcal, gflash.betaEnergyPi0FirstHcal)->Draw("histsame");
//      ss <<"eHadronic"<<jentry<<".png";
//      c1->Print((ss.str()).c_str(),"png");
//      ss.str("");
//      delete c1;
//    }
    if(sim_ePi0First+sim_ePi0Late !=0) fPi0->Fill((sim_ePi0First+sim_ePi0Late)/sim_eTotal);
    if(sim_ePi0First!=0 && sim_ePi0Late !=0) fPi0L->Fill(sim_ePi0Late/(sim_ePi0First+sim_ePi0Late));
    if(sim_ePi0First+sim_ePi0Late ==0) C=0;
    if(sim_ePi0First!=0 && sim_ePi0Late ==0) C=1;   
    if(sim_ePi0First==0 && sim_ePi0Late !=0) C=2;
    if(sim_ePi0First!=0 && sim_ePi0Late !=0) C=3;
    Class->Fill(C + 0.5,1/N);

//starting points
    SP->Fill(sim_pvIneInt_z);
    if(sim_pvIneInt_z<1290) fSP->Fill(0.5,1/N);
    if(sim_pvIneInt_z>1290 && sim_pvIneInt_z<1790) fSP->Fill(1.5,1/N);
    if(sim_pvIneInt_z>1790 && sim_pvIneInt_z<2950) fSP->Fill(2.5,1/N);
    if(sim_pvIneInt_z>2950) fSP->Fill(3.5,1/N);
//    h2_SpVsECALeOverHCALe->Fill(sim_pvIneInt_z,h_eHadronic_ECAL->Integral()/h_eHadronic_HCAL->Integral());
//Hadronic energy profile
    if(gflash.theHistEnergyHadronicEcal->Integral()>0 && gflash.EcaloHcalHadronic> 0.05){
        HadEPFullsim_spECAL_ECAL->Add(gflash.theHistEnergyHadronicEcal);
        HadEPFullsim_spECAL_ECALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyHadronicEcal,gflash.betaEnergyHadronicEcal));
        Alpha_HadEPFullsim_spECAL_ECAL->Fill(gflash.alphaEnergyHadronicEcal);
        Beta_HadEPFullsim_spECAL_ECAL->Fill(gflash.betaEnergyHadronicEcal);
        AlphaBetaCorr_HadEPFullsim_spECAL_ECAL->Fill(gflash.alphaEnergyHadronicEcal,gflash.betaEnergyHadronicEcal);        
      if(gflash.theHistEnergyHadronicHcal > 0 && (1-gflash.EcaloHcalHadronic)> 0.05){
        HadEPFullsim_spECAL_HCAL->Add(gflash.theHistEnergyHadronicHcal);
        HadEPFullsim_spECAL_HCALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyHadronicHcal,gflash.betaEnergyHadronicHcal));
        Alpha_HadEPFullsim_spECAL_HCAL->Fill(gflash.alphaEnergyHadronicHcal);
        Beta_HadEPFullsim_spECAL_HCAL->Fill(gflash.betaEnergyHadronicHcal);
        AlphaBetaCorr_HadEPFullsim_spECAL_HCAL->Fill(gflash.alphaEnergyHadronicHcal,gflash.alphaEnergyHadronicHcal);
      }
    }
    if(gflash.theHistEnergyHadronicEcal->Integral()==0 && gflash.theHistEnergyHadronicHcal > 0){
        HadEPFullsim_spHCAL->Add(gflash.theHistEnergyHadronicHcal);
        HadEPFullsim_spHCALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyHadronicHcal,gflash.betaEnergyHadronicHcal));
        Alpha_HadEPFullsim_spHCAL->Fill(gflash.alphaEnergyHadronicHcal);
        Beta_HadEPFullsim_spHCAL->Fill(gflash.betaEnergyHadronicHcal);
        AlphaBetaCorr_HadEPFullsim_spHCAL->Fill(gflash.alphaEnergyHadronicHcal,gflash.betaEnergyHadronicHcal);
    }
//First pi0 energy profile
    if(gflash.theHistEnergyPi0FirstEcal->Integral()>0 && gflash.EcaloHcalPi0First>0.05){
      FirstPi0EPFullsim_spECAL_ECAL->Add(gflash.theHistEnergyPi0FirstEcal);
      FirstPi0EPFullsim_spECAL_ECALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyPi0FirstEcal,gflash.betaEnergyPi0FirstEcal));
      if(gflash.theHistEnergyPi0FirstHcal->Integral()>0 && (1-gflash.EcaloHcalPi0First)>0.05){
        FirstPi0EPFullsim_spECAL_HCAL->Add(gflash.theHistEnergyPi0FirstHcal);
        FirstPi0EPFullsim_spECAL_HCALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyPi0FirstHcal,gflash.betaEnergyPi0FirstHcal));
      }
    }
    if(gflash.theHistEnergyPi0FirstEcal->Integral()==0 && gflash.theHistEnergyPi0FirstHcal->Integral()>0){
      FirstPi0EPFullsim_spHCAL->Add(gflash.theHistEnergyPi0FirstHcal);      
      FirstPi0EPFullsim_spHCALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyPi0FirstHcal,gflash.betaEnergyPi0FirstHcal));
    }
//Late pi0 energy profile
    if(gflash.theHistEnergyPi0LateEcal->Integral()>0 && gflash.EcaloHcalPi0Late>0.05){
      LatePi0EPFullsim_spECAL_ECAL->Add(gflash.theHistEnergyPi0LateEcal);
      LatePi0EPFullsim_spECAL_ECALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyPi0LateEcal,gflash.betaEnergyPi0LateEcal));
      if(gflash.theHistEnergyPi0LateHcal->Integral()>0 && (1-gflash.EcaloHcalPi0Late)>0.05){
        LatePi0EPFullsim_spECAL_HCAL->Add(gflash.theHistEnergyPi0LateHcal);
        LatePi0EPFullsim_spECAL_HCALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyPi0LateHcal,gflash.betaEnergyPi0LateHcal));
        if((gflash.getRandomProfile(gflash.alphaEnergyPi0LateHcal,gflash.betaEnergyPi0LateHcal))->Integral()==0) cout<<"kkkkkkkkkkkkkkkkkkkkkkk"<<endl;
      }
    }
    if(gflash.theHistEnergyPi0LateEcal->Integral()==0 && gflash.theHistEnergyPi0LateHcal->Integral()>0){
      LatePi0EPFullsim_spHCAL->Add(gflash.theHistEnergyPi0LateHcal);
      LatePi0EPFullsim_spHCALFit->Add(gflash.getRandomProfile(gflash.alphaEnergyPi0LateHcal,gflash.betaEnergyPi0LateHcal));
    }
    gflash.clean();
  }


  HadEPFullsim_spHCAL->Scale(1/HadEPFullsim_spHCAL->Integral());
  HadEPFullsim_spHCALFit->Scale(1/HadEPFullsim_spHCALFit->Integral());
  HadEPFullsim_spECAL_HCAL->Scale(1/HadEPFullsim_spECAL_HCAL->Integral());
  HadEPFullsim_spECAL_ECAL->Scale(1/HadEPFullsim_spECAL_ECAL->Integral());
  HadEPFullsim_spECAL_HCALFit->Scale(1/HadEPFullsim_spECAL_HCALFit->Integral());
  HadEPFullsim_spECAL_ECALFit->Scale(1/HadEPFullsim_spECAL_ECALFit->Integral());
  FirstPi0EPFullsim_spECAL_ECAL->Scale(1/FirstPi0EPFullsim_spECAL_ECAL->Integral());
  FirstPi0EPFullsim_spECAL_HCAL->Scale(1/FirstPi0EPFullsim_spECAL_HCAL->Integral());
  FirstPi0EPFullsim_spECAL_ECALFit->Scale(1/FirstPi0EPFullsim_spECAL_ECALFit->Integral());
  FirstPi0EPFullsim_spECAL_HCALFit->Scale(1/FirstPi0EPFullsim_spECAL_HCALFit->Integral());
  FirstPi0EPFullsim_spHCAL->Scale(1/FirstPi0EPFullsim_spHCAL->Integral());
  FirstPi0EPFullsim_spHCALFit->Scale(1/FirstPi0EPFullsim_spHCALFit->Integral());

  LatePi0EPFullsim_spECAL_ECAL->Scale(1/LatePi0EPFullsim_spECAL_ECAL->Integral());
  LatePi0EPFullsim_spECAL_HCAL->Scale(1/LatePi0EPFullsim_spECAL_HCAL->Integral());
  LatePi0EPFullsim_spECAL_ECALFit->Scale(1/LatePi0EPFullsim_spECAL_ECALFit->Integral());
  LatePi0EPFullsim_spECAL_HCALFit->Scale(1/LatePi0EPFullsim_spECAL_HCALFit->Integral());
  LatePi0EPFullsim_spHCAL->Scale(1/LatePi0EPFullsim_spHCAL->Integral());
  LatePi0EPFullsim_spHCALFit->Scale(1/LatePi0EPFullsim_spHCALFit->Integral());
  Alpha_HadEPFullsim_spHCAL->Scale(1/Alpha_HadEPFullsim_spHCAL->Integral());
  Alpha_HadEPFullsim_spECAL_HCAL->Scale(1/Alpha_HadEPFullsim_spECAL_HCAL->Integral());
  Alpha_HadEPFullsim_spECAL_ECAL->Scale(1/Alpha_HadEPFullsim_spECAL_ECAL->Integral());
  Beta_HadEPFullsim_spHCAL->Scale(1/Beta_HadEPFullsim_spHCAL->Integral());
  Beta_HadEPFullsim_spECAL_HCAL->Scale(1/Beta_HadEPFullsim_spECAL_HCAL->Integral());
  Beta_HadEPFullsim_spECAL_ECAL->Scale(1/Beta_HadEPFullsim_spECAL_ECAL->Integral());

  const Int_t nx = 4;
  const char *type[nx] = {"H","H+#pi_{0}^{f}","H+#pi_{0}^{l}","H+#pi_{0}^{f}+#pi_{0}^{l}"};
  Int_t i;
  for (i=1;i<=nx;i++) {
     Class->GetXaxis()->SetBinLabel(i,type[i-1]);
     Class->GetXaxis()->SetLabelSize(0.07);
  }


  const char *SPtype[nx] = {"Before ECAL","In ECAL","IN HCAL","After HCAL"};
  for (i=1;i<=nx;i++) {
     fSP->GetXaxis()->SetBinLabel(i,SPtype[i-1]);
     fSP->GetXaxis()->SetLabelSize(0.07);
  }


  fPi0->Write("",TObject::kOverwrite);
  fDp->Write("",TObject::kOverwrite);
  fPi0L->Write("",TObject::kOverwrite);
  fHadECAL->Write("",TObject::kOverwrite);
  Class->Write("",TObject::kOverwrite);
  HadEPFullsim_spHCAL->Write("",TObject::kOverwrite);
  HadEPFullsim_spECAL_HCAL->Write("",TObject::kOverwrite);
  HadEPFullsim_spECAL_ECAL->Write("",TObject::kOverwrite);
  HadEPFullsim_spECAL_HCALFit->Write("",TObject::kOverwrite);
  HadEPFullsim_spECAL_ECALFit->Write("",TObject::kOverwrite);
  HadEPFullsim_spHCALFit->Write("",TObject::kOverwrite);
  FirstPi0EPFullsim_spECAL_ECAL->Write("",TObject::kOverwrite);      
  FirstPi0EPFullsim_spECAL_HCAL->Write("",TObject::kOverwrite);      
  FirstPi0EPFullsim_spECAL_ECALFit->Write("",TObject::kOverwrite);   
  FirstPi0EPFullsim_spECAL_HCALFit->Write("",TObject::kOverwrite);   
  FirstPi0EPFullsim_spHCAL->Write("",TObject::kOverwrite);           
  FirstPi0EPFullsim_spHCALFit->Write("",TObject::kOverwrite);        

  LatePi0EPFullsim_spECAL_ECAL->Write("",TObject::kOverwrite);       
  LatePi0EPFullsim_spECAL_HCAL->Write("",TObject::kOverwrite);       
  LatePi0EPFullsim_spECAL_ECALFit->Write("",TObject::kOverwrite);    
  LatePi0EPFullsim_spECAL_HCALFit->Write("",TObject::kOverwrite);    
  LatePi0EPFullsim_spHCAL->Write("",TObject::kOverwrite);            
  LatePi0EPFullsim_spHCALFit->Write("",TObject::kOverwrite);         
  SP->Write("",TObject::kOverwrite);
  fSP->Write("",TObject::kOverwrite);
  h2_SpVsECALeOverHCALe->Write("",TObject::kOverwrite);
  h2_classVsSP->Write("",TObject::kOverwrite);

  Alpha_HadEPFullsim_spHCAL->Write("",TObject::kOverwrite);
  Alpha_HadEPFullsim_spECAL_ECAL ->Write("",TObject::kOverwrite);
  Alpha_HadEPFullsim_spECAL_HCAL->Write("",TObject::kOverwrite);
  Beta_HadEPFullsim_spHCAL->Write("",TObject::kOverwrite);
  Beta_HadEPFullsim_spECAL_ECAL  ->Write("",TObject::kOverwrite);
  Beta_HadEPFullsim_spECAL_HCAL->Write("",TObject::kOverwrite);
  AlphaBetaCorr_HadEPFullsim_spHCAL->Write("",TObject::kOverwrite);
  AlphaBetaCorr_HadEPFullsim_spECAL_ECAL->Write("",TObject::kOverwrite);
  AlphaBetaCorr_HadEPFullsim_spECAL_HCAL->Write("",TObject::kOverwrite);

/*
//try to do fastsimulation
//showers start in Hcal
  double meanalpha,meanbeta,RMSalpha,RMSbeta,corr;
  double x,y,alpha,beta,z1,z2;
  int n, nsudo;
  meanalpha = AlphaBetaCorr_HadEPFullsim_spHCAL->GetMean(1);
  meanbeta = AlphaBetaCorr_HadEPFullsim_spHCAL->GetMean(2);
  RMSalpha = AlphaBetaCorr_HadEPFullsim_spHCAL->GetRMS(1);
  RMSbeta = AlphaBetaCorr_HadEPFullsim_spHCAL->GetRMS(2);
  corr = AlphaBetaCorr_HadEPFullsim_spHCAL->GetCorrelationFactor();
  x = sqrt((1+corr)/2);
  y = sqrt((1-corr)/2);
  n=1000;
  nsudo=1000;
  std::vector<TH1F*> hists;
  for (int i = 0; i < nsudo; ++i){
    z1=gRandom->Gaus(0, 1);
    z2=gRandom->Gaus(0, 1);
    alpha = meanalpha + RMSalpha * x * z1 + RMSalpha *y *z2;
    beta = meanbeta + RMSbeta * x * z1 - RMSbeta *y *z2;
    Alpha_HadEPFastsim_spHCAL->Fill(alpha);
    Beta_HadEPFastsim_spHCAL->Fill(beta);
    std::stringstream randname;
    randname <<i;
    hists.push_back(RandomGammaProfile(exp(alpha) ,exp(beta), 10, 30, (randname.str()).c_str(),1));
  }
   for (int i = 1; i < hists.size(); ++i){
     hists[i]->Add(hists[i-1]);
   }

   hists[hists.size()-1]->Scale(1/hists[hists.size()-1]->Integral());
   hists[hists.size()-1]->SetName("HadEPFastsim_spHCAL");
   hists[hists.size()-1]->Write("",TObject::kOverwrite);
   for (int i = 1; i < hists.size(); ++i) delete hists[i];

//showers start in ECAL / Hcal part
  meanalpha = AlphaBetaCorr_HadEPFullsim_spECAL_HCAL->GetMean(1);
  meanbeta = AlphaBetaCorr_HadEPFullsim_spECAL_HCAL->GetMean(2);
  RMSalpha = AlphaBetaCorr_HadEPFullsim_spECAL_HCAL->GetRMS(1);
  RMSbeta = AlphaBetaCorr_HadEPFullsim_spECAL_HCAL->GetRMS(2);
  corr = AlphaBetaCorr_HadEPFullsim_spECAL_HCAL->GetCorrelationFactor();
  x = sqrt((1+corr)/2);
  y = sqrt((1-corr)/2);
  n=1000;
  nsudo=1000;
  std::vector<TH1F*> hists1;
  for (int i = 0; i < nsudo; ++i){
    z1=gRandom->Gaus(0, 1);
    z2=gRandom->Gaus(0, 1);
    alpha = meanalpha + RMSalpha * x * z1 + RMSalpha *y *z2;
    beta = meanbeta + RMSbeta * x * z1 - RMSbeta *y *z2;
    Alpha_HadEPFastsim_spECAL_HCAL->Fill(alpha);
    Beta_HadEPFastsim_spECAL_HCAL->Fill(beta);
    std::stringstream randname;
    randname <<i;
    hists1.push_back(RandomGammaProfile(exp(alpha) ,exp(beta), 10, 30, (randname.str()).c_str(),1));
  }
   for (int i = 1; i < hists1.size(); ++i){
     hists1[i]->Add(hists1[i-1]);
   }

   hists1[hists1.size()-1]->Scale(1/hists1[hists1.size()-1]->Integral());
   hists1[hists1.size()-1]->SetName("HadEPFastsim_spECAL_HCAL");
   hists1[hists1.size()-1]->Write("",TObject::kOverwrite);
   for (int i = 1; i < hists1.size(); ++i) delete hists1[i];
//showers start in ECAL / Ecal part
  meanalpha = AlphaBetaCorr_HadEPFullsim_spECAL_ECAL->GetMean(1);
  meanbeta = AlphaBetaCorr_HadEPFullsim_spECAL_ECAL->GetMean(2);
  RMSalpha = AlphaBetaCorr_HadEPFullsim_spECAL_ECAL->GetRMS(1);
  RMSbeta = AlphaBetaCorr_HadEPFullsim_spECAL_ECAL->GetRMS(2);
  corr = AlphaBetaCorr_HadEPFullsim_spECAL_ECAL->GetCorrelationFactor();
  x = sqrt((1+corr)/2);
  y = sqrt((1-corr)/2);
  n=1000;
  nsudo=1000;
  std::vector<TH1F*> hists2;
  for (int i = 0; i < nsudo; ++i){
    z1=gRandom->Gaus(0, 1);
    z2=gRandom->Gaus(0, 1);
    alpha = meanalpha + RMSalpha * x * z1 + RMSalpha *y *z2;
    beta = meanbeta + RMSbeta * x * z1 - RMSbeta *y *z2;
    Alpha_HadEPFastsim_spECAL_ECAL->Fill(alpha);
    Beta_HadEPFastsim_spECAL_ECAL->Fill(beta);
    std::stringstream randname;
    randname <<i;
    hists2.push_back(RandomGammaProfile(exp(alpha) ,exp(beta), 10, 30, (randname.str()).c_str(),1));
  }
   for (int i = 1; i < hists2.size(); ++i){
     hists2[i]->Add(hists2[i-1]);
   }

   hists2[hists2.size()-1]->Scale(1/hists2[hists2.size()-1]->Integral());
   hists2[hists2.size()-1]->SetName("HadEPFastsim_spECAL_ECAL");
   hists2[hists2.size()-1]->Write("",TObject::kOverwrite);
   for (int i = 1; i < hists2.size(); ++i) delete hists2[i];

  Alpha_HadEPFastsim_spHCAL->Scale(1/Alpha_HadEPFastsim_spHCAL->Integral());
  Alpha_HadEPFastsim_spECAL_HCAL->Scale(1/Alpha_HadEPFastsim_spECAL_HCAL->Integral());
  Alpha_HadEPFastsim_spECAL_ECAL->Scale(1/Alpha_HadEPFastsim_spECAL_ECAL->Integral());
  Beta_HadEPFastsim_spHCAL->Scale(1/Beta_HadEPFastsim_spHCAL->Integral());
  Beta_HadEPFastsim_spECAL_HCAL->Scale(1/Beta_HadEPFastsim_spECAL_HCAL->Integral());
  Beta_HadEPFastsim_spECAL_ECAL->Scale(1/Beta_HadEPFastsim_spECAL_ECAL->Integral());
  Alpha_HadEPFastsim_spHCAL->Write("",TObject::kOverwrite);
  Alpha_HadEPFastsim_spECAL_HCAL->Write("",TObject::kOverwrite);
  Alpha_HadEPFastsim_spECAL_ECAL->Write("",TObject::kOverwrite);
  Beta_HadEPFastsim_spHCAL->Write("",TObject::kOverwrite);
  Beta_HadEPFastsim_spECAL_HCAL->Write("",TObject::kOverwrite);
  Beta_HadEPFastsim_spECAL_ECAL->Write("",TObject::kOverwrite);
  f->Close();
*/
}
