//Mandatory includes
#include "tes_use.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

//Include the hh files for events kinds you will use
/*#include "TRecoGigaTrackerEvent.hh"
#include "TRecoRICHEvent.hh"
#include "TRecoCHANTIEvent.hh"
#include "TRecoCedarEvent.hh"
#include "TRecoCHODEvent.hh"
#include "TRecoMUV1Event.hh"
#include "TRecoMUV2Event.hh"
#include "TRecoMUV3Event.hh"
#include "TRecoMUV0Event.hh"
#include "TRecoHACEvent.hh"
#include "TRecoIRCEvent.hh"
#include "TRecoLAVEvent.hh"
#include "TRecoSACEvent.hh"
#include "TRecoLKrEvent.hh"
#include "TRecoSpectrometerEvent.hh"*/

//Additional includes
/*#include <TString.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TCanvas.h>*/

tes_use::tes_use(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "tes_use";

	RequestTree("tes_gen/test", "fCandidates", "TClonesArray", new TClonesArray("KinePart", 10));

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems wen opening twice the same tree.
	//Example with RecoEvent
	//	RequestTree("GigaTracker", new TRecoGigaTrackerEvent);
	//Example with MC Event
	//	RequestTree("GigaTracker", new TGigaTrackerEvent);

	//Initialize DetectorAcceptance if needed
	//use of global instance
	//	fDetectorAcceptanceInstance = GetDetectorAcceptanceInstance();
	//use of local instance
	//	fDetectorAcceptanceInstance = new DetectorAcceptance("./NA62.gdml");
}

void tes_use::InitOutput(){
	//Call
	//	RegisterOutput("outputName", &variableName)
	//for each variable that should be in the output of the Analyzer
	//The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)
	//variableName should be the name of a variable declared in the definition of the class

	//Call
	//	AddParam("paramName", "paramType", &variableName, defaultValue);
	//for each parameter of the analyzer. These parameters can be set when starting the FW from the command line with the -p option.
	//paramName is the name of the parameter in the command line
	//paramType is the type of the parameter (allowed : bool, int, long, float, double, char, string, TString)
	//variableName is the name of the variable that should be declared in the definition of the class<br>
	//defaultValue is the default value if not specified in the command line

	//To create a new TTree in the output file, call
	//	void OpenNewTree("TTreeName", "TTreeTitle");
	//TTreeName is the name of the TTree (will be used to refer to this TTree later)
	//TTreeTitle is the title of the TTree
	//Call
	//	void AddBranch<VariableType>("TTreeName", "BranchName", &variable);
	//VariableType is the type of the variable for this branch (fundamental data type or class)
	//TTreeName is the name of the TTree to add this branch
	//BranchName is the name of the branch
	//pointer is a pointer to the variable (should be declared in the header file)
}

void tes_use::InitHist(){
	//Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors (anything derived from TH1 or TGraph)
	//	BookHisto(name, histogram*, isAutoUpdate)
	//If isAutotUpdate is true, this histogram will be drawn and updated regularly during the processing. (default=false)
	//The refresh interval can be set with (default=10)
	// SetUpdateInterval(interval)
	//Defining plots as AutoUpdate and setting the interval can also be done at runtime with a configuration file
	//Example
	//	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));

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

void tes_use::DefineMCSimple(MCSimple *fMCSimple){
	//Add particles you want to recover from fMCSimple
	//	int particleID = fMCSimple->AddParticle(parentID, pdgCode)
	//parentID : 	0=no parent (=beam particle)
	//		...
	//Example
	//	int kaonID = fMCSimple->AddParticle(0, 321); //ask for beam Kaon
	//	fMCSimple->AddParticle(kaonID, 211); //ask for positive pion from initial kaon decay
	//Some PDG codes (add minus for antiparticles) :
	//	K+ 		= 321
	//	pi+		= 211
	//	pi0		= 111
	//	gamma	= 22
	//	e-		= 11
	//	mu-		= 13
	//	nu_e	= 12
	//	nu_mu	= 14
}

void NewBurstUser(){

}

void tes_use::Process(int iEvent, MCSimple &fMCSimple, Event* MCTruthEvent){
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line
	//if(fMCSimple.fStatus == MCSimple::kMissing){printIncompleteMCWarning(iEvent);return;}
	//If the analyzer can run without MC data, comment the line
	//if(fMCSimple.fStatus == MCSimple::kEmpty){printNoMCWarning();return;}

	TClonesArray *a = GetObject<TClonesArray>("tes_gen/test");
	cout << a->GetEntries() << endl;
	//cout << ((KinePart*)a->At(0))->GetPDGcode() << endl;
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

void tes_use::PostProcess(){

}

void tes_use::ExportPlot(){
	//If you want to save them all, just call
	//	SaveAllPlots();
	//Or you can just save the ones you want with
	//	histogram->Write()
}

void tes_use::DrawPlot(){
	//If you want to draw all the plots, just call
	//	DrawAllPlots();
	//Or do as usual (TCanvas, Draw, ...)
}
