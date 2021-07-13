#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <THashTable.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include "include/DataStructs.h"
#include "include/CompassHit.h"
#include "include/GainMatcher.h"
#include "include/SabreMap.h"

R__LOAD_LIBRARY(lib/libGainMatcher.dylib)
R__LOAD_LIBRARY(lib/libSPSDict.dylib)
R__LOAD_LIBRARY(lib/libSabreMap.dylib)


void MyFill(THashTable *table, std::string name, int bins, double min, double max, double value) {
  TH1F *histo = (TH1F*) table->FindObject(name.c_str());
  if(histo == NULL) {
    TH1F* h = new TH1F(name.c_str(), name.c_str(), bins, min, max);
    h->Fill(value);
    table->Add(h);
  } else {
    histo->Fill(value);
  }
}

void MyFill(THashTable *table, std::string name, int binsx, double minx, double maxx, double valuex
                                               , int binsy, double miny, double maxy, double valuey) {
  TH2F *histo = (TH2F*) table->FindObject(name.c_str());
  if(histo == NULL) {
    TH2F* h = new TH2F(name.c_str(), name.c_str(), binsx, minx, maxx, binsy, miny, maxy);
    h->Fill(valuex, valuey);
    table->Add(h);
  } else {
    histo->Fill(valuex, valuey);
  }
}

void SabreSingles_raw() {

  GainMatcher gains;
  gains.SetFile("etc/March2020_gainmatch_2.0V_5486Am241.txt");

  std::unordered_map<int, sabrechan> smap;

  FillSabreMap("etc/ChannelMap_March2020_flipping.dat", smap);

  TFile *input = TFile::Open("/Volumes/LaCie/9BMarch2020/raw_root_singles/compass_run_37.root", "READ");
  TTree *intree = (TTree*) input->Get("Data");

  ULong64_t ts;
  UShort_t e, c, b, es;
  UInt_t f;
  intree->SetBranchAddress("Timestamp", &ts); 
  intree->SetBranchAddress("Channel", &c); 
  intree->SetBranchAddress("Board", &b); 
  intree->SetBranchAddress("Energy", &e); 
  intree->SetBranchAddress("EnergyShort", &es); 
  intree->SetBranchAddress("Flags", &f); 
  intree->SetMaxVirtualSize(4000000000);

  TFile *output = TFile::Open("/Volumes/LaCie/9BMarch2020/analyzed_singles/singles_run37.root","RECREATE");
  THashTable *hash = new THashTable();

  int gchan;
  DPPChannel hit;
  cout<<"Total Number of Entries: "<<intree->GetEntries()<<endl;
  for(ULong64_t i=0; i<intree->GetEntries(); i++) {
    intree->GetEntry(i);
    std::cout<<"\rNumber of entries processed: "<<i<<std::flush;

    hit.Timestamp = ts;
    hit.Energy = e;
    hit.Channel = c;
    hit.Board = b;
    hit.EnergyShort = es;
    hit.Flags = f;

    gchan = hit.Channel + hit.Board*16;

    sabrechan& schan = smap[gchan];

    if(schan.side_pos.first == "RING" && hit.Energy>20) {
      MyFill(hash, Form("detector%i_ring%i_energy",schan.detID,hit.Channel), 280, 0, 28, hit.Energy*gains.GetScaler(gchan));
    }
  }

  input->Close();
  hash->Write();
  output->Close();
}

void SabreSingles_analyzed() {

  std::unordered_map<int, sabrechan> smap;
  FillSabreMap("etc/ChannelMap_March2020_flipping.dat", smap);

  TFile *input = TFile::Open("/Volumes/LaCie/9BMarch2020/combined/downscaled_singles.root", "READ");
  TTree *intree = (TTree*) input->Get("SPSTree");

  ProcessedEvent *event_address = new ProcessedEvent();
  intree->SetBranchAddress("event", &event_address);
  intree->SetMaxVirtualSize(4000000000);
  
  TFile *output = TFile::Open("/Volumes/LaCie/9BMarch2020/analyzed_singles/downscale_histograms.root", "RECREATE"); 
  THashTable *hash = new THashTable();

  std::cout<<"Total entries: "<<intree->GetEntries()<<std::endl;
  for(ULong64_t i=0; i<intree->GetEntries(); i++) {
    std::cout<<"\rNumber of entries processed: "<<i<<std::flush;
    intree->GetEntry(i);
    
    for(int j=0; j<5; j++) {
      for(unsigned int k=0; k<(event_address->sabreArray[j].rings.size()); k++) {
        MyFill(hash, Form("detector%i_ring%i_energy",j,event_address->sabreArray[j].rings[k].Ch), 280,0,28, event_address->sabreArray[j].rings[k].Long);
      }
    }   
  }

  input->Close();
  hash->Write();
  output->Close();
  delete event_address;
}

void SabreSingles() {
  //SabreSingles_raw();
  SabreSingles_analyzed();
}

