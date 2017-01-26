#ifndef PI0ID_HH
#define PI0ID_HH

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
class TRecoLKrCandidate;

//Change everywhere Pi0ID by you Analyzer name
//Add the protected and private members and methods you want to add

class Pi0ID : public Analyzer
{
	public:
		Pi0ID(BaseAnalysis *ba);
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
		vector<TRecoLKrCandidate*> LKrClusters;

		//Add your parameters (allowed : bool, char, string, TString, int, long, float, double)
};
#endif
