//Mandatory includes
#include "PhotonID_SelectFirsts.hh"
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
#include "TRecoLKrEvent.hh"
#include "TRecoSpectrometerEvent.hh"*/

//Additional includes
/*#include <TString.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TCanvas.h>*/
#include <TGraphAsymmErrors.h>


PhotonID_SelectFirsts::PhotonID_SelectFirsts(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "PhotonID_SelectFirsts";

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	AddTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event
	//	AddTree("GigaTracker", new TGigaTrackerEvent);

	//Initialize DetectorAcceptance if needed
	//	fGeom = new DetectorAcceptance("./NA62.gdml");
}


void PhotonID_SelectFirsts::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*)
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));

	BookHisto("multiplicity", new TH1D("multiplicity", "Number of clusters left after energy cut", 11, -0.5, 10.5));

	BookHisto("totalEvents", new TH1I("totalEvents", "Total number of events", 1, 0, 1));
	BookHisto("goodEvents", new TH1I("goodEvents", "Number of good events", 1, 0, 1));
	BookHisto("graphGoodEvents", new TGraphAsymmErrors());

	BookHisto("BadEnergyCut", new TH1D("BadFinal", "Events where StrawRemoval analyzer doesn't returns a kOValid state for SuspectCluster", 1, 0, 1));
	BookHisto("BadclusterG1", new TH1D("BadclusterG1", "Events where Acceptance analyzer doesn't returns a kOValid state for clusterG1", 1, 0, 1));
	BookHisto("BadclusterG2", new TH1D("BadclusterG2", "Events where Acceptance analyzer doesn't returns a kOValid state for clusterG2", 1, 0, 1));


}

void PhotonID_SelectFirsts::InitOutput(){
	//Call
	//RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class
	RegisterOutput("photonCandidates", &lkrClusters);
}

void PhotonID_SelectFirsts::DefineMCSimple(MCSimple *fMCSimple){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple->AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//	int kaonID = fMCSimple->AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple->AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay

}

void PhotonID_SelectFirsts::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	OutputState state;
	lkrClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("PhotonID_SeedFraction.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadEnergyCut", 0);
		return;
	}
	TRecoLKrCandidate *clusterG1 = *(TRecoLKrCandidate**)GetOutput("associatePhotons.clusterG1", state);
	if(state!=kOValid){
		FillHisto("BadclusterG1", 0);
		return;
	}
	TRecoLKrCandidate *clusterG2 = *(TRecoLKrCandidate**)GetOutput("associatePhotons.clusterG2", state);
	if(state!=kOValid){
		FillHisto("BadclusterG2", 0);
		return;
	}

	SetOutputState("photonCandidates", kOValid);

	FillHisto("multiplicity", lkrClusters.size());

	FillHisto("totalEvents", 0);

	if(lkrClusters.size()>=2){
		if( (clusterG1==lkrClusters[0] || clusterG1==lkrClusters[1]) && (clusterG2==lkrClusters[0] || clusterG2==lkrClusters[1]) ) FillHisto("goodEvents", 0);
	}

}

void PhotonID_SelectFirsts::PostProcess(){

}

void PhotonID_SelectFirsts::ExportPlot(){
	TGraphAsymmErrors *good = (TGraphAsymmErrors*)fGraph["graphGoodEvents"];
	good->BayesDivide(fHisto["goodEvents"], fHisto["totalEvents"]);
	//If you want to save them all, just call
	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void PhotonID_SelectFirsts::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
