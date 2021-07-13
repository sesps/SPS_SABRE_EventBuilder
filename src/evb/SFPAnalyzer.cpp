/*SFPAnalyzer.cpp
 *Class designed to analyze coincidence events. Currently only implemented for focal plane
 *data. Additional changes for SABRE would include this file and the sructure ProcessedEvent
 *in DataStructs.h. Based on code written by S. Balak, K. Macon, and E. Good.
 *
 *Gordon M. Oct. 2019
 *
 *Refurbished and updated Jan 2020 by GWM. Now uses both focal plane and SABRE data
 */
#include "EventBuilder.h"
#include "SFPAnalyzer.h"

using namespace std;

/*Constructor takes in kinematic parameters for generating focal plane weights*/
SFPAnalyzer::SFPAnalyzer(int zt, int at, int zp, int ap, int ze, int ae, double ep,
                         double angle, double b) {
  zfp = Delta_Z(zt, at, zp, ap, ze, ae, ep, angle, b);
  event_address = new CoincEvent();
  rootObj = new THashTable();
  GetWeights();
}

SFPAnalyzer::~SFPAnalyzer() {
  rootObj->Clear();
  delete rootObj;
  delete event_address;
}

void SFPAnalyzer::Reset() {
  pevent = blank; //set output back to blank
}

/*Use functions from FP_kinematics to calculate weights for xavg
 *While this seems kind of funny, it is mathematically equivalent to making a line
 *from the two focal plane points and finding the intersection with 
 *the kinematic focal plane
 */
void SFPAnalyzer::GetWeights() {
  w1 = (Wire_Dist()/2.0-zfp)/Wire_Dist();
  w2 = 1.0-w1;
  cout<<"w1: "<<w1<<" w2: "<<w2<<endl;
}

