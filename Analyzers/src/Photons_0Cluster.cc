//Mandatory includes
#include "Photons_0Cluster.hh"
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

Photons_0Cluster::Photons_0Cluster(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "Photons_0Cluster";
	RequestTree("LKr", new TRecoLKrEvent);

	//fGeom = GetDetectorAcceptanceInstance();
}


void Photons_0Cluster::InitHist(){
	BookHisto("ClusterOnLKr", new TH2D("ClusterOnLKr", "Extrapolated position of photon on LKr (inside LKr)", 200, -2000, 2000, 200, -2000, 2000));
	BookHisto("ClusterOutLKr", new TH2D("ClusterOutLKr", "Extrapolated position of photon on LKr (outside LKr)", 200, -2000, 2000, 200, -2000, 2000));
	BookHisto("OutsideLKr", new TH1D("OutsideLKr", "Number of events outside LKr acceptance (blarge angle)", 1, 0, 1));
	BookHisto("BeamPipe", new TH1D("BeamPipe", "Number of events outside LKr acceptance (beam pipe)", 1, 0, 1));
	BookHisto("beamPipe", new TH1D("beamPipe", "Number of events in beam pipe", 1, 0, 1));

	BookHisto("endPos", new TH1D("endPos", "EndPos of Photons inside LKr", 200, 140000, 300000));
}

void Photons_0Cluster::InitOutput(){
}

void Photons_0Cluster::DefineMCSimple(){
	fMCSimple.AddParticle(0, 22);
}

void Photons_0Cluster::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	TVector3 pos;
	int LKrStartPos = 240413;
	bool outside = false;
	double radius;

	//fGeom->FillPath(fMCSimple[22][0]->GetProdPos().Vect(), fMCSimple[22][0]->GetInitialMomentum());
	pos = propagate(fMCSimple[22][0]->GetProdPos().Vect(), fMCSimple[22][0]->GetInitialMomentum(), LKrStartPos);
	radius = sqrt(pow(pos.X(), 2) + pow(pos.Y(), 2));
	if(radius<180) FillHisto("beamPipe", 0);
	if(radius<180 || radius>1000) outside = true;

	if(outside){
		FillHisto("ClusterOutLKr", pos.x(), pos.Y());
	}
	else{
		FillHisto("ClusterOnLKr", pos.x(), pos.Y());
		FillHisto("endPos", fMCSimple[22][0]->GetEndPos().Z());
		if(fMCSimple[22][0]->GetEndPos().Z()>240000) ExportEvent();
	}

}

void Photons_0Cluster::PostProcess(){

}

void Photons_0Cluster::ExportPlot(){
	//If you want to save them all, just call
	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void Photons_0Cluster::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
