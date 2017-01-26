//Mandatory includes
#include "SinglePhoton.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"

SinglePhoton::SinglePhoton(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "SinglePhoton";

	RequestTree("LKr", new TRecoLKrEvent);
	//fGeom = GetDetectorAcceptanceInstance();
}


void SinglePhoton::InitHist(){
	BookHisto("NCandidates", new TH1D("NCandidates", "", 20, 0, 20));

	BookHisto("Single_EndProc", new TH1D("Single_EndProc", "", 0, 0, 0));
	BookHisto("Single_EndPos", new TH1D("Single_EndPos", "", 1000, 150000, 260000));
	BookHisto("Single_RadioXZ", new TH2D("Single_RadioXZ", "", 1000, 150000, 260000, 1000, -4000, 4000));
	BookHisto("Single_RadioYZ", new TH2D("Single_RadioYZ", "", 1000, 150000, 260000, 1000, -4000, 4000));
	BookHisto("Single_LKrEnergy", new TH1D("Single_LKrEnergy", "", 10000, 0, 75));
	BookHisto("Single_Calib", new TH2D("Single_Calib", "", 200, 0, 75, 200, 0, 75));
	BookHisto("Single_CalibLate", new TH2D("Single_CalibLate", "", 200, 0, 75, 200, 0, 75));
	BookHisto("Single_Energy", new TH1D("Single_Energy", "", 10000, 0, 75));
	BookHisto("Single_LKrClusterPos", new TH2D("Single_LKrClusterPos", "", 500, -1000, 1000, 500, -1000, 1000));
	BookHisto("Single_PosDiff", new TH1D("Single_PosDiff", "", 100, 0, 1000));

	BookHisto("Multi_EndProc", new TH1D("Multi_EndProc", "", 0, 0, 0));
	BookHisto("Multi_EndPos", new TH1D("Multi_EndPos", "", 1000, 150000, 260000));
	BookHisto("Multi_RadioXZ", new TH2D("Multi_RadioXZ", "", 1000, 150000, 260000, 1000, -4000, 4000));
	BookHisto("Multi_RadioYZ", new TH2D("Multi_RadioYZ", "", 1000, 150000, 260000, 1000, -4000, 4000));
	BookHisto("Multi_LKrEnergy", new TH1D("Multi_LKrEnergy", "", 10000, 0, 75));
	BookHisto("Multi_Calib", new TH2D("Multi_Calib", "", 200, 0, 75, 200, 0, 75));
	BookHisto("Multi_Energy", new TH1D("Multi_Energy", "", 10000, 0, 75));
	BookHisto("Multi_ClusterSumEnergy", new TH1D("Multi_ClusterSumEnergy", "", 10000, 0, 75));
	BookHisto("Multi_LKrClusterPos", new TH2D("Multi_LKrClusterPos", "", 500, -1000, 1000, 500, -1000, 1000));
	BookHisto("Multi_PosDiff", new TH1D("Multi_PosDiff", "", 100, 0, 1000));

	BookHisto("No_EndProc", new TH1D("No_EndProc", "", 0, 0, 0));
	BookHisto("No_EndPos", new TH1D("No_EndPos", "", 1000, 150000, 260000));
	BookHisto("No_RadioXZ", new TH2D("No_RadioXZ", "", 1000, 150000, 260000, 1000, -4000, 4000));
	BookHisto("No_RadioYZ", new TH2D("No_RadioYZ", "", 1000, 150000, 260000, 1000, -4000, 4000));
	BookHisto("No_LKrEnergy", new TH1D("No_LKrEnergy", "", 10000, 0, 75));
	BookHisto("No_HWCluster", new TH2D("No_HWCluster", "", 100, 0, 75, 10, 0, 10));
	BookHisto("No_HWClusterEnergy", new TH2D("No_HWClusterEnergy", "", 100, 0, 75, 100, 0, 75));

	BookHisto("No_StopIn_Straw1", new TH1D("No_StopIn_Straw1", "", 100, 183000, 185000));
	BookHisto("No_StopIn_Straw2", new TH1D("No_StopIn_Straw2", "", 100, 193000, 195000));
	BookHisto("No_StopIn_Straw3", new TH1D("No_StopIn_Straw3", "", 100, 204000, 206000));
	BookHisto("No_StopIn_Straw4", new TH1D("No_StopIn_Straw4", "", 100, 218000, 219606));
	BookHisto("No_StopIn_RICH", new TH1D("No_StopIn_RICH", "", 100, 219606, 237670));
	BookHisto("No_StopIn_CHOD", new TH1D("No_StopIn_CHOD", "", 100, 238800, 239540));
	BookHisto("No_StopIn_IRC", new TH1D("No_StopIn_IRC", "", 100, 239540, 240388));
	BookHisto("No_StopIn_LKr", new TH1D("No_StopIn_LKr", "", 100, 240390, 243224));
	BookHisto("No_StopIn_Else", new TH1D("No_StopIn_Else", "", 100, 180000, 250000));

	BookHisto("Multi_StopIn_Straw1", new TH1D("Multi_StopIn_Straw1", "", 100, 183000, 185000));
	BookHisto("Multi_StopIn_Straw2", new TH1D("Multi_StopIn_Straw2", "", 100, 193000, 195000));
	BookHisto("Multi_StopIn_Straw3", new TH1D("Multi_StopIn_Straw3", "", 100, 204000, 206000));
	BookHisto("Multi_StopIn_Straw4", new TH1D("Multi_StopIn_Straw4", "", 100, 218000, 219606));
	BookHisto("Multi_StopIn_RICH", new TH1D("Multi_StopIn_RICH", "", 100, 219606, 237670));
	BookHisto("Multi_StopIn_CHOD", new TH1D("Multi_StopIn_CHOD", "", 100, 238800, 239540));
	BookHisto("Multi_StopIn_IRC", new TH1D("Multi_StopIn_IRC", "", 100, 239540, 240388));
	BookHisto("Multi_StopIn_LKr", new TH1D("Multi_StopIn_LKr", "", 100, 240390, 243224));
	BookHisto("Multi_StopIn_Else", new TH1D("Multi_StopIn_Else", "", 100, 180000, 250000));

	BookHisto("Single_StopIn_Straw1", new TH1D("Single_StopIn_Straw1", "", 100, 183000, 185000));
	BookHisto("Single_StopIn_Straw2", new TH1D("Single_StopIn_Straw2", "", 100, 193000, 195000));
	BookHisto("Single_StopIn_Straw3", new TH1D("Single_StopIn_Straw3", "", 100, 204000, 206000));
	BookHisto("Single_StopIn_Straw4", new TH1D("Single_StopIn_Straw4", "", 100, 218000, 219606));
	BookHisto("Single_StopIn_RICH", new TH1D("Single_StopIn_RICH", "", 100, 219606, 237670));
	BookHisto("Single_StopIn_CHOD", new TH1D("Single_StopIn_CHOD", "", 100, 238800, 239540));
	BookHisto("Single_StopIn_IRC", new TH1D("Single_StopIn_IRC", "", 100, 239540, 240388));
	BookHisto("Single_StopIn_LKr", new TH1D("Single_StopIn_LKr", "", 100, 240390, 243224));
	BookHisto("Single_StopIn_Else", new TH1D("Single_StopIn_Else", "", 100, 180000, 250000));

	BookCounter("Total");
	BookCounter("InAcceptance");
	BookCounter("Outside");
	BookCounter("Inside");
	BookCounter("SingleCluster");
	BookCounter("MultiCluster");
	BookCounter("0Cluster");
	BookCounter("No_EnergyLeftInLKr");
	BookCounter("No_EnergyLeftInLKrWithHWCluster");

	NewEventFraction("distrib");
	SetSignificantDigits("distrib", 2);
	//AddCounterToEventFraction("distrib", "Total");
	AddCounterToEventFraction("distrib", "InAcceptance");
	DefineSampleSizeCounter("distrib", "InAcceptance");
	AddCounterToEventFraction("distrib", "Outside");
	AddCounterToEventFraction("distrib", "Inside");
	AddCounterToEventFraction("distrib", "SingleCluster");
	AddCounterToEventFraction("distrib", "MultiCluster");
	AddCounterToEventFraction("distrib", "0Cluster");
	AddCounterToEventFraction("distrib", "No_EnergyLeftInLKr");
	AddCounterToEventFraction("distrib", "No_EnergyLeftInLKrWithHWCluster");



	BookCounter("Single_StopIn_Straw1");
	BookCounter("Single_StopIn_Straw2");
	BookCounter("Single_StopIn_Straw3");
	BookCounter("Single_StopIn_Straw4");
	BookCounter("Single_StopIn_RICH");
	BookCounter("Single_StopIn_CHOD");
	BookCounter("Single_StopIn_IRC");
	BookCounter("Single_StopIn_Else");
	BookCounter("Single_StopIn_LKr");
	NewEventFraction("Single_Detector");
	SetSignificantDigits("Single_Detector", 2);
	AddCounterToEventFraction("Single_Detector", "Inside");
	AddCounterToEventFraction("Single_Detector", "Single_StopIn_Straw1");
	AddCounterToEventFraction("Single_Detector", "Single_StopIn_Straw2");
	AddCounterToEventFraction("Single_Detector", "Single_StopIn_Straw3");
	AddCounterToEventFraction("Single_Detector", "Single_StopIn_Straw4");
	AddCounterToEventFraction("Single_Detector", "Single_StopIn_RICH");
	AddCounterToEventFraction("Single_Detector", "Single_StopIn_CHOD");
	AddCounterToEventFraction("Single_Detector", "Single_StopIn_IRC");
	AddCounterToEventFraction("Single_Detector", "Single_StopIn_Else");
	DefineSampleSizeCounter("Single_Detector", "Inside");

	BookCounter("No_StopIn_Straw1");
	BookCounter("No_StopIn_Straw2");
	BookCounter("No_StopIn_Straw3");
	BookCounter("No_StopIn_Straw4");
	BookCounter("No_StopIn_RICH");
	BookCounter("No_StopIn_CHOD");
	BookCounter("No_StopIn_IRC");
	BookCounter("No_StopIn_Else");
	BookCounter("No_StopIn_LKr");
	NewEventFraction("No_Detector");
	SetSignificantDigits("No_Detector", 2);
	AddCounterToEventFraction("No_Detector", "Inside");
	AddCounterToEventFraction("No_Detector", "No_StopIn_Straw1");
	AddCounterToEventFraction("No_Detector", "No_StopIn_Straw2");
	AddCounterToEventFraction("No_Detector", "No_StopIn_Straw3");
	AddCounterToEventFraction("No_Detector", "No_StopIn_Straw4");
	AddCounterToEventFraction("No_Detector", "No_StopIn_RICH");
	AddCounterToEventFraction("No_Detector", "No_StopIn_CHOD");
	AddCounterToEventFraction("No_Detector", "No_StopIn_IRC");
	AddCounterToEventFraction("No_Detector", "No_StopIn_Else");
	DefineSampleSizeCounter("No_Detector", "Inside");

	BookCounter("Multi_StopIn_Straw1");
	BookCounter("Multi_StopIn_Straw2");
	BookCounter("Multi_StopIn_Straw3");
	BookCounter("Multi_StopIn_Straw4");
	BookCounter("Multi_StopIn_RICH");
	BookCounter("Multi_StopIn_CHOD");
	BookCounter("Multi_StopIn_IRC");
	BookCounter("Multi_StopIn_Else");
	BookCounter("Multi_StopIn_LKr");
	NewEventFraction("Multi_Detector");
	SetSignificantDigits("Multi_Detector", 2);
	AddCounterToEventFraction("Multi_Detector", "Inside");
	AddCounterToEventFraction("Multi_Detector", "Multi_StopIn_Straw1");
	AddCounterToEventFraction("Multi_Detector", "Multi_StopIn_Straw2");
	AddCounterToEventFraction("Multi_Detector", "Multi_StopIn_Straw3");
	AddCounterToEventFraction("Multi_Detector", "Multi_StopIn_Straw4");
	AddCounterToEventFraction("Multi_Detector", "Multi_StopIn_RICH");
	AddCounterToEventFraction("Multi_Detector", "Multi_StopIn_CHOD");
	AddCounterToEventFraction("Multi_Detector", "Multi_StopIn_IRC");
	AddCounterToEventFraction("Multi_Detector", "Multi_StopIn_Else");
	DefineSampleSizeCounter("Multi_Detector", "Inside");
}

