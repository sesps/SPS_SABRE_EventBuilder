/*SlowSort.cpp
 *Class designed to first time-order raw data, and then based on a given coincidence window
 *sort the raw data into coincidence structures. Utilizes dictionary elements DPPChannel and
 *CoincEvent. Based on work by S. Balak, K. Macon, and E. Good from LSU. 
 *
 *Gordon M. Oct. 2019
 *
 *Refurbished and updated Jan 2020 GWM
 */
#include "SlowSort.h"

namespace EventBuilder {

	/*Sort the Sabre Data in order of descending energy*/
	bool SabreSort(const DetectorHit& i, const DetectorHit& j) {
	  return (i.Long>j.Long);
	}
	
	/*Constructor takes input of coincidence window size, and fills sabre channel map*/
	SlowSort::SlowSort() :
		m_coincWindow(-1.0), m_eventFlag(false)
	{
		event_stats = new TH2F("coinc_event_stats","coinc_events_stats;global channel;number of coincident hits;counts",144,0,144,20,0,20);
	}
	
	SlowSort::SlowSort(double windowSize, const std::string& mapfile) :
		m_coincWindow(windowSize), m_eventFlag(false), m_event(), cmap(mapfile)
	{
		event_stats = new TH2F("coinc_event_stats","coinc_events_stats;global channel;number of coincident hits;counts",144,0,144,20,0,20);
		InitVariableMaps();
	}
	
	SlowSort::~SlowSort() {}
	
	/**EXPERIMENT MODS go here**/
	void SlowSort::InitVariableMaps() 
	{
	
		/*For SABRE: Each SABRE det has ring&wedge, so add the detID to the
		  SABRERING/WEDGE attribute to differentiate*/
		varMap[DetAttribute::SabreRing0] = &m_event.sabreArray[0].rings;
		varMap[DetAttribute::SabreRing1] = &m_event.sabreArray[1].rings;
		varMap[DetAttribute::SabreRing2] = &m_event.sabreArray[2].rings;
		varMap[DetAttribute::SabreRing3] = &m_event.sabreArray[3].rings;
		varMap[DetAttribute::SabreRing4] = &m_event.sabreArray[4].rings;
		varMap[DetAttribute::SabreWedge0] = &m_event.sabreArray[0].wedges;
		varMap[DetAttribute::SabreWedge1] = &m_event.sabreArray[1].wedges;
		varMap[DetAttribute::SabreWedge2] = &m_event.sabreArray[2].wedges;
		varMap[DetAttribute::SabreWedge3] = &m_event.sabreArray[3].wedges;
		varMap[DetAttribute::SabreWedge4] = &m_event.sabreArray[4].wedges;
	
		/*For focal plane: Only one focal plane, so each variable is uniquely
		  identified by its attribute
		*/
		varMap[DetAttribute::ScintLeft] = &m_event.focalPlane.scintL;
		varMap[DetAttribute::ScintRight] = &m_event.focalPlane.scintR;
		varMap[DetAttribute::Cathode] = &m_event.focalPlane.cathode;
		varMap[DetAttribute::DelayFR] = &m_event.focalPlane.delayFR;
		varMap[DetAttribute::DelayFL] = &m_event.focalPlane.delayFL;
		varMap[DetAttribute::DelayBL] = &m_event.focalPlane.delayBL;
		varMap[DetAttribute::DelayBR] = &m_event.focalPlane.delayBR;
		varMap[DetAttribute::AnodeFront] = &m_event.focalPlane.anodeF;
		varMap[DetAttribute::AnodeBack] = &m_event.focalPlane.anodeB;
		varMap[DetAttribute::Monitor] = &m_event.focalPlane.monitor;
	
	}
	
	/*Reset output structure to blank*/
	void SlowSort::Reset() 
	{
		m_event = m_blank;
	}
	
	bool SlowSort::AddHitToEvent(CompassHit& mhit) 
	{
		DPPChannel curHit;
		curHit.Timestamp = mhit.timestamp;
		curHit.Energy = mhit.energy;
		curHit.EnergyShort = mhit.energyShort;
		curHit.Channel = mhit.channel;
		curHit.Board = mhit.board;
		curHit.Flags = mhit.flags;
	
		if(m_hitList.empty()) 
		{
			startTime = curHit.Timestamp;
			m_hitList.push_back(curHit);
		} 
		else if (curHit.Timestamp < previousHitTime)
			return false;
		else if ((curHit.Timestamp - startTime) < m_coincWindow)
			m_hitList.push_back(curHit);
		else 
		{
			ProcessEvent();
			m_hitList.clear();
			startTime = curHit.Timestamp;
			m_hitList.push_back(curHit);
			m_eventFlag = true;
		}
	
		return true;
	}
	
	void SlowSort::FlushHitsToEvent()
	{
		if(m_hitList.empty())
		{
			m_eventFlag = false;
			return;
		}
	
		ProcessEvent();
		m_hitList.clear();
		m_eventFlag = true;
	}
	
	const CoincEvent& SlowSort::GetEvent()
	{
		m_eventFlag = false;
		return m_event;
	}
	
	/*Function called when an event outside the coincidence window is detected
	 *Process all of the hits in the list, and write them to the sorted tree
	 */
	void SlowSort::ProcessEvent()
	{
		Reset();
		DetectorHit dhit;
		int gchan;
		int size = m_hitList.size();
		for(DPPChannel& curHit: m_hitList)
		{
			gchan = curHit.Channel + curHit.Board*16; //global channel
			event_stats->Fill(gchan, size);
			dhit.Time = curHit.Timestamp/1.0e3;
			dhit.Ch = gchan;
			dhit.Long = curHit.Energy;
			dhit.Short = curHit.EnergyShort;
			auto channel_info = cmap.FindChannel(gchan);
	
			if(channel_info == cmap.End())
			{
				EVB_WARN("At SlowSort::ProcessEvent() -- Data Assignment Error! Global channel {0} found but not assigned in ChannelMap! Skipping data.",gchan);
				continue;
			}
			  
			if(channel_info->second.type == DetType::FocalPlane)
			{
			  auto variable = varMap.find(channel_info->second.attribute);
			  if(variable !=  varMap.end())
			    variable->second->push_back(dhit);
			} 
			else if(channel_info->second.type == DetType::Sabre)
			{
				auto variable = varMap.find(channel_info->second.attribute);
				if(variable != varMap.end())
					variable->second->push_back(dhit);
			}
			else 
			{
				EVB_WARN("At SlowSort::ProcessEvent() -- Data Assignment Error! Channel ({0}, {1}, {2}) exists in ChannelMap, but does not have an assigned variable! Skipping data.",
						gchan, channel_info->second.type, channel_info->second.attribute);
			}
		}
		//Organize the SABRE data in descending energy order
		for(int s=0; s<5; s++)
		{
			sort(m_event.sabreArray[s].rings.begin(), m_event.sabreArray[s].rings.end(), SabreSort);
			sort(m_event.sabreArray[s].wedges.begin(), m_event.sabreArray[s].wedges.end(), SabreSort);
		}
	}

}