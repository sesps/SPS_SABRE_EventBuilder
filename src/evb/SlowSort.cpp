/*SlowSort.cpp
 *Class designed to first time-order raw data, and then based on a given coincidence window
 *sort the raw data into coincidence structures. Utilizes dictionary elements DPPChannel and
 *CoincEvent. Based on work by S. Balak, K. Macon, and E. Good from LSU. 
 *
 *Gordon M. Oct. 2019
 *
 *Refurbished and updated Jan 2020 GWM
 */
#include "EventBuilder.h"
#include "SlowSort.h"

/*Sort the Sabre Data in order of descending energy*/
bool SabreSort(DetectorHit i, DetectorHit j) {
  return (i.Long>j.Long);
}

/*Constructor takes input of coincidence window size, and fills sabre channel map*/
SlowSort::SlowSort() :
  coincWindow(-1.0), eventFlag(false), event(), cmap()
{
  event_stats = new TH2F("coinc_event_stats","coinc_events_stats;global channel;number of coincident hits;counts",144,0,144,20,0,20);
}

SlowSort::SlowSort(double windowSize, const string& mapfile) :
  coincWindow(windowSize), eventFlag(false), event(), cmap(mapfile)
{
  event_stats = new TH2F("coinc_event_stats","coinc_events_stats;global channel;number of coincident hits;counts",144,0,144,20,0,20);
  InitVariableMaps();
}

SlowSort::~SlowSort() {
}

/**EXPERIMENT MODS go here**/
void SlowSort::InitVariableMaps() {

  /*For SABRE: Each SABRE det has ring&wedge, so add the detID to the
    SABRERING/WEDGE attribute to differentiate*/
  for(int i=0; i<5; i++) {
    sabreVMap[SABRERING + i] = &event.sabreArray[i].rings;
    sabreVMap[SABREWEDGE + i] = &event.sabreArray[i].wedges;
  }

  /*For focal plane: Only one focal plane, so each variable is uniquely
    identified by its attribute
  */
  fpVMap[SCINTLEFT] = &event.focalPlane.scintL;
  fpVMap[SCINTRIGHT] = &event.focalPlane.scintR;
  fpVMap[CATHODE] = &event.focalPlane.cathode;
  fpVMap[DELAYFR] = &event.focalPlane.delayFR;
  fpVMap[DELAYFL] = &event.focalPlane.delayFL;
  fpVMap[DELAYBL] = &event.focalPlane.delayBL;
  fpVMap[DELAYBR] = &event.focalPlane.delayBR;
  fpVMap[ANODEFRONT] = &event.focalPlane.anodeF;
  fpVMap[ANODEBACK] = &event.focalPlane.anodeB;
  fpVMap[MONITOR] = &event.focalPlane.monitor;

}

/*Reset output structure to blank*/
void SlowSort::Reset() {
  event = blank;
}

bool SlowSort::AddHitToEvent(CompassHit& mhit) {
  DPPChannel curHit;
  curHit.Timestamp = mhit.timestamp;
  curHit.Energy = mhit.lgate;
  curHit.EnergyShort = mhit.sgate;
  curHit.Channel = mhit.channel;
  curHit.Board = mhit.board;
  curHit.Flags = mhit.flags;

  if(hitList.empty()) {
    startTime = curHit.Timestamp;
    hitList.push_back(curHit);
  } else if (curHit.Timestamp < previousHitTime) {
    return false;
  } else if ((curHit.Timestamp - startTime) < coincWindow) {
    hitList.push_back(curHit);
  } else {
    ProcessEvent();
    hitList.clear();
    startTime = curHit.Timestamp;
    hitList.push_back(curHit);
    eventFlag = true;
  }

  return true;
}

void SlowSort::FlushHitsToEvent() {
  if(hitList.empty()) {
    eventFlag = false;
    return;
  }

  ProcessEvent();
  hitList.clear();
  eventFlag = true;
}

CoincEvent SlowSort::GetEvent() {
  eventFlag = false;
  return event;
}

/*Function called when a start of a coincidence event is detected*/
void SlowSort::StartEvent() {
  if(hitList.size() != 0) {
    cerr<<"Attempting to initalize hitList when not cleared!! Check processing order."<<endl;
  }
  startTime = hit.Timestamp;
  hitList.push_back(hit);
}

/*Function called when an event outside the coincidence window is detected
 *Process all of the hits in the list, and write them to the sorted tree
 */
void SlowSort::ProcessEvent() {
  Reset();
  DetectorHit dhit;
  int gchan;
  int size = hitList.size();
  for(DPPChannel& curHit: hitList) {
    gchan = curHit.Channel + curHit.Board*16; //global channel
    event_stats->Fill(gchan, size);
    dhit.Time = curHit.Timestamp/1.0e3;
    dhit.Ch = gchan;
    dhit.Long = curHit.Energy;
    dhit.Short = curHit.EnergyShort;
    auto channel_info = cmap.FindChannel(gchan);
    if(channel_info == cmap.End()) {
      continue;
    }
    if(channel_info->second.detectorType == SABRERING || channel_info->second.detectorType == SABREWEDGE) {

      auto variable = sabreVMap.find(channel_info->second.detectorType + channel_info->second.detectorID);
      if(variable != sabreVMap.end()) {
        variable->second->push_back(dhit);
      }

    } else if(channel_info->second.detectorType == FOCALPLANE) {
      
      auto variable = fpVMap.find(channel_info->second.detectorPart);
      if(variable !=  fpVMap.end()) {
        variable->second->push_back(dhit);
      }

    } else {
      std::cout<<std::endl;
      std::cout<<"------Data Assignment Error!-------"<<std::endl;
      std::cout<<"Channel is present in channel map, but does not have a variable assigned in variable map!"<<std::endl;
      std::cout<<"global channel number: "<<gchan<<" channel detector type: "<<channel_info->second.detectorType<<std::endl;
      std::cout<<"Skipping this hit..."<<std::endl;
      std::cout<<"-----------------------------------"<<std::endl;

    }
  }
  //Organize the SABRE data in descending energy order
  for(int s=0; s<5; s++) {
    sort(event.sabreArray[s].rings.begin(), event.sabreArray[s].rings.end(), SabreSort);
    sort(event.sabreArray[s].wedges.begin(), event.sabreArray[s].wedges.end(), SabreSort);
  }
}
