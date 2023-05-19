/*SFPAnalyzer.h
 *Class designed to analyze coincidence events. Currently only implemented for focal plane
 *data. Additional changes for SABRE would include this file and the sructure ProcessedEvent
 *in DataStructs.h. Based on code written by S. Balak, K. Macon, and E. Good.
 *
 *Gordon M. Oct. 2019
 *
 *Refurbished and updated Jan 2020 by GWM. Now uses both focal plane and SABRE data
 */
#ifndef SFPANALYZER_H
#define SFPANALYZER_H

#include "DataStructs.h"
#include "FP_kinematics.h"
#include "EVBParameters.h"

namespace EventBuilder {

	class SFPAnalyzer
	{
	public:
		SFPAnalyzer(const EVBParameters& params);
		~SFPAnalyzer();
		ProcessedEvent GetProcessedEvent(CoincEvent& event);
		inline void ClearHashTable() { rootObj->Clear(); }
		inline THashTable* GetHashTable() { return rootObj; }
	
	private:
		void Reset(); //Sets ouput structure back to "zero"
		void GetWeights(); //weights for xavg
		void AnalyzeEvent(CoincEvent& event);
	
		/*Fill wrappers for use with THashTable*/
		void MyFill(const std::string& name, int binsx, double minx, double maxx, double valuex,
					int binsy, double miny, double maxy, double valuey);
		void MyFill(const std::string& name, int binsx, double minx, double maxx, double valuex);
	
		CoincEvent *event_address; //Input branch address
		ProcessedEvent pevent, blank; //output branch and reset
	
		double w1, w2, zfp;
	
		THashTable *rootObj; //root storage
	};

}

#endif
