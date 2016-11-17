#ifndef SINGLEPHOTON_HH
#define SINGLEPHOTON_HH

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
class TRecoLKrEvent;

//Change everywhere SinglePhoton by you Analyzer name
//Add the protected and private members and methods you want to add

class SinglePhoton : public Analyzer
{
	public:
		SinglePhoton(BaseAnalysis *ba);
		void InitHist();
		void InitOutput();
		void DefineMCSimple();
		void Process(int i);
		void PostProcess();
		void ExportPlot();
		void DrawPlot();
	private:
		void Publish();
	protected:
		DetectorAcceptance *fGeom; ///<Pointer to DetectorAcceptance instance. Initialize if used.
		double CountHitsEnergy(TRecoLKrEvent *evt);
		int getMostEnergetic(TRecoLKrEvent *evt);
		TString whichDetector(TString prefix, int pos);

		//Add the variables that should be registered as output

		//Add your parameters (allowed : bool, char, string, TString, int, long, float, double)
};
#endif
