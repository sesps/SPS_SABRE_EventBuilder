/*
	EVBApp.h
	Class which represents the API of the event building environment. Wraps together the core concepts
	of the event builder, from conversion to plotting. Even intended to be able to archive data.
	Currently under development.

	Written by G.W. McCann Oct. 2020
*/
#ifndef EVBAPP_H
#define EVBAPP_H

#include "RunCollector.h"
#include <TGProgressBar.h>

namespace EventBuilder {
	
	class EVBApp {
	public:
		EVBApp();
		~EVBApp();
	
		bool ReadConfigFile(const std::string& filename);
		void WriteConfigFile(const std::string& filename);
	
		void PlotHistograms();
		void MergeROOTFiles();
		void Convert2SortedRoot();
		void Convert2FastSortedRoot();
		void Convert2RawRoot();
		void Convert2SlowAnalyzedRoot();
		void Convert2FastAnalyzedRoot();
	
		void SetRunRange(int rmin, int rmax);
		void SetWorkDirectory(const std::string& fullpath);
		void SetChannelMap(const std::string& name);
		void SetBoardShiftFile(const std::string& name);
		void SetSlowCoincidenceWindow(double window);
		void SetFastWindowIonChamber(double window);
		void SetFastWindowSABRE(double window);
		void SetCutList(const std::string& name);
		void SetScalerFile(const std::string& fullpath);
		bool SetKinematicParameters(int zt, int at, int zp, int ap, int ze, int ae, double b, double theta, double bke);
	
		inline int GetRunMin() const { return m_rmin; }
		inline int GetRunMax() const { return m_rmax; }
		inline std::string GetWorkDirectory() const { return m_workspace; }
		inline int GetTargetZ() const { return m_ZT; }
		inline int GetTargetA() const { return m_AT; }
		inline int GetProjectileZ() const { return m_ZP; }
		inline int GetProjectileA() const { return m_AP; }
		inline int GetEjectileZ() const { return m_ZE; }
		inline int GetEjectileA() const { return m_AE; }
		inline int GetResidualZ() const { return m_ZR; }
		inline int GetResidualA() const { return m_AR; }
		inline double GetBField() const { return m_B; }
		inline double GetBeamKE() const { return m_BKE; }
		inline double GetTheta() const { return m_Theta; }
		inline double GetSlowCoincidenceWindow() const { return m_SlowWindow; }
		inline double GetFastWindowIonChamber() const { return m_FastWindowIonCh; }
		inline double GetFastWindowSABRE() const { return m_FastWindowSABRE; }
		inline std::string GetChannelMap() const { return m_mapfile; }
		inline std::string GetBoardShiftFile() const { return m_shiftfile; }
		inline std::string GetCutList() const { return m_cutList; }
		inline std::string GetScalerFile() const { return m_scalerfile; }
	
		inline void AttachProgressBar(TGProgressBar* pb) { m_pb = pb; }
	
		enum Operation 
		{
			Convert,
			ConvertSlow,
			ConvertSlowA,
			ConvertFast,
			ConvertFastA,
			Merge,
			Plot
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
	
		RunCollector grabber;
	
		TGProgressBar* m_pb;
	
	};

}
#endif
