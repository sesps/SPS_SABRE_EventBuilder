/*SFPPlotter.h
 *Class for generating histogram files for SPS-SABRE data
 *Intended use case is generating a TChain of multiple analyzed files and making
 *histograms of the larger data set.
 *
 *Created Jan 2020 by GWM
 */

#include "EventBuilder.h"
#include "SFPPlotter.h"
#include <TSystem.h>

/*Generates storage and initializes pointers*/
SFPPlotter::SFPPlotter() {
  rootObj = new THashTable();
  rootObj->SetOwner(false);//THashTable doesnt own members; avoid double delete
  event_address = new ProcessedEvent();
  chain = new TChain("SPSTree");
  cutFlag = false;
  m_pb = NULL;
}

SFPPlotter::~SFPPlotter() {
  delete event_address;
}

/*2D histogram fill wrapper*/
void SFPPlotter::MyFill(const string& name, int binsx, double minx, double maxx, double valuex,
                                      int binsy, double miny, double maxy, double valuey) {
  TH2F *histo = (TH2F*) rootObj->FindObject(name.c_str());
  if(histo != NULL) {
    histo->Fill(valuex, valuey);
  } else {
    TH2F *h = new TH2F(name.c_str(), name.c_str(), binsx, minx, maxx, binsy, miny, maxy);
    h->Fill(valuex, valuey);
    rootObj->Add(h);
  }
}

/*1D histogram fill wrapper*/
void SFPPlotter::MyFill(const string& name, int binsx, double minx, double maxx, double valuex) {
  TH1F *histo = (TH1F*) rootObj->FindObject(name.c_str());
  if(histo != NULL) {
    histo->Fill(valuex);
  } else {
    TH1F *h = new TH1F(name.c_str(), name.c_str(), binsx, minx, maxx);
    h->Fill(valuex);
    rootObj->Add(h);
  }
}

void SFPPlotter::ApplyCutlist(const string& listname) {
  cutter.SetCuts(listname);
  cutFlag = true;
}