void SinglePhoton::InitOutput(){
}

void SinglePhoton::DefineMCSimple(){
	//	K+ 		= 321
	//	pi+		= 211
	//	pi0		= 111
	//	gamma	= 22
	//	e-		= 11
	//	mu-		= 13
	//	nu_e	= 12
	//	nu_mu	= 14
	fMCSimple.AddParticle(0, 22);
}

void SinglePhoton::ProcessSpecialTriggers(int){
	/// \MemberDescr
	/// \param iEvent : Special event number
	///
	/// Process method for special triggers. Called on each special trigger event after each start of burst.\n
	/// \EndMemberDescr
}

void SinglePhoton::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;


	TRecoLKrEvent *lkr = (TRecoLKrEvent*)GetEvent("LKr");

	//Candidates
	TRecoLKrCandidate *lkrCand;

	//LKr definition
	int LKrStartPos = 240413;

	TVector3 extrapolatedPos;
	double radius;
	double energyInHits;
	double energy;

	bool outside = false;

	FillHisto("NCandidates", lkr->GetNCandidates());
	IncrementCounter("Total");
	extrapolatedPos = propagate(fMCSimple[22][0]->GetProdPos().Vect(), fMCSimple[22][0]->GetInitialMomentum(), LKrStartPos);

	radius = sqrt(pow(extrapolatedPos.X(), 2) + pow(extrapolatedPos.Y(), 2));
	//Take only events with photons in LKr
	if(radius<180 || radius>1000) outside=true;
	else IncrementCounter("InAcceptance");
	if(fMCSimple[22][0]->GetEndPos().Z()<240413){
		outside=true;
		IncrementCounter("Outside");
	}
	if(outside) return;

	IncrementCounter("Inside");

	if(lkr->GetNCandidates()==1){
		//Single cluster event
		IncrementCounter("SingleCluster");
		lkrCand = (TRecoLKrCandidate*)lkr->GetCandidate(0);
		FillHisto("Single_EndProc", fMCSimple[22][0]->GetEndProcessName(), 1);
		FillHisto("Single_EndPos", fMCSimple[22][0]->GetEndPos().Z());
		FillHisto("Single_RadioXZ", fMCSimple[22][0]->GetEndPos().Z(), fMCSimple[22][0]->GetEndPos().X());
		FillHisto("Single_RadioYZ", fMCSimple[22][0]->GetEndPos().Z(), fMCSimple[22][0]->GetEndPos().Y());
		FillHisto("Single_LKrEnergy", CountHitsEnergy(lkr));
		if(fMCSimple[22][0]->GetEndPos().Z()>240388) FillHisto("Single_CalibLate", fMCSimple[22][0]->GetInitialEnergy()/1000., lkrCand->GetClusterEnergy());
		FillHisto("Single_Calib", fMCSimple[22][0]->GetInitialEnergy()/1000., lkrCand->GetClusterEnergy());
		FillHisto("Single_Energy", lkrCand->GetClusterEnergy());
		FillHisto("Single_LKrClusterPos", lkrCand->GetClusterX()*10., lkrCand->GetClusterY()*10.);
		FillHisto("Single_PosDiff", sqrt(pow(extrapolatedPos.X()-lkrCand->GetClusterX()*10., 2) + pow(extrapolatedPos.Y()-lkrCand->GetClusterY()*10., 2)));

		FillHisto(whichDetector("Single_",fMCSimple[22][0]->GetEndPos().Z()), fMCSimple[22][0]->GetEndPos().Z());
		IncrementCounter(whichDetector("Single_",fMCSimple[22][0]->GetEndPos().Z()));

		double e,M,m;

		e = lkrCand->GetClusterEnergy();
		m = fMCSimple[22][0]->GetInitialEnergy()/2000.0 - 5;
		M = fMCSimple[22][0]->GetInitialEnergy()/2000.0 + 5;
		if(e<M && e>m) ExportEvent();
	}
	else if(lkr->GetNCandidates()==0){
		//No cluster event
		IncrementCounter("0Cluster");
		FillHisto("No_EndProc", fMCSimple[22][0]->GetEndProcessName(), 1);
		FillHisto("No_EndPos", fMCSimple[22][0]->GetEndPos().Z());
		FillHisto("No_RadioXZ", fMCSimple[22][0]->GetEndPos().Z(), fMCSimple[22][0]->GetEndPos().X());
		FillHisto("No_RadioYZ", fMCSimple[22][0]->GetEndPos().Z(), fMCSimple[22][0]->GetEndPos().Y());

		energyInHits = CountHitsEnergy(lkr);
		FillHisto("No_LKrEnergy", energyInHits);

		FillHisto("No_HWCluster", energyInHits, lkr->GetHWClusterCount());
		if(lkr->GetHWClusterCount()==1) FillHisto("No_HWClusterEnergy", energyInHits, lkr->GetHWClusterE(0));

		if(energyInHits>2){
			IncrementCounter("No_EnergyLeftInLKr");
			if(lkr->GetHWClusterCount()>0) IncrementCounter("No_EnergyLeftInLKrWithHWCluster");
		}

		FillHisto(whichDetector("No_",fMCSimple[22][0]->GetEndPos().Z()), fMCSimple[22][0]->GetEndPos().Z());
		IncrementCounter(whichDetector("No_",fMCSimple[22][0]->GetEndPos().Z()));
	}
	else{
		//Multi Cluster event
		IncrementCounter("MultiCluster");
		energy = 0;
		for(int i=0; i<lkr->GetNCandidates(); i++){
			lkrCand = (TRecoLKrCandidate*)lkr->GetCandidate(i);
			energy += lkrCand->GetClusterEnergy();
		}

		FillHisto("Multi_ClusterSumEnergy", energy);
		lkrCand = (TRecoLKrCandidate*)lkr->GetCandidate(getMostEnergetic(lkr));

		FillHisto("Multi_EndProc", fMCSimple[22][0]->GetEndProcessName(), 1);
		FillHisto("Multi_EndPos", fMCSimple[22][0]->GetEndPos().Z());
		FillHisto("Multi_RadioXZ", fMCSimple[22][0]->GetEndPos().Z(), fMCSimple[22][0]->GetEndPos().X());
		FillHisto("Multi_RadioYZ", fMCSimple[22][0]->GetEndPos().Z(), fMCSimple[22][0]->GetEndPos().Y());
		FillHisto("Multi_LKrEnergy", CountHitsEnergy(lkr));
		FillHisto("Multi_Calib", fMCSimple[22][0]->GetInitialEnergy()/1000., lkrCand->GetClusterEnergy());
		FillHisto("Multi_Energy", lkrCand->GetClusterEnergy());
		FillHisto("Multi_LKrClusterPos", lkrCand->GetClusterX()*10., lkrCand->GetClusterY()*10.);
		FillHisto("Multi_PosDiff", sqrt(pow(extrapolatedPos.X()-lkrCand->GetClusterX()*10., 2) + pow(extrapolatedPos.Y()-lkrCand->GetClusterY()*10., 2)));

		FillHisto(whichDetector("Multi_",fMCSimple[22][0]->GetEndPos().Z()), fMCSimple[22][0]->GetEndPos().Z());
		IncrementCounter(whichDetector("Multi_",fMCSimple[22][0]->GetEndPos().Z()));
	}

}

