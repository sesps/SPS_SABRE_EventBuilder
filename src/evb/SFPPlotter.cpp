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
SFPPlotter::SFPPlotter()
{
	rootObj = new THashTable();
	rootObj->SetOwner(false);//THashTable doesnt own members; avoid double delete
	event_address = new ProcessedEvent();
	chain = new TChain("SPSTree");
	m_pb = nullptr;
}

SFPPlotter::~SFPPlotter() 
{
	delete event_address;
}

/*2D histogram fill wrapper*/
void SFPPlotter::MyFill(THashTable* table, const std::string& name, int binsx, double minx, double maxx, double valuex,
						int binsy, double miny, double maxy, double valuey)
{
	TH2F *histo = (TH2F*) rootObj->FindObject(name.c_str());
	if(histo != nullptr) 
		histo->Fill(valuex, valuey);
	else
	{
		TH2F *h = new TH2F(name.c_str(), name.c_str(), binsx, minx, maxx, binsy, miny, maxy);
		h->Fill(valuex, valuey);
		rootObj->Add(h);
	}
}

/*1D histogram fill wrapper*/
void SFPPlotter::MyFill(THashTable* table, const std::string& name, int binsx, double minx, double maxx, double valuex)
{
	TH1F *histo = (TH1F*) rootObj->FindObject(name.c_str());
	if(histo != nullptr)
		histo->Fill(valuex);
	else 
	{
		TH1F *h = new TH1F(name.c_str(), name.c_str(), binsx, minx, maxx);
		h->Fill(valuex);
		rootObj->Add(h);
	}
}

