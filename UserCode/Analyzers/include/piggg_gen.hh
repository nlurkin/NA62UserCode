#ifndef PIGGG_GEN_HH
#define PIGGG_GEN_HH

#include <stdlib.h>
#include <vector>
#include "Analyzer.hh"
#include "MCSimple.hh"
#include "DetectorAcceptance.hh"
#include <TCanvas.h>
using namespace std;

class TH1I;
class TH2F;
class TGraph;
class TTree;

//Change everywhere piggg_gen by you Analyzer name
//Add the protected and private members and methods you want to add

class piggg_gen : public Analyzer
{
	public:
		piggg_gen(BaseAnalysis *ba);
		void InitHist();
		void InitOutput();
		void DefineMCSimple(MCSimple *fMCSimple);
		void Process(int i, MCSimple &fMCSimple, Event* MCTruthEvent);
		void PostProcess();
		void ExportPlot();
		void DrawPlot();
		void NewBurstUser();
	private:
		void Publish();
	protected:
		//Add the variables that should be registered as output
		bool pass;
		double W,T;
		//Add your parameters (allowed : bool, char, string, TString, int, long, float, double)

		//Add the variables to branch to the output trees
};
#endif