/*Makes histograms where only rejection is unset data*/
void SFPPlotter::MakeUncutHistograms(ProcessedEvent ev) {
  MyFill("x1NoCuts_bothplanes",600,-300,300,ev.x2);
  MyFill("x2NoCuts_bothplanes",600,-300,300,ev.x2);
  MyFill("xavgNoCuts_bothplanes",600,-300,300,ev.xavg);
  MyFill("xavgNoCuts_theta_bothplanes",600,-300,300,ev.xavg,100,0,TMath::Pi()/2.,ev.theta);
  
  MyFill("x1_delayBackRightE_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.delayBackRightE);
  MyFill("x2_delayBackRightE_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.delayBackRightE);
  MyFill("xavg_delayBackRightE_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.delayBackRightE);
  MyFill("x1_x2_NoCuts",600,-300,300,ev.x1,600,-300,300,ev.x2);

  Double_t delayBackAvgE = (ev.delayBackRightE+ev.delayBackLeftE)/2.0;
  MyFill("x1_delayBackAvgE_NoCuts",600,-300,300,ev.x1,512,0,4096,delayBackAvgE);
  MyFill("x2_delayBackAvgE_NoCuts",600,-300,300,ev.x2,512,0,4096,delayBackAvgE);
  MyFill("xavg_delayBackAvgE_NoCuts",600,-300,300,ev.xavg,512,0,4096,delayBackAvgE);
  Double_t delayFrontAvgE = (ev.delayFrontRightE+ev.delayFrontLeftE)/2.0;
  MyFill("x1_delayFrontAvgE_NoCuts",600,-300,300,ev.x1,512,0,4096,delayFrontAvgE);
  MyFill("x2_delayFrontAvgE_NoCuts",600,-300,300,ev.x2,512,0,4096,delayFrontAvgE);
  MyFill("xavg_delayFrontAvgE_NoCuts",600,-300,300,ev.xavg,512,0,4096,delayFrontAvgE);

  MyFill("scintLeft_anodeBack_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeBack);
  MyFill("scintLeft_anodeFront_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeFront);
  MyFill("scintLeft_cathode_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.cathode);

  MyFill("x1_scintLeft_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.scintLeft);
  MyFill("x2_scintLeft_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.scintLeft);
  MyFill("xavg_scintLeft_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.scintLeft);

  MyFill("x1_anodeBack_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.anodeBack);
  MyFill("x2_anodeBack_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.anodeBack);
  MyFill("xavg_anodeBack_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.anodeBack);

  MyFill("x1_anodeFront_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.anodeFront);
  MyFill("x2_anodeFront_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.anodeFront);
  MyFill("xavg_anodeFront_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.anodeFront);

  MyFill("x1_cathode_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.cathode);
  MyFill("x2_cathode_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.cathode);
  MyFill("xavg_cathode_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.cathode);

  /****Timing relative to back anode****/
  if(ev.anodeBackTime != -1 && ev.scintLeftTime != -1) {
    Double_t anodeRelFT = ev.anodeFrontTime - ev.anodeBackTime;
    Double_t delayRelFT = ev.delayFrontMaxTime - ev.anodeBackTime;
    Double_t delayRelBT = ev.delayBackMaxTime - ev.anodeBackTime;
    Double_t anodeRelBT = ev.anodeBackTime - ev.scintLeftTime;
    Double_t delayRelFT_toScint = ev.delayFrontMaxTime - ev.scintLeftTime;
    Double_t delayRelBT_toScint = ev.delayBackMaxTime - ev.scintLeftTime;
    MyFill("anodeRelFrontTime_NoCuts",1000,-3000,3500, anodeRelFT);
    MyFill("delayRelFrontTime_NoCuts",1000,-3000,-3500,delayRelFT);
    MyFill("delayRelBackTime_NoCuts",1000,-3000,-3500,delayRelBT);
    for(int i=0; i<5; i++) {
      if(ev.sabreRingE[i] != -1) {
        Double_t sabreRelRT = ev.sabreRingTime[i] - ev.anodeBackTime;
        Double_t sabreRelWT = ev.sabreWedgeTime[i] - ev.anodeBackTime;
        Double_t sabreRelRT_toScint = ev.sabreRingTime[i] - ev.scintLeftTime;
        Double_t sabreRelWT_toScint = ev.sabreWedgeTime[i] - ev.scintLeftTime;
        MyFill("xavg_sabrefcoinc_NoCuts",600,-300,300, ev.xavg);
        MyFill("sabreRelRingTime_NoCuts",1000,-3000,3500, sabreRelRT);
        MyFill("sabreRelWedgeTime_NoCuts",1000,-3000,3500, sabreRelWT);
        MyFill("sabreRelRingTime_toScint",1000,-3000,3500,sabreRelRT_toScint);
        MyFill("sabreRelWedgeTime_toScint",1000,-3000,3500,sabreRelWT_toScint);
        MyFill("sabreRelRTScint_sabreRelRTAnode",500,-3000,3500,sabreRelRT_toScint,500,-3000,3500,sabreRelRT);
        MyFill("sabreRelRTScint_sabreRingChannel",500,-3000,3500,sabreRelRT_toScint,144,0,144,ev.sabreRingChannel[i]);
        MyFill("sabreRelRTAnode_sabreRingChannel",500,-3000,3500,sabreRelRT,144,0,144,ev.sabreRingChannel[i]);
        MyFill("sabreRelWTScint_sabreWedgeChannel",500,-3000,3500,sabreRelWT_toScint,144,0,144,ev.sabreWedgeChannel[i]);
        MyFill("sabreRelRT_sabreRelWT",500,-3000,3500,sabreRelRT,500,-3000,3500,sabreRelWT);
        MyFill("sabreRelRT_sabreRelWT_scint",500,-3000,3500,sabreRelRT_toScint,500,-3000,3500,sabreRelWT_toScint);
        MyFill("sabreRelRTScint_anodeRelT",500,-3000,3500,sabreRelRT_toScint,500,-3000,3500,anodeRelBT);
      }
    }
    MyFill("anodeBackRelTime_toScint",1000,-3000,3500,anodeRelBT);
    MyFill("delayRelBackTime_toScint",1000,-3000,3500,delayRelBT_toScint);
    MyFill("delayRelFrontTime_toScint",1000,-3000,3500,delayRelFT_toScint);
  } else {
    MyFill("noscinttime_counter_NoCuts",2,0,1,1);
  }
 
  int count = 0;  
  for(int i=0; i<5; i++) { 
    if(ev.sabreRingE[i] != -1) { //Again, at this point front&back are required
      MyFill("sabreRingE_NoCuts",2000,0,20,ev.sabreRingE[i]);
      MyFill("sabreRingChannel_sabreRingE_NoCuts",144,0,144,ev.sabreRingChannel[i],200,0,20,ev.sabreRingE[i]);
      MyFill("sabreWedgeE_NoCuts",2000,0,20,ev.sabreWedgeE[i]);
      MyFill("sabreWedgeChannel_sabreWedgeE_NoCuts",144,0,144,ev.sabreWedgeChannel[i],200,0,20,ev.sabreWedgeE[i]);
    } else {
      count++;
    }
  }
  if(count == 80) {
    MyFill("xavg_bothplanes_sabreanticoinc_NoCuts",600,-300,300,ev.xavg);
  }
  if(ev.x1 != -1e6 && ev.x2 == -1e6) {
    MyFill("x1NoCuts_only1plane",600,-300,300,ev.x1);
  } else if(ev.x2 != -1e6 && ev.x1 == -1e6) {
    MyFill("x2NoCuts_only1plane",600,-300,300,ev.x2);
  } else if(ev.x1 == -1e6 && ev.x2 == -1e6) {
    MyFill("nopos_counter",2,0,1,1);
  }
}

/*Makes histograms with cuts & gates implemented*/
void SFPPlotter::MakeCutHistograms(ProcessedEvent ev) {
  if(!cutter.IsValid()) return;
  if(cutter.IsInside(&ev)) {
    MyFill("x1_bothplanes_Cut",600,-300,300,ev.x1);
    MyFill("x2_bothplanes_Cut",600,-300,300,ev.x2);
    MyFill("xavg_bothplanes_Cut",600,-300,300,ev.xavg);
    MyFill("x1_x2_Cut",600,-300,300,ev.x1, 600,-300,300,ev.x2);
    MyFill("xavg_theta_Cut_bothplanes",600,-300,300,ev.xavg,100,0,TMath::Pi()/2.,ev.theta);
    
    MyFill("x1_delayBackRightE_Cut",600,-300,300,ev.x1,512,0,4096,ev.delayBackRightE);
    MyFill("x2_delayBackRightE_Cut",600,-300,300,ev.x2,512,0,4096,ev.delayBackRightE);
    MyFill("xavg_delayBackRightE_Cut",600,-300,300,ev.xavg,512,0,4096,ev.delayBackRightE);

    Double_t delayBackAvgE = (ev.delayBackRightE+ev.delayBackLeftE)/2.0;
    MyFill("x1_delayBackAvgE_Cut",600,-300,300,ev.x1,512,0,4096,delayBackAvgE);
    MyFill("x2_delayBackAvgE_Cut",600,-300,300,ev.x2,512,0,4096,delayBackAvgE);
    MyFill("xavg_delayBackAvgE_Cut",600,-300,300,ev.xavg,512,0,4096,delayBackAvgE);
    Double_t delayFrontAvgE = (ev.delayFrontRightE+ev.delayFrontLeftE)/2.0;
    MyFill("x1_delayFrontAvgE_Cut",600,-300,300,ev.x1,512,0,4096,delayFrontAvgE);
    MyFill("x2_delayFrontAvgE_Cut",600,-300,300,ev.x2,512,0,4096,delayFrontAvgE);
    MyFill("xavg_delayFrontAvgE_Cut",600,-300,300,ev.xavg,512,0,4096,delayFrontAvgE);

    MyFill("scintLeft_anodeBack_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeBack);
    MyFill("scintLeft_anodeFront_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeFront);
    MyFill("scintLeft_cathode_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.cathode);

    MyFill("x1_scintLeft_Cut",600,-300,300,ev.x1,512,0,4096,ev.scintLeft);
    MyFill("x2_scintLeft_Cut",600,-300,300,ev.x2,512,0,4096,ev.scintLeft);
    MyFill("xavg_scintLeft_Cut",600,-300,300,ev.xavg,512,0,4096,ev.scintLeft);

    MyFill("x1_anodeBack_Cut",600,-300,300,ev.x1,512,0,4096,ev.anodeBack);
    MyFill("x2_anodeBack_Cut",600,-300,300,ev.x2,512,0,4096,ev.anodeBack);
    MyFill("xavg_anodeBack_Cut",600,-300,300,ev.xavg,512,0,4096,ev.anodeBack);
  
    MyFill("x1_anodeFront_Cut",600,-300,300,ev.x1,512,0,4096,ev.anodeFront);
    MyFill("x2_anodeFront_Cut",600,-300,300,ev.x2,512,0,4096,ev.anodeFront);
    MyFill("xavg_anodeFront_Cut",600,-300,300,ev.xavg,512,0,4096,ev.anodeFront);
  
    MyFill("x1_cathode_Cut",600,-300,300,ev.x1,512,0,4096,ev.cathode);
    MyFill("x2_cathode_Cut",600,-300,300,ev.x2,512,0,4096,ev.cathode);
    MyFill("xavg_cathode_Cut",600,-300,300,ev.xavg,512,0,4096,ev.cathode);

    /****Timing relative to back anode****/
    if(ev.anodeBackTime != -1 && ev.scintLeftTime != -1) {
      Double_t anodeRelFT = ev.anodeFrontTime - ev.anodeBackTime;
      Double_t anodeRelBT = ev.anodeBackTime - ev.anodeBackTime;
      Double_t anodeRelFT_toScint = ev.anodeFrontTime-ev.scintLeftTime;
      MyFill("anodeRelBackTime_Cut",1000,-3000,3500, anodeRelBT);
      MyFill("anodeRelFrontTime_Cut",1000,-3000,3500, anodeRelFT);
      MyFill("anodeRelTime_toScint_Cut",1000,-3000,3500,anodeRelFT_toScint);
      for(int i=0; i<5; i++) {
        if(ev.sabreRingE[i] != -1) {
          Double_t sabreRelRT = ev.sabreRingTime[i] - ev.anodeBackTime;
          Double_t sabreRelWT = ev.sabreWedgeTime[i] - ev.anodeBackTime;
          MyFill("sabreRelRingTime_Cut",1000,-3000,3500, sabreRelRT);
          MyFill("sabreRelWedgeTime_Cut",1000,-3000,3500, sabreRelWT);
        } 
      }
    } else {
      MyFill("noscinttime_counter_Cut",2,0,1,1);
    }
    
    int count = 0;
    for(int i=0; i<5; i++) {
      if(ev.sabreRingE[i] != -1) {
        MyFill("sabreRingE_Cut",2000,0,20,ev.sabreRingE[i]);
        MyFill("xavg_Cut_sabrefcoinc",600,-300,300,ev.xavg);
        MyFill("xavg_sabreRingE_Cut",600,-300,300,ev.xavg,200,0,20,ev.sabreRingE[i]);
        MyFill("sabreWedgeE_Cut",2000,0,20,ev.sabreWedgeE[i]);
        MyFill("xavg_sabreWedgeE_Cut",600,-300,300,ev.xavg,200,0,20,ev.sabreWedgeE[i]);
      } else {
        count++;
      }
    }
    if(count == 80) {
      MyFill("xavg_bothplanes_sabreanticoinc_Cut",600,-300,300,ev.xavg);
    }
  }
}

/*Runs a list of files given from a RunMusher/Collector class*/
void SFPPlotter::Run(vector<TString> files, const string& output) {
  Chain(files);
  chain->SetBranchAddress("event", &event_address);
  TFile *outfile = new TFile(output.c_str(), "RECREATE");

  long blentries = chain->GetEntries();
  if(m_pb) SetProgressBar(blentries);
  cout<<"Total number of events: "<<blentries<<endl;

  long count=0, flush=blentries*0.01, nflushes=0;
  if(flush == 0) flush = 1;

  for(long double i=0; i<chain->GetEntries(); i++) {
    count++;
    if(count == flush) {
      if(m_pb) {
        m_pb->Increment(count);
        gSystem->ProcessEvents();
        count = 0;
      } else {
        nflushes++;
        count=0;
        std::cout<<"\rPercent of data processed: "<<nflushes*10<<"%"<<std::flush;
      }
    }
    chain->GetEntry(i);
    MakeUncutHistograms(*event_address);
    if(cutFlag) MakeCutHistograms(*event_address);
  }
  cout<<endl;
  outfile->cd();
  rootObj->Write();
  if(cutFlag && cutter.IsValid()) {
    auto clist = cutter.GetCuts();
    for(unsigned int i=0; i<clist.size(); i++) {
      clist[i]->Write();
    }
  }
  delete rootObj;
  outfile->Close();
  delete outfile;
}

/*Link all files*/
void SFPPlotter::Chain(vector<TString> files) {
  for(unsigned int i=0; i<files.size(); i++) {
    chain->Add(files[i]); 
  }
}


void SFPPlotter::SetProgressBar(long total) {
  m_pb->SetMax(total);
  m_pb->SetMin(0);
  m_pb->SetPosition(0);
  gSystem->ProcessEvents();
}
