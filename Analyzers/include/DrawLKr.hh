#ifndef DRAWLKR_HH
#define DRAWLKR_HH

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

//Change everywhere DrawLKr by you Analyzer name
//Add the protected and private members and methods you want to add

class DrawLKr : public Analyzer
{
	public:
		DrawLKr(BaseAnalysis *ba);
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
		void draw_LKr(TH2I* hist);

		//Add the variables that should be registered as output
		TCanvas *c;
};
#endif
