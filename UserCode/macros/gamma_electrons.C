#include <TLegend.h>

void draw(TH1* g, TH1* e){
	TCanvas *c = new TCanvas();

	TLegend *l = new TLegend(0.1,0.8,0.48,0.9);
	l->AddEntry(g, "file1");
	l->AddEntry(e, "file2");
	double scale = 1/g->Integral()*10;
	g->SetLineColor(kGreen);
	g->SetMarkerColor(kGreen);
	g->Scale(scale);
	g->Draw();
	e->SetLineColor(kRed);
	e->SetMarkerColor(kRed);
	scale = 1/e->Integral()*10;
	e->Scale(scale);
	e->Draw("SAME");
	l->Draw();
}

int gamma_electrons(TString f1Name, TString f2Name){
	TFile *f1 = TFile::Open(f1Name, "READ");

	TH1D *gmultiplicity = f1->Get("ShowerLKr/multiplicity;");
	TH1D *gstartPos = f1->Get("ShowerLKr/startPos");
	TH2D *gLKrPosition = f1->Get("ShowerLKr/LKrPosition");
	/*TH1D *gClusterEnergySingle = f1->Get("ShowerLKr/ClusterEnergySingle");
	TH2I *gclusterRecoSingle = f1->Get("ShowerLKr/clusterRecoSingle");
	TH2I *gClusterPositionSingle = f1->Get("ShowerLKr/ClusterPositionSingle");

	TH1D *gGetClusterChi2RMSSingle = f1->Get("ShowerLKr/GetClusterChi2RMSSingle");
	TH1D *gGetCluster77EnergySingle = f1->Get("ShowerLKr/GetCluster77EnergySingle");
	TH1D *gGetClusterKe3EnergySingle = f1->Get("ShowerLKr/GetClusterKe3EnergySingle");
	TH1D *gGetClusterRMSXSingle = f1->Get("ShowerLKr/GetClusterRMSXSingle");
	TH1D *gGetClusterRMSYSingle = f1->Get("ShowerLKr/GetClusterRMSYSingle");
	TH1D *gGetClusterRMSSingle = f1->Get("ShowerLKr/GetClusterRMSSingle");
	TH1D *gGetClusterStatusSingle = f1->Get("ShowerLKr/GetClusterStatusSingle");
	TH1D *gGetClusterTimeSingle = f1->Get("ShowerLKr/GetClusterTimeSingle");
	TH1D *gGetClusterTimeLateralCellSingle = f1->Get("ShowerLKr/GetClusterTimeLateralCellSingle");
	TH1D *gGetClusterUEnergySingle = f1->Get("ShowerLKr/GetClusterUEnergySingle");
	TH1D *gGetClusterUTimeSingle = f1->Get("ShowerLKr/GetClusterUTimeSingle");*/

	TH1D *gClusterEnergyMult = f1->Get("ShowerLKr/GetClusterEnergy");
	TH2D *gclusterRecoMult = f1->Get("ShowerLKr/clusterReco");
	TH2D *gClusterPositionMult = f1->Get("ShowerLKr/ClusterPosition");

	TH1D *gGetClusterChi2RMSMult = f1->Get("ShowerLKr/GetClusterChi2RMS");
	TH1D *gGetCluster77EnergyMult = f1->Get("ShowerLKr/GetCluster77Energy");
	TH1D *gGetClusterKe3EnergyMult = f1->Get("ShowerLKr/GetClusterKe3Energy");
	TH1D *gGetClusterRMSXMult = f1->Get("ShowerLKr/GetClusterRMSX");
	TH1D *gGetClusterRMSYMult = f1->Get("ShowerLKr/GetClusterRMSY");
	TH1D *gGetClusterRMSMult = f1->Get("ShowerLKr/GetClusterRMS");
	TH1D *gGetClusterStatusMult = f1->Get("ShowerLKr/GetClusterStatus");
	TH1D *gGetClusterTimeMult = f1->Get("ShowerLKr/GetClusterTime");
	TH1D *gGetClusterTimeLateralCellMult = f1->Get("ShowerLKr/GetClusterTimeLateralCell");
	TH1D *gGetClusterUEnergyMult = f1->Get("ShowerLKr/GetClusterUEnergy");
	TH1D *gGetClusterUTimeMult = f1->Get("ShowerLKr/GetClusterUTime");
	TH1D *gGetClusterEnergyError = f1->Get("ShowerLKr/GetClusterEnergyError");
	TH1D *gGetClusterEnoise = f1->Get("ShowerLKr/GetClusterEnoise");
	TH1D *gGetClusterSeedEnergy = f1->Get("ShowerLKr/GetClusterSeedEnergy");

	gmultiplicity->SetName("gmultiplicity");
	gstartPos->SetName("gstartPos");
	gLKrPosition->SetName("gLKrPosition");
	/*gClusterEnergySingle->SetName("gClusterEnergySingle");
	gclusterRecoSingle->SetName("gClusterRecoSingle");
	gClusterPositionSingle->SetName("gClusterPositionSingle");

	gGetClusterChi2RMSSingle->SetName("gGetClusterChi2RMSSingle");
	gGetCluster77EnergySingle->SetName("gGetCluster77EnergySingle");
	gGetClusterKe3EnergySingle->SetName("gGetClusterKe3EnergySingle");
	gGetClusterRMSXSingle->SetName("gGetClusterRMSXSingle");
	gGetClusterRMSYSingle->SetName("gGetClusterRMSYSingle");
	gGetClusterRMSSingle->SetName("gGetClusterRMSSingle");
	gGetClusterStatusSingle->SetName("gGetClusterStatusSingle");
	gGetClusterTimeSingle->SetName("gGetClusterTimeSingle");
	gGetClusterTimeLateralCellSingle->SetName("gGetClusterTimeLateralCellSingle");
	gGetClusterUEnergySingle->SetName("gGetClusterUEnergySingle");
	gGetClusterUTimeSingle->SetName("gGetClusterUTimeSingle");*/

	gClusterEnergyMult->SetName("gClusterEnergy");
	gclusterRecoMult->SetName("gclusterReco");
	gClusterPositionMult->SetName("gClusterPosition");

	gGetClusterChi2RMSMult->SetName("gGetClusterChi2RMS");
	gGetCluster77EnergyMult->SetName("gGetCluster77Energy");
	gGetClusterKe3EnergyMult->SetName("gGetClusterKe3Energy");
	gGetClusterRMSXMult->SetName("gGetClusterRMSX");
	gGetClusterRMSYMult->SetName("gGetClusterRMSY");
	gGetClusterRMSMult->SetName("gGetClusterRMS");
	gGetClusterStatusMult->SetName("gGetClusterStatus");
	gGetClusterTimeMult->SetName("gGetClusterTime");
	gGetClusterTimeLateralCellMult->SetName("gGetClusterTimeLateralCell");
	gGetClusterUEnergyMult->SetName("gGetClusterUEnergy");
	gGetClusterUTimeMult->SetName("gGetClusterUTime");
	gGetClusterEnergyError->SetName("gGetClusterEnergyError");
	gGetClusterEnoise->SetName("gGetClusterEnoise");
	gGetClusterSeedEnergy->SetName("gGetClusterSeedEnergy");


	TFile *f2 = TFile::Open(f2Name, "READ");

	TH1D *emultiplicity = f2->Get("ShowerLKr/multiplicity");
	TH1D *estartPos = f2->Get("ShowerLKr/startPos");
	TH2D *eLKrPosition = f2->Get("ShowerLKr/LKrPosition");
	/*TH1D *eClusterEnergySingle = f2->Get("ShowerLKr/ClusterEnergySingle");
	TH2D *eclusterRecoSingle = f2->Get("ShowerLKr/clusterRecoSingle");
	TH2D *eClusterPositionSingle = f2->Get("ShowerLKr/ClusterPositionSingle");

	TH1D *eGetClusterChi2RMSSingle = f2->Get("ShowerLKr/GetClusterChi2RMSSingle");
	TH1D *eGetCluster77EnergySingle = f2->Get("ShowerLKr/GetCluster77EnergySingle");
	TH1D *eGetClusterKe3EnergySingle = f2->Get("ShowerLKr/GetClusterKe3EnergySingle");
	TH1D *eGetClusterRMSXSingle = f2->Get("ShowerLKr/GetClusterRMSXSingle");
	TH1D *eGetClusterRMSYSingle = f2->Get("ShowerLKr/GetClusterRMSYSingle");
	TH1D *eGetClusterRMSSingle = f2->Get("ShowerLKr/GetClusterRMSSingle");
	TH1D *eGetClusterStatusSingle = f2->Get("ShowerLKr/GetClusterStatusSingle");
	TH1D *eGetClusterTimeSingle = f2->Get("ShowerLKr/GetClusterTimeSingle");
	TH1D *eGetClusterTimeLateralCellSingle = f2->Get("ShowerLKr/GetClusterTimeLateralCellSingle");
	TH1D *eGetClusterUEnergySingle = f2->Get("ShowerLKr/GetClusterUEnergySingle");
	TH1D *eGetClusterUTimeSingle = f2->Get("ShowerLKr/GetClusterUTimeSingle");*/

	TH1D *eClusterEnergyMult = f2->Get("ShowerLKr/GetClusterEnergy");
	TH2D *eclusterRecoMult = f2->Get("ShowerLKr/clusterReco");
	TH2D *eClusterPositionMult = f2->Get("ShowerLKr/ClusterPosition");

	TH1D *eGetClusterChi2RMSMult = f2->Get("ShowerLKr/GetClusterChi2RMS");
	TH1D *eGetCluster77EnergyMult = f2->Get("ShowerLKr/GetCluster77Energy");
	TH1D *eGetClusterKe3EnergyMult = f2->Get("ShowerLKr/GetClusterKe3Energy");
	TH1D *eGetClusterRMSXMult = f2->Get("ShowerLKr/GetClusterRMSX");
	TH1D *eGetClusterRMSYMult = f2->Get("ShowerLKr/GetClusterRMSY");
	TH1D *eGetClusterRMSMult = f2->Get("ShowerLKr/GetClusterRMS");
	TH1D *eGetClusterStatusMult = f2->Get("ShowerLKr/GetClusterStatus");
	TH1D *eGetClusterTimeMult = f2->Get("ShowerLKr/GetClusterTime");
	TH1D *eGetClusterTimeLateralCellMult = f2->Get("ShowerLKr/GetClusterTimeLateralCell");
	TH1D *eGetClusterUEnergyMult = f2->Get("ShowerLKr/GetClusterUEnergy");
	TH1D *eGetClusterUTimeMult = f2->Get("ShowerLKr/GetClusterUTime");
	TH1D *eGetClusterEnergyError = f2->Get("ShowerLKr/GetClusterEnergyError");
	TH1D *eGetClusterEnoise = f2->Get("ShowerLKr/GetClusterEnoise");
	TH1D *eGetClusterSeedEnergy = f2->Get("ShowerLKr/GetClusterSeedEnergy");


	emultiplicity->SetName("emultiplicity");
	estartPos->SetName("estartPos");
	eLKrPosition->SetName("eLKrPosition");
	/*eClusterEnergySingle->SetName("eClusterEnergySingle");
	eclusterRecoSingle->SetName("eClusterRecoSingle");
	eClusterPositionSingle->SetName("eClusterPositionSingle");

	eGetClusterChi2RMSSingle->SetName("eGetClusterChi2RMSSingle");
	eGetCluster77EnergySingle->SetName("eGetCluster77EnergySingle");
	eGetClusterKe3EnergySingle->SetName("eGetClusterKe3EnergySingle");
	eGetClusterRMSXSingle->SetName("eGetClusterRMSXSingle");
	eGetClusterRMSYSingle->SetName("eGetClusterRMSYSingle");
	eGetClusterRMSSingle->SetName("eGetClusterRMSSingle");
	eGetClusterStatusSingle->SetName("eGetClusterStatusSingle");
	eGetClusterTimeSingle->SetName("eGetClusterTimeSingle");
	eGetClusterTimeLateralCellSingle->SetName("eGetClusterTimeLateralCellSingle");
	eGetClusterUEnergySingle->SetName("eGetClusterUEnergySingle");
	eGetClusterUTimeSingle->SetName("eGetClusterUTimeSingle");*/

	eClusterEnergyMult->SetName("eClusterEnergy");
	eclusterRecoMult->SetName("eclusterReco");
	eClusterPositionMult->SetName("eClusterPosition");

	eGetClusterChi2RMSMult->SetName("eGetClusterChi2RMS");
	eGetCluster77EnergyMult->SetName("eGetCluster77Energy");
	eGetClusterKe3EnergyMult->SetName("eGetClusterKe3Energy");
	eGetClusterRMSXMult->SetName("eGetClusterRMSX");
	eGetClusterRMSYMult->SetName("eGetClusterRMSY");
	eGetClusterRMSMult->SetName("eGetClusterRMS");
	eGetClusterStatusMult->SetName("eGetClusterStatus");
	eGetClusterTimeMult->SetName("eGetClusterTime");
	eGetClusterTimeLateralCellMult->SetName("eGetClusterTimeLateralCell");
	eGetClusterUEnergyMult->SetName("eGetClusterUEnergy");
	eGetClusterUTimeMult->SetName("eGetClusterUTime");
	eGetClusterEnergyError->SetName("eGetClusterEnergyError");
	eGetClusterEnoise->SetName("eGetClusterEnoise");
	eGetClusterSeedEnergy->SetName("eGetClusterSeedEnergy");


	draw(gmultiplicity, emultiplicity);
	draw(gstartPos,	estartPos);
	draw(gLKrPosition, eLKrPosition);
	/*draw(gClusterEnergySingle, eClusterEnergySingle);
	draw(gclusterRecoSingle, eclusterRecoSingle);
	draw(gClusterPositionSingle, eClusterPositionSingle);

	draw(gGetClusterChi2RMSSingle, eGetClusterChi2RMSSingle);
	draw(gGetCluster77EnergySingle, eGetCluster77EnergySingle);
	draw(gGetClusterKe3EnergySingle, eGetClusterKe3EnergySingle);
	draw(gGetClusterRMSXSingle, eGetClusterRMSXSingle);
	draw(gGetClusterRMSYSingle, eGetClusterRMSYSingle);
	draw(gGetClusterRMSSingle, eGetClusterRMSSingle);
	draw(gGetClusterStatusSingle, eGetClusterStatusSingle);
	draw(gGetClusterTimeSingle, eGetClusterTimeSingle);
	draw(gGetClusterTimeLateralCellSingle, eGetClusterTimeLateralCellSingle);
	draw(gGetClusterUEnergySingle, eGetClusterUEnergySingle);
	draw(gGetClusterUTimeSingle, eGetClusterUTimeSingle);*/

	draw(gClusterEnergyMult, eClusterEnergyMult);
	draw(gclusterRecoMult, eclusterRecoMult);
	draw(gClusterPositionMult, eClusterPositionMult);

	draw(gGetClusterChi2RMSMult, eGetClusterChi2RMSMult);
	draw(gGetCluster77EnergyMult, eGetCluster77EnergyMult);
	draw(gGetClusterKe3EnergyMult, eGetClusterKe3EnergyMult);
	draw(gGetClusterRMSXMult, eGetClusterRMSXMult);
	draw(gGetClusterRMSYMult, eGetClusterRMSYMult);
	draw(gGetClusterRMSMult, eGetClusterRMSMult);
	draw(gGetClusterStatusMult, eGetClusterStatusMult);
	draw(gGetClusterTimeMult, eGetClusterTimeMult);
	draw(gGetClusterTimeLateralCellMult, eGetClusterTimeLateralCellMult);
	draw(gGetClusterUEnergyMult, eGetClusterUEnergyMult);
	draw(gGetClusterUTimeMult, eGetClusterUTimeMult);
	draw(gGetClusterEnergyError, eGetClusterEnergyError);
	draw(gGetClusterEnoise, eGetClusterEnoise);
	draw(gGetClusterSeedEnergy, eGetClusterSeedEnergy);
}
