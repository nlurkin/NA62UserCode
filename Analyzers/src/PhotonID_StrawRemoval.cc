#define LOOPI
//Mandatory includes
#include "PhotonID_StrawRemoval.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

//Include the hh files for events kinds you will use
#include "TRecoLKrEvent.hh"
#include "TRecoSpectrometerEvent.hh"

#include <TGraphAsymmErrors.h>

PhotonID_StrawRemoval::PhotonID_StrawRemoval(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "PhotonID_StrawRemoval";

	RequestTree("Spectrometer", new TRecoSpectrometerEvent);
}


void PhotonID_StrawRemoval::InitHist(){
	BookHisto("BadVertex", new TH1D("BadVertex", "Events where Vertex analyzer doesn't returns a kOValid state", 1, 0, 1));
	BookHisto("BadLKrClusters", new TH1D("BadLKrClusters", "Events where Acceptance analyzer doesn't returns a kOValid state for LKrCluster", 1, 0, 1));

	BookHisto("multiplicity", new TH1D("multiplicity", "Number of clusters left after pi+ removing", 11, -0.5, 10.5));
	BookHisto("strawMultiplicity", new TH1D("strawMultiplicity", "Number of candidates in straw", 11, -0.5, 10.5));

	BookHisto("StrawToLKr", new TH2D("StrawToLKr", "Extrapolated position on LKr of a Straw track", 200, -2000, 2000, 200, -2000, 2000));
	BookHisto("Difference", new TH1D("Difference", "d(clus_i, trackOnLKr_j)", 200, -2000, 2000));
	BookHisto("dEndPos", new TH1D("dEndPos", "d(trackOnLKr_j, mc_endpos)", 200, -2000, 2000));
	BookHisto("dEndPosX", new TH1D("dEndPosX", "d(trackOnLKr_j, mc_endpos)_X", 200, -2000, 2000));
	BookHisto("dEndPosY", new TH1D("dEndPosY", "d(trackOnLKr_j, mc_endpos)_Y", 200, -2000, 2000));
	BookHisto("propRealReco", new TH1D("propRealReco", "d(propagate(MC), propagate(Reco))", 200, -2000, 2000));

	BookHisto("totalEvents", new TH1I("totalEvents", "Total number of events", 101, -0.5, 1000.5));
	BookHisto("removedPip", new TH1I("removedPip", "Number of events where at least 1 pi+ cluster has been removed", 101, -0.5, 1000.5));
	BookHisto("removedGamma", new TH1I("removedGamma", "Number of events where at least 1 gamma cluster has been removed", 101, -0.5, 1000.5));
	BookHisto("graphRemovedPip", new TGraphAsymmErrors());
	BookHisto("graphRemovedGamma", new TGraphAsymmErrors());
	BookHisto("thrEfficiency", new TGraphAsymmErrors);
}

void PhotonID_StrawRemoval::InitOutput(){
	RegisterOutput("LKrClusters", &LKrClusters);
}

void PhotonID_StrawRemoval::DefineMCSimple(){
	fMCSimple.AddParticle(0, 22);
	fMCSimple.AddParticle(0, 211);
}

void PhotonID_StrawRemoval::ProcessSpecialTriggers(int){
	/// \MemberDescr
	/// \param iEvent : Special event number
	///
	/// Process method for special triggers. Called on each special trigger event after each start of burst.\n
	/// \EndMemberDescr
}

