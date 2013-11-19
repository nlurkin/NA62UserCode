//Mandatory includes
#include "ShowerLKr.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"

ShowerLKr::ShowerLKr(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "ShowerLKr";

	RequestTree("LKr", new TRecoLKrEvent);
}


void ShowerLKr::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*)
	//Example
	//	BookHisto("PartEnergy", new TH2D("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));
	BookHisto("multiplicity", new TH1D("multiplicity", "Photons number/event", 10, 0, 10));
	BookHisto("startPos", new TH1D("startPos", "Photons prod pos", 500, 0, 300000));
	BookHisto("LKrPosition", new TH2D("LKrPosition", "Position of photon on LKr (MC)", 1000, -10000, 10000, 1000, -10000, 10000));


	/*BookHisto("ClusterEnergySingle", new TH1D("ClusterEnergySingle", "Energy of g1", 100, 0, 75000));
	BookHisto("clusterRecoSingle", new TH2D("clusterRecoSingle", "g1 Reco vs. Real", 100, 0, 75000, 100, 0, 75000));
	BookHisto("ClusterPositionSingle", new TH2D("ClusterPositionSingle", "Position of photon on LKr (Reco)", 1000, -1000, 1000, 1000, -1000, 1000));

	BookHisto("GetClusterChi2RMSSingle", new TH1D("GetClusterChi2RMSSingle", "GetClusterChi2RMSSingle", 200, 0, 700));
	BookHisto("GetCluster77EnergySingle", new TH1D("GetCluster77EnergySingle", "GetCluster77EnergySingle", 150, 0, 75));
	BookHisto("GetClusterKe3EnergySingle", new TH1D("GetClusterKe3EnergySingle", "GetClusterKe3EnergySingle", 150, 0, 75));
	BookHisto("GetClusterRMSXSingle", new TH1D("GetClusterRMSXSingle", "GetClusterRMSXSingle", 150, 0, 1.5));
	BookHisto("GetClusterRMSYSingle", new TH1D("GetClusterRMSYSingle", "GetClusterRMSYSingle", 150, 0, 1.5));
	BookHisto("GetClusterRMSSingle", new TH1D("GetClusterRMSSingle", "GetClusterRMSSingle", 150, 0, 1.5));
	BookHisto("GetClusterStatusSingle", new TH1D("GetClusterStatusSingle", "GetClusterStatusSingle", 6, 0, 6));
	BookHisto("GetClusterTimeSingle", new TH1D("GetClusterTimeSingle", "GetClusterTimeSingle", 200, 0, 200));
	BookHisto("GetClusterTimeLateralCellSingle", new TH1D("GetClusterTimeLateralCellSingle", "GetClusterTimeLateralCellSingle", 100, -400, 700));
	BookHisto("GetClusterUEnergySingle", new TH1D("GetClusterUEnergySingle", "GetClusterUEnergySingle", 150, 0, 75));
	BookHisto("GetClusterUTimeSingle", new TH1D("GetClusterUTimeSingle", "GetClusterUTimeSingle", 120, 80, 200));*/

	BookHisto("GetClusterEnergy", new TH1D("GetClusterEnergy", "Energy of g1", 100, 0, 75000));
	BookHisto("clusterReco", new TH2D("clusterReco", "g1 Reco vs. Real", 100, 0, 75000, 100, 0, 75000));
	BookHisto("ClusterPosition", new TH2D("ClusterPosition", "Position of photon on LKr (Reco)", 1000, -1000, 1000, 1000, -1000, 1000));

	BookHisto("GetClusterChi2RMS", new TH1D("GetClusterChi2RMS", "GetClusterChi2RMS", 200, 0, 700));
	BookHisto("GetCluster77Energy", new TH1D("GetCluster77Energy", "GetCluster77Energy", 150, 0, 75));
	BookHisto("GetClusterKe3Energy", new TH1D("GetClusterKe3Energy", "GetClusterKe3Energy", 150, 0, 75));
	BookHisto("GetClusterRMSX", new TH1D("GetClusterRMSX", "GetClusterRMSX", 150, 0, 1.5));
	BookHisto("GetClusterRMSY", new TH1D("GetClusterRMSY", "GetClusterRMSY", 150, 0, 1.5));
	BookHisto("GetClusterRMS", new TH1D("GetClusterRMS", "GetClusterRMS", 300, 0, 3));
	BookHisto("GetClusterStatus", new TH1D("GetClusterStatus", "GetClusterStatus", 6, 0, 6));
	BookHisto("GetClusterTime", new TH1D("GetClusterTime", "GetClusterTime", 200, 0, 200));
	BookHisto("GetClusterTimeLateralCell", new TH1D("GetClusterTimeLateralCell", "GetClusterTimeLateralCell", 100, -400, 700));
	BookHisto("GetClusterUEnergy", new TH1D("GetClusterUEnergy", "GetClusterUEnergy", 150, 0, 75));
	BookHisto("GetClusterUTime", new TH1D("GetClusterUTime", "GetClusterUTime", 120, 80, 200));

	BookHisto("GetClusterEnergyError", new TH1D("GetClusterEnergyError", "GetClusterEnergyError", 200, 0, 2));
	BookHisto("GetClusterEnoise", new TH1D("GetClusterEnoise", "GetClusterEnoise", 200, -1, 1));
	BookHisto("GetClusterSeedEnergy", new TH1D("GetClusterSeedEnergy", "GetClusterSeedEnergy", 300, 0, 30));

	BookHisto("angleZ", new TH1D("angleZ", "angleZ", 200, 0, 10));

}

