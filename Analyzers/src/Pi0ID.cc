//Mandatory includes
#include "Pi0ID.hh"
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
#include <TGraphAsymmErrors.h>

Pi0ID::Pi0ID(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "Pi0ID";

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


void Pi0ID::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*)
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));

	BookHisto("multiplicity", new TH1D("multiplicity", "Number of clusters left after frac removing", 11, -0.5, 10.5));
	BookHisto("totalEvents", new TH1I("totalEvents", "Total number of events", 61, -0.005, 0.605));
	BookHisto("goodEvents", new TH1I("goodEvents", "Number of good events", 61, -0.005, 0.605));
	BookHisto("removedEvents", new TH1I("removedEvents", "Number of events with removed candidates", 61, -0.005, 0.605));
	BookHisto("graphGoodEvents", new TGraphAsymmErrors());
	BookHisto("graphRemovedEvents", new TGraphAsymmErrors());


	BookHisto("RecoZTrue", new TH1D("RecoZTrue", "Reconstructed vertex Z assuming 2 photons coming from pi0 (good clusters)", 200, 0, 300000));
	BookHisto("RecoZpip", new TH1D("RecoZpip", "Reconstructed vertex Z assuming 2 photons coming from pi0 (bad clusters)", 200, 0, 300000));
	BookHisto("Z", new TH1D("Z", "MC vertex Z", 200, 0, 300000));
	BookHisto("ZDiff", new TH1D("ZDiff", "vertex Z (MC-Reco)", 200, -1000, 1000));

	BookHisto("BadLKrCluster", new TH1D("BadLKrCluster", "Events where StrawRemoval analyzer doesn't returns a kOValid state for SuspectCluster", 1, 0, 1));
	BookHisto("BadclusterG1", new TH1D("BadclusterG1", "Events where Acceptance analyzer doesn't returns a kOValid state for clusterG1", 1, 0, 1));
	BookHisto("BadclusterG2", new TH1D("BadclusterG2", "Events where Acceptance analyzer doesn't returns a kOValid state for clusterG2", 1, 0, 1));

	BookHisto("IDTotal", new TH1D("IDTotal", "Total number of events", 1, 0, 1));
	BookHisto("ID2Clusters", new TH1D("ID2Clusters", "Number of events with 2 clusters", 1, 0, 1));
	BookHisto("IDSuccess", new TH1D("IDSuccess", "Number of events with 2 clusters and no good cluster removed", 1, 0, 1));
	BookHisto("PositiveID", new TGraphAsymmErrors);
	BookHisto("Positive2ClustersID", new TGraphAsymmErrors);

	BookHisto("thrEfficiency", new TGraphAsymmErrors());
}

void Pi0ID::InitOutput(){
	//Call
	//	RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class
	RegisterOutput("LKrClusters", &LKrClusters);

	//Call
	//	AddParam("paramName", "paramType", &variableName, defaultValue);
	//for each parameter of the analyzer. These parameters can be set when starting the FW from the command line with the -p option.
	//paramName is the name of the parameter in the command line
	//paramType is the type of the parameter (allowed : bool, int, long, float, double, char, string, TString)
	//variableName is the name of the variable that should be declared in the definition of the class<br>
	//defaultValue is the default value if not specified in the command line
}

void Pi0ID::DefineMCSimple(){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple.AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	int kaonID = fMCSimple.AddParticle(0, 321); //ask for beam Kaon
	fMCSimple.AddParticle(kaonID, 111); //ask for positive pion from initial kaon decay

}

