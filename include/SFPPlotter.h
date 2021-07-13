/*SFPPlotter.h
 *Class for generating histogram files for SPS-SABRE data
 *Intended use case is generating a TChain of multiple analyzed files and making
 *histograms of the larger data set.
 *
 *Created Jan 2020 by GWM
 */

#ifndef SFPCLEANER_H
#define SFPCLEANER_H

#include "DataStructs.h"
#include "CutHandler.h"
#include <TGProgressBar.h>

using namespace std;

class SFPPlotter {
  public:
    SFPPlotter();
    SFPPlotter(bool tf);
    ~SFPPlotter();
    inline void AttachProgressBar(TGProgressBar* pb) { m_pb = pb; };
    void ApplyCutlist(const string& listname);
    void Run(vector<TString> files, const string& output);

  private:
    void SetProgressBar(long total);
    void Chain(vector<TString> files); //Form TChain
    void MakeUncutHistograms(ProcessedEvent ev);
    void MakeCutHistograms(ProcessedEvent ev);

    /*Histogram fill wrapper functions*/
    void MyFill(const string& name, int binsx, double minx, double maxx, double valuex,
                             int binsy, double miny, double maxy, double valuey);
    void MyFill(const string& name, int binsx, double minx, double maxx, double valuex);

    ProcessedEvent *event_address;

    /*ROOT Storage*/
    THashTable *rootObj;

    /*Cuts*/
    CutHandler cutter;
    bool cutFlag;
    
    TChain *chain;

    TGProgressBar* m_pb; //GUI progress

};

#endif
