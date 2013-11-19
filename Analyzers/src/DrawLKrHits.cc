//Mandatory includes
#include "DrawLKrHits.hh"
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
#include "TRecoLKrEvent.hh"
#include "TRecoSpectrometerEvent.hh"*/

#include "TLKrEvent.hh"

//Additional includes
/*#include <TString.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TCanvas.h>*/

DrawLKrHits::DrawLKrHits(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "DrawLKrHits";

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	AddTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event
	//	AddTree("GigaTracker", new TGigaTrackerEvent);
	RequestTree("LKr", new TLKrEvent);

	//Initialize DetectorAcceptance if needed
	//	fGeom = new DetectorAcceptance("./NA62.gdml");

	c = new TCanvas("toto");
}


void DrawLKrHits::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*)
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));

}

void DrawLKrHits::InitOutput(){
	//Call
	//RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class
}

void DrawLKrHits::DefineMCSimple(MCSimple *fMCSimple){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple->AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//	int kaonID = fMCSimple->AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple->AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	/*int kaonID = fMCSimple->AddParticle(0, 321); //ask for beam Kaon
	fMCSimple->AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	int pi0 = fMCSimple->AddParticle(kaonID, 111); //ask for positive pion from initial kaon decay
	fMCSimple->AddParticle(pi0, 22); //ask for positive pion from initial kaon decay
	fMCSimple->AddParticle(pi0, 22); //ask for positive pion from initial kaon decay*/
	fMCSimple->AddParticle(0, 22);
}

void DrawLKrHits::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	int LKrStartPos = 240413;
	//Event
	TLKrEvent *lkr = (TLKrEvent*)GetEvent("LKr");

	//Candidates
	TLKrHit *lkrHit;

	//int iList[] = {100, 101, 107, 114, 115, 118, 11, 121, 135, 137, 147, 156, 160, 171 , 172, 176, 183};
	//set<int> list;
	//list.insert(iList, iList+sizeof(iList)/sizeof(int));

	//if(list.find(iEvent)!=list.end()){
		TH2D *clus = new TH2D("LKrCluster", "Clusters on LKr", 100, -1500, 1500, 100, -1500, 1500);
		TH2D *real = new TH2D("LKrReal", "Real gamma on LKr", 100, -1500, 1500, 100, -1500, 1500);
		//TH2D *pip = new TH2D("LKrStraw", "Real pip on LKr", 100, -1500, 1500, 100, -1500, 1500);

		TVector3 gamma1 = propagate(fMCSimple["gamma"][0]->GetProdPos().Vect(), fMCSimple["gamma"][0]->GetInitialMomentum(), LKrStartPos);
		//TVector3 gamma2 = propagate(fMCSimple["gamma"][1]->GetProdPos().Vect(), fMCSimple["gamma"][1]->GetInitialMomentum(), LKrStartPos);
		//TVector3 straw = spec_propagate(fMCSimple["pi+"][0]->GetProdPos().Vect(), fMCSimple["pi+"][0]->GetInitialMomentum(), +1, LKrStartPos);

		real->Fill(gamma1.X(), gamma1.Y());
		//real->Fill(gamma2.X(), gamma2.Y());
		//pip->Fill(straw.X(), straw.Y());

		for(int i=0; i<lkr->GetNHits(); i++){
			lkrHit = (TLKrHit*)lkr->GetHits()->At(i);
			clus->Fill(lkrHit->GetPosition().X(), lkrHit->GetPosition().Y(), lkrHit->GetEnergy()*1000);
		}

		real->SetMarkerColor(kRed);
		real->SetMarkerStyle(kFullCircle);
		real->SetMarkerSize(0.5);
		/*pip->SetMarkerColor(kGreen);
		pip->SetMarkerStyle(kFullCircle);
		pip->SetMarkerSize(0.5);*/
		clus->SetMarkerColor(kBlue);
		clus->Draw("");
		real->Draw("SAME");
		//pip->Draw("SAME");


		c->Update();

		c->SaveAs(TString("Hits_")+(Long_t)iEvent+TString(".png"));
		delete real;
		delete clus;
		//delete pip;
	//}
}

void DrawLKrHits::PostProcess(){

}

void DrawLKrHits::ExportPlot(){
	//If you want to save them all, just call
	//	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void DrawLKrHits::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
