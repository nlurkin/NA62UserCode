//Mandatory includes
#include "piggg_HW.hh"
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
#include "TRecoSACEvent.hh"*/
#include "TRecoLKrEvent.hh"

//Additional includes
/*#include <TString.h>
#include <TH1I.h>
#include <TH2I.h>
#include <TGraph.h>
#include <TCanvas.h>*/
#include <TGraphAsymmErrors.h>

/// \class piggg_HW
/// \Brief 
/// Describe your Analyzer
/// \EndBrief
///
/// \Detailed
/// Describe your Analyzer
/// \EndDetailed

piggg_HW::piggg_HW(BaseAnalysis *ba) : Analyzer(ba)
{
	/// \MemberDescr
	/// \param ba : parent BaseAnalysis
	///
	/// Set the name of your analyzer\n
	/// Select the trees you want to get\n
	/// If you want to use DetectorAcceptance, initialize it.

	fAnalyzerName = "piggg_HW";

	//Specify the trees you want to use and the event class corresponding
	//Don't try to load MCTruth tree (RUN_0 or Event). Use the MCTruthEvent in Process function instead. Problems when opening twice the same tree.
	//Example with RecoEvent
	RequestTree("LKr", new TRecoLKrEvent);
	//Example with MC Event
	//	RequestTree("GigaTracker", new TGigaTrackerEvent);
	//Example with generic tree
	//	RequestTree<MyClass>("MyTree", "BranchName", "MyClass", new MyClass);

	//Initialize DetectorAcceptance if needed
	//use of global instance
	//	fDetectorAcceptanceInstance = GetDetectorAcceptanceInstance();
	//use of local instance
	//	fDetectorAcceptanceInstance = new DetectorAcceptance("./NA62.root");
}

void piggg_HW::InitOutput(){
	RegisterOutput("pass", &pass);

	/// \MemberDescr
	/// Register the output variables of the analyzer.\n
	/// Call\n
	/// 	RegisterOutput("outputName", &variableName)\n
	/// for each variable that should be in the output of the Analyzer\n
	/// The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)\n
	/// variableName should be the name of a variable declared in the definition of the class\n
	/// \n
	/// Call\n
	/// 	AddParam("paramName", "paramType", &variableName, defaultValue);\n
	/// for each parameter of the analyzer. These parameters can be set when starting the FW from the command line with the -p option.\n
	/// paramName is the name of the parameter in the command line\n
	/// paramType is the type of the parameter (allowed : bool, int, long, float, double, char, string, TString)\n
	/// variableName is the name of the variable that should be declared in the definition of the class\n
	/// defaultValue is the default value if not specified in the command line\n
	/// \n
	/// To create a new TTree in the output file, call\n
	/// 	void OpenNewTree("TTreeName", "TTreeTitle");\n
	/// TTreeName is the name of the TTree (will be used to refer to this TTree later)\n
	/// TTreeTitle is the title of the TTree\n
	/// Call\n
	/// 	void AddBranch<VariableType>("TTreeName", "BranchName", &pointer);\n
	/// VariableType is the type of the variable for this branch (fundamental data type or class)\n
	/// TTreeName is the name of the TTree to add this branch\n
	/// BranchName is the name of the branch\n
	/// pointer is a pointer to the variable (should be declared in the header file)\n
	/// \n
	/// To create a standard TTree containing KineParts (for candidates) use\n
	///     CreateStandardTree("TTreeName", "TTreeTitle");
	/// \EndMemberDescr
}

