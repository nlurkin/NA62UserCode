#ifndef PHOTONID_HWCELLS_HH
#define PHOTONID_HWCELLS_HH

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

//Change everywhere PhotonID_HWCells by you Analyzer name
//Add the protected and private members and methods you want to add

class PhotonID_HWCells : public Analyzer
{
	public:
		PhotonID_HWCells(BaseAnalysis *ba);
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

		//Add the variables that should be registered as output
		vector<TRecoLKrCandidate*> LKrClusters;
		int getClusterIndexFromHW(double x, double y, double e);

		//Add your parameters (allowed : bool, char, string, TString, int, long, float, double)
};
#endif
