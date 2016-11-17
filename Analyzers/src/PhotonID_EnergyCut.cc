#define LOOP
//Mandatory includes
#include "PhotonID_EnergyCut.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"

#include <TGraphAsymmErrors.h>

PhotonID_EnergyCut::PhotonID_EnergyCut(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "PhotonID_EnergyCut";
}


void PhotonID_EnergyCut::InitHist(){
	BookHisto("multiplicity", new TH1D("multiplicity", "Number of clusters left after energy cut", 11, -0.5, 10.5));

	BookHisto("cluster1Energy", new TH1D("cluster1Energy", "Energy of most energetic cluster", 200, 0, 75000));
	BookHisto("cluster2Energy", new TH1D("cluster2Energy", "Energy of second most energetic cluster", 200, 0, 75000));
	BookHisto("cluster3Energy", new TH1D("cluster3Energy", "Energy of third most energetic cluster", 200, 0, 75000));
	BookHisto("cluster4Energy", new TH1D("cluster4Energy", "Energy of fourth most energetic cluster", 200, 0, 75000));
	BookHisto("cluster5Energy", new TH1D("cluster5Energy", "Energy of fifth most energetic cluster", 200, 0, 75000));

	BookHisto("totalEvents", new TH1I("totalEvents", "Total number of events", 50, 0, 5000));
	BookHisto("removedGamma", new TH1I("removedGamma", "Number of events where at least 1 gamma cluster has been removed", 50, 0, 5000));
	BookHisto("removedPip", new TH1I("removedPip", "Number of events where at least 1 pi+ cluster has been removed", 50, 0, 5000));
	BookHisto("graphRemovedGamma", new TGraphAsymmErrors());
	BookHisto("graphRemovedPip", new TGraphAsymmErrors());
	BookHisto("thrEfficiency", new TGraphAsymmErrors());

	BookHisto("BadLKrCluster", new TH1D("BadLKrCluster", "Events where StrawRemoval analyzer doesn't returns a kOValid state for SuspectCluster", 1, 0, 1));
}

void PhotonID_EnergyCut::InitOutput(){
	RegisterOutput("LKrClusters", &LKrClusters);
}

void PhotonID_EnergyCut::DefineMCSimple(){
	fMCSimple.AddParticle(0, 22);
	fMCSimple.AddParticle(0, 211);
}

void PhotonID_EnergyCut::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	//Event type
	bool isGamma = false;
	bool isPip = false;

	//Cut variables
	double thresholdEnergy;

	//LOOP variables
	vector<TRecoLKrCandidate*> temp;
	int removed = 0;
	bool first = true;

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

	if(LKrClusters.size()>=1) FillHisto("cluster1Energy", LKrClusters[0]->GetClusterEnergy()*1000);
	if(LKrClusters.size()>=2) FillHisto("cluster2Energy", LKrClusters[1]->GetClusterEnergy()*1000);
	if(LKrClusters.size()>=3) FillHisto("cluster3Energy", LKrClusters[2]->GetClusterEnergy()*1000);
	if(LKrClusters.size()>=4) FillHisto("cluster4Energy", LKrClusters[3]->GetClusterEnergy()*1000);
	if(LKrClusters.size()>=5) FillHisto("cluster5Energy", LKrClusters[4]->GetClusterEnergy()*1000);

	thresholdEnergy=2000;

#ifdef LOOP
	temp = LKrClusters;
	for(thresholdEnergy=0; thresholdEnergy<=5000; thresholdEnergy+=100){
		LKrClusters = temp;
		removed = 0;
#endif

		FillHisto("totalEvents", thresholdEnergy, (int)LKrClusters.size());

		for(unsigned int i=0; i<LKrClusters.size(); i++){
			if(LKrClusters[i]->GetClusterEnergy()*1000 < thresholdEnergy){
				LKrClusters.erase(LKrClusters.begin()+i);
				i--;
				removed++;
			}
		}

		if(removed>0){
			if(isGamma) FillHisto("removedGamma", thresholdEnergy, removed);
			if(isPip) FillHisto("removedPip", thresholdEnergy, removed);
		}

#ifdef LOOP
		first = false;
	}
#endif
	FillHisto("multiplicity", LKrClusters.size());


}

void PhotonID_EnergyCut::PostProcess(){

}

void PhotonID_EnergyCut::ExportPlot(){
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

void PhotonID_EnergyCut::DrawPlot(){
}
