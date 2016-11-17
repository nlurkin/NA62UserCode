//Mandatory includes
#include "clusterNN.hh"
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

#include "NeuralNetwork.hh"

clusterNN::clusterNN(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "clusterNN";

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	RequestRecoTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event
	//	RequestMCTree("GigaTracker", new TGigaTrackerEvent);
	//RequestRecoTree("LKr", new TRecoLKrEvent);
	RequestTree("LKr", new TRecoLKrEvent);

	//Initialize DetectorAcceptance if needed
	//use of global instance
	//	fGeom = GetDetectorAcceptanceInstance();
	//use of local instance
	//	fGeom = new DetectorAcceptance("./NA62.gdml");
	nn = new NeuralNetwork();

	//nn->AddInput("fClusterRMSX", &fClusterRMSX, "fClusterRMSX/F");
	//nn->AddInput("fClusterRMSY", &fClusterRMSY, "fClusterRMSY/F");
	//nn->AddInput("fClusterTime", &fClusterTime, "fClusterTime/F");
	//nn->AddInput("fNCells", &fNCells, "fNCells/I");
	nn->AddInput("fClusterEnergy", &fClusterEnergy, "fClusterEnergy/F");
	nn->AddInput("fClusterSeedEnergy", &fClusterSeedEnergy, "fClusterSeedEnergy/F");
	//nn->AddInput("fClusterEnergyError", &fClusterEnergyError, "fClusterEnergyError/F");

	nn->AddLayer(3);
	nn->AddLayer(2);
}


void clusterNN::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*)
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));

}

void clusterNN::InitOutput(){
	//Call
	//RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class
}

void clusterNN::DefineMCSimple(){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple.AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//	int kaonID = fMCSimple.AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple.AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	fMCSimple.AddParticle(0,22);

}

void clusterNN::Process(int iEvent){
	Event*  MCTruthEvent = NULL;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	//if(fMCSimple.status == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	//if(fMCSimple.status == MCSimple::kEmpty) return;

	OutputState state;


	TRecoLKrEvent *lkr = (TRecoLKrEvent*)GetEvent("LKr");
	TRecoLKrCandidate *cand;

	for(int i=0; i<lkr->GetNCandidates(); i++){
		cand = (TRecoLKrCandidate*)lkr->GetCandidate(i);
		fClusterRMSX = cand->GetClusterRMSX();
		fClusterRMSY = cand->GetClusterRMSY();
		fClusterTime = cand->GetClusterTime();
		fNCells = cand->GetNCells();
		fClusterEnergy = cand->GetClusterEnergy();
		fClusterStatus = cand->GetClusterStatus();
		fClusterEnergyError = cand->GetClusterEnergyError();
		fClusterSeedEnergy = cand->GetClusterSeedEnergy();
		if(fMCSimple[22].size()>0) nn->FillSignal();
		else nn->FillBackground();
	}
}

void clusterNN::PostProcess(){

}

void clusterNN::ExportPlot(){
	nn->Train(50000);
	nn->Save("Testpipgamma.txt");
	//cout << nn->Evaluate() << endl;


	//If you want to save them all, just call
	//	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void clusterNN::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
