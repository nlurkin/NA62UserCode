//Mandatory includes
#include "BasicSelection.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

//Include the hh files for events kinds you will use
/*
#include "TRecoIRCEvent.hh"
#include "TRecoLAVEvent.hh"
#include "TRecoSACEvent.hh"
#include "TRecoLKrEvent.hh"*/
#include "TRecoGigaTrackerEvent.hh"
#include "TRecoSpectrometerEvent.hh"

//Additional includes
/*#include <TString.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TCanvas.h>*/

BasicSelection::BasicSelection(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "BasicSelection";

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	AddTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event
	//	AddTree("GigaTracker", new TGigaTrackerEvent);

	RequestTree("GigaTracker", new TRecoGigaTrackerEvent);
	RequestTree("Spectrometer", new TRecoSpectrometerEvent);

	//Initialize DetectorAcceptance if needed
	//	fGeom = new DetectorAcceptance("./NA62.gdml");
}


void BasicSelection::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*)
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));
	BookHisto("Selection", new TH1D("Selection", "Level of selection", 10, 0, 10));
	BookHisto("StrawEnergy", new TH1D("StrawEnergy", "Energy of straw candidates", 100, 0, 75000));
	BookHisto("StrawMultiplicity", new TH1D("StrawMultiplicity", "Straw multiplicity", 10, 0, 10));
	BookHisto("GTKMultiplicity", new TH1D("GTKMultiplicity", "Gtk Multiplicity", 10, 0, 10));

}

void BasicSelection::InitOutput(){
	//Call
	//RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class

	RegisterOutput("selectionLevel", &selectionLevel);
}

void BasicSelection::DefineMCSimple(MCSimple *fMCSimple){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple->AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//	int kaonID = fMCSimple->AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple->AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay

}

void BasicSelection::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	//if(fMCSimple.status == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	//if(fMCSimple.status == MCSimple::kEmpty) return;

	TRecoSpectrometerEvent *straw = (TRecoSpectrometerEvent*)GetEvent("Spectrometer");
	TRecoGigaTrackerEvent *gtk = (TRecoGigaTrackerEvent*)GetEvent("GigaTracker");
	TRecoSpectrometerCandidate *strawCand;

	double strawMinEnergy = 15000;
	double strawMaxEnergy = 55000;
	selectionLevel = 3;

	bool strawPt = true;

	FillHisto("GTKMultiplicity", gtk->GetNCandidates());
	if(gtk->GetNCandidates()==1) selectionLevel--;
	FillHisto("StrawMultiplicity", straw->GetNCandidates());
	if(straw->GetNCandidates()>=1){
		selectionLevel--;
		for(int i=0; i<straw->GetNCandidates(); i++){
			strawCand = (TRecoSpectrometerCandidate*)straw->GetCandidate(i);
			FillHisto("StrawEnergy", strawCand->GetMomentum().E());
			if(strawCand->GetMomentum().E()<strawMinEnergy || strawCand->GetMomentum().E()>strawMaxEnergy){
				strawPt = false;
			}
		}
	}
	if(strawPt) selectionLevel--;

	FillHisto("Selection", selectionLevel);

	SetOutputState("selectionLevel", kOValid);
	//You can retrieve MC particles from the fMCSimple Set with (return a vector<KinePart*>
	//	fMCSimple["particleName"]
	//	fMCSimple[pdgID]
	//Exemple
	//	fMCSimple["k+"][index] for the kaon
	//	fMCSimple["pi+"][index] for the positive pion
	//	fMCSimple["gamma"][index] for the photon
	//The number in the brackets is the index of the particle (if you asked for two photons in the set, you can ask fMCSimple["gamma"][0] for the first one and fMCSimple["gamma"][1] for the second)

	//You can retrieve the events from the trees with
	//	(eventClass*)fEvent["treeName"];

	//You can retrieve the histograms you booked (for drawing, changing, filling, ...) with
	//	fHisto["histoName"] for TH1
	//	fHisto2["histoName"] for TH2
	//	fGraph["graphName"] for TGraph and TGraphAsymmErrors
	//Be carefull !! If the histogram you ask for doesn't exist or you ask for an existing histogram
	//in the wrong recipient (e.g. th2 in fHisto), program will segfault.
	//To fill the histograms without risk of segfault, you can use
	//	FillHisto("histoName", values)
	//where values are the same parameters as if you call histogram->Fill(values) (x,y,weight,...)
	//Instead of segfault, this function print an error message

	//For use of fGeom, read DetectorAcceptance class

	//To use the output of a different analyzer, use
	//outputType var = *(outputType*)GetOutput("analyzerName.outputName", state);
	//Where outputType is the variable type and state is of type outputState
	//This function returns a void* and you have to explicitly cast it to the real object type
	//State is set with the state of the variable (kOUninit, kOInvalid ,kOValid). The value of the output should only be trusted if state == kOValid
	//example : TLorentzVector vertex = *(TLorentzVector*)GetOutput("simpleVertexAnalyzer.vertex", state);

	//Before starting the processing of an event, the state flag of each output variable is reset to kOUninit
	//When setting the value of an output variable, don't forget to set appropriately the state flag to either kOValid or kOInvalid
	//to indicate if the value can/can't be used in other analyzer
	//SetOutputState("outputName", kOValid);
}

void BasicSelection::PostProcess(){

}

void BasicSelection::ExportPlot(){
	//If you want to save them all, just call
		SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void BasicSelection::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
