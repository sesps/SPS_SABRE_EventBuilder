/*
	ChannelMap.cpp
	Class which acts as the go between for global compass channels (board#*16 + channel) and
	physical detector information. Used in the event builder to assign compass data to real values
	in a simple way. Takes in a definition file and parses it into an unordered_map container.

	Written by G.W. McCann Oct. 2020
*/
#include "EventBuilder.h"
#include "ChannelMap.h"

ChannelMap::ChannelMap() :
	is_valid(false)
{
}

ChannelMap::ChannelMap(const std::string& name) :
	is_valid(false)
{
	FillMap(name);
}

ChannelMap::~ChannelMap() {}

bool ChannelMap::FillMap(const std::string& name) {
	std::ifstream input(name);
	if(!input.is_open()) {
		is_valid = false;
		return is_valid;
	}
	std::string junk, type, partname;
	int gchan, id;

	std::getline(input, junk);
	std::getline(input, junk);
	Channel this_chan;
	while(input>>gchan) {
		//Set default values
		this_chan.detectorType = -1;
		this_chan.detectorID = -1;
		this_chan.detectorPart = -1;
		input>>id>>type>>partname;
		if(type == "SABRERING") {
			this_chan.detectorType = SABRERING;
			this_chan.detectorID = id;
			this_chan.detectorPart = std::stoi(partname);
		} else if(type == "SABREWEDGE") {
			this_chan.detectorType = SABREWEDGE;
			this_chan.detectorID = id;
			this_chan.detectorPart = std::stoi(partname);
		} else if (type == "FOCALPLANE") {
			this_chan.detectorType = FOCALPLANE;
			this_chan.detectorID = id;
			if(partname == "SCINTRIGHT") this_chan.detectorPart = SCINTRIGHT;
			else if(partname == "SCINTLEFT") this_chan.detectorPart = SCINTLEFT;
			else if(partname == "DELAYFR") this_chan.detectorPart = DELAYFR;
			else if(partname == "DELAYFL") this_chan.detectorPart = DELAYFL;
			else if(partname == "DELAYBR") this_chan.detectorPart = DELAYBR;
			else if(partname == "DELAYBL") this_chan.detectorPart = DELAYBL;
			else if(partname == "CATHODE") this_chan.detectorPart = CATHODE;
			else if(partname == "ANODEFRONT") this_chan.detectorPart = ANODEFRONT;
			else if(partname == "ANODEBACK") this_chan.detectorPart = ANODEBACK;
			else if(partname == "MONITOR") this_chan.detectorPart = MONITOR;
		}

		cmap[gchan] = this_chan;
	}

	input.close();
	is_valid = true;
	return is_valid;
}
