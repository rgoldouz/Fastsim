#ifndef Gflash_H
#define Gflash_H
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include <iostream>
#include <random>

using namespace std;
class Gflash {
public:
  Gflash();
  ~Gflash();
  void initialize(             double energy,
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
  );

  void clean();
  TH1F* getRandomProfile(double alpha, double beta, TH1F* rnd);
//  Double_t gammaProfile(Double_t *x,Double_t *par);

  int theShowerType;
  double theEnergy;
  double theShowerSP;
  double theDepositEnergy;
  double theEnergyPi0First;
  double theEnergypi0Late;
  TH1F *theHistEnergyHadronicEcal;
  TH1F *theHistEnergyHadronicHcal;
  TH1F *theHistEnergyPi0FirstEcal;
  TH1F *theHistEnergyPi0FirstHcal;
  TH1F *theHistEnergyPi0LateEcal;
  TH1F *theHistEnergyPi0LateHcal;       

  double alphaEnergyHadronicEcal;  
  double alphaEnergyHadronicHcal; 
  double alphaEnergyPi0FirstEcal;
  double alphaEnergyPi0FirstHcal; 
  double alphaEnergyPi0LateEcal; 
  double alphaEnergyPi0LateHcal;
  double betaEnergyHadronicEcal; 
  double betaEnergyHadronicHcal; 
  double betaEnergyPi0FirstEcal;
  double betaEnergyPi0FirstHcal; 
  double betaEnergyPi0LateEcal;
  double betaEnergyPi0LateHcal; 
  double EcaloHcalHadronic = 0;
  double EcaloHcalPi0First = 0;
  double EcaloHcalPi0Late = 0;

private:
  double EcalRadLen =89;
  double HcalRadLen =143;
  double HcalIntLen =150.5;
  double EcalIntLen =300;
  int n=1000;
  TH1F *randomHist;
  std::default_random_engine generator;
};

#endif
