//Mandatory includes
#include "ID_HWClusterPi.hh"
#include <stdlib.h>
#include <iostream>
#include "MCSimple.hh"
#include "functions.hh"
#include "Event.hh"
#include <TChain.h>
using namespace std;

#include "TRecoLKrEvent.hh"

/// \class ID_HWClusterPi
/// \Brief 
/// Describe your Analyzer
/// \EndBrief
///
/// \Detailed
/// Describe your Analyzer
/// \EndDetailed

ID_HWClusterPi::ID_HWClusterPi(BaseAnalysis *ba) : Analyzer(ba)
{
	fAnalyzerName = "ID_HWClusterPi";

	RequestTree("LKr", new TRecoLKrEvent);
}

void ID_HWClusterPi::InitOutput(){
	RegisterOutput("PiHWCluster", &HWPiID);
        /// \MemberDescr 
        /// Register the output variables of the analyzer.\n
        /// Call\n
        /// 	RegisterOutput("outputName", &variableName)\n
        /// for each variable that should be in the output of the Analyzer\n
        /// The name of the analyzer will be prepended to the outputName (to avoid collisions with other analyzers)\n
        /// variableName should be the name of a variable declared in the definition of the class\n
        /// \n
        /// Call\n
	AddParam("PiNumber", "int", &piNumber, 1);
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

void ID_HWClusterPi::InitHist(){
	BookHisto("HWClusterNumber", new TH1D("HWClusterNumber", "Number of HW Clusters", 16, -0.5, 15.5));
	BookHisto("SCellNumber", new TH1D("SCellNumber", "Number of SuperCell", 11, -0.5, 10.5));
	BookHisto("EDep", new TH1D("EDep", "Deposited energy for 1 SCell HW clusters", 15, 0, 15));

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

void ID_HWClusterPi::DefineMCSimple(){
}

void ID_HWClusterPi::StartOfRunUser(){
}

void ID_HWClusterPi::StartOfBurstUser(){
}

void ID_HWClusterPi::EndOfBurstUser(){
}

void ID_HWClusterPi::ProcessSpecialTriggers(int){
	/// \MemberDescr
	/// \param iEvent : Special event number
	///
	/// Process method for special triggers. Called on each special trigger event after each start of burst.\n
	/// \EndMemberDescr
}

void ID_HWClusterPi::Process(int iEvent){
	Event*  MCTruthEvent = NULL;
	if(GetWithMC())  MCTruthEvent= GetMCEvent();

	TRecoLKrEvent *lkrEvent;
	int HWClusterCount, SingleSCellClusterCount = 0;
	int SCellCount;

	lkrEvent = (TRecoLKrEvent*)GetEvent("LKr");
	HWClusterCount = lkrEvent->GetHWClusterCount();

	FillHisto("HWClusterNumber", HWClusterCount);
	for(int i=0; i<HWClusterCount; i++){
		//1. Number of supercells
		SCellCount = lkrEvent->GetHWClusterCountSCells(i);
		FillHisto("SCellNumber", SCellCount);

		if(SCellCount==1){
			SingleSCellClusterCount++;
			//2. In case 1 -> less than 1.2 GeV
			FillHisto("EDep", lkrEvent->GetHWClusterE(i));
			if(lkrEvent->GetHWClusterE(i)<1.2){
				HWPiID.push_back(i);
			}
		}
	}

	if(SingleSCellClusterCount<=piNumber){
		SetOutputState("PiHWCluster", kOValid);
	}
	else{
		SetOutputState("PiHWCluster", kOInvalid);
	}
}

void ID_HWClusterPi::PostProcess(){
}

void ID_HWClusterPi::EndOfRunUser(){
}

void ID_HWClusterPi::ExportPlot(){
	SaveAllPlots();
}

void ID_HWClusterPi::DrawPlot(){
	DrawAllPlots();
}
