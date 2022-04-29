/*SlowSort.h
 *Class designed to first time-order raw data, and then based on a given coincidence window
 *sort the raw data into coincidence structures. Utilizes dictionary elements DPPChannel and
 *CoincEvent. Based on work by S. Balak, K. Macon, and E. Good from LSU. 
 *
 *Gordon M. Oct. 2019
 *
 *Refurbished and updated Jan 2020 GWM
 */
#ifndef SLOW_SORT_H
#define SLOW_SORT_H

#include "CompassHit.h"
#include "DataStructs.h"
#include "ChannelMap.h"
#include <TH2.h>
#include <unordered_map>

namespace EventBuilder {

	class SlowSort 
	{
	
	public:
		SlowSort();
		SlowSort(double windowSize, const std::string& mapfile);
		~SlowSort();
		inline void SetWindowSize(double window) { m_coincWindow = window; }
		inline bool SetMapFile(const std::string& mapfile) { return cmap.FillMap(mapfile); }
		bool AddHitToEvent(CompassHit& mhit);
		const CoincEvent& GetEvent();
		inline TH2F* GetEventStats() { return event_stats; }
		void FlushHitsToEvent(); //For use with *last* hit list
		inline bool IsEventReady() { return m_eventFlag; }
	
	private:
		void InitVariableMaps();
		void Reset();
		void ProcessEvent();
	
		double m_coincWindow;
		std::vector<DPPChannel> m_hitList;
		bool m_eventFlag;
		CoincEvent m_event;
		CoincEvent m_blank;
		
		double startTime, previousHitTime;    
		std::unordered_map<DetAttribute, std::vector<DetectorHit>*> varMap;
	
		TH2F* event_stats;
	
		ChannelMap cmap;
	 
	};

}

#endif
