#include <TROOT.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TCutG.h>
#include <TTree.h>
#include <iostream>
#include "include/DataStructs.h"
R__LOAD_LIBRARY(lib/libSPSDict.dylib)

void Plotter() {
  TFile* input = TFile::Open("/Volumes/LaCie/9BMarch2020/combined/main_data_carbonCal.root","READ");
  TTree* intree = (TTree*) input->Get("SPSTree");

  TFile *cutfile = TFile::Open("/Volumes/LaCie/9BMarch2020/maindata_12C_extraTheta.root","READ");
  TCutG *cut = (TCutG*) cutfile->Get("CUTG");
  cut->SetName("extraTheta");
  cut->SetLineColor(kRed);
  cut->SetVarX("xavg");
  cut->SetVarY("theta");

  ProcessedEvent *event_address = new ProcessedEvent();
  intree->SetBranchAddress("event", &event_address);

  TFile *outfile = TFile::Open("/Volumes/LaCie/9BMarch2020/9b_histograms/carbonCal_thetaGating.root","RECREATE");
  TH2F *xtheta = new TH2F("xtheta","xtheta",600,-300,300,300,0,1.75);
  TH2F *xde = new TH2F("xde","xde",600,-300,300,512,0,4096);
  TH2F *xde_thetaCut = new TH2F("xde_thetaCut","xde_thetaCut",600,-300,300,512,0,4096);
  xde_thetaCut->SetLineColor(kRed);
  xde_thetaCut->SetMarkerColor(kRed);
  TH2F *xde_antithetaCut = new TH2F("xde_antithetaCut","xde_antithetaCut",600,-300,300,512,0,4096);
  xde_antithetaCut->SetLineColor(kBlack);
  xde_antithetaCut->SetMarkerColor(kBlue);
  TH1F *anodeRelTime = new TH1F("anodeRelTime","anodeRelTime",1000,-1000,1000);
  TH1F *anodeRelTime_thetaCut = new TH1F("anodeRelTime_thetaCut","anodeRelTime_thetacut",1000,-1000,1000);
  anodeRelTime_thetaCut->SetLineColor(kRed);
  TH1F *anodeRelTime_antithetaCut = new TH1F("anodeRelTime_antithetaCut","anodeRelTime_antithetacut",1000,-1000,1000);
  anodeRelTime_antithetaCut->SetLineColor(kBlack);

  std::cout<<"Total number of Entries: "<<intree->GetEntries()<<std::endl;
  for(ULong64_t i=0; i<intree->GetEntries(); i++) {
    std::cout<<"\rNumber of events processed: "<<i<<std::flush;
    intree->GetEntry(i);
    double anodeRT = event_address->anodeBackTime - event_address->scintLeftTime;

    if(anodeRT != 0) {
      xtheta->Fill(event_address->xavg, event_address->theta);
      xde->Fill(event_address->xavg, event_address->delayBackRightE);
      anodeRelTime->Fill(anodeRT);
      if(cut->IsInside(event_address->xavg, event_address->theta)) {
        xde_thetaCut->Fill(event_address->xavg, event_address->delayBackRightE);
        anodeRelTime_thetaCut->Fill(anodeRT);
      } else {
        xde_antithetaCut->Fill(event_address->xavg, event_address->delayBackRightE);
        anodeRelTime_antithetaCut->Fill(anodeRT);
      }
    }
  }

  TCanvas *c1 = new TCanvas();
  c1->Divide(2,2);
  c1->cd(1);
  xde->Draw("colz");
  c1->cd(2);
  xde_antithetaCut->Draw();
  xde_thetaCut->Draw("same");
  c1->BuildLegend();
  c1->cd(3);
  anodeRelTime->Draw();
  anodeRelTime_antithetaCut->Draw("same");
  anodeRelTime_thetaCut->Draw("same");
  c1->BuildLegend();
  c1->cd(4);
  xtheta->Draw();
  cut->Draw("same");
  
  input->Close();
  cutfile->Close();
  
  outfile->cd();
  xtheta->Write();
  xde->Write();
  xde_thetaCut->Write();
  xde_antithetaCut->Write();
  anodeRelTime->Write();
  anodeRelTime_thetaCut->Write();
  anodeRelTime_antithetaCut->Write();
  c1->Write();

  c1->Close();
  outfile->Close();

 
}
