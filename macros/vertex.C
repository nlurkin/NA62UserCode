void printVector3(TVector3 a){
	cout << "(" << a.X() << "," << a.Y() << "," << a.Z() << ") Mag: " << a.Mag() << endl;
}

TVector3 GetIntersection(TVector3 pos1, TVector3 p1, TVector3 pos2, TVector3 p2, TVector3 &w){

	TVector3 d0 = pos1-pos2;
	double a = p1.Mag2();
	double b = p1*p2;
	double c = p2.Mag2();
	double d = p1*d0;
	double e = p2*d0;

	double s = (b*e-c*d)/(a*c-b*b);
	double t = (a*e-b*d)/(a*c-b*b);

	TVector3 vdist = d0 + (s*p1 - t*p2);
	w = vdist;
	
	/*cout << a << " " << b << " " << c << " " << d << " " << e << endl;
	cout << endl << s << " " << t << endl;
	printVector3(vdist);*/
	//dist = vdist.Mag();

	return pos1 + s*p1 - 0.5*vdist;
}


int vertex(){

	//generate x,y,z
	TRandom3 r;
	
	double p[3];
	double momentum[6];
	TVector3 pos1, pos2, mom1, mom2, vertex, w;
	TH1D *x = new TH1D("x", "x", 200, -2, 2);
	TH1D *y = new TH1D("y", "y", 200, -2, 2);
	TH1D *z = new TH1D("z", "z", 200, -2, 2);
	TH1D *dist = new TH1D("dist", "dist", 200, -2, 2);

	pos1.SetXYZ(1,1,1.5);
	pos2.SetXYZ(1,1,0.5);
	mom1.SetXYZ(1,0,0);
	mom2.SetXYZ(0,1,0);
	printVector3(GetIntersection(pos1, mom1, pos2, mom2, w));
	/*int total = 5000;
	for(int i=0; i<total; i++){
		printf("%i/%i\r", i, total);
		fflush(stdout);
		r.RndmArray(3, p);
		r.RndmArray(6, momentum);

		mom1.SetXYZ(momentum[1],momentum[2],momentum[3]);
		mom2.SetXYZ(momentum[4],momentum[5],momentum[6]);
		pos1.SetXYZ(p[0],p[1],p[2]-.5);
		pos2.SetXYZ(p[0],p[1],p[2]+.5);

		pos1 = pos1 + mom1.Unit()*(r.Rndm(0)-1);
		pos2 = pos2 + mom2.Unit()*(r.Rndm(0)-1);


		vertex = GetIntersection(pos1, mom1, pos2, mom2, w);

		x->Fill(vertex.X()-p[0]);
		y->Fill(vertex.Y()-p[1]);
		z->Fill(vertex.Z()-p[2]);
		dist->Fill(w.Mag());
	}
	TCanvas *c1 = new TCanvas("c1");

	c1->Divide(2,2);
	c1->cd(1);
	x->Draw();
	c1->cd(2);
	y->Draw();
	c1->cd(3);
	z->Draw();
	c1->cd(4);
	dist->Draw();

	c1->Draw();*/
}
