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
#include "ProgressCallback.h"

namespace EventBuilder {

	struct EVBParameters
	{
		std::filesystem::path workspaceDir = "";
		std::filesystem::path channelMapFile = "";
		std::filesystem::path timeShiftFile = "";
		std::filesystem::path cutListFile = "";
		std::filesystem::path scalerFile = "";

		int runMin = 0;
		int runMax = 0;

		double slowCoincidenceWindow = 3.0e6;
		double fastCoincidenceWindowIonCh = 0.0;
		double fastCoincidenceWindowSABRE = 0.0;

		int ZT = 6;
		int AT = 12;
		int ZP = 1;
		int AP = 2;
		int ZE = 1;
		int AE = 1;

		double BField = 7.8; //kG
		double spsAngle = 15.0; //degrees
		double beamEnergy = 16.0; //MeV
	};
	
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
	
		inline void SetParameters(const EVBParameters& params) { m_params = params; }
		inline EVBParameters& GetParameters() { return m_params; }

		void DefaultProgressCallback(long curVal, long totalVal);
		inline void SetProgressCallbackFunc(const ProgressCallbackFunc& function) { m_progressCallback = function; }
		inline void SetProgressFraction(double frac) { m_progressFraction = frac; }
	
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
		EVBParameters m_params;
		RunCollector grabber;
		double m_progressFraction;
		ProgressCallbackFunc m_progressCallback;
	
	};

}
#endif
