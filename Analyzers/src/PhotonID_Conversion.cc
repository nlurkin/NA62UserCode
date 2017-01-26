//Mandatory includes
#include "PhotonID_Conversion.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

//Include the hh files for events kinds you will use
/*#include "TRecoGigaTrackerEvent.hh"
#include "TRecoIRCEvent.hh"
#include "TRecoLAVEvent.hh"
#include "TRecoSACEvent.hh"
#include "TRecoSpectrometerEvent.hh"*/
#include "TRecoLKrEvent.hh"

//Additional includes
/*#include <TString.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TCanvas.h>*/

PhotonID_Conversion::PhotonID_Conversion(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "PhotonID_Conversion";

	RequestTree("LKr", new TRecoLKrEvent);
}


void PhotonID_Conversion::InitHist(){
	BookHisto("candidateTime", new TH1D("candidateTime", "LKr candidate time", 2000, -1000, 1000));
	BookHisto("allOut", new TH1D("allOut", "Events where all out of time", 1, 0, 1));
	BookHisto("notAllOut", new TH1D("notAllOut", "Events where at least one in time", 1, 0, 1));
}

void PhotonID_Conversion::InitOutput(){
	//Call
	//	RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class
}

void PhotonID_Conversion::DefineMCSimple(){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple.AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//	int kaonID = fMCSimple.AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple.AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	fMCSimple.AddParticle(0,22);
}

void PhotonID_Conversion::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	TRecoLKrEvent *evt = (TRecoLKrEvent*)GetEvent("LKr");
	TRecoLKrCandidate *lkr;
	double time;
	bool inTime = false;
	bool outTime = false;

	for(unsigned int i=0; i<evt->GetNCandidates(); i++){
		lkr = (TRecoLKrCandidate*)evt->GetCandidate(i);

		time = lkr->GetTime();

		FillHisto("candidateTime", time);
		if(time==0) inTime = true;
		else outTime = true;
	}

	if(outTime){
		if(inTime) FillHisto("notAllOut", 0);
		else FillHisto("allOut", 0);
	}
}

void PhotonID_Conversion::PostProcess(){

}

void PhotonID_Conversion::ExportPlot(){
	//If you want to save them all, just call
	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void PhotonID_Conversion::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
