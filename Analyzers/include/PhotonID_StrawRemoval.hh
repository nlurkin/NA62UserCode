#ifndef PHOTONID_STRAWREMOVAL_HH
#define PHOTONID_STRAWREMOVAL_HH

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

class TRecoLKrCandidate;

//Change everywhere PhotonID_StrawRemoval by you Analyzer name
//Add the protected and private members and methods you want to add

class PhotonID_StrawRemoval : public Analyzer
{
	public:
		PhotonID_StrawRemoval(BaseAnalysis *ba);
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
		int getMostEnergetic(vector<double> e);
		DetectorAcceptance *fGeom; ///<Pointer to DetectorAcceptance instance. Initialize if used.

		//Add the variables that should be registered as output
		vector<TRecoLKrCandidate*> LKrClusters;
};
#endif
