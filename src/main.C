#include "Analysis.h"
int main(){
TChain* TC;
TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_2GeV_10*.root");
Analysis GFRezaAnalysis(TC);
GFRezaAnalysis.Loop("RezaAnalysis_2gev");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_5GeV_10*.root");
Analysis GFRezaAnalysis1(TC);
GFRezaAnalysis1.Loop("RezaAnalysis_5gev");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_10GeV_10*.root");
Analysis GFRezaAnalysis2(TC);
GFRezaAnalysis2.Loop("RezaAnalysis_10gev");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_20GeV_10*.root");
Analysis GFRezaAnalysis3(TC);
GFRezaAnalysis3.Loop("RezaAnalysis_100gev");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_50GeV_10*.root");
Analysis GFRezaAnalysis4(TC);
GFRezaAnalysis4.Loop("RezaAnalysis_50gev");
delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_100GeV_10*.root");
Analysis GFRezaAnalysis5(TC);
GFRezaAnalysis5.Loop("RezaAnalysis_100gev");
delete TC;

}