void piggg_HW::InitHist(){

	BookHisto("W", new TH1D("W_HW", "W distribution", 20, 0, 1));
	BookHisto("T_star", new TH1D("T_star_HW", "T^* Distribution", 100, 0, 1000));

	BookHisto("W_HWPi", new TH1D("W_HWPi", "W distribution", 20, 0, 1));
	BookHisto("T_star_HWPi", new TH1D("T_star_HWPi", "T^* Distribution", 100, 0, 1000));

	BookHisto("Ratio_WHW", new TGraphAsymmErrors());
	BookHisto("Ratio_THW", new TGraphAsymmErrors());
	BookHisto("Ratio_WHWPi", new TGraphAsymmErrors());
	BookHisto("Ratio_THWPi", new TGraphAsymmErrors());
	/// \MemberDescr
	/// Book and Initialize histograms in this function.\n
	/// Same function to Book TH1, TH2, TGraph and TGraphAsymmErrors (anything derived from TH1 or TGraph)\n
	///  BookHisto(name, histogram*)\n
	/// If isAutotUpdate is true, this histogram will be drawn and updated regularly during the processing. (default=false)
	/// The refresh interval can be set with (default=10)
	///  SetUpdateInterval(interval)
	/// Defining plots as AutoUpdate and setting the interval can also be done at runtime with a configuration file
	/// Example\n
	/// 	BookHisto("PartEnergy", new TH2I("PartEnergy", "Energy as a function of particle", 0, 0, 0, Bins, MinEnergy, MaxEnergy));\n
	/// \n
	/// Booking of counters and creation of EventFraction can be done here\n
	///	BookCounter(name)\n
	///	NewEventFraction(name)\n
	/// Example\n
	/// 	BookCounter("Total");\n
	/// 	BookCounter("PassCuts");\n
	/// 	NewEventFraction("Cuts");\n
	/// \n
	/// Add the counters to the EventFraction\n
	/// 	AddCounterToEventFraction(EventFractionName, CounterName)\n
	/// Example\n
	/// 	AddCounterToEventFraction("Cuts", "Total");\n
	/// 	AddCounterToEventFraction("Cuts", "PassCuts");\n
	/// \n
	/// Then define which counter represents the sample size\n
	/// 	DefineSampleSizeCounter(EventFractionName, CounterName)\n
	/// Example\n
	/// 	DefineSampleSizeCounter("Cuts", "Total");\n
	/// \n
	/// You can retrieve histograms from the input ROOT file (Anything derived from TH1) with\n
	///  RequestHistogram("TDirectoryName", "HistogramName", appendOnNewFile);\n
	/// appendOnNewFile is a boolean. If set to true, each time a new file is opened the content\n
	/// of the histogram will be appended to the content of the previous one. If set to false, the content \n
	/// of the histogram is replaced each time a new file is opened.
	/// \EndMemberDescr
}

void piggg_HW::DefineMCSimple(){
	/// \MemberDescr
	/// \param fMCSimple : MCSimple
	///
	/// Setup of fMCSimple. You must specify the generated MC particles you want.\n
	/// Add particles you want to recover from fMCSimple\n
	/// int particleID = fMCSimple.AddParticle(parentID, pdgCode)\n
	/// parentID : 	0=no parent (=beam particle)\n
	/// 	...\n
	/// Example : you want to retrieve the kaon from the beam, the pi0 an pi+ from the beam kaon and the 2 photons coming from the previous pi0 decay :\n
	/// 	int kaonID = AddParticle(0, 321) //Ask beam kaon (sequence ID=1)\n
	/// 	AddParticle(kaonID, 211) //Ask pi+ from previous kaon (sequence ID=2)\n
	/// 	int pi0ID = AddParticle(kaonID, 111) //Ask pi0 from previous kaon (sequence ID=3)\n
	/// 	AddParticle(pi0ID, 22) //Ask first gamma from previous pi0 (sequence ID=4)\n
	/// 	AddParticle(pi0ID, 22) //Ask second gamma from previous pi0 (sequence ID=4)
	///
	/// @see ROOT TDatabasePDG for a list of PDG codes and particle naming convention
	/// \EndMemberDescr
}

void piggg_HW::StartOfRunUser(){
	/// \MemberDescr
	/// This method is called at the beginning of the processing (corresponding to a start of run in the normal NA62 data taking)\n
	/// Do here your start of run processing if any
	/// \EndMemberDescr
}

