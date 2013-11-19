//Mandatory includes
#include "DrawLKr.hh"
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

DrawLKr::DrawLKr(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "DrawLKr";

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	AddTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event
	//	AddTree("GigaTracker", new TGigaTrackerEvent);
	RequestTree("LKr", new TRecoLKrEvent);

	//Initialize DetectorAcceptance if needed
	//	fGeom = new DetectorAcceptance("./NA62.gdml");

	c = new TCanvas("toto");
}


void DrawLKr::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*)
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));

}

void DrawLKr::InitOutput(){
	//Call
	//RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class
}

void DrawLKr::DefineMCSimple(MCSimple *fMCSimple){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple->AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	/*int kaonID = fMCSimple->AddParticle(0, 321); //ask for beam Kaon
	fMCSimple->AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	int pi0 = fMCSimple->AddParticle(kaonID, 111); //ask for positive pion from initial kaon decay
	fMCSimple->AddParticle(pi0, 22); //ask for positive pion from initial kaon decay
	fMCSimple->AddParticle(pi0, 22); //ask for positive pion from initial kaon decay*/
	fMCSimple->AddParticle(0, 22);

}

void DrawLKr::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	int LKrStartPos = 240413;
	//Event
	TRecoLKrEvent *lkr = (TRecoLKrEvent*)GetEvent("LKr");

	//OutputState state;
	//vector<TRecoLKrCandidate*> lkr = *(vector<TRecoLKrCandidate*>*)GetOutput("associatePhotons.BadG1" ,state);

	//Candidates
	TRecoLKrCandidate *lkrCand;

	//if(lkr->GetNCandidates()>3){
	//if(lkr.size()>2){
		TH2D *clus = new TH2D("LKrCluster", "Clusters on LKr", 100, -1500, 1500, 100, -1500, 1500);
		TH2D *realG1 = new TH2D("LKrRealG1", "Real gamma1 on LKr", 100, -1500, 1500, 100, -1500, 1500);
		//TH2D *realG2 = new TH2D("LKrRealG2", "Real gamma2 on LKr", 100, -1500, 1500, 100, -1500, 1500);
		//TH2D *pip = new TH2D("LKrStraw", "Real pip on LKr", 100, -1500, 1500, 100, -1500, 1500);
		TH2I *lkrShape = new TH2I("lkrShape", "LKr shape", 1000, -1500, 1500, 1000, -1500, 1500);

		lkrShape->SetMarkerColor(1);
        lkrShape->SetMarkerStyle(7);
        lkrShape->SetMarkerSize(1);
        draw_LKr(lkrShape);

		TVector3 gamma1 = propagate(fMCSimple["gamma"][0]->GetProdPos().Vect(), fMCSimple["gamma"][0]->GetInitialMomentum(), LKrStartPos);
		//TVector3 gamma2 = propagate(fMCSimple["gamma"][1]->GetProdPos().Vect(), fMCSimple["gamma"][1]->GetInitialMomentum(), LKrStartPos);
		//TVector3 straw = spec_propagate(fMCSimple["pi+"][0]->GetProdPos().Vect(), fMCSimple["pi+"][0]->GetInitialMomentum(), +1, LKrStartPos);

		realG1->Fill(gamma1.X(), gamma1.Y());
		//realG2->Fill(gamma2.X(), gamma2.Y());
		//pip->Fill(straw.X(), straw.Y());

		for(int i=0; i<lkr->GetNCandidates(); i++){
		//for(unsigned int i=0; i<lkr.size(); i++){
			lkrCand = (TRecoLKrCandidate*)lkr->GetCandidate(i);
			//lkrCand = lkr[i];
			clus->Fill(lkrCand->GetClusterX()*10, lkrCand->GetClusterY()*10, sqrt(pow(lkrCand->GetClusterRMSX(), 2)+pow(lkrCand->GetClusterRMSY(), 2))*lkrCand->GetClusterEnergy()*1000);
		}

		realG1->SetMarkerColor(kRed);
		realG1->SetMarkerStyle(kFullCircle);
		realG1->SetMarkerSize(0.5);
		/*realG2->SetMarkerColor(6);
		realG2->SetMarkerStyle(kFullCircle);
		realG2->SetMarkerSize(0.5);
		pip->SetMarkerColor(kGreen);
		pip->SetMarkerStyle(kFullCircle);
		pip->SetMarkerSize(0.5);*/
		clus->SetMarkerColor(kBlue);
		lkrShape->Draw("");
		clus->Draw("BOXSAME");
		realG1->Draw("SAME");
		//realG2->Draw("SAME");
		//pip->Draw("SAME");


		c->Update();

		c->SaveAs(TString("Event_")+(Long_t)iEvent+TString(".png"));
		delete realG1;
		//delete realG2;
		delete clus;
		//delete pip;

		//fMCSimple[22][0]->Dump();
		//fMCSimple[22][1]->Dump();
	//}
}

void DrawLKr::PostProcess(){

}

void DrawLKr::ExportPlot(){
	//If you want to save them all, just call
	//	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void DrawLKr::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}

void DrawLKr::draw_LKr(TH2I* hist){
        double x, y;

        //Segment haut et bas
        for(x=-513.194; x<532.93; x=x+1){
                hist->Fill(x,1253.37);
                hist->Fill(x,-1253.37);
        }

        //Segment gauche et droit
        for(y=-542.8; y<542.8; y=y+1){
                hist->Fill(-1263.24,y);
                hist->Fill(1243.5,y);
        }

        //Segments diagonaux
        for(x=-1263.24; x<-513.194; x=x+1){
                y = 1787 + x;
                hist->Fill(x,y);
                hist->Fill(-x,-y);
                hist->Fill(x,-y);
                hist->Fill(-x,y);
        }

        //Hole
        for(x=83; x>=0; x=x-1){
        	y = sqrt(83*83 - x*x);
        	hist->Fill(x,y);
        	hist->Fill(-x,y);
        	hist->Fill(x,-y);
        	hist->Fill(-x,-y);
        }
}
