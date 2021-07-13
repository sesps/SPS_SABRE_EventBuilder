/*DataStructs.h
 *Data structures for analysis. To be implemented as a dictionary for ROOT in LinkDef
 *Based on: FocalPlane_SABRE.h
 *Gordon M. Oct. 2019
 */
#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

using namespace std;

struct DPPChannel {
  Double_t Timestamp;
  Int_t Channel, Board, Energy, EnergyShort;
  Int_t Flags;
};

struct DetectorHit {
  Double_t Long=-1, Short=-1, Time=-1;
  Int_t Ch=-1;
};

struct SabreDetector {
  vector<DetectorHit> rings;
  vector<DetectorHit> wedges;
};

struct FPDetector {
  vector<DetectorHit> delayFL, delayFR, delayBL, delayBR;
  vector<DetectorHit> anodeF, anodeB, scintL, scintR, cathode;
  vector<DetectorHit> monitor;
};

struct CoincEvent {
  FPDetector focalPlane;
  SabreDetector sabreArray[5]; //index = ChannelMap Id# -1
};

struct ProcessedEvent {
  Double_t fp1_tdiff = -1e6, fp2_tdiff = -1e6, fp1_tsum = -1, fp2_tsum = -1,
           fp1_tcheck = -1, fp2_tcheck = -1;
  Double_t fp1_y=-1, fp2_y=-1;
  Double_t anodeFront = -1, anodeBack = -1, scintRight = -1, scintLeft = -1;
  Double_t scintRightShort = -1, scintLeftShort = -1;
  Double_t cathode = -1;
  Double_t xavg = -1e6, x1 = -1e6, x2 = -1e6;
  Double_t theta = -1e6;
  Double_t sabreRingE[5] = {-1,-1,-1,-1,-1}, sabreWedgeE[5] = {-1,-1,-1,-1,-1};
  Double_t sabreRingChannel[5] = {-1,-1,-1,-1,-1}, sabreWedgeChannel[5] = {-1,-1,-1,-1,-1};
  Double_t sabreRingTime[5] = {-1,-1,-1,-1,-1}, sabreWedgeTime[5] = {-1,-1,-1,-1,-1};
  
  Double_t delayFrontRightE = -1, delayFrontLeftE = -1;
  Double_t delayBackRightE = -1, delayBackLeftE = -1;
  Double_t delayFrontRightShort = -1, delayFrontLeftShort = -1;
  Double_t delayBackRightShort = -1, delayBackLeftShort = -1;
  Double_t anodeFrontTime = -1, anodeBackTime = -1;
  Double_t scintRightTime = -1, scintLeftTime = -1;
  Double_t delayFrontMaxTime = -1, delayBackMaxTime = -1;
  Double_t delayFrontLeftTime = -1, delayFrontRightTime = -1;
  Double_t delayBackLeftTime = -1, delayBackRightTime = -1;
  Double_t cathodeTime = -1;

  Double_t monitorE = -1, monitorShort = -1;
  Double_t monitorTime = -1;


  SabreDetector sabreArray[5]; //index = ChannelMap Id# -1
};

#endif
