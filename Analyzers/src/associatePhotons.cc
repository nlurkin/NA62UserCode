//Mandatory includes
#include "associatePhotons.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"
#include <TBrowser.h>

associatePhotons::associatePhotons(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "associatePhotons";

	RequestTree("LKr", new TRecoLKrEvent);
}


void associatePhotons::InitHist(){
	//BookHisto("BadMC", new TH1D("BadMC", "Events with bad MC signature", 1, 0, 1));
	BookHisto("ClusterOnLKr", new TH2D("ClusterOnLKr", "Position of cluster on LKr", 200, -2000, 2000, 200, -2000, 2000));
	BookHisto("multiplicity", new TH1D("multiplicity", "Number of clusters after acceptance selection", 11, -0.5, 10.5));

	BookHisto("cluster1Energy", new TH1D("cluster1Energy", "Energy of most energetic cluster", 200, 0, 75000));
	BookHisto("cluster2Energy", new TH1D("cluster2Energy", "Energy of second most energetic cluster", 200, 0, 75000));
	BookHisto("cluster3Energy", new TH1D("cluster3Energy", "Energy of third most energetic cluster", 200, 0, 75000));
	BookHisto("cluster4Energy", new TH1D("cluster4Energy", "Energy of fourth most energetic cluster", 200, 0, 75000));
	BookHisto("cluster5Energy", new TH1D("cluster5Energy", "Energy of fifth most energetic cluster", 200, 0, 75000));

	/*BookHisto("gAssociationReal", new TH1D("gAssociationReal", "d(clus_best, g)", 200, -50, 50));
	BookHisto("dSelectedEnergy", new TH1D("dSelectedEnergy", "Difference between selected cluster energy and real energy", 100, -10000, 10000));*/

	/*BookHisto("ProdPos", new TH1D("ProdPos", "Prod position of not associated gamma", 200, 0, 300000));
	BookHisto("EndPos", new TH1D("EndPos", "End position of not associated gamma", 200, 0, 300000));*/
}

void associatePhotons::InitOutput(){
	RegisterOutput("LKrClusters", &LKrClusters);
	RegisterOutput("clusterG", &clusterG);

	AddParam("Export", "bool", &allowExport, false);
}

void associatePhotons::DefineMCSimple(){
	/*fMCSimple.AddParticle(0, 22);
	fMCSimple.AddParticle(0, 211);*/
}

void associatePhotons::ProcessSpecialTriggers(int){
	/// \MemberDescr
	/// \param iEvent : Special event number
	///
	/// Process method for special triggers. Called on each special trigger event after each start of burst.\n
	/// \EndMemberDescr
}

void associatePhotons::Process(int iEvent){
	Event*  MCTruthEvent = NULL;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	/*if(fMCSimple.status == MCSimple::kEmpty){
		FillHisto("BadMC", 0);
		return;
	}*/

	//Event
	TRecoLKrEvent *lkr = (TRecoLKrEvent*)GetEvent("LKr");

	//Candidates
	TRecoLKrCandidate *lkrCand;

	//Positions on LKr
	vector<double> clusterEnergy, tempVDouble;
	vector<TVector3> clusterLKr, tempVTV3;
	vector<TRecoLKrCandidate*> tempRecoLKr;

	TVector3 cluster;

	//LKr definition
	int LKrStartPos = 240413;

	TVector3 gamma;
	double distance;
	int d_sel=-1;
	double d;
	int thresholdgamma = 50;

	double calibMult = 0.9744;
	double calibConst = -366.5;
	int vId;

	//Get Clusters
	for(int i=0; i<lkr->GetNCandidates(); i++){
		lkrCand = (TRecoLKrCandidate*)lkr->GetCandidate(i);
		//lkrCand->SetClusterEnergy(lkrCand->GetClusterEnergy()*calibMult + calibConst);
		cluster.SetXYZ(lkrCand->GetClusterX()*10, lkrCand->GetClusterY()*10, LKrStartPos);
		tempVTV3.push_back(cluster);
		tempRecoLKr.push_back(lkrCand);
		tempVDouble.push_back(lkrCand->GetClusterEnergy()*1000);
		FillHisto("ClusterOnLKr", cluster.X(), cluster.Y());
	}

	//Sorting cluster by energy
	while(tempVDouble.size()>0){
		vId = getMostEnergetic(tempVDouble);
		LKrClusters.push_back(tempRecoLKr[vId]);
		clusterEnergy.push_back(tempVDouble[vId]);
		clusterLKr.push_back(tempVTV3[vId]);
		tempVDouble.erase(tempVDouble.begin()+vId);
		tempRecoLKr.erase(tempRecoLKr.begin()+vId);
		tempVTV3.erase(tempVTV3.begin()+vId);
	}

	SetOutputState("LKrClusters", kOValid);
	FillHisto("multiplicity", LKrClusters.size());

	if(clusterEnergy.size()>=1) FillHisto("cluster1Energy", clusterEnergy[0]);
	if(clusterEnergy.size()>=2) FillHisto("cluster2Energy", clusterEnergy[1]);
	if(clusterEnergy.size()>=3) FillHisto("cluster3Energy", clusterEnergy[2]);
	if(clusterEnergy.size()>=4) FillHisto("cluster4Energy", clusterEnergy[3]);
	if(clusterEnergy.size()>=5) FillHisto("cluster5Energy", clusterEnergy[4]);

	/*FillHisto("ProdPos", fMCSimple[211][0]->GetProdPos().Z());
	FillHisto("EndPos", fMCSimple[211][0]->GetEndPos().Z());*/

	//if(clusterEnergy.size()>1) ExportEvent();

	/*//return;

	gamma = propagate(fMCSimple["gamma"][0]->GetProdPos().Vect(), fMCSimple["gamma"][0]->GetInitialMomentum(), LKrStartPos);

	//Associate clusters to photons
	distance = 10000;
	for(unsigned int i=0; i<clusterLKr.size(); i++){
		d = distance3D(gamma, clusterLKr[i]);
		if(d<distance){
			d_sel = i;
			distance = d;
		}
	}
	if(distance>thresholdgamma){
		//Association not found
		d_sel = -1;
		distance = 10000;
		SetOutputState("clusterG", kOInvalid);
		FillHisto("ProdPos", fMCSimple[22][0]->GetProdPos().Z());
		FillHisto("EndPos", fMCSimple[22][0]->GetEndPos().Z());
		if(allowExport) ExportEvent();
	}
	else{
		clusterG = LKrClusters[d_sel];
		SetOutputState("clusterG", kOValid);
		FillHisto("dSelectedEnergy", clusterEnergy[d_sel]-fMCSimple["gamma"][0]->GetInitialEnergy());
	}

	FillHisto("gAssociationReal", distance);*/
}

void associatePhotons::PostProcess(){
	LKrClusters.clear();
}

void associatePhotons::ExportPlot(){
	SaveAllPlots();
}

void associatePhotons::DrawPlot(){
}


int associatePhotons::getMostEnergetic(vector<double> e){
	vector<double>::iterator it;
	double max = 0;
	int i=0;
	int index=-1;
	for(it=e.begin(); it!=e.end(); it++){
		if(*it>max){
			max = *it;
			index = i;
		}
		i++;
	}
	return index;
}
