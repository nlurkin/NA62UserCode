#ifndef PHOTONID_RMSSELECTION_HH
#define PHOTONID_RMSSELECTION_HH

#include <stdlib.h>
#include <vector>
#include "Analyzer.hh"
#include "MCSimple.hh"
#include "DetectorAcceptance.hh"

using namespace std;

class TH1I;
class TH2F;
class TGraph;
class TCanvas;
class TTree;
class TRecoLKrCandidate;

//Change everywhere PhotonID_RMSSelection by you Analyzer name
//Add the protected and private members and methods you want to add

class PhotonID_RMSSelection : public Analyzer
{
	public:
		PhotonID_RMSSelection(BaseAnalysis *ba);
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
		vector<TRecoLKrCandidate*> LKrClusters;

		bool loose;
};
#endif
