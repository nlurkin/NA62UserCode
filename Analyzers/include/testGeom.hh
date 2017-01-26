#ifndef TESTGEOM_HH
#define TESTGEOM_HH

#include <stdlib.h>
#include <vector>
#include "Analyzer.hh"
#include "MCSimple.hh"
#include "DetectorAcceptance.hh"
#include <TCanvas.h>

class TH1I;
class TH2F;
class TGraph;
class TTree;

//Change everywhere testGeom by you Analyzer name
//Add the protected and private members and methods you want to add

class testGeom : public Analyzer
{
	public:
		testGeom(BaseAnalysis *ba);
		void InitHist();
		void InitOutput();
		void DefineMCSimple();
		void ProcessSpecialTriggers(int iEvent);
		void Process(int i);
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

		//Add your parameters (allowed : bool, char, string, TString, int, long, float, double)

		//Add the variables to branch to the output trees
};
#endif
