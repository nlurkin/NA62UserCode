//Mandatory includes
#include "Photon_MultiCluster.hh"
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
#include "TRecoLKrEvent.hh"
//Additional includes
/*#include <TString.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TCanvas.h>*/

Photon_MultiCluster::Photon_MultiCluster(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "Photon_MultiCluster";
	RequestTree("LKr", new TRecoLKrEvent);

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	RequestTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event
	//	RequestTree("GigaTracker", new TGigaTrackerEvent);

	//Initialize DetectorAcceptance if needed
	//use of global instance
	//	fGeom = GetDetectorAcceptanceInstance();
	//use of local instance
	//	fGeom = new DetectorAcceptance("./NA62.gdml");
}


void Photon_MultiCluster::InitHist(){
	BookHisto("ClusterOnLKr", new TH2D("ClusterOnLKr", "Extrapolated position of photon on LKr (inside LKr)", 200, -2000, 2000, 200, -2000, 2000));
	BookHisto("endPos", new TH1D("endPos", "EndPos of Photons inside LKr", 200, 140000, 300000));

	BookHisto("distanceLKr", new TH1D("distanceLKr", "Distance between extrapolated position of photon on LKr and reco cluster", 200, 0, 1000));
	BookHisto("distanceOutLKr", new TH1D("distanceOutLKr", "Distance between extrapolated position of photon on LKr and reco cluster", 200, 0, 1000));

	BookHisto("energy", new TH1D("energy", "Cluster energy", 200, 0, 75000));
	BookHisto("energyFrac", new TH1D("energyFrac", "Cluster energy/gamma energy", 200, 0, 1));
}

void Photon_MultiCluster::InitOutput(){

}

void Photon_MultiCluster::DefineMCSimple(MCSimple *fMCSimple){
	fMCSimple->AddParticle(0, 22);
}

void Photon_MultiCluster::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	TVector3 pos;
	int LKrStartPos = 240413;
	double distance;

	TRecoLKrEvent *evt;
	TRecoLKrCandidate *lkr;

	evt = (TRecoLKrEvent*)GetEvent("LKr");
	pos = propagate(fMCSimple[22][0]->GetProdPos().Vect(), fMCSimple[22][0]->GetInitialMomentum(), LKrStartPos);
	FillHisto("ClusterOnLKr", pos.x(), pos.Y());
	FillHisto("endPos", fMCSimple[22][0]->GetEndPos().Z());

	if(fMCSimple[22][0]->GetEndPos().Z()<220ssqss000 && fMCSimple[22][0]->GetEndPos().Z()>182000){

		for(unsigned int i=0; i<evt->GetNCandidates(); i++){
			lkr = (TRecoLKrCandidate*)evt->GetCandidate(i);
			distance = sqrt(pow(pos.X()-lkr->GetClusterX(), 2) + pow(pos.Y()-lkr->GetClusterY(), 2));
			if(fMCSimple[22][0]->GetEndPos().Z()<240000) FillHisto("distanceOutLKr", distance);
			else FillHisto("distanceLKr", distance);

			FillHisto("energy", lkr->GetClusterEnergy()*1000);
			FillHisto("energyFrac", lkr->GetClusterEnergy()*1000/fMCSimple[22][0]->GetInitialEnergy());
		}
		ExportEvent();
	}


}

void Photon_MultiCluster::PostProcess(){

}

void Photon_MultiCluster::ExportPlot(){
	SaveAllPlots();
}

void Photon_MultiCluster::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