/*2D histogram fill wrapper for use with THashTable (faster)*/
void SFPAnalyzer::MyFill(const string& name, int binsx, double minx, double maxx, double valuex,
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

/*1D histogram fill wrapper for use with THashTable (faster)*/
void SFPAnalyzer::MyFill(const string& name, int binsx, double minx, double maxx, double valuex) {
  TH1F *histo = (TH1F*) rootObj->FindObject(name.c_str());
  if(histo != NULL) {
    histo->Fill(valuex);
  } else {
    TH1F *h = new TH1F(name.c_str(), name.c_str(), binsx, minx, maxx);
    h->Fill(valuex);
    rootObj->Add(h);
  }
}

void SFPAnalyzer::AnalyzeEvent(CoincEvent& event) {
  Reset();
  if(!event.focalPlane.anodeF.empty()) {
    pevent.anodeFront = event.focalPlane.anodeF[0].Long;
    pevent.anodeFrontTime = event.focalPlane.anodeF[0].Time;
  }
  if(!event.focalPlane.anodeB.empty()) {
    pevent.anodeBack = event.focalPlane.anodeB[0].Long;
    pevent.anodeBackTime = event.focalPlane.anodeB[0].Time;
  }
  if(!event.focalPlane.scintL.empty()) {
    pevent.scintLeft = event.focalPlane.scintL[0].Long;
    pevent.scintLeftShort = event.focalPlane.scintL[0].Short;
    pevent.scintLeftTime = event.focalPlane.scintL[0].Time;
  }
  if(!event.focalPlane.scintR.empty()) {
    pevent.scintRight = event.focalPlane.scintR[0].Long;
    pevent.scintRightShort = event.focalPlane.scintR[0].Short;
    pevent.scintRightTime = event.focalPlane.scintR[0].Time;
  }
  if(!event.focalPlane.cathode.empty()) {
    pevent.cathode = event.focalPlane.cathode[0].Long;
    pevent.cathodeTime = event.focalPlane.cathode[0].Time;
  }
  if(!event.focalPlane.monitor.empty()) {
    pevent.monitorE = event.focalPlane.monitor[0].Long;
    pevent.monitorShort = event.focalPlane.monitor[0].Short;
    pevent.monitorTime = event.focalPlane.monitor[0].Time;
  }

  /*Delay lines and all that*/
  if(!event.focalPlane.delayFR.empty()) {
    pevent.delayFrontRightE = event.focalPlane.delayFR[0].Long;
    pevent.delayFrontRightTime = event.focalPlane.delayFR[0].Time;
    pevent.delayFrontRightShort = event.focalPlane.delayFR[0].Short;
  }
  if(!event.focalPlane.delayFL.empty()) {
    pevent.delayFrontLeftE = event.focalPlane.delayFL[0].Long;
    pevent.delayFrontLeftTime = event.focalPlane.delayFL[0].Time;
    pevent.delayFrontLeftShort = event.focalPlane.delayFL[0].Short;
  }
  if(!event.focalPlane.delayBR.empty()) {
    pevent.delayBackRightE = event.focalPlane.delayBR[0].Long;
    pevent.delayBackRightTime = event.focalPlane.delayBR[0].Time;
    pevent.delayBackRightShort = event.focalPlane.delayBR[0].Short;
  }
  if(!event.focalPlane.delayBL.empty()) {
    pevent.delayBackLeftE = event.focalPlane.delayBL[0].Long;
    pevent.delayBackLeftTime = event.focalPlane.delayBL[0].Time;
    pevent.delayBackLeftShort = event.focalPlane.delayBL[0].Short;
  }
  if(!event.focalPlane.delayFL.empty() && !event.focalPlane.delayFR.empty()) { 
    pevent.fp1_tdiff = (event.focalPlane.delayFL[0].Time-event.focalPlane.delayFR[0].Time)*0.5;
    pevent.fp1_tsum = (event.focalPlane.delayFL[0].Time+event.focalPlane.delayFR[0].Time);
    pevent.fp1_tcheck = (pevent.fp1_tsum)/2.0-pevent.anodeFrontTime;
    pevent.delayFrontMaxTime = max(event.focalPlane.delayFL[0].Time, event.focalPlane.delayFR[0].Time);
    pevent.x1 = pevent.fp1_tdiff*1.0/1.98; //position from time, based on total delay
    //pevent.x1 = 0.52*pevent.fp1_tdiff - 0.128; //position from time, based on delay chips
    MyFill("x1",1200,-300,300,pevent.x1);
    MyFill("x1 vs anodeBack",600,-300,300,pevent.x1,512,0,4096,pevent.anodeBack);
  }
  if(!event.focalPlane.delayBL.empty() && !event.focalPlane.delayBR.empty()) {
    pevent.fp2_tdiff = (event.focalPlane.delayBL[0].Time-event.focalPlane.delayBR[0].Time)*0.5;
    pevent.fp2_tsum = (event.focalPlane.delayBL[0].Time+event.focalPlane.delayBR[0].Time);
    pevent.fp2_tcheck = (pevent.fp2_tsum)/2.0-pevent.anodeBackTime;
    pevent.delayBackMaxTime = max(event.focalPlane.delayBL[0].Time, event.focalPlane.delayBR[0].Time);
    pevent.x2 = pevent.fp2_tdiff*1.0/2.10; //position from time, based on total delay
    //pevent.x2 = 0.48*pevent.fp2_tdiff - 2.365; //position from time, based on delay chips
    MyFill("x2",1200,-300,300,pevent.x2);
    MyFill("x2 vs anodeBack",600,-300,300,pevent.x2,512,0,4096,pevent.anodeBack);
  }
  /*SABRE data*/
  for(int j=0; j<5; j++) {
    if(!event.sabreArray[j].rings.empty()) {
      pevent.sabreRingE[j] = event.sabreArray[j].rings[0].Long;
      pevent.sabreRingChannel[j] = event.sabreArray[j].rings[0].Ch;
      pevent.sabreRingTime[j] = event.sabreArray[j].rings[0].Time;
    }
    if(!event.sabreArray[j].wedges.empty()) {
      pevent.sabreWedgeE[j] = event.sabreArray[j].wedges[0].Long;
      pevent.sabreWedgeChannel[j] = event.sabreArray[j].wedges[0].Ch;
      pevent.sabreWedgeTime[j] = event.sabreArray[j].wedges[0].Time;
    }
    /*Aaaand passes on all of the rest. 4/24/20 GWM*/
    pevent.sabreArray[j] = event.sabreArray[j];
  }

  /*Make some histograms and xavg*/
  MyFill("anodeBack vs scintLeft",512,0,4096,pevent.scintLeft,512,0,4096,pevent.anodeBack);
  if(pevent.x1 != -1e6 && pevent.x2 != -1e6) {
    pevent.xavg = pevent.x1*w1+pevent.x2*w2;
    MyFill("xavg",1200,-300,300,pevent.xavg);
    if((pevent.x2-pevent.x1) > 0) {
      pevent.theta = atan((pevent.x2-pevent.x1)/36.0);
    } else if((pevent.x2-pevent.x1) < 0) {
      pevent.theta = TMath::Pi() + atan((pevent.x2-pevent.x1)/36.0);
    } else {
      pevent.theta = TMath::Pi()/2.0;
    }
    MyFill("xavg vs theta",600,-300,300,pevent.xavg,314,0,3.14,pevent.theta);
    MyFill("x1 vs x2",600,-300,300,pevent.x1,600,-300,300,pevent.x2);
  }
  if(pevent.anodeFrontTime != -1 && pevent.scintRightTime != -1) {
    pevent.fp1_y = pevent.anodeFrontTime-pevent.scintRightTime;
  }
  if(pevent.anodeBackTime != -1 && pevent.scintRightTime != -1) {
    pevent.fp2_y = pevent.anodeBackTime-pevent.scintRightTime;
  }
}

ProcessedEvent SFPAnalyzer::GetProcessedEvent(CoincEvent& event) {
  AnalyzeEvent(event);
  return pevent;
}
