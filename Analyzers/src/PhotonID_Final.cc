//TODO Try using kinematic information p(K), p(pip) connus

#include "PhotonID_Final.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"

#include <TGraphAsymmErrors.h>

PhotonID_Final::PhotonID_Final(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "PhotonID_Final";
}


void PhotonID_Final::InitHist(){
	BookHisto("BadAssociateCluster", new TH1D("BadAssociateCluster", "Bad LKrClusters from associatePhotons", 1, 0, 1));
	BookHisto("BadStrawRemovalCluster", new TH1D("BadStrawRemovalCluster", "Bad LKrClusters from StrawRemoval", 1, 0, 1));
	BookHisto("BadRMSCluster", new TH1D("BadRMSCluster", "Bad LKrClusters from RMSSelection", 1, 0, 1));
	BookHisto("BadSeedCluster", new TH1D("BadSeedCluster", "Bad LKrClusters from SeedFraction", 1, 0, 1));
	BookHisto("BadHWCellsCluster", new TH1D("BadHWCellsCluster", "Bad LKrClusters from HWCells", 1, 0, 1));
	BookHisto("BadEnergyCluster", new TH1D("BadEnergyCluster", "Bad LKrClusters from EnergyCut", 1, 0, 1));

	BookHisto("SelectionTable", new TH2D("SelectionTable", "Table of selection",0,0,0, 0,0,0));
}

void PhotonID_Final::InitOutput(){
	RegisterOutput("photonCandidates", &photonCandidates);
}

void PhotonID_Final::DefineMCSimple(){
}

void PhotonID_Final::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	OutputState state;
	//Get initial clusters
	vector<TRecoLKrCandidate*> InitClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("associatePhotons.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadAssociateCluster", 0);
		return;
	}

	//Get StrawRemoval clusters
	vector<TRecoLKrCandidate*> StrawRMClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("PhotonID_StrawRemoval.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadStrawRemovalCluster", 0);
		return;
	}

	//Get RMSSelection clusters
	vector<TRecoLKrCandidate*> RMSClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("PhotonID_RMSSelection.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadRMSCluster", 0);
		return;
	}

	//Get SeedFraction clusters
	vector<TRecoLKrCandidate*> SeedClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("PhotonID_SeedFraction.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadSeedCluster", 0);
		return;
	}

	//Get HWCells clusters
	vector<TRecoLKrCandidate*> HWCellsClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("PhotonID_HWCells.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadHWCellsCluster", 0);
		return;
	}

	//Get EnergyCut clusters
	vector<TRecoLKrCandidate*> ECutClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("PhotonID_EnergyCut.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadEnergyCluster", 0);
		return;
	}

	bool straw, rms, seed, hwcells, energy;

	for(unsigned int i=0; i<InitClusters.size(); i++){
		straw = !findSame(InitClusters[i], StrawRMClusters);
		rms = !findSame(InitClusters[i], RMSClusters);
		seed = !findSame(InitClusters[i], SeedClusters);
		hwcells = !findSame(InitClusters[i], HWCellsClusters);
		energy = !findSame(InitClusters[i], ECutClusters);

		if(straw){
			if(!rms && !seed && !hwcells && !energy) FillHisto("SelectionTable", "StrawRemoval", "None", 1);
			if(rms) FillHisto("SelectionTable", "StrawRemoval", "RMSSelection", 1);
			if(seed) FillHisto("SelectionTable", "StrawRemoval", "SeedFraction", 1);
			if(hwcells) FillHisto("SelectionTable", "StrawRemoval", "HWCells", 1);
			if(energy) FillHisto("SelectionTable", "StrawRemoval", "EnergyCut", 1);
		}
		if(rms){
			if(!straw && !seed && !hwcells && !energy) FillHisto("SelectionTable", "RMSSelection", "None", 1);
			if(straw) FillHisto("SelectionTable", "RMSSelection", "StrawRemoval", 1);
			if(seed) FillHisto("SelectionTable", "RMSSelection", "SeedFraction", 1);
			if(hwcells) FillHisto("SelectionTable", "RMSSelection", "HWCells", 1);
			if(energy) FillHisto("SelectionTable", "RMSSelection", "EnergyCut", 1);
		}
		if(seed){
			if(!rms && !straw && !hwcells && !energy) FillHisto("SelectionTable", "SeedFraction", "None", 1);
			if(rms) FillHisto("SelectionTable", "SeedFraction", "RMSSelection", 1);
			if(straw) FillHisto("SelectionTable", "SeedFraction", "StrawRemoval", 1);
			if(hwcells) FillHisto("SelectionTable", "SeedFraction", "HWCells", 1);
			if(energy) FillHisto("SelectionTable", "SeedFraction", "EnergyCut", 1);
		}
		if(hwcells){
			if(!rms && !straw && !seed && !energy) FillHisto("SelectionTable", "HWCells", "None", 1);
			if(rms) FillHisto("SelectionTable", "HWCells", "RMSSelection", 1);
			if(straw) FillHisto("SelectionTable", "HWCells", "StrawRemoval", 1);
			if(seed) FillHisto("SelectionTable", "HWCells", "SeedFraction", 1);
			if(energy) FillHisto("SelectionTable", "HWCells", "EnergyCut", 1);
		}
		if(energy){
			if(!rms && !seed && !straw && !hwcells) FillHisto("SelectionTable", "EnergyCut", "None", 1);
			if(rms) FillHisto("SelectionTable", "EnergyCut", "RMSSelection", 1);
			if(straw) FillHisto("SelectionTable", "EnergyCut", "StrawRemoval", 1);
			if(seed) FillHisto("SelectionTable", "EnergyCut", "SeedFraction", 1);
			if(hwcells) FillHisto("SelectionTable", "EnergyCut", "HWCells", 1);
		}
		if(!straw && !rms && !seed && !hwcells && !energy) FillHisto("SelectionTable", "None", "None", 1);
	}
}

void PhotonID_Final::PostProcess(){
}

void PhotonID_Final::ExportPlot(){
	SaveAllPlots();
}

void PhotonID_Final::DrawPlot(){
}

bool PhotonID_Final::findSame(TRecoLKrCandidate* cand, vector<TRecoLKrCandidate*> list){

	return true;
}