void ShowerLKr::InitOutput(){
	//Call
	//RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class

	AddParam("ParticleType", "int", &particleType, 22);
}

void ShowerLKr::DefineMCSimple(MCSimple *fMCSimple){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple->AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//	int kaonID = fMCSimple->AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple->AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	fMCSimple->AddParticle(0, particleType);

}

void ShowerLKr::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	//if(fMCSimple.status == MCSimple::kEmpty) return;

	int initParticle = particleType;

	TVector3 lkrPos;
	TVector3 z(0, 0, 1);
	TRecoLKrEvent *lkr = (TRecoLKrEvent*)GetEvent("LKr");
	TRecoLKrCandidate *cand;
	lkrPos = propagate(fMCSimple[initParticle][0]->GetProdPos().Vect(), fMCSimple[initParticle][0]->GetInitialMomentum(), 240413);

	FillHisto("LKrPosition", lkrPos.x(), lkrPos.y());
	FillHisto("startPos", fMCSimple[initParticle][0]->GetProdPos().Z());
	FillHisto("multiplicity", lkr->GetNCandidates());
	FillHisto("angleZ", z.Angle(fMCSimple[initParticle][0]->GetInitialMomentum()));

	/*if(lkr->GetNCandidates()==1){
			cand = (TRecoLKrCandidate*)lkr->GetCandidate(0);
			FillHisto("ClusterPositionSingle", cand->GetClusterX(), cand->GetClusterY());
			FillHisto("ClusterEnergySingle", cand->GetClusterEnergy());
			FillHisto("GetClusterChi2RMSSingle", cand->GetClusterChi2RMS());
			FillHisto("GetCluster77EnergySingle", cand->GetCluster77Energy());
			FillHisto("GetClusterKe3EnergySingle", cand->GetClusterKe3Energy());
			FillHisto("GetClusterRMSXSingle", cand->GetClusterRMSX());
			FillHisto("GetClusterRMSYSingle", cand->GetClusterRMSY());
			FillHisto("GetClusterRMSSingle", sqrt(pow(cand->GetClusterRMSX(), 2) + pow(cand->GetClusterRMSY(), 2)));
			FillHisto("GetClusterStatusSingle", cand->GetClusterStatus());
			FillHisto("GetClusterTimeSingle", cand->GetClusterTime());
			FillHisto("GetClusterTimeLateralCellSingle", cand->GetClusterTimeLateralCell());
			FillHisto("GetClusterUEnergySingle", cand->GetClusterUEnergy());
			FillHisto("GetClusterUTimeSingle", cand->GetClusterUTime());
			FillHisto("clusterRecoSingle", fMCSimple[initParticle][0]->GetInitialEnergy(), cand->GetClusterEnergy());
		}*/
	for(int i=0; i<lkr->GetNCandidates(); i++){
		cand = (TRecoLKrCandidate*)lkr->GetCandidate(i);

		FillHisto("ClusterPosition", cand->GetClusterX(), cand->GetClusterY());
		FillHisto("GetClusterEnergy", cand->GetClusterEnergy());
		FillHisto("GetClusterChi2RMS", cand->GetClusterChi2RMS());
		FillHisto("GetCluster77Energy", cand->GetCluster77Energy());
		FillHisto("GetClusterKe3Energy", cand->GetClusterKe3Energy());
		FillHisto("GetClusterRMSX", cand->GetClusterRMSX());
		FillHisto("GetClusterRMSY", cand->GetClusterRMSY());
		FillHisto("GetClusterRMS", sqrt(pow(cand->GetClusterRMSX(), 2) + pow(cand->GetClusterRMSY(), 2)));
		FillHisto("GetClusterStatus", cand->GetClusterStatus());
		FillHisto("GetClusterTime", cand->GetClusterTime());
		FillHisto("GetClusterTimeLateralCell", cand->GetClusterTimeLateralCell());
		FillHisto("GetClusterUEnergy", cand->GetClusterUEnergy());
		FillHisto("GetClusterUTime", cand->GetClusterUTime());
		FillHisto("clusterReco", fMCSimple[initParticle][0]->GetInitialEnergy(), cand->GetClusterEnergy());

		FillHisto("GetClusterEnergyError", cand->GetClusterEnergyError());
		FillHisto("GetClusterEnoise", cand->GetClusterEnoise());
		FillHisto("GetClusterSeedEnergy", cand->GetClusterSeedEnergy());
	}

	//You can retrieve MC particles from the fMCSimple Set with (return a vector<KinePart*>
	//	fMCSimple["particleName"]
	//	fMCSimple[pdgID]
	//Exemple
	//	fMCSimple["k+"][index] for the kaon
	//	fMCSimple["pi+"][index] for the positive pion
	//	fMCSimple["gamma"][index] for the photon
	//The number in the brackets is the index of the particle (if you asked for two photons in the set, you can ask fMCSimple["gamma"][0] for the first one and fMCSimple["gamma"][1] for the second)

	//You can retrieve the events from the trees with
	//	(eventClass*)fEvent["treeName"];

	//You can retrieve the histograms you booked (for drawing, changing, filling, ...) with
	//	fHisto["histoName"] for TH1
	//	fHisto2["histoName"] for TH2
	//	fGraph["graphName"] for TGraph and TGraphAsymmErrors
	//Be carefull !! If the histogram you ask for doesn't exist or you ask for an existing histogram
	//in the wrong recipient (e.g. th2 in fHisto), program will segfault.
	//To fill the histograms without risk of segfault, you can use
	//	FillHisto("histoName", values)
	//where values are the same parameters as if you call histogram->Fill(values) (x,y,weight,...)
	//Instead of segfault, this function print an error message

	//For use of fGeom, read DetectorAcceptance class

	//To use the output of a different analyzer, use
	//outputType var = *(outputType*)GetOutput("analyzerName.outputName", state);
	//Where outputType is the variable type and state is of type outputState
	//This function returns a void* and you have to explicitly cast it to the real object type
	//State is set with the state of the variable (kOUninit, kOInvalid ,kOValid). The value of the output should only be trusted if state == kOValid
	//example : TLorentzVector vertex = *(TLorentzVector*)GetOutput("simpleVertexAnalyzer.vertex", state);

	//Before starting the processing of an event, the state flag of each output variable is reset to kOUninit
	//When setting the value of an output variable, don't forget to set appropriately the state flag to either kOValid or kOInvalid
	//to indicate if the value can/can't be used in other analyzer
	//SetOutputState("outputName", kOValid);
}

void ShowerLKr::PostProcess(){

}

void ShowerLKr::ExportPlot(){
	//If you want to save them all, just call
		SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void ShowerLKr::DrawPlot(){
	//If you want to draw all the plots, just call
		DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
