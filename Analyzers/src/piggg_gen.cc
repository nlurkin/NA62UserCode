//Mandatory includes
#include "piggg_gen.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

piggg_gen::piggg_gen(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "piggg_gen";

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	RequestTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event

	//Initialize DetectorAcceptance if needed
	//use of global instance
	fDetectorAcceptanceInstance = GetDetectorAcceptanceInstance();
	fParticleInterface = ParticleInterface::GetParticleInterface();

	pass = false;
	W = 0;
	T = 0;
}

void piggg_gen::InitOutput(){
	RegisterOutput("pass", &pass);
	RegisterOutput("MCW", &W);
	RegisterOutput("MCT_star", &T);
	//Call
	//	AddParam("paramName", "paramType", &variableName, defaultValue);
	//for each parameter of the analyzer. These parameters can be set when starting the FW from the command line with the -p option.
	//paramName is the name of the parameter in the command line
	//paramType is the type of the parameter (allowed : bool, int, long, float, double, char, string, TString)
	//variableName is the name of the variable that should be declared in the definition of the class<br>
	//defaultValue is the default value if not specified in the command line

	//To create a new TTree in the output file, call
	//	OpenNewTree("TTreeName", "TTreeTitle");
	//TTreeName is the name of the TTree (will be used to refer to this TTree later)
	//TTreeTitle is the title of the TTree
	//Call
	//	void AddBranch<VariableType>("TTreeName", "BranchName", &variable);
	//VariableType is the type of the variable for this branch (fundamental data type or class)
	//TTreeName is the name of the TTree to add this branch
	//BranchName is the name of the branch
	//variable is a pointer to the variable (should be declared in the header file)
}

void piggg_gen::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors
	//	BookHisto(name, histogram*, isAutoUpdate)
	//If isAutotUpdate is true, this histogram will be drawn and updated regularly during the processing. (default=false)
	//The refresh interval can be set with (default=10)
	// SetUpdateInterval(interval)
	//Defining plots as AutoUpdate and setting the interval can also be done at runtime with a configuration file
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));

	BookHisto("W", new TH1D("W", "W distribution", 20, 0, 1));
	BookHisto("T_star", new TH1D("T_star", "T^* Distribution", 100, 0, 1000));
	BookHisto("rad_gamma_E", new TH1D("rad_gamma_E", "Radiative photon energy", 100, 0, 75000));

	BookHisto("W_geom", new TH1D("W_geom", "W distribution (acceptance)", 20, 0, 1));
	BookHisto("T_star_geom", new TH1D("T_star_geom", "T^* Distribution (acceptance)", 100, 0, 1000));


	BookCounter("TotalEvents");
	BookCounter("Pi+Fail");
	BookCounter("g1Fail");
	BookCounter("g2Fail");
	BookCounter("g3Fail");

	NewEventFraction("GeometryFails");
	AddCounterToEventFraction("GeometryFails", "TotalEvents");
	AddCounterToEventFraction("GeometryFails", "Pi+Fail");
	AddCounterToEventFraction("GeometryFails", "g1Fail");
	AddCounterToEventFraction("GeometryFails", "g2Fail");
	AddCounterToEventFraction("GeometryFails", "g3Fail");

	DefineSampleSizeCounter("GeometryFails", "TotalEvents");
	//Booking of counters and creation of EventFraction can be done here
	//	BookCounter(name)
	//	NewEventFraction(name)
	//Example
	//	BookCounter("Total");
	//	BookCounter("PassCuts");
	//	NewEventFraction("Cuts");

	//Add the counters to the EventFraction
	//	AddCounterToEventFraction(EventFractionName, CounterName)
	//Example
	//	AddCounterToEventFraction("Cuts", "Total");
	//	AddCounterToEventFraction("Cuts", "PassCuts");

	//Then define which counter represents the sample size
	//	DefineSampleSizeCounter(EventFractionName, CounterName)
	//Exemple
	//	DefineSampleSizeCounter("Cuts", "Total");
}

void piggg_gen::DefineMCSimple(){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple.AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//	int kaonID = fMCSimple.AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple.AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	//Some PDG codes (add minus for antiparticles) :
	//	K+ 		= 321
	//	pi+		= 211
	//	pi0		= 111
	//	gamma	= 22
	//	e-		= 11
	//	mu-		= 13
	//	nu_e	= 12
	//	nu_mu	= 14

	int kaonID = fMCSimple.AddParticle(0, 321);
	fMCSimple.AddParticle(kaonID, 211);
	int pi0ID = fMCSimple.AddParticle(kaonID, 111);
	fMCSimple.AddParticle(pi0ID, 22);
	fMCSimple.AddParticle(pi0ID, 22);
	fMCSimple.AddParticle(kaonID, 22);
}

void piggg_gen::ProcessSpecialTriggers(int){
	/// \MemberDescr
	/// \param iEvent : Special event number
	///
	/// Process method for special triggers. Called on each special trigger event after each start of burst.\n
	/// \EndMemberDescr
}

