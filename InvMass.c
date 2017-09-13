
void InvMass() {
//Load data file
  TFile *file = TFile::Open("/cms2r0/babymaker/babies/2017_08_14/data/merged_miniso_data_DY/TotalMergedSkim_miniso_data_DY.root");
//Initialize tree & variables
  TTree *tree = (TTree*)file->Get("tree00");
  std::vector<float> *mu_pt(0); TBranch *bmu_pt = nullptr;
  std::vector<float> *mu_eta(0); TBranch *bmu_eta = nullptr; 
  std::vector<float> *mu_phi(0); TBranch *bmu_phi = nullptr;
  std::vector<float> *mu_miniso(0); TBranch *bmu_miniso = nullptr;
  std::vector<bool> *mu_sigid(0); TBranch *bmu_sigid = nullptr;
  std::vector<int> *mu_sign(0); TBranch *bmu_sign = nullptr;
//Use only needed Branches
  tree->SetBranchStatus("*",0);
  tree->SetBranchStatus("mus_pt",1);
  tree->SetBranchStatus("mus_eta",1); 
  tree->SetBranchStatus("mus_phi",1);
  tree->SetBranchStatus("mus_miniso",1);
  tree->SetBranchStatus("mus_sigid",1);
  tree->SetBranchStatus("mus_charge",1);
//Set Branch Addresses
  tree->SetBranchAddress("mus_pt",&mu_pt, &bmu_pt);
  tree->SetBranchAddress("mus_eta",&mu_eta, &bmu_eta);
  tree->SetBranchAddress("mus_phi",&mu_phi, &bmu_phi);
  tree->SetBranchAddress("mus_miniso",&mu_miniso, &bmu_miniso);
  tree->SetBranchAddress("mus_sigid",&mu_sigid, &bmu_sigid);
  tree->SetBranchAddress("mus_charge",&mu_sign, &bmu_sign);
//Define Histograms
  TH1D* invmassmu = new TH1D("invmassmu","Dimuon Invariant Mass",40000,0,200);
//Lepton variables
  int lep1(0), lep2(0);
  int lep1sign(0);
  double dilm(0);
  TLorentzVector l1, l2, dil;
//Loop through events (and mus/els)
  for(int i=0; i < tree->GetEntries(); i++) { 
	tree->GetEntry(i);
	lep1 = -1;
	lep2 = -1;
	lep1sign = 0;
	l1.SetPxPyPzE(0,0,0,1);
	l2.SetPxPyPzE(0,0,0,1);
	dilm = 0;
	if(mu_pt->size() > 0) {
	//Loop through muons
	for(int j=0; j < mu_pt->size(); j++) {
		if(lep1 < 0) {
			if(mu_pt->at(j) > 20 && fabs(mu_eta->at(j)) < 2.4 && mu_sigid->at(j) && mu_miniso->at(j) < .2) { //Checks for good muon
				lep1 = j;
				lep1sign = mu_sign->at(j); } }
		//If already found good muon, look for another with opposite sign
		if(lep1 >= 0 && lep2 < 0 && (mu_pt->at(j) > 20 && fabs(mu_eta->at(j)) < 2.4 && mu_sigid->at(j) && mu_miniso->at(j) < .2 && mu_sign->at(j) != lep1sign)) lep2 = j;
		if(lep2 >= 0) continue; 
		}}
	//If two good muons were found, calculate their invariant mass and fill the histogram
	if(lep1 >= 0 && lep2 >= 0) {
		l1.SetPtEtaPhiM(mu_pt->at(lep1),mu_eta->at(lep1),mu_phi->at(lep1), .1057);
		l2.SetPtEtaPhiM(mu_pt->at(lep2),mu_eta->at(lep2),mu_phi->at(lep2), .1057);
		dil = l1+l2;
		dilm = dil.M();
		invmassmu->Fill(dilm);
	}
  }
//Write final histograms to file
  TFile out("invmass_DY.root", "RECREATE");
//Invariant mass
  invmassmu->Write();
  out.Close();	
  return;
}
