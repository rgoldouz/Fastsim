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

Gflash::Gflash()
    : theShowerType(-1),
      theEnergy(0){}

Gflash::~Gflash() { }

void Gflash::clean(){
  delete theHistEnergyHadronicEcal;
  delete theHistEnergyHadronicHcal;
  delete theHistEnergyPi0FirstEcal;
  delete theHistEnergyPi0FirstHcal;
  delete theHistEnergyPi0LateEcal;
  delete theHistEnergyPi0LateHcal;
  delete randomHist; 
}

void Gflash::initialize(       double energy,
                               double showerStartingPoint,
                               double depositEnergy,
                               double energyPi0First,
                               double energypi0Late,
                               TH1F *HistEnergyHadronicEcal,
                               TH1F *HistEnergyHadronicHcal,
                               TH1F *HistEnergyPi0FirstEcal,
                               TH1F *HistEnergyPi0FirstHcal,
                               TH1F *HistEnergyPi0LateEcal,
                               TH1F *HistEnergyPi0LateHcal
) {
  theEnergy = energy;
  theShowerSP = showerStartingPoint;
  theDepositEnergy = depositEnergy;
  theEnergyPi0First = energyPi0First;
  theEnergypi0Late = energypi0Late;
  theHistEnergyHadronicEcal = new TH1F("theHistEnergyHadronicEcal","theHistEnergyHadronicEcal", 30, 0, 15);
  theHistEnergyHadronicHcal = new TH1F("theHistEnergyHadronicHcal","theHistEnergyHadronicHcal", 30, 0, 15);
  theHistEnergyPi0FirstEcal = new TH1F("theHistEnergyPi0FirstEcal","theHistEnergyPi0FirstEcal", 30, 0, 15);
  theHistEnergyPi0FirstHcal = new TH1F("theHistEnergyPi0FirstHcal","theHistEnergyPi0FirstHcal", 30, 0, 15);
  theHistEnergyPi0LateEcal  = new TH1F("theHistEnergyPi0LateEcal","theHistEnergyPi0LateEcal", 30, 0, 15);
  theHistEnergyPi0LateHcal  = new TH1F("theHistEnergyPi0LateHcal","theHistEnergyPi0LateHcal", 30, 0, 15);

  for (int b = 1; b < HistEnergyHadronicEcal->GetNbinsX(); ++b){
    theHistEnergyHadronicEcal->Fill((HistEnergyHadronicEcal->GetXaxis()->GetBinCenter(b) - showerStartingPoint)/EcalIntLen, HistEnergyHadronicEcal->GetBinContent(b));
    theHistEnergyHadronicHcal->Fill((HistEnergyHadronicHcal->GetXaxis()->GetBinCenter(b) - showerStartingPoint)/HcalIntLen, HistEnergyHadronicHcal->GetBinContent(b));
    theHistEnergyPi0FirstEcal->Fill((HistEnergyPi0FirstEcal->GetXaxis()->GetBinCenter(b) - showerStartingPoint)/EcalRadLen, HistEnergyPi0FirstEcal->GetBinContent(b));
    theHistEnergyPi0FirstHcal->Fill((HistEnergyPi0FirstHcal->GetXaxis()->GetBinCenter(b) - showerStartingPoint)/HcalRadLen, HistEnergyPi0FirstHcal->GetBinContent(b));
    theHistEnergyPi0LateEcal->Fill((HistEnergyPi0LateEcal->GetXaxis()->GetBinCenter(b) - showerStartingPoint)/EcalIntLen, HistEnergyPi0LateEcal->GetBinContent(b));
    theHistEnergyPi0LateHcal->Fill((HistEnergyPi0LateHcal->GetXaxis()->GetBinCenter(b) - showerStartingPoint)/HcalIntLen, HistEnergyPi0LateHcal->GetBinContent(b));
  }

  alphaEnergyHadronicEcal = pow(theHistEnergyHadronicEcal->GetMean(),2)/pow(theHistEnergyHadronicEcal->GetRMS(),2);
  betaEnergyHadronicEcal = theHistEnergyHadronicEcal->GetMean()/pow(theHistEnergyHadronicEcal->GetRMS(),2);

  alphaEnergyHadronicHcal = pow(theHistEnergyHadronicHcal->GetMean(),2)/pow(theHistEnergyHadronicHcal->GetRMS(),2);
  betaEnergyHadronicHcal = theHistEnergyHadronicHcal->GetMean()/pow(theHistEnergyHadronicHcal->GetRMS(),2);

  alphaEnergyPi0FirstEcal = pow(theHistEnergyPi0FirstEcal->GetMean(),2)/pow(theHistEnergyPi0FirstEcal->GetRMS(),2);
  betaEnergyPi0FirstEcal = theHistEnergyPi0FirstEcal->GetMean()/pow(theHistEnergyPi0FirstEcal->GetRMS(),2);

  alphaEnergyPi0FirstHcal = pow(theHistEnergyPi0FirstHcal->GetMean(),2)/pow(theHistEnergyPi0FirstHcal->GetRMS(),2);
  betaEnergyPi0FirstHcal = theHistEnergyPi0FirstHcal->GetMean()/pow(theHistEnergyPi0FirstHcal->GetRMS(),2);

  alphaEnergyPi0LateEcal = pow(theHistEnergyPi0LateEcal->GetMean(),2)/pow(theHistEnergyPi0LateEcal->GetRMS(),2);
  betaEnergyPi0LateEcal = theHistEnergyPi0LateEcal->GetMean()/pow(theHistEnergyPi0LateEcal->GetRMS(),2);

  alphaEnergyPi0LateHcal = pow(theHistEnergyPi0LateHcal->GetMean(),2)/pow(theHistEnergyPi0LateHcal->GetRMS(),2);
  betaEnergyPi0LateHcal = theHistEnergyPi0LateHcal->GetMean()/pow(theHistEnergyPi0LateHcal->GetRMS(),2);

  if(theHistEnergyHadronicHcal->Integral()+theHistEnergyHadronicEcal->Integral()>0) EcaloHcalHadronic = theHistEnergyHadronicEcal->Integral()/(theHistEnergyHadronicHcal->Integral()+theHistEnergyHadronicEcal->Integral());
  if(theHistEnergyPi0FirstEcal->Integral()+theHistEnergyPi0FirstHcal->Integral()>0) EcaloHcalPi0First = theHistEnergyPi0FirstEcal->Integral() /(theHistEnergyPi0FirstEcal->Integral()+theHistEnergyPi0FirstHcal->Integral());
  if(theHistEnergyPi0LateEcal->Integral()+theHistEnergyPi0LateHcal->Integral()>0) EcaloHcalPi0Late = theHistEnergyPi0LateEcal->Integral() /(theHistEnergyPi0LateEcal->Integral()+theHistEnergyPi0LateHcal->Integral());

  if(theHistEnergyPi0FirstHcal->Integral()>0) EcaloHcalPi0First = theHistEnergyPi0FirstEcal->Integral()/theHistEnergyPi0FirstHcal->Integral();
  if(theHistEnergyPi0LateHcal->Integral()>0) EcaloHcalPi0Late = theHistEnergyPi0LateEcal->Integral()/theHistEnergyPi0LateHcal->Integral();

  if (theHistEnergyHadronicEcal->Integral()>0) theHistEnergyHadronicEcal->Scale(1/theHistEnergyHadronicEcal->Integral());
  if (theHistEnergyHadronicHcal->Integral()>0) theHistEnergyHadronicHcal->Scale(1/theHistEnergyHadronicHcal->Integral());
  if (theHistEnergyPi0FirstEcal->Integral()>0) theHistEnergyPi0FirstEcal->Scale(1/theHistEnergyPi0FirstEcal->Integral());
  if (theHistEnergyPi0FirstHcal->Integral()>0) theHistEnergyPi0FirstHcal->Scale(1/theHistEnergyPi0FirstHcal->Integral());
  if (theHistEnergyPi0LateEcal->Integral()>0) theHistEnergyPi0LateEcal->Scale(1/theHistEnergyPi0LateEcal->Integral());  
  if (theHistEnergyPi0LateHcal->Integral()>0) theHistEnergyPi0LateHcal->Scale(1/theHistEnergyPi0LateHcal->Integral());

  randomHist = new TH1F("Random profile","Random profile",30,0,15);
}


TH1F* Gflash::getRandomProfile(double alpha, double beta){
  std::gamma_distribution<double> distribution(alpha,1/beta);
  for (int j = 0; j < n; ++j){
     randomHist->Fill(distribution(generator));
  }
  if (randomHist->Integral()>0 ) randomHist->Scale(1/randomHist->Integral());
  else cout<<"Warning: randomHist integral==0"<<endl;

  if (randomHist->Integral()==0) randomHist->FillRandom("gaus", 1000);
  return randomHist;
}
