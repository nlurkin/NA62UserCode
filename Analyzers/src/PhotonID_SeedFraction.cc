#define LOOPI

//Mandatory includes
#include "PhotonID_SeedFraction.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"

#include <TGraphAsymmErrors.h>

PhotonID_SeedFraction::PhotonID_SeedFraction(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "PhotonID_SeedFraction";
}


void PhotonID_SeedFraction::InitHist(){
	BookHisto("multiplicity", new TH1D("multiplicity", "Number of clusters left after bad RMS removing", 11, -0.5, 10.5));
	BookHisto("totalEvents", new TH1I("totalEvents", "Total number of events", 61, -0.005, 0.605));
	BookHisto("removedGamma", new TH1I("removedGamma", "Number of events where at least 1 gamma cluster has been removed", 61, -0.005, 0.605));
	BookHisto("removedPip", new TH1I("removedPip", "Number of events where at least 1 pi+ cluster has been removed", 61, -0.005, 0.605));
	BookHisto("graphRemovedGamma", new TGraphAsymmErrors());
	BookHisto("graphRemovedPip", new TGraphAsymmErrors());
	BookHisto("thrEfficiency", new TGraphAsymmErrors());

	BookHisto("SeedEnergyFrac", new TH1D("SeedEnergyFrac", "Fraction of energy in the seed for all clusters", 100, 0, 1.5));

	BookHisto("BadLKrCluster", new TH1D("BadLKrCluster", "Events where StrawRemoval analyzer doesn't returns a kOValid state for SuspectCluster", 1, 0, 1));

}

void PhotonID_SeedFraction::InitOutput(){
	RegisterOutput("LKrClusters", &LKrClusters);
}

void PhotonID_SeedFraction::DefineMCSimple(){
	fMCSimple.AddParticle(0,22);
	fMCSimple.AddParticle(0,211);
}

void PhotonID_SeedFraction::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	TRecoLKrCandidate *lkrCand;

	//LOOP variables
	vector<TRecoLKrCandidate*> tempV;
	bool first = true;

	//Event type
	bool isGamma = false;
	bool isPip = false;

	//Cut variables
	double frac;
	double thrFrac;
	int removed = 0;

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

	thrFrac=0.24;
#ifdef LOOP
	//Initialize reset after threshold testing loop
	tempV = LKrClusters;
	first = true;
	for(int i=0; i<60; i++){
		LKrClusters = tempV;
		thrFrac = 0.0 + 0.01*i;
		removed = 0;
#endif
		FillHisto("totalEvents", thrFrac);

		for(unsigned int i=0; i<LKrClusters.size(); i++){
			lkrCand = LKrClusters[i];

			frac = lkrCand->GetClusterSeedEnergy()/lkrCand->GetClusterEnergy();
			if(first) FillHisto("SeedEnergyFrac", frac);

			if(frac<thrFrac){
				//Bad frac, remove cluster
				LKrClusters.erase(LKrClusters.begin()+i);
				removed++;
				i--;
			}
		}

		if(removed>0){
			if(isGamma) FillHisto("removedGamma", thrFrac, removed);
			if(isPip) FillHisto("removedPip", thrFrac, removed);
		}

#ifdef LOOP
		first = false;
	}
#endif

	FillHisto("multiplicity", LKrClusters.size());
}

void PhotonID_SeedFraction::PostProcess(){

}

void PhotonID_SeedFraction::ExportPlot(){
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

void PhotonID_SeedFraction::DrawPlot(){
}