void piggg_gen::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	if(fMCSimple.fStatus == MCSimple::kMissing) return;
	//If the analyzer can run without MC data, comment the line
	if(fMCSimple.fStatus == MCSimple::kEmpty) return;

	double pipGamma;

	TLorentzVector PK, q, pip, rest;
	TVector3 boost, b;

	double mK = fParticleInterface->FindParticle("K+")->Mass()*1000;
	double mpip = fParticleInterface->FindParticle("pi+")->Mass()*1000;

	// Generator related
	//Get particles momenta in the lab
	PK = fMCSimple["K+"][0]->GetInitial4Momentum();
	q = fMCSimple["gamma"][2]->GetInitial4Momentum();
	pip = fMCSimple["pi+"][0]->GetInitial4Momentum();

	//Get the lorentz parameters to get back to the kaon restframe
	boost = -PK.BoostVector();
	b.SetXYZ(-PK.X()/PK.E(), -PK.Y()/PK.E(), -PK.Z()/PK.E()); //Should be the same

	//Verify sameness
	if(distance3D(boost, b)>0) cout << "Boost vectors are different in event " << iEvent << endl;

	//Get the pi+ momenta in the kaon restframe
	rest = pip;
	rest.Boost(boost);

	//Pi+ gamma factor
	pipGamma = 1/sqrt(1-rest.BoostVector().Mag2());

	T = (pipGamma-1)*mpip;
	W = ((PK.Dot(q))*(pip.Dot(q)))/pow(mK*mpip,2);

	FillHisto("rad_gamma_E", q.E());
	FillHisto("W", W);
	FillHisto("T_star", T);

	bool pipAcceptance = false;
	bool g1Acceptance = false;
	bool g2Acceptance = false;
	bool g3Acceptance = false;

	// Acceptance related
	IncrementCounter("TotalEvents");

	fDetectorAcceptanceInstance->FillPath(fMCSimple["pi+"][0]->GetProdPos().Vect(), pip.Vect());
	pipAcceptance = fDetectorAcceptanceInstance->GetDetAcceptance(DetectorAcceptance::kSpectrometer);
	if(!pipAcceptance) IncrementCounter("Pi+Fail");

	fDetectorAcceptanceInstance->CleanDetPath();
	fDetectorAcceptanceInstance->FillPath(fMCSimple["gamma"][0]->GetProdPos().Vect(), fMCSimple["gamma"][0]->GetInitialMomentum());
	g1Acceptance = fDetectorAcceptanceInstance->GetDetAcceptance(DetectorAcceptance::kLKr);
	if(!g1Acceptance) IncrementCounter("g1Fail");

	fDetectorAcceptanceInstance->CleanDetPath();
	fDetectorAcceptanceInstance->FillPath(fMCSimple["gamma"][1]->GetProdPos().Vect(), fMCSimple["gamma"][1]->GetInitialMomentum());
	g2Acceptance = fDetectorAcceptanceInstance->GetDetAcceptance(DetectorAcceptance::kLKr);
	if(!g2Acceptance) IncrementCounter("g2Fail");

	fDetectorAcceptanceInstance->CleanDetPath();
	fDetectorAcceptanceInstance->FillPath(fMCSimple["gamma"][2]->GetProdPos().Vect(), fMCSimple["gamma"][2]->GetInitialMomentum());
	g3Acceptance = fDetectorAcceptanceInstance->GetDetAcceptance(DetectorAcceptance::kLKr);
	if(!g3Acceptance) IncrementCounter("g3Fail");

	if(pipAcceptance && g1Acceptance && g2Acceptance && g3Acceptance){
		pass = true;
		FillHisto("W_geom", W);
		FillHisto("T_star_geom", T);

		SetOutputState("MCW", Analyzer::kOValid);
		SetOutputState("MCT_star", Analyzer::kOValid);
	}
	else{
		pass = false;
		SetOutputState("MCW", Analyzer::kOInvalid);
		SetOutputState("MCT_star", Analyzer::kOInvalid);
	}
	SetOutputState("pass", Analyzer::kOValid);





	//You can retrieve MC particles from the fMCSimple Set with (return a vector<KinePart*>
	//	fMCSimple["particleName"]
	//	fMCSimple[pdgID]
	//Example
	//	fMCSimple["k+"][index] for the kaon
	//	fMCSimple["pi+"][index] for the positive pion
	//	fMCSimple["gamma"][index] for the photon
	//The number in the brackets is the index of the particle (if you asked for two photons in the set, you can ask fMCSimple["gamma"][0] for the first one and fMCSimple["gamma"][1] for the second)

	//You can retrieve the events from the trees with
	//	(eventClass*)GetEvent("treeName");

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

	//Modify a counter with one of the following methods
	//	IncrementCounter(name)
	//	IncrementCounter(name, delta)
	//	DecrementCounter(name)
	//	DecrementCounter(name, delta)
	//	SetCounterValue(name, value)

	//For use of fGeom, read DetectorAcceptance class

	//To use the output of a different analyzer, use
	//	outputType var = *(outputType*)GetOutput("analyzerName.outputName", state);
	//Where outputType is the variable type and state is of type outputState
	//This function returns a void* and you have to explicitly cast it to the real object type
	//State is set with the state of the variable (kOUninit, kOInvalid ,kOValid). The value of the output should only be trusted if state == kOValid
	//example : TLorentzVector vertex = *(TLorentzVector*)GetOutput("simpleVertexAnalyzer.vertex", state);

	//Before starting the processing of an event, the state flag of each output variable is reset to kOUninit
	//When setting the value of an output variable, don't forget to set appropriately the state flag to either kOValid or kOInvalid
	//to indicate if the value can/can't be used in other analyzer
	//	SetOutputState("outputName", kOValid);

	//If you want to save this event in the output file, call
	//	ExportEvent();
	//The structure of all the trees that have been opened (by all Analyzer) will be copied in the output file
	//and the events for which at least one analyzer called ExportEvent() will be written in the output trees
}

void piggg_gen::PostProcess(){

}

void piggg_gen::ExportPlot(){
	//If you want to save them all, just call
	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void piggg_gen::DrawPlot(){
	//If you want to draw all the plots, just call
	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}

void piggg_gen::NewBurstUser(){
}

