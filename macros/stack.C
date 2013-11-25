/*
 * stack
 *
 *  Created on: 8 Oct 2013
 *      Author: ncl
 */




stack(TString f1Name){
	TFile *f1 = TFile::Open(f1Name);

	TH1D *W, *W_geom;
	TH1D *W_HW, *W_HWPi;
	W = (TH1D*)f1->Get("piggg_gen/W");
	W_geom = (TH1D*)f1->Get("piggg_gen/W_geom");

	W_HW = (TH1D*)f1->Get("piggg_HW/W_HW");
	W_HWPi = (TH1D*)f1->Get("piggg_HW/W_HWPi");

	W->Draw();
	W_geom->Draw("SAME");
	W_HW->Draw("SAME");
	W_HWPi->Draw("SAME");
}
