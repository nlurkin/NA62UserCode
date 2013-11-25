#ifndef PIGGG_HW_HH
#define PIGGG_HW_HH

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

//Change everywhere piggg_HW by you Analyzer name
//Add the protected and private members and methods you want to add

class piggg_HW : public Analyzer
{
	public:
		piggg_HW(BaseAnalysis *ba);
		void InitHist();
		void InitOutput();
		void DefineMCSimple(MCSimple *fMCSimple);
		void Process(int i, MCSimple &fMCSimple, Event* MCTruthEvent);
		void StartOfBurstUser();
		void EndOfBurstUser();
		void StartOfRunUser();
		void EndOfRunUser();
		void PostProcess();
		void ExportPlot();
		void DrawPlot();
	private:
		void Publish();
	protected:
		//Add the variables that should be registered as output
		bool pass;

		//Add your parameters (allowed : bool, char, string, TString, int, long, float, double)

		//Add the variables to branch to the output trees
};
#endif
