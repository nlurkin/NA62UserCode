#include "testGeo.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;


/// \class testGeo
/// \Brief 
/// Detector acceptance test class
/// \EndBrief
///
/// \Detailed
/// Use a set of K+->pi+... to obtain results for a realistic set of particles.
/// The pi+ is propagated up to LKr in 4 different ways:\n
///	1)With FillPath of DetectorAcceptance with an electric charge=0 (no effect of magnetic field)\n
///	2)In straight line up to the resulting Z coordinate of 1)\n
///	3)With FillPath of DetectorAcceptance with an electric charge=+1 (take the magnetic field into account)\n
/// 4)Using MagPropagate for the analytical propagation taking the magnetic field into account (up to the resulting Z of 3)\n
/// \n
/// We then compare:\n
/// - The distance between 1) and 2) to check the FillPath propagation of neutral particles\n
/// - The distance between 3) and 4) to check the FillPath propagation of charged particles\n
/// - The distance between 2) and 4) to have an idea of the error made if the magnetic field is not taken into account\n
/// \n
/// The two first should be very close to 0 as the last should spread above 0.
/// \EndDetailed

testGeo::testGeo(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "testGeo";
	fDetectorAcceptanceInstance = GetDetectorAcceptanceInstance();
}

void testGeo::InitOutput(){
}

void testGeo::InitHist(){
	BookHisto("dneutral", new TH1D("dneutral", "Distance difference between simple propagate and detectorAcceptance propagate (neutral)", 200,0,5));
	BookHisto("dcharged", new TH1D("dcharged", "Distance difference between magPropagate and detectorAcceptance propagate (charged)", 200,0,5));
	BookHisto("dmag", new TH1D("dmag", "Distance difference between simple propagate and magPropagate (charged)", 200,0,5));

	BookHisto("PathLength", new TH2D("PathLength", "Path length in detectors.", 0,0,0,1000, 0, 1000));
}

void testGeo::DefineMCSimple(MCSimple *fMCSimple){
	//Get the pi+
	int kaonID = fMCSimple->AddParticle(0, 321);
	fMCSimple->AddParticle(kaonID, 211);
}

void testGeo::StartOfRunUser(){
}

void testGeo::StartOfBurstUser(){
}

void testGeo::EndOfBurstUser(){
}

void testGeo::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	//We need the pi+. Skip the event if not found
	if(fMCSimple.fStatus == MCSimple::kMissing){printIncompleteMCWarning(iEvent);return;}

	TVector3 startPos,startMomentum;
	TVector3 magPropPos,magPropMomentum;
	TVector3 simplePropPos;
	TVector3 detAccQPos;
	TVector3 detAccNPos;

	//Start from a simulated particle
	startPos = fMCSimple["pi+"][0]->GetProdPos().Vect();
	startMomentum = fMCSimple["pi+"][0]->GetInitialMomentum();

	//Propagate neutrals
	fDetectorAcceptanceInstance->FillPath(startPos, startMomentum, 0,0);
	if(fDetectorAcceptanceInstance->GetDetAcceptance(DetectorAcceptance::kLKr)){
		detAccNPos = fDetectorAcceptanceInstance->GetDetPosition(DetectorAcceptance::kLKr);
		simplePropPos = propagate(startPos, startMomentum, detAccNPos.Z());
		FillHisto("dneutral", distance3D(detAccNPos, simplePropPos));
	}

	//Propagate charged
	fDetectorAcceptanceInstance->FillPath(startPos, startMomentum, 0,1);
	if(fDetectorAcceptanceInstance->GetDetAcceptance(DetectorAcceptance::kLKr)){
		detAccQPos = fDetectorAcceptanceInstance->GetDetPosition(DetectorAcceptance::kLKr);
		fDetectorAcceptanceInstance->MagPropagate(startPos, startMomentum, 1, detAccQPos.Z(), magPropPos, magPropMomentum);
		FillHisto("dcharged", distance3D(detAccQPos, magPropPos));
		simplePropPos = propagate(startPos, startMomentum, magPropPos.Z());
		FillHisto("dmag", distance3D(magPropPos, simplePropPos));
	}


	TString names[14] = {"kCEDAR", "kGTK", "kCHANTI", "kLAV", "kSpectrometer", "kIRC", "kCHOD", "kLKr", "kSAC", "kMUV0", "kMUV1", "kMUV2", "kMUV3", "kHAC"};
	cout << "Traveled " << fDetectorAcceptanceInstance->GetDetLength(DetectorAcceptance::kSpectrometer) << " mm in straw" << endl;
	for(int i=0; i<14; i++){
		if(fDetectorAcceptanceInstance->GetDetAcceptance((DetectorAcceptance::volume)i)){
			FillHisto("PathLength", names[i], fDetectorAcceptanceInstance->GetDetLength((DetectorAcceptance::volume)i),1);
			if(fDetectorAcceptanceInstance->GetDetLength((DetectorAcceptance::volume)i)==0) cout << names[i] << endl;
		}
	}
}

void testGeo::PostProcess(){

}

void testGeo::EndOfRunUser(){

}

void testGeo::ExportPlot(){
	SaveAllPlots();
}

void testGeo::DrawPlot(){
	DrawAllPlots();
}