void piggg_HW::StartOfBurstUser(){
	/// \MemberDescr
	/// This method is called when a new file is opened in the ROOT TChain (corresponding to a start/end of burst in the normal NA62 data taking) + at the beginning of the first file\n
	/// Do here your start/end of burst processing if any
	/// \EndMemberDescr
}

void piggg_HW::EndOfBurstUser(){
	/// \MemberDescr
	/// This method is called when a new file is opened in the ROOT TChain (corresponding to a start/end of burst in the normal NA62 data taking) + at the end of the last file\n
	/// Do here your start/end of burst processing if any
	/// \EndMemberDescr
}

void piggg_HW::ProcessSpecialTriggers(int){
	/// \MemberDescr
	/// \param iEvent : Special event number
	///
	/// Process method for special triggers. Called on each special trigger event after each start of burst.\n
	/// \EndMemberDescr
}

void piggg_HW::Process(int iEvent){
	Event*  MCTruthEvent;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();

	double W,T;
	bool passGeom;
	//vector<int> pipCluster;
	OutputState state;

	TRecoLKrEvent *lkrEvent;

	passGeom = *GetOutput<bool>("piggg_gen.pass", state);
	W = *GetOutput<double>("piggg_gen.MCW", state);
	T = *GetOutput<double>("piggg_gen.MCT_star", state);

	SetOutputState("pass", kOInvalid);
	pass = false;
	if(passGeom){
		lkrEvent = (TRecoLKrEvent*)GetEvent("LKr");

		if(lkrEvent->GetHWClusterCount()==4){
			pass = true;
			FillHisto("W", W);
			FillHisto("T_star", T);
		}
		SetOutputState("pass", kOValid);
	}

	if(pass){
		GetOutput< vector<int> >("ID_HWClusterPi.PiHWCluster", state);

		if(state==kOValid){
			pass = true;
			FillHisto("W_HWPi", W);
			FillHisto("T_star_HWPi", T);
		}
		else{
			pass = false;
		}
	}


	/// You can retrieve the events from the trees with\n
	/// 	(eventClass*)GetEvent("treeName");\n
	/// You can retrieve data from generic TTrees with\n
	/// 	GetObject<MyClass>("treeName");\n
	/// \n
	/// You can retrieve the histograms you booked (for drawing, changing, filling, ...) with\n
	/// 	fHisto["histoName"] for TH1\n
	/// 	fHisto2["histoName"] for TH2\n
	/// 	fGraph["graphName"] for TGraph and TGraphAsymmErrors\n
	/// Be carefull !! If the histogram you ask for doesn't exist or you ask for an existing histogram\n
	/// in the wrong recipient (e.g. th2 in fHisto), program will segfault.\n
	/// To fill the histograms without risk of segfault, you can use\n
	/// 	FillHisto("histoName", values)\n
	/// where values are the same parameters as if you call histogram->Fill(values) (x,y,weight,...)\n
	/// Instead of segfault, this function print an error message\n
	/// \n
	/// Modify a counter with one of the following methods\n
	/// 	IncrementCounter(name)\n
	/// 	IncrementCounter(name, delta)\n
	/// 	DecrementCounter(name)\n
	/// 	DecrementCounter(name, delta)\n
	/// 	SetCounterValue(name, value)\n
	/// \n
	/// For use of fGeom, read DetectorAcceptance class\n
	/// \n
	/// To use the output of a different analyzer, use\n
	/// 	outputType *var = GetOutput<outputType>("analyzerName.outputName", state);\n
	/// Where outputType is the variable type and state is of type outputState\n
	/// This function returns a void* and you have to explicitly cast it to the real object type\n
	/// State is set with the state of the variable (kOUninit, kOInvalid ,kOValid). The value of the output should only be trusted if state == kOValid\n
	/// example : TLorentzVector vertex = *(TLorentzVector*)GetOutput("simpleVertexAnalyzer.vertex", state);\n
	/// \n
	/// Before starting the processing of an event, the state flag of each output variable is reset to kOUninit\n
	/// When setting the value of an output variable, don't forget to set appropriately the state flag to either kOValid or kOInvalid\n
	/// to indicate if the value can/can't be used in other analyzer\n
	/// 	SetOutputState("outputName", kOValid);\n
	/// 	\n
	/// If you want to save this event in the output file, call\n
	/// 	ExportEvent();\n
	/// The structure of all the trees that have been opened (by all Analyzer) will be copied in the output file\n
	/// and the events for which at least one analyzer called ExportEvent() will be written in the output trees.\n
	/// If you want to append a candidate in one of your standard output Tree, use\n
	/// 	KinePart *candidate = CreateStandardCandidate("treeName");\n
	/// and fill the properties of your candidate. It will be automatically written in the output tree.\n
	/// \EndMemberDescr
	//
	//Ask the fMCSimple to have the complete set of particles we specified
	//If the analyzer can run without the complete set, comment the line

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
	//You can retrieve data from generic TTrees with
	//	GetObject<MyClass>("treeName");

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

	//If you want to append a candidate in one of your standard output Tree, use
	//	KinePart *candidate = CreateStandardCandidate("treeName");
	//and fill the properties of your candidate. It will be automatically written in the output tree.
}

