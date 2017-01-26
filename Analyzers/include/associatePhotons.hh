#ifndef ASSOCIATEPHOTONS_HH
#define ASSOCIATEPHOTONS_HH

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

//Change everywhere associatePhotons by you Analyzer name
//Add the protected and private members and methods you want to add

class associatePhotons : public Analyzer
{
	public:
		associatePhotons(BaseAnalysis *ba);
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

		int getMostEnergetic(vector<double> e);

		//Add the variables that should be registered as output
		vector<TRecoLKrCandidate*> LKrClusters;
		TRecoLKrCandidate *clusterG;

		bool allowExport;
};
#endif