void PhotonID_StrawRemoval::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	//LKr definition
	int LKrStartPos = 240413;

	//Straw
	TRecoSpectrometerEvent *straw = (TRecoSpectrometerEvent*)GetEvent("Spectrometer");
	TRecoSpectrometerCandidate *strawCand;
	vector<TVector3> extrapolatedOnLKr;

	//Clusters
	vector<double> clusterEnergy, tempVDouble;
	vector<TVector3> clusterPosition, tempVTV3;
	vector<TRecoLKrCandidate*> tempRecoLKr;

	//Variables for association
	double distanceStraw;
	int straw_sel;
	double d;
	int thresholdstraw = 100;

	//positions
	TVector3 cluster;
	TVector3 g;

	//
	bool isGamma = false;
	bool isPip = false;
	bool first = true;
	int removed = 0;
	OutputState state;

	//What kind of event do we have?
	if(fMCSimple.Size(22)>0) isGamma = true;
	else if(fMCSimple.Size(211)>0) isPip = true;

	//Get Vertex
	TVector3 vertex;
	if(isGamma) vertex = fMCSimple[22][0]->GetProdPos().Vect();
	else if(isPip) vertex = fMCSimple[211][0]->GetProdPos().Vect();
	//TVector3 vertex = *(TVector3*)GetOutput("VertexCDA.vertex", state);
	//if(state!=kOValid){
	//	FillHisto("BadVertex", 0);
	//	return;
	//}

	//Get clusters
	vector<TRecoLKrCandidate*> lkr = *(vector<TRecoLKrCandidate*>*)GetOutput("associatePhotons.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadLKrClusters", 0);
		return;
	}
	//Extract some usefull info
	for(unsigned int i=0; i<lkr.size(); i++){
		cluster.SetXYZ(lkr[i]->GetClusterX()*10, lkr[i]->GetClusterY()*10, LKrStartPos);
		clusterPosition.push_back(cluster);
		clusterEnergy.push_back(lkr[i]->GetClusterEnergy()*1000);
		LKrClusters.push_back(lkr[i]);
	}

	//Event is valid
	SetOutputState("LKrClusters", kOValid);

	//Get Straw candidates
	FillHisto("strawMultiplicity", straw->GetNCandidates());
	for(int i=0; i<straw->GetNCandidates(); i++){
		ExportEvent();
		strawCand = (TRecoSpectrometerCandidate*)straw->GetCandidate(i);

		//Extrapolate//	No cluster should be removed, else fail position on LKr
		extrapolatedOnLKr.push_back(spec_propagate(vertex, strawCand->GetMomentum().Vect(), strawCand->GetCharge(), LKrStartPos));

		//Compare propagation of candidate with propagation of MC
		if(isPip) {
			FillHisto("propRealReco", distance3D(extrapolatedOnLKr[i], spec_propagate(fMCSimple["pi+"][0]->GetProdPos().Vect(), fMCSimple["pi+"][0]->GetInitialMomentum(), 1, LKrStartPos)));
			FillHisto("dEndPos", distance2D(extrapolatedOnLKr[i], fMCSimple["pi+"][0]->GetEndPos().Vect(), "XY"));
			FillHisto("dEndPosX", extrapolatedOnLKr[i].X()-fMCSimple["pi+"][0]->GetEndPos().X());
			FillHisto("dEndPosY", extrapolatedOnLKr[i].Y()-fMCSimple["pi+"][0]->GetEndPos().Y());
		}
		FillHisto("StrawToLKr", extrapolatedOnLKr[i].X(), extrapolatedOnLKr[i].Y());
	}

	//Photon case
	if(isGamma){
		g = propagate(fMCSimple[22][0]->GetProdPos().Vect(), fMCSimple[22][0]->GetInitialMomentum(), LKrStartPos);

	}


	thresholdstraw = 200;
#ifdef LOOP
	//Initialize reset after threshold testing loop
	tempVDouble = clusterEnergy;
	tempVTV3 = clusterPosition;
	tempRecoLKr = LKrClusters;
	for(thresholdstraw=0; thresholdstraw<=1000; thresholdstraw+=10){
		clusterEnergy = tempVDouble;
		clusterPosition = tempVTV3;
		LKrClusters = tempRecoLKr;
		removed = 0;
#endif

		FillHisto("totalEvents", thresholdstraw, (int)clusterPosition.size());

		//Associate straw tracks with clusters
		for(unsigned int i=0; i<extrapolatedOnLKr.size(); i++){

			//Init selection
			distanceStraw = 10000;
			straw_sel = -1;
			//Loop to select the closest cluster
			for(unsigned int j=0; j<clusterPosition.size(); j++){
				d = distance3D(extrapolatedOnLKr[i], clusterPosition[j]);
				if(d<distanceStraw){
					straw_sel = j;
					distanceStraw = d;
				}
			}

			//If closest cluster is closer than threshold from expected position,
			//assume it's a pi+ cluster and remove it
			if(distanceStraw<thresholdstraw){
				if(first && (straw_sel>-1) ) FillHisto("Difference", distance3D(extrapolatedOnLKr[i], clusterPosition[straw_sel]));
				clusterPosition.erase(clusterPosition.begin()+straw_sel);
				LKrClusters.erase(LKrClusters.begin()+straw_sel);
				removed++;
			}
		}

		if(removed>0){
			if(isGamma) FillHisto("removedGamma", thresholdstraw, removed);
			if(isPip) FillHisto("removedPip", thresholdstraw, removed);
		}
#ifdef LOOP
		first = false;
	}
#endif

	//is the cluster still present
	bool found1 = false;
	//bool found2 = false;
	bool found2 = true;
	for(unsigned int i=0; i<LKrClusters.size(); i++){
		if(LKrClusters[i]==clusterG1) found1 = true;
		//if(LKrClusters[i]==clusterG2) found2 = true;
	}

	//if(LKrClusters.size()==2) FillHisto("ID2Clusters", 0);
	if(found1 && found2){
		//FillHisto("goodEvents", thresholdstraw);
		//if(LKrClusters.size()==2) FillHisto("IDSuccess", 0);
	}
	else{
		//ExportEvent();
	}
	first = false;
	//}
	FillHisto("multiplicity", LKrClusters.size());
}

void PhotonID_StrawRemoval::PostProcess(){
	LKrClusters.clear();
}

void PhotonID_StrawRemoval::ExportPlot(){
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

void PhotonID_StrawRemoval::DrawPlot(){
}

int PhotonID_StrawRemoval::getMostEnergetic(vector<double> e){
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