/*Makes histograms where only rejection is unset data*/
void SFPPlotter::MakeUncutHistograms(const ProcessedEvent& ev, THashTable* table)
{
	MyFill(table,"x1NoCuts_bothplanes",600,-300,300,ev.x2);
	MyFill(table,"x2NoCuts_bothplanes",600,-300,300,ev.x2);
	MyFill(table,"xavgNoCuts_bothplanes",600,-300,300,ev.xavg);
	MyFill(table,"xavgNoCuts_theta_bothplanes",600,-300,300,ev.xavg,100,0,TMath::Pi()/2.,ev.theta);
	
	MyFill(table,"x1_delayBackRightE_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.delayBackRightE);
	MyFill(table,"x2_delayBackRightE_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.delayBackRightE);
	MyFill(table,"xavg_delayBackRightE_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.delayBackRightE);
	MyFill(table,"x1_x2_NoCuts",600,-300,300,ev.x1,600,-300,300,ev.x2);

	Double_t delayBackAvgE = (ev.delayBackRightE+ev.delayBackLeftE)/2.0;
	MyFill(table,"x1_delayBackAvgE_NoCuts",600,-300,300,ev.x1,512,0,4096,delayBackAvgE);
	MyFill(table,"x2_delayBackAvgE_NoCuts",600,-300,300,ev.x2,512,0,4096,delayBackAvgE);
	MyFill(table,"xavg_delayBackAvgE_NoCuts",600,-300,300,ev.xavg,512,0,4096,delayBackAvgE);
	Double_t delayFrontAvgE = (ev.delayFrontRightE+ev.delayFrontLeftE)/2.0;
	MyFill(table,"x1_delayFrontAvgE_NoCuts",600,-300,300,ev.x1,512,0,4096,delayFrontAvgE);
	MyFill(table,"x2_delayFrontAvgE_NoCuts",600,-300,300,ev.x2,512,0,4096,delayFrontAvgE);
	MyFill(table,"xavg_delayFrontAvgE_NoCuts",600,-300,300,ev.xavg,512,0,4096,delayFrontAvgE);

	MyFill(table,"scintLeft_anodeBack_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeBack);
	MyFill(table,"scintLeft_anodeFront_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeFront);
	MyFill(table,"scintLeft_cathode_NoCuts",512,0,4096,ev.scintLeft,512,0,4096,ev.cathode);

	MyFill(table,"x1_scintLeft_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.scintLeft);
	MyFill(table,"x2_scintLeft_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.scintLeft);
	MyFill(table,"xavg_scintLeft_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.scintLeft);

	MyFill(table,"x1_anodeBack_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.anodeBack);
	MyFill(table,"x2_anodeBack_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.anodeBack);
	MyFill(table,"xavg_anodeBack_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.anodeBack);

	MyFill(table,"x1_anodeFront_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.anodeFront);
	MyFill(table,"x2_anodeFront_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.anodeFront);
	MyFill(table,"xavg_anodeFront_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.anodeFront);

	MyFill(table,"x1_cathode_NoCuts",600,-300,300,ev.x1,512,0,4096,ev.cathode);
	MyFill(table,"x2_cathode_NoCuts",600,-300,300,ev.x2,512,0,4096,ev.cathode);
	MyFill(table,"xavg_cathode_NoCuts",600,-300,300,ev.xavg,512,0,4096,ev.cathode);

	/****Timing relative to back anode****/
	if(ev.anodeBackTime != -1 && ev.scintLeftTime != -1)
	{
		Double_t anodeRelFT = ev.anodeFrontTime - ev.anodeBackTime;
		Double_t delayRelFT = ev.delayFrontMaxTime - ev.anodeBackTime;
		Double_t delayRelBT = ev.delayBackMaxTime - ev.anodeBackTime;
		Double_t anodeRelBT = ev.anodeBackTime - ev.scintLeftTime;
		Double_t delayRelFT_toScint = ev.delayFrontMaxTime - ev.scintLeftTime;
		Double_t delayRelBT_toScint = ev.delayBackMaxTime - ev.scintLeftTime;
		MyFill(table,"anodeRelFrontTime_NoCuts",1000,-3000,3500, anodeRelFT);
		MyFill(table,"delayRelFrontTime_NoCuts",1000,-3000,-3500,delayRelFT);
		MyFill(table,"delayRelBackTime_NoCuts",1000,-3000,-3500,delayRelBT);
		for(int i=0; i<5; i++) 
		{
			if(ev.sabreRingE[i] != -1)
			{
				Double_t sabreRelRT = ev.sabreRingTime[i] - ev.anodeBackTime;
				Double_t sabreRelWT = ev.sabreWedgeTime[i] - ev.anodeBackTime;
				Double_t sabreRelRT_toScint = ev.sabreRingTime[i] - ev.scintLeftTime;
				Double_t sabreRelWT_toScint = ev.sabreWedgeTime[i] - ev.scintLeftTime;
				MyFill(table,"xavg_sabrefcoinc_NoCuts",600,-300,300, ev.xavg);
				MyFill(table,"sabreRelRingTime_NoCuts",1000,-3000,3500, sabreRelRT);
				MyFill(table,"sabreRelWedgeTime_NoCuts",1000,-3000,3500, sabreRelWT);
				MyFill(table,"sabreRelRingTime_toScint",1000,-3000,3500,sabreRelRT_toScint);
				MyFill(table,"sabreRelWedgeTime_toScint",1000,-3000,3500,sabreRelWT_toScint);
				MyFill(table,"sabreRelRTScint_sabreRelRTAnode",500,-3000,3500,sabreRelRT_toScint,500,-3000,3500,sabreRelRT);
				MyFill(table,"sabreRelRTScint_sabreRingChannel",500,-3000,3500,sabreRelRT_toScint,144,0,144,ev.sabreRingChannel[i]);
				MyFill(table,"sabreRelRTAnode_sabreRingChannel",500,-3000,3500,sabreRelRT,144,0,144,ev.sabreRingChannel[i]);
				MyFill(table,"sabreRelWTScint_sabreWedgeChannel",500,-3000,3500,sabreRelWT_toScint,144,0,144,ev.sabreWedgeChannel[i]);
				MyFill(table,"sabreRelRT_sabreRelWT",500,-3000,3500,sabreRelRT,500,-3000,3500,sabreRelWT);
				MyFill(table,"sabreRelRT_sabreRelWT_scint",500,-3000,3500,sabreRelRT_toScint,500,-3000,3500,sabreRelWT_toScint);
				MyFill(table,"sabreRelRTScint_anodeRelT",500,-3000,3500,sabreRelRT_toScint,500,-3000,3500,anodeRelBT);
			}
		}
		MyFill(table,"anodeBackRelTime_toScint",1000,-3000,3500,anodeRelBT);
		MyFill(table,"delayRelBackTime_toScint",1000,-3000,3500,delayRelBT_toScint);
		MyFill(table,"delayRelFrontTime_toScint",1000,-3000,3500,delayRelFT_toScint);
	} 
	else
		MyFill(table,"noscinttime_counter_NoCuts",2,0,1,1);
	
	for(int i=0; i<5; i++) 
	{ 
		if(ev.sabreRingE[i] != -1)  //Again, at this point front&back are required
		{
			MyFill(table,"sabreRingE_NoCuts",2000,0,20,ev.sabreRingE[i]);
			MyFill(table,"sabreRingChannel_sabreRingE_NoCuts",144,0,144,ev.sabreRingChannel[i],200,0,20,ev.sabreRingE[i]);
			MyFill(table,"sabreWedgeE_NoCuts",2000,0,20,ev.sabreWedgeE[i]);
			MyFill(table,"sabreWedgeChannel_sabreWedgeE_NoCuts",144,0,144,ev.sabreWedgeChannel[i],200,0,20,ev.sabreWedgeE[i]);
		}
	}
	
	if(ev.x1 != -1e6 && ev.x2 == -1e6)
		MyFill(table,"x1NoCuts_only1plane",600,-300,300,ev.x1);
	else if(ev.x2 != -1e6 && ev.x1 == -1e6)
		MyFill(table,"x2NoCuts_only1plane",600,-300,300,ev.x2);
	else if(ev.x1 == -1e6 && ev.x2 == -1e6)
		MyFill(table,"nopos_counter",2,0,1,1);
}

/*Makes histograms with cuts & gates implemented*/
void SFPPlotter::MakeCutHistograms(const ProcessedEvent& ev, THashTable* table) 
{
	if(!cutter.IsInside(&ev)) 
		return;

	MyFill(table,"x1_bothplanes_Cut",600,-300,300,ev.x1);
	MyFill(table,"x2_bothplanes_Cut",600,-300,300,ev.x2);
	MyFill(table,"xavg_bothplanes_Cut",600,-300,300,ev.xavg);
	MyFill(table,"x1_x2_Cut",600,-300,300,ev.x1, 600,-300,300,ev.x2);
	MyFill(table,"xavg_theta_Cut_bothplanes",600,-300,300,ev.xavg,100,0,TMath::Pi()/2.,ev.theta);
	
	MyFill(table,"x1_delayBackRightE_Cut",600,-300,300,ev.x1,512,0,4096,ev.delayBackRightE);
	MyFill(table,"x2_delayBackRightE_Cut",600,-300,300,ev.x2,512,0,4096,ev.delayBackRightE);
	MyFill(table,"xavg_delayBackRightE_Cut",600,-300,300,ev.xavg,512,0,4096,ev.delayBackRightE);

	Double_t delayBackAvgE = (ev.delayBackRightE+ev.delayBackLeftE)/2.0;
	MyFill(table,"x1_delayBackAvgE_Cut",600,-300,300,ev.x1,512,0,4096,delayBackAvgE);
	MyFill(table,"x2_delayBackAvgE_Cut",600,-300,300,ev.x2,512,0,4096,delayBackAvgE);
	MyFill(table,"xavg_delayBackAvgE_Cut",600,-300,300,ev.xavg,512,0,4096,delayBackAvgE);
	Double_t delayFrontAvgE = (ev.delayFrontRightE+ev.delayFrontLeftE)/2.0;
	MyFill(table,"x1_delayFrontAvgE_Cut",600,-300,300,ev.x1,512,0,4096,delayFrontAvgE);
	MyFill(table,"x2_delayFrontAvgE_Cut",600,-300,300,ev.x2,512,0,4096,delayFrontAvgE);
	MyFill(table,"xavg_delayFrontAvgE_Cut",600,-300,300,ev.xavg,512,0,4096,delayFrontAvgE);

	MyFill(table,"scintLeft_anodeBack_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeBack);
	MyFill(table,"scintLeft_anodeFront_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.anodeFront);
	MyFill(table,"scintLeft_cathode_Cut",512,0,4096,ev.scintLeft,512,0,4096,ev.cathode);

	MyFill(table,"x1_scintLeft_Cut",600,-300,300,ev.x1,512,0,4096,ev.scintLeft);
	MyFill(table,"x2_scintLeft_Cut",600,-300,300,ev.x2,512,0,4096,ev.scintLeft);
	MyFill(table,"xavg_scintLeft_Cut",600,-300,300,ev.xavg,512,0,4096,ev.scintLeft);

	MyFill(table,"x1_anodeBack_Cut",600,-300,300,ev.x1,512,0,4096,ev.anodeBack);
	MyFill(table,"x2_anodeBack_Cut",600,-300,300,ev.x2,512,0,4096,ev.anodeBack);
	MyFill(table,"xavg_anodeBack_Cut",600,-300,300,ev.xavg,512,0,4096,ev.anodeBack);
	
	MyFill(table,"x1_anodeFront_Cut",600,-300,300,ev.x1,512,0,4096,ev.anodeFront);
	MyFill(table,"x2_anodeFront_Cut",600,-300,300,ev.x2,512,0,4096,ev.anodeFront);
	MyFill(table,"xavg_anodeFront_Cut",600,-300,300,ev.xavg,512,0,4096,ev.anodeFront);
	
	MyFill(table,"x1_cathode_Cut",600,-300,300,ev.x1,512,0,4096,ev.cathode);
	MyFill(table,"x2_cathode_Cut",600,-300,300,ev.x2,512,0,4096,ev.cathode);
	MyFill(table,"xavg_cathode_Cut",600,-300,300,ev.xavg,512,0,4096,ev.cathode);

	/****Timing relative to back anode****/
	if(ev.anodeBackTime != -1 && ev.scintLeftTime != -1) 
	{
		Double_t anodeRelFT = ev.anodeFrontTime - ev.anodeBackTime;
		Double_t anodeRelBT = ev.anodeBackTime - ev.anodeBackTime;
		Double_t anodeRelFT_toScint = ev.anodeFrontTime-ev.scintLeftTime;
		MyFill(table,"anodeRelBackTime_Cut",1000,-3000,3500, anodeRelBT);
		MyFill(table,"anodeRelFrontTime_Cut",1000,-3000,3500, anodeRelFT);
		MyFill(table,"anodeRelTime_toScint_Cut",1000,-3000,3500,anodeRelFT_toScint);
		for(int i=0; i<5; i++) 
		{
			if(ev.sabreRingE[i] != -1) 
			{
				Double_t sabreRelRT = ev.sabreRingTime[i] - ev.anodeBackTime;
				Double_t sabreRelWT = ev.sabreWedgeTime[i] - ev.anodeBackTime;
				MyFill(table,"sabreRelRingTime_Cut",1000,-3000,3500, sabreRelRT);
				MyFill(table,"sabreRelWedgeTime_Cut",1000,-3000,3500, sabreRelWT);
			} 
		}
	} 
	else
	{
		MyFill(table,"noscinttime_counter_Cut",2,0,1,1);
	}
	
	for(int i=0; i<5; i++) 
	{
		if(ev.sabreRingE[i] != -1)
		{
			MyFill(table,"sabreRingE_Cut",2000,0,20,ev.sabreRingE[i]);
			MyFill(table,"xavg_Cut_sabrefcoinc",600,-300,300,ev.xavg);
			MyFill(table,"xavg_sabreRingE_Cut",600,-300,300,ev.xavg,200,0,20,ev.sabreRingE[i]);
			MyFill(table,"sabreWedgeE_Cut",2000,0,20,ev.sabreWedgeE[i]);
			MyFill(table,"xavg_sabreWedgeE_Cut",600,-300,300,ev.xavg,200,0,20,ev.sabreWedgeE[i]);
		}
	}
}

/*Runs a list of files given from a RunCollector class*/
void SFPPlotter::Run(const std::vector<std::string>& files, const std::string& output)
{
	TFile *outfile = TFile::Open(output.c_str(), "RECREATE");
	TChain* chain = new TChain("SPSTree");
	for(unsigned int i=0; i<files.size(); i++)
		chain->Add(files[i].c_str()); 
	chain->SetBranchAddress("event", &event_address);
	THastTable* table = new THashTable();

	long blentries = chain->GetEntries();
	if(m_pb) 
		SetProgressBar(blentries);
	cout<<"Total number of events: "<<blentries<<endl;

	long count=0, flush_val=blentries*0.01, flush_count=0;

	for(long i=0; i<chain->GetEntries(); i++) 
	{
		count++;
		if(count == flush_val)
		{
			if(m_pb) {
				m_pb->Increment(count);
				gSystem->ProcessEvents();
				count = 0;
			} else {
				flush_count++;
				count=0;
				std::cout<<"\rPercent of data processed: "<<flush_count*10<<"%"<<std::flush;
			}
		}
		chain->GetEntry(i);
		MakeUncutHistograms(*event_address);
		if(cutter.IsValid()) MakeCutHistograms(*event_address);
	}
	cout<<endl;
	outfile->cd();
	rootObj->Write();
	if(cutter.IsValid()) 
	{
		auto clist = cutter.GetCuts();
		for(unsigned int i=0; i<clist.size(); i++) 
		  clist[i]->Write();
	}
	delete rootObj;
	outfile->Close();
	delete outfile;
}


void SFPPlotter::SetProgressBar(long total) 
{
	m_pb->SetMax(total);
	m_pb->SetMin(0);
	m_pb->SetPosition(0);
	gSystem->ProcessEvents();
}
