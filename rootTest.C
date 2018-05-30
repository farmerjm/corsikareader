

void rootTest() {
  std::cout << "hi" << std::endl;
  TFile histMuon("diff_0_on_coarse_muon.root", "READ");
  histMuon.ls();
  histMuon.GetObject("hist", hist);
  gStyle->SetPalette(51);
  TCanvas c1;
  hist->Draw("colz");
  c1.Print("testload.png");

}
