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

class SFPPlotter 
{
public:
	SFPPlotter();
	~SFPPlotter();
	inline void AttachProgressBar(TGProgressBar* pb) { m_pb = pb; }
	inline void ApplyCutlist(const std::string& listname) { cutter.SetCuts(listname); }
	void Run(const std::vector<std::string>& files, const string& output);

private:
	void SetProgressBar(long total);
	void Chain(const std::vector<std::string>& files); //Form TChain
	void MakeUncutHistograms(const ProcessedEvent& ev, THashTable* table);
	void MakeCutHistograms(const ProcessedEvent& ev, THashTable* table);

	/*Histogram fill wrapper functions*/
	void MyFill(THashTable* table, const std::string& name, int binsx, double minx, double maxx, double valuex,
				int binsy, double miny, double maxy, double valuey);
	void MyFill(THashTable* table, const std::string& name, int binsx, double minx, double maxx, double valuex);

	ProcessedEvent *event_address;

	/*Cuts*/
	CutHandler cutter;
	
	TGProgressBar* m_pb; //GUI progress

};

#endif
