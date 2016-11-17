#define LOOPI

//Mandatory includes
#include "PhotonID_RMSSelection.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"

#include <TGraphAsymmErrors.h>

PhotonID_RMSSelection::PhotonID_RMSSelection(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "PhotonID_RMSSelection";
}


void PhotonID_RMSSelection::InitHist(){
	BookHisto("BadLKrClusters", new TH1D("BadLKrClusters", "Events where Acceptance analyzer doesn't returns a kOValid state for LKrCluster", 1, 0, 1));
	BookHisto("multiplicity", new TH1D("multiplicity", "Number of clusters left after bad RMS removing", 11, -0.5, 10.5));

	//Threshold low
	BookHisto("totalLowEvents", new TH1I("totalLowEvents", "Total number of events", 61, 0.595, 1.205));
	BookHisto("removedLowPip", new TH1I("removedLowPip", "Number of events where at least 1 pi+ cluster has been removed", 61, 0.595, 1.205));
	BookHisto("removedLowGamma", new TH1I("removedLowGamma", "Number of events where at least 1 gamma cluster has been removed", 61, 0.595, 1.205));
	BookHisto("graphRemovedLowPip", new TGraphAsymmErrors());
	BookHisto("graphRemovedLowGamma", new TGraphAsymmErrors());
	BookHisto("thrLowEfficiency", new TGraphAsymmErrors());

	//Threshold high
	BookHisto("totalHighEvents", new TH1I("totalHighEvents", "Total number of events", 61, 1.395, 2.005));
	BookHisto("removedHighPip", new TH1I("removedHighPip", "Number of events where at least 1 pi+ cluster has been removed", 61, 1.395, 2.005));
	BookHisto("removedHighGamma", new TH1I("removedHighGamma", "Number of events where at least 1 gamma cluster has been removed", 61, 1.395, 2.005));
	BookHisto("graphRemovedHighPip", new TGraphAsymmErrors());
	BookHisto("graphRemovedHighGamma", new TGraphAsymmErrors());
	BookHisto("thrHighEfficiency", new TGraphAsymmErrors());

	BookHisto("clusterRMS", new TH1D("clusterRMS", "Clusters RMS", 200, 0, 2));
}

void PhotonID_RMSSelection::InitOutput(){
	RegisterOutput("LKrClusters", &LKrClusters);
}

void PhotonID_RMSSelection::DefineMCSimple(){
	fMCSimple.AddParticle(0, 22);
	fMCSimple.AddParticle(0, 211);
}

void PhotonID_RMSSelection::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	TRecoLKrCandidate *lkrCand;
	vector<TRecoLKrCandidate*> tempV;
	bool first = true;

	//Event type
	bool isGamma = false;
	bool isPip = false;

	double rms;
	double thrLow, thrHigh;
	int removedLow = 0;
	int removedHigh = 0;

	//What kind of event do we have?
	if(fMCSimple.Size(22)>0) isGamma = true;
	else if(fMCSimple.Size(211)>0) isPip = true;

	OutputState state;

	//Get clusters
	LKrClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("associatePhotons.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadLKrClusters", 0);
		return;
	}

	//Event is valid
	SetOutputState("LKrClusters", kOValid);

	thrLow = 0.92;
	thrHigh = 1.5;

