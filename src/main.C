#include "Analysis.h"
int main(){
TChain* TC;
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_2GeV_10*.root");
//Analysis GFRezaAnalysis(TC);
//GFRezaAnalysis.Loop("RezaAnalysis_2gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_5GeV_10*.root");
//Analysis GFRezaAnalysis1(TC);
//GFRezaAnalysis1.Loop("RezaAnalysis_5gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_10GeV_10*.root");
//Analysis GFRezaAnalysis2(TC);
//GFRezaAnalysis2.Loop("RezaAnalysis_10gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_20GeV_10*.root");
//Analysis GFRezaAnalysis3(TC);
//GFRezaAnalysis3.Loop("RezaAnalysis_20gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_50GeV_10*.root");
//Analysis GFRezaAnalysis4(TC);
//GFRezaAnalysis4.Loop("RezaAnalysis_50gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_100GeV_10*.root");
//Analysis GFRezaAnalysis5(TC);
//GFRezaAnalysis5.Loop("RezaAnalysis_100gev");
//delete TC;

//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_1GeV_10*.root");
//Analysis GFRezaAnalysis6(TC);
//GFRezaAnalysis6.Loop("RezaAnalysis_1gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_7GeV_10*.root");
//Analysis GFRezaAnalysis7(TC);
//GFRezaAnalysis7.Loop("RezaAnalysis_7gev");
//delete TC;
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_30GeV_10*.root");
//Analysis GFRezaAnalysis8(TC);
//GFRezaAnalysis8.Loop("RezaAnalysis_30gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_40GeV_10*.root");
//Analysis GFRezaAnalysis9(TC);
//GFRezaAnalysis9.Loop("RezaAnalysis_40gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_70GeV_10*.root");
//Analysis GFRezaAnalysis10(TC);
//GFRezaAnalysis10.Loop("RezaAnalysis_70gev");
//delete TC;
//
//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_150GeV_10*.root");
//Analysis GFRezaAnalysis11(TC);
//GFRezaAnalysis11.Loop("RezaAnalysis_150gev");
//delete TC;

TC = new TChain("g4SimHits/eventTree") ;
TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_15GeV_10*.root");
Analysis GFRezaAnalysis12(TC);
GFRezaAnalysis12.Loop("RezaAnalysis_15gev");
delete TC;


//TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_200GeV_10*.root");
//Analysis GFRezaAnalysis13(TC);
//GFRezaAnalysis13.Loop("RezaAnalysis_200gev");
//delete TC;


}
