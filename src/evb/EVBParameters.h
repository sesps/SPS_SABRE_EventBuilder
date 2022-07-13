#ifndef EVB_PARAMETERS_H
#define EVB_PARAMETERS_H

namespace EventBuilder {

	struct EVBParameters
	{
		std::string workspaceDir = "";
		std::string channelMapFile = "";
		std::string timeShiftFile = "";
		std::string cutListFile = "";
		std::string scalerFile = "";

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
}

#endif