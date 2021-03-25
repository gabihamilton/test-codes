/*
- Implement an histogram vector
- Choose bins wisely
- Practice with Github
*/

void histvectorial() {
    TFile *input = TFile::Open("~/work/clas-data/data_Fe_skimmed.root");
    TTree *tree = (TTree*) input->Get("ntuple_data");

    TFile *output = TFile::Open("vectorhist.root","RECREATE");

    std::vector<TH1F*> histPhiPQ_D;
    std::vector<TH1F*> histPhiPQ_X;

    // Creating histograms
    int Nhist = 3;
    for (int j=0; j<Nhist; j++){
        float Q2min = j*5./Nhist;
        float Q2max = (j+1)*5./Nhist;

        auto histogramD_name = new TH1F( Form("histPhiPQ_D_%d",j) , 
            Form("Target D%d (%.3f < Q^{2} < %.3f)  ;#phi_{PQ}(degree);counts",j,Q2min,Q2max), 50, -180, 180);
        histPhiPQ_D.push_back(histogramD_name);

        auto histogramX_name = new TH1F( Form("histPhiPQ_X_%d",j) ,
            Form("Target X%d (%.3f < Q^{2} < %.3f)  ;#phi_{PQ}(degree);counts",j,Q2min,Q2max), 50, -180, 180);
        histPhiPQ_X.push_back(histogramX_name);
    }

    float PhiPQ=-99;
    float pid=-99;
    float TargType=-99;
    float Q2=-99;

    tree->SetBranchAddress("PhiPQ",&PhiPQ);
    tree->SetBranchAddress("pid",&pid);
    tree->SetBranchAddress("TargType",&TargType);
    tree->SetBranchAddress("Q2",&Q2);
    int Nentries = tree->GetEntries();

    for (int i=0; i<Nentries; i++) {
        tree->GetEntry(i);

        // if (i==25) break;
        if (pid != 211) continue;

        if (TargType == 1) { //Deuterium target
            // std::cout << "Pion in deuterium with:" << Q2 << std::endl;
            for (int n=1; n<Nhist+1; n++){
                // std::cout << "Pion in deuterium with cut!" << std::endl;
                if (Q2 < n*5./Nhist) {
                    histPhiPQ_D[n-1]->Fill(PhiPQ);
                    break;
                }
            }
        }

        if (TargType == 2) { //Iron (Fe) target
            for (int n=1; n<Nhist+1; n++){
                if (Q2 < n*5./Nhist) {
                    histPhiPQ_X[n-1]->Fill(PhiPQ);
                    break;
                }
            }
        }

    }

    // auto *canv = new TCanvas("canv", "Pion PhiPQ, Q2 cut");
    // canv->Divide(3,2);
    // for (int i=0; i<Nhist; i++){
    //     canv->cd(i);
    //     histPhiPQ_D[i]->Draw();
    //     canv->cd(i+3);
    //     histPhiPQ_X[i]->Draw();
    // }
    // canv->SaveAs("PhiPQ_Q2cut.png");

    output->Write();
    output->Close();
}