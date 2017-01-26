#ifndef PHOTONID_SELECTFIRSTS_HH
#define PHOTONID_SELECTFIRSTS_HH

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

//Change everywhere PhotonID_SelectFirsts by you Analyzer name
//Add the protected and private members and methods you want to add

class PhotonID_SelectFirsts : public Analyzer
{
	public:
		PhotonID_SelectFirsts(BaseAnalysis *ba);
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
		vector<TRecoLKrCandidate*> lkrClusters;
};
#endif
