#ifndef DRAWLKR_HH
#define DRAWLKR_HH

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

//Change everywhere DrawLKr by you Analyzer name
//Add the protected and private members and methods you want to add

class DrawLKr : public Analyzer
{
	public:
		DrawLKr(BaseAnalysis *ba);
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
		void draw_LKr(TH2I* hist);

		//Add the variables that should be registered as output
		TCanvas *c;
};
#endif