void piggg_HW::PostProcess(){
	/// \MemberDescr
	/// This function is called after an event has been processed by all analyzers. It could be used to free some memory allocated
	/// during the Process.
	/// \EndMemberDescr

}

void piggg_HW::EndOfRunUser(){
	/// \MemberDescr
	/// This method is called at the end of the processing (corresponding to a end of run in the normal NA62 data taking\n)
	/// Do here your end of run processing if any
	/// \EndMemberDescr

	OutputState status;
	gROOT->Cd("../piggg_gen");
	TH1D *W_base = (TH1D*)gDirectory->Get("W");
	TH1D *T_base = (TH1D*)gDirectory->Get("T_star");
	gROOT->Cd("../piggg_HW");
	TH1D *WHW = (TH1D*)fHisto["W"];
	TH1D *THW = (TH1D*)fHisto["T_star"];
	TH1D *WHWPi = (TH1D*)fHisto["W_HWPi"];
	TH1D *THWPi = (TH1D*)fHisto["T_star_HWPi"];
	TGraphAsymmErrors *RWHW = (TGraphAsymmErrors*)fGraph["Ratio_WHW"];
	TGraphAsymmErrors *TWHW = (TGraphAsymmErrors*)fGraph["Ratio_THW"];
	TGraphAsymmErrors *RWHWPi = (TGraphAsymmErrors*)fGraph["Ratio_WHWPi"];
	TGraphAsymmErrors *TWHWPi = (TGraphAsymmErrors*)fGraph["Ratio_THWPi"];

	RWHW->BayesDivide(WHW, W_base);
	TWHW->BayesDivide(THW, T_base);
	RWHWPi->BayesDivide(WHWPi, W_base);
	TWHWPi->BayesDivide(THWPi, T_base);
}

void piggg_HW::ExportPlot(){
	/// \MemberDescr
	/// This method is called at the end of processing to save plots.\n
	/// If you want to save them all, just call\n
	/// 	SaveAllPlots();\n
	/// Or you can just save the ones you want with\n
	/// 	histogram->Write()\n
	/// \EndMemberDescr

	SaveAllPlots();
}

void piggg_HW::DrawPlot(){
	/// \MemberDescr
	/// This method is called at the end of processing to draw plots.
	/// If you want to draw all the plots, just call
	/// 	DrawAllPlots();
	/// Or do as usual (TCanvas, Draw, ...)
	/// \EndMemberDescr
	DrawAllPlots();
}
