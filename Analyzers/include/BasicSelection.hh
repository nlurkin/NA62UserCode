#ifndef BASICSELECTION_HH
#define BASICSELECTION_HH

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

//Change everywhere BasicSelection by you Analyzer name
//Add the protected and private members and methods you want to add

class BasicSelection : public Analyzer
{
	public:
		BasicSelection(BaseAnalysis *ba);
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
		int selectionLevel;
};
#endif
