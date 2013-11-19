//Mandatory includes
#include "pipAnalysis.hh"
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
#include "TRecoSACEvent.hh"*/
#include "TRecoLKrEvent.hh"

//Additional includes
/*#include <TString.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TCanvas.h>*/

pipAnalysis::pipAnalysis(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "pipAnalysis";

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	RequestRecoTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event
	//	RequestMCTree("GigaTracker", new TGigaTrackerEvent);
	RequestTree("LKr", new TRecoLKrEvent);

	//Initialize DetectorAcceptance if needed
	//use of global instance
	//	fGeom = GetDetectorAcceptanceInstance();
	//use of local instance
	//	fGeom = new DetectorAcceptance("./NA62.gdml");
}


void pipAnalysis::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*)
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of partice", 0, 0, 0, Bins, MinEnergy, MaxEnergy));

	BookHisto("ClusterDistance", new TH1D("ClusterDistance", "Distance between cluster and pip", 200, 0, 2000));
	BookHisto("pipEnergy", new TH1D("pipEnergy", "Pip energy", 75, 0, 75000));
	BookHisto("NCells", new TH1D("NCells", "Number of cells", 120, 0, 120));

	for(Long_t i=0; i<120; i++){
		BookHisto(TString("RMSXCell") + i, new TH1D(TString("RMSXCell") + i, TString("RMSX for NCells=") + i, 150, 0, 1.5));
	}

}

void pipAnalysis::InitOutput(){
	//Call
	//	RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class

	//Call
	//	AddParam("paramName", "paramType", &variableName, defaultValue);
	//for each parameter of the analyzer. These parameters can be set when starting the FW from the command line with the -p option.
	//paramName is the name of the parameter in the command line
	//paramType is the type of the parameter (allowed : bool, int, long, float, double, char, string, TString)
	//variableName is the name of the variable that should be declared in the definition of the class<br>
	//defaultValue is the default value if not specified in the command line
}

void pipAnalysis::DefineMCSimple(MCSimple *fMCSimple){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple->AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//int kaonID = fMCSimple->AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple->AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	fMCSimple->AddParticle(0, 211);

}

void pipAnalysis::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	int LKrStartPos = 240413;
	//Event
	TRecoLKrEvent *lkr = (TRecoLKrEvent*)GetEvent("LKr");

	TRecoLKrCandidate *lkrCand;

	TVector3 straw = spec_propagate(fMCSimple["pi+"][0]->GetProdPos().Vect(), fMCSimple["pi+"][0]->GetInitialMomentum(), +1, LKrStartPos);
	FillHisto("pipEnergy", fMCSimple["pi+"][0]->GetInitialEnergy());

	for(int i=0; i<lkr->GetNCandidates(); i++){
		lkrCand = (TRecoLKrCandidate*)lkr->GetCandidate(i);
		FillHisto("NCells", lkrCand->GetNCells());
		FillHisto("ClusterDistance", sqrt(pow(straw.X()-lkrCand->GetClusterX(), 2)+ pow(straw.Y()-lkrCand->GetClusterY(), 2)));

		FillHisto(TString("RMSXCell") + (Long_t)lkrCand->GetNCells(), lkrCand->GetClusterRMSX());
	}
}

void pipAnalysis::PostProcess(){

}

void pipAnalysis::ExportPlot(){
	//If you want to save them all, just call
	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void pipAnalysis::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
