#ifndef ID_HWCLUSTERPI_HH
#define ID_HWCLUSTERPI_HH

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

//Change everywhere ID_HWClusterPi by you Analyzer name
//Add the protected and private members and methods you want to add

class ID_HWClusterPi : public Analyzer
{
	public:
		ID_HWClusterPi(BaseAnalysis *ba);
		void InitHist();
		void InitOutput();
		void DefineMCSimple();
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
		vector<int> HWPiID;
		//Add your parameters (allowed : bool, char, string, TString, int, long, float, double)
		int piNumber;
		//Add the variables to branch to the output trees
};
#endif