double SinglePhoton::CountHitsEnergy(TRecoLKrEvent *evt){
	double energy = 0.;
	TLKrHit *h;

	for(int i=0; i<evt->GetNHits(); i++){
		h = (TLKrHit*)evt->GetHits()->At(i);
		energy += h->GetEnergy();
	}
	return energy;
}

void SinglePhoton::PostProcess(){
}

void SinglePhoton::ExportPlot(){
	//If you want to save them all, just call
	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void SinglePhoton::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}

int SinglePhoton::getMostEnergetic(TRecoLKrEvent *evt){
	double max = 0;
	int i=0;
	int index=-1;
	TRecoLKrCandidate *cand;

	for(i=0; i<evt->GetNCandidates(); i++){
		cand = (TRecoLKrCandidate*)evt->GetCandidate(i);
		if(cand->GetClusterEnergy()>max){
			max = cand->GetClusterEnergy();
			index = i;
		}
	}
	return index;
}

TString SinglePhoton::whichDetector(TString prefix, int pos){
	TString suffix;
	if(pos>183000 && pos<185000) suffix = "StopIn_Straw1";
	else if(pos>193000 && pos<195000) suffix = "StopIn_Straw2";
	else if(pos>204000 && pos<206000) suffix = "StopIn_Straw3";
	else if(pos>218000 && pos<219606) suffix = "StopIn_Straw4";
	else if(pos>219606 && pos<237670) suffix = "StopIn_RICH";
	else if(pos>238800 && pos<239540) suffix = "StopIn_CHOD";
	else if(pos>239540 && pos<240388) suffix = "StopIn_IRC";
	else if(pos>240390 && pos<243224) suffix = "StopIn_LKr";
	else suffix = "StopIn_Else";

	return prefix + suffix;
}
