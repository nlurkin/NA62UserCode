#ifndef PHOTONS_0CLUSTER_HH
#define PHOTONS_0CLUSTER_HH

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

//Change everywhere Photons_0Cluster by you Analyzer name
//Add the protected and private members and methods you want to add

class Photons_0Cluster : public Analyzer
{
	public:
		Photons_0Cluster(BaseAnalysis *ba);
		void InitHist();
		void InitOutput();
		void DefineMCSimple(MCSimple *fMCSimple);
		void Process(int i, MCSimple &fMCSimple, Event* MCTruthEvent);
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
