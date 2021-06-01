#include "Analysis.h"
int main(){
TChain* TC;
TC = new TChain("g4SimHits/eventTree") ;
//TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_50GeV_042.root");
TC ->Add("/eos/cms/store/group/comm_fastsim/200k/RezaAnalysis_100GeV_10*.root");
Analysis GFRezaAnalysis(TC);
GFRezaAnalysis.Loop("RezaAnalysis_100gev");
delete TC;

}
