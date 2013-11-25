#define LOOPI
//Mandatory includes
#include "PhotonID_HWCells.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"

#include <TGraphAsymmErrors.h>

PhotonID_HWCells::PhotonID_HWCells(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "PhotonID_HWCells";

	RequestTree("LKr", new TRecoLKrEvent);
}

void PhotonID_HWCells::InitHist(){
	BookHisto("multiplicity", new TH1D("multiplicity", "Number of clusters left after bad RMS removing", 11, -0.5, 10.5));
	BookHisto("totalEvents", new TH1I("totalEvents", "Total number of events", 11, -0.5, 10.5));
	BookHisto("removedGamma", new TH1I("removedGamma", "Number of events where at least 1 gamma cluster has been removed", 11, -0.5, 10.5));
	BookHisto("removedPip", new TH1I("removedPip", "Number of events where at least 1 pi+ cluster has been removed", 11, -0.5, 10.5));
	BookHisto("graphRemovedGamma", new TGraphAsymmErrors());
	BookHisto("graphRemovedPip", new TGraphAsymmErrors());

	BookHisto("NHWCells", new TH1D("NHWCells", "Number of HW cells for all clusters", 11, -0.5, 10.5));

	BookHisto("BadLKrCluster", new TH1D("BadLKrCluster", "Events where StrawRemoval analyzer doesn't returns a kOValid state for SuspectCluster", 1, 0, 1));

	BookHisto("thrEfficiency", new TGraphAsymmErrors());
}

void PhotonID_HWCells::InitOutput(){
	RegisterOutput("LKrClusters", &LKrClusters);
}

void PhotonID_HWCells::DefineMCSimple(MCSimple *fMCSimple){
	fMCSimple->AddParticle(0, 22);
	fMCSimple->AddParticle(0, 211);
}

void PhotonID_HWCells::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	//LOOP variables
	vector<TRecoLKrCandidate*> tempV;
	bool first = true;
	int removed = 0;

	//Event type
	bool isGamma = false;
	bool isPip = false;

	//Cut variables
	int ncells;
	int iHW;
	int thrCells=1;

	//Event
	TRecoLKrEvent *lkr = (TRecoLKrEvent*)GetEvent("LKr");

	//What kind of event do we have?
	if(fMCSimple.Size(22)>0) isGamma = true;
	else if(fMCSimple.Size(211)>0) isPip = true;

	//Get clusters
	OutputState state;
	LKrClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("associatePhotons.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadLKrCluster", 0);
		return;
	}

	//Event is valid
	SetOutputState("LKrClusters", kOValid);

	thrCells = 2;
#ifdef LOOP
	tempV = LKrClusters;
	for(thrCells=0; thrCells<10; thrCells++){
		LKrClusters = tempV;
		removed = 0;
#endif

		FillHisto("totalEvents", thrCells);

		for(int i=0; i<lkr->GetHWClusterCount(); i++){
			ncells = lkr->GetHWClusterCountSCells(i);
			iHW = getClusterIndexFromHW(lkr->GetHWClusterX(i), lkr->GetHWClusterY(i), lkr->GetHWClusterE(i));

			if(iHW!=-1){

				//Check if it's a gamma cluster
				if(first) FillHisto("NHWCells", ncells);
				if(ncells<thrCells){
					//Bad NCells, remove cluster
					LKrClusters.erase(LKrClusters.begin()+iHW);
					removed++;
					i--;
				}
			}
		}

		if(removed>0){
			if(isGamma) FillHisto("removedGamma", thrCells, removed);
			if(isPip) FillHisto("removedPip", thrCells, removed);
		}

#ifdef LOOP
		first = false;
	}
#endif

	FillHisto("multiplicity", LKrClusters.size());
}

void PhotonID_HWCells::PostProcess(){

}

void PhotonID_HWCells::ExportPlot(){
	TGraphAsymmErrors *rmGamma = (TGraphAsymmErrors*)fGraph["graphRemovedGamma"];
	TGraphAsymmErrors *rmPip = (TGraphAsymmErrors*)fGraph["graphRemovedPip"];
	TGraphAsymmErrors *eff = (TGraphAsymmErrors*)fGraph["thrEfficiency"];

	rmGamma->BayesDivide(fHisto["removedGamma"], fHisto["totalEvents"]);
	rmPip->BayesDivide(fHisto["removedPip"], fHisto["totalEvents"]);

	double x, y1,y2;
	double ey1[2], ey2[2];
	for(int i=0; i<rmGamma->GetN(); i++){
		rmGamma->GetPoint(i,x,y1);
		ey1[0] = rmGamma->GetErrorYlow(i);
		ey1[1] = rmGamma->GetErrorYhigh(i);
		rmPip->GetPoint(i,x,y2);
		ey2[0] = rmPip->GetErrorYlow(i);
		ey2[1] = rmPip->GetErrorYhigh(i);

		eff->SetPoint(i, y1,y2);
		eff->SetPointError(i, ey1[0], ey1[1], ey2[0], ey2[1]);
	}
	eff->GetXaxis()->SetTitle("Gamma removed (%)");
	eff->GetYaxis()->SetTitle("Pip removed (%)");

	SaveAllPlots();
}

void PhotonID_HWCells::DrawPlot(){
}


int PhotonID_HWCells::getClusterIndexFromHW(double x, double y, double e){
	double r;
	int sel=-1;
	for(unsigned int i=0; i<LKrClusters.size(); i++){
		r = sqrt(pow(x-LKrClusters[i]->GetClusterX(), 2) + pow(y-LKrClusters[i]->GetClusterY(), 2) + pow(e-LKrClusters[i]->GetClusterEnergy(), 2));

		if(r<5) sel = i;
	}
	return sel;
}
