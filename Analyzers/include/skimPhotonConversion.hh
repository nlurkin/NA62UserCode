#ifndef SKIMPHOTONCONVERSION_HH
#define SKIMPHOTONCONVERSION_HH

#include <stdlib.h>
#include <vector>
#include "Analyzer.hh"
#include "MCSimple.hh"
#include "DetectorAcceptance.hh"


class TH1I;
class TH2F;
class TGraph;
class TCanvas;
class TTree;

//Change everywhere skimPhotonConversion by you Analyzer name
//Add the protected and private members and methods you want to add

class skimPhotonConversion : public Analyzer
{
	public:
		skimPhotonConversion(BaseAnalysis *ba);
		void InitHist();
		void InitOutput();
		void DefineMCSimple();
		void ProcessSpecialTriggers(int iEvent);
		void Process(int i);
		void PostProcess();
		void ExportPlot();
		void DrawPlot();
	private:
		void Publish();
	protected:
		DetectorAcceptance *fGeom; ///<Pointer to DetectorAcceptance instance. Initialize if used.

		//Add the variables that should be registered as output

		//Add your parameters (allowed : bool, char, string, TString, int, long, float, double)
};
#endif