#ifdef LOOP
	//Initialize reset after threshold testing loop
	tempV = LKrClusters;
	for(int i=0; i<60; i++){
		LKrClusters = tempV;
		//LKrClusters.clear();
		thrLow = 0.6 + 0.01*i;
		thrHigh = 1.4 + 0.01*i;
		removedHigh = 0;
		removedLow = 0;
#endif

		FillHisto("totalLowEvents", thrLow, (int)LKrClusters.size());
		FillHisto("totalHighEvents", thrHigh, (int)LKrClusters.size());

		for(unsigned int i=0; i<LKrClusters.size(); i++){
			lkrCand = LKrClusters[i];

			rms = sqrt(pow(lkrCand->GetClusterRMSX(), 2)+pow(lkrCand->GetClusterRMSY(), 2));
			if(first) FillHisto("clusterRMS", rms);

			if(rms < thrLow){
				LKrClusters.erase(LKrClusters.begin()+i);
				removedLow++;
				i--;
			}
			else if(rms > thrHigh){
				LKrClusters.erase(LKrClusters.begin()+i);
				removedHigh++;
				i--;
			}
		}

		if(removedLow>0){
			if(isGamma) FillHisto("removedLowGamma", thrLow, removedLow);
			if(isPip) FillHisto("removedLowPip", thrLow, removedLow);
		}
		if(removedHigh>0){
			if(isGamma) FillHisto("removedHighGamma", thrHigh, removedHigh);
			if(isPip) FillHisto("removedHighPip", thrHigh, removedHigh);
		}


		first = false;
#ifdef LOOP
	}
#endif

	FillHisto("multiplicity", LKrClusters.size());
}

void PhotonID_RMSSelection::PostProcess(){
	LKrClusters.clear();
}

void PhotonID_RMSSelection::ExportPlot(){
	TGraphAsymmErrors *rmGammaL = (TGraphAsymmErrors*)fGraph["graphRemovedLowGamma"];
	TGraphAsymmErrors *rmPipL = (TGraphAsymmErrors*)fGraph["graphRemovedLowPip"];
	TGraphAsymmErrors *effL = (TGraphAsymmErrors*)fGraph["thrLowEfficiency"];

	rmGammaL->BayesDivide(fHisto["removedLowGamma"], fHisto["totalLowEvents"]);
	rmPipL->BayesDivide(fHisto["removedLowPip"], fHisto["totalLowEvents"]);

	double x, y1,y2;
	double ey1[2], ey2[2];
	for(int i=0; i<rmGammaL->GetN(); i++){
		rmGammaL->GetPoint(i,x,y1);
		ey1[0] = rmGammaL->GetErrorYlow(i);
		ey1[1] = rmGammaL->GetErrorYhigh(i);
		rmPipL->GetPoint(i,x,y2);
		ey2[0] = rmPipL->GetErrorYlow(i);
		ey2[1] = rmPipL->GetErrorYhigh(i);

		effL->SetPoint(i, y1,y2);
		effL->SetPointError(i, ey1[0], ey1[1], ey2[0], ey2[1]);
	}
	effL->GetXaxis()->SetTitle("Gamma removed (%)");
	effL->GetYaxis()->SetTitle("Pip removed (%)");

	TGraphAsymmErrors *rmGammaH = (TGraphAsymmErrors*)fGraph["graphRemovedHighGamma"];
	TGraphAsymmErrors *rmPipH = (TGraphAsymmErrors*)fGraph["graphRemovedHighPip"];
	TGraphAsymmErrors *effH = (TGraphAsymmErrors*)fGraph["thrHighEfficiency"];

	rmGammaH->BayesDivide(fHisto["removedHighGamma"], fHisto["totalHighEvents"]);
	rmPipH->BayesDivide(fHisto["removedHighPip"], fHisto["totalHighEvents"]);

	for(int i=0; i<rmGammaH->GetN(); i++){
		rmGammaH->GetPoint(i,x,y1);
		ey1[0] = rmGammaH->GetErrorYlow(i);
		ey1[1] = rmGammaH->GetErrorYhigh(i);
		rmPipH->GetPoint(i,x,y2);
		ey2[0] = rmPipH->GetErrorYlow(i);
		ey2[1] = rmPipH->GetErrorYhigh(i);

		effH->SetPoint(i, y1,y2);
		effH->SetPointError(i, ey1[0], ey1[1], ey2[0], ey2[1]);
	}
	effH->GetXaxis()->SetTitle("Gamma removed (%)");
	effH->GetYaxis()->SetTitle("Pip removed (%)");

	SaveAllPlots();
}

void PhotonID_RMSSelection::DrawPlot(){
}