void Pi0ID::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	bool withMC = false;
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	if(fMCSimple.fStatus == MCSimple::kComplete) withMC = true;
	//If the analyzer can run without MC data, comment the line
	//if(fMCSimple.status == MCSimple::kEmpty) return;

	vector<TRecoLKrCandidate*> tempV;

	bool g1Exists, g2Exists;

	OutputState state;
	LKrClusters = *(vector<TRecoLKrCandidate*>*)GetOutput("PhotonID_cells.LKrClusters", state);
	if(state!=kOValid){
		FillHisto("BadLKrCluster", 0);
		return;
	}
	TRecoLKrCandidate *clusterG1 = *(TRecoLKrCandidate**)GetOutput("associatePhotons.clusterG1", state);
	if(state!=kOValid){
		FillHisto("BadclusterG1", 0);
		return;
	}
	TRecoLKrCandidate *clusterG2 = *(TRecoLKrCandidate**)GetOutput("associatePhotons.clusterG2", state);
	if(state!=kOValid){
		FillHisto("BadclusterG2", 0);
		return;
	}

	SetOutputState("LKrClusters", kOValid);

	bool removed = false;
	int LKrStartPos = 240413;

	TVector3 pos1, pos2;
	double E1, E2;
	double mpi0sq = 134.9766*134.9766;

	/*double x, y;
	double a;*/

	double zsq, z;


	double r1, r2;
	double cp;
	double a;

	double A, B, C, Delta;


	int max = LKrClusters.size();

	if(withMC) FillHisto("Z", fMCSimple["pi0"][0]->GetProdPos().Z());

	double thrLow;

	//tempV = LKrClusters;

	/*bool first = true;
	for(int i=0; i<60; i++){
		LKrClusters = tempV;
		thrLow = 0.0 + 0.01*i;
		FillHisto("totalEvents", thrLow);
		FillHisto("IDTotal", 0);
		g1Exists = false;
		g2Exists = false;*/

	for(int i=0; i<max-1; i++){
		for(int j=i+1; j<max; j++){
			pos1.SetXYZ(LKrClusters[i]->GetClusterX()*10, LKrClusters[i]->GetClusterY()*10, LKrStartPos);
			pos2.SetXYZ(LKrClusters[j]->GetClusterX()*10, LKrClusters[j]->GetClusterY()*10, LKrStartPos);
			E1 = LKrClusters[i]->GetClusterEnergy()*1000;
			E2 = LKrClusters[j]->GetClusterEnergy()*1000;

			r1 = pow(pos1.X(), 2.0)+pow(pos1.Y(), 2.0);
			r2 = pow(pos2.X(), 2.0)+pow(pos2.Y(), 2.0);
			cp = pos1.X()*pos2.X() + pos1.Y()*pos2.Y();
			a = 1. - mpi0sq/(2.*E1*E2);

			A = 1. - a*a;
			B = 2.*cp - (r1 + r2)*a*a;
			C = cp*cp - r1*r2*a*a;

			Delta = B*B - 4.*A*C;

			zsq = (-B + sqrt(Delta))/(2.*A);
			z = 241080 - sqrt(zsq) + 500;

			//if(first){
			//if( (LKrClusters[i]==clusterG1 && LKrClusters[j]==clusterG2) || (LKrClusters[j]==clusterG1 && LKrClusters[i]==clusterG2)){
			if( (LKrClusters[i]==clusterG1 || LKrClusters[j]==clusterG2) || (LKrClusters[j]==clusterG1 || LKrClusters[i]==clusterG2)){
				FillHisto("RecoZTrue", z);
				if(withMC) FillHisto("ZDiff", fMCSimple["pi0"][0]->GetProdPos().Z()-z);
			}
			else{
				FillHisto("RecoZpip", z);
			}
			//}

			//if(z<thrLow) LKrClusters.erase
		}
	}

	//if(removed) FillHisto("removedEvents", thrLow);

	/*//is the cluster still present
		bool found1 = false;
		bool found2 = false;
		for(unsigned int i=0; i<LKrClusters.size(); i++){
			if(LKrClusters[i]==clusterG1) found1 = true;
			if(LKrClusters[i]==clusterG2) found2 = true;
		}

		if(g1Exists==false) found1=true;
		if(g2Exists==false) found2=true;

		if(LKrClusters.size()==2) FillHisto("ID2Clusters", 0);
		if(found1 && found2){
			FillHisto("goodEvents", thrLow);
			if(LKrClusters.size()==2) FillHisto("IDSuccess", 0);
		}
		first = false;
	}
	FillHisto("multiplicity", LKrClusters.size());*/
}

void Pi0ID::PostProcess(){

}

void Pi0ID::ExportPlot(){
	//If you want to save them all, just call
	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void Pi0ID::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
