/*
	GWMEventBuilder.h
	Class which represents the API of the event building environment. Wraps together the core concepts
	of the event builder, from conversion to plotting. Even intended to be able to archive data.
	Currently under development.

	Written by G.W. McCann Oct. 2020
*/
#ifndef GWMEVENTBUILDER_H
#define GWMEVENTBUILDER_H

#include "RunCollector.h"
#include <TGProgressBar.h>

class GWMEventBuilder {
public:
	GWMEventBuilder();
	~GWMEventBuilder();

	bool ReadConfigFile(const std::string& filename);
	void WriteConfigFile(const std::string& filename);

	void PlotHistograms();
	void MergeROOTFiles();
	void Convert2SortedRoot();
	void Convert2FastSortedRoot();
	void Convert2RawRoot();
	void Convert2SlowAnalyzedRoot();
	void Convert2FastAnalyzedRoot();

	inline void SetAnalysisType(int type) { m_analysisType = type;};
	inline int GetAnalysisType() { return m_analysisType; };

	inline void SetRunRange(int rmin, int rmax) { m_rmin = rmin; m_rmax = rmax; };
	inline void SetWorkDirectory(const std::string& fullpath) { m_workspace = fullpath; };
	inline void SetChannelMap(const std::string& name) { m_mapfile = name; };
	inline void SetBoardShiftFile(const std::string& name) { m_shiftfile = name; };
	inline void SetSlowCoincidenceWindow(double window) { m_SlowWindow = window; };
	inline void SetFastWindowIonChamber(double window) { m_FastWindowIonCh = window; };
	inline void SetFastWindowSABRE(double window) { m_FastWindowSABRE = window; };
	inline void SetCutList(const std::string& name) { m_cutList = name; };
	inline void SetScalerFile(const std::string& fullpath) { m_scalerfile = fullpath; };
	bool SetKinematicParameters(int zt, int at, int zp, int ap, int ze, int ae, double b, double theta, double bke);

	inline int GetRunMin() {return m_rmin;};
	inline int GetRunMax() {return m_rmax;};
	inline std::string GetWorkDirectory() {return m_workspace;};
	inline int GetTargetZ() {return m_ZT;};
	inline int GetTargetA() {return m_AT;};
	inline int GetProjectileZ() {return m_ZP;};
	inline int GetProjectileA() {return m_AP;};
	inline int GetEjectileZ() {return m_ZE;};
	inline int GetEjectileA() {return m_AE;};
	inline int GetResidualZ() {return m_ZR;};
	inline int GetResidualA() {return m_AR;};
	inline double GetBField() {return m_B;};
	inline double GetBeamKE() {return m_BKE;};
	inline double GetTheta() {return m_Theta;};
	inline double GetSlowCoincidenceWindow() { return m_SlowWindow; };
	inline double GetFastWindowIonChamber() { return m_FastWindowIonCh; };
	inline double GetFastWindowSABRE() { return m_FastWindowSABRE; };
	inline std::string GetChannelMap() { return m_mapfile; };
	inline std::string GetBoardShiftFile() { return m_shiftfile; };
	inline std::string GetCutList() { return m_cutList; };
	inline std::string GetScalerFile() { return m_scalerfile; };

	inline void AttachProgressBar(TGProgressBar* pb) { m_pb = pb; };

	enum BuildType {
		CONVERT,
		CONVERT_S,
		CONVERT_SA,
		CONVERT_F,
		CONVERT_FA,
		MERGE,
		PLOT
	};

private:

	int m_rmin, m_rmax;
	int m_ZT, m_AT, m_ZP, m_AP, m_ZE, m_AE, m_ZR, m_AR;
	double m_B, m_Theta, m_BKE;

	std::string m_workspace;
	std::string m_mapfile, m_shiftfile;
	std::string m_cutList;
	std::string m_scalerfile;

	double m_SlowWindow;
	double m_FastWindowIonCh;
	double m_FastWindowSABRE;

	int m_analysisType;

	RunCollector grabber;

	TGProgressBar* m_pb;

};

#endif
