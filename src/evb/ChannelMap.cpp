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

bool ChannelMap::FillMap(const std::string& name) 
{
	std::ifstream input(name);
	if(!input.is_open()) 
	{
		is_valid = false;
		return is_valid;
	}
	std::string junk, type, partname;
	int gchan, id;

	std::getline(input, junk);
	std::getline(input, junk);
	Channel this_chan;
	while(input>>gchan) 
	{
		//Set default values
		this_chan.detectorType = -1;
		this_chan.detectorID = -1;
		this_chan.detectorPart = -1;
		input>>id>>type>>partname;
		if(type == "SABRERING") 
		{
			this_chan.detectorType = DetAttribute::SabreRing;
			this_chan.detectorID = id;
			this_chan.detectorPart = std::stoi(partname);
		} 
		else if(type == "SABREWEDGE") 
		{
			this_chan.detectorType = DetAttribute::SabreWedge;
			this_chan.detectorID = id;
			this_chan.detectorPart = std::stoi(partname);
		} 
		else if (type == "FOCALPLANE") 
		{
			this_chan.detectorType = FOCALPLANE;
			this_chan.detectorID = id;
			if(partname == "SCINTRIGHT") this_chan.detectorPart = DetAttribute::ScintRight;
			else if(partname == "SCINTLEFT") this_chan.detectorPart = DetAttribute::ScintLeft;
			else if(partname == "DELAYFR") this_chan.detectorPart = DetAttribute::DelayFR;
			else if(partname == "DELAYFL") this_chan.detectorPart = DetAttribute::DelayFL;
			else if(partname == "DELAYBR") this_chan.detectorPart = DetAttribute::DelayBR;
			else if(partname == "DELAYBL") this_chan.detectorPart = DetAttribute::DelayBL;
			else if(partname == "CATHODE") this_chan.detectorPart = DetAttribute::Cathode;
			else if(partname == "ANODEFRONT") this_chan.detectorPart = DetAttribute::AnodeFront;
			else if(partname == "ANODEBACK") this_chan.detectorPart = DetAttribute::AnodeBack;
			else if(partname == "MONITOR") this_chan.detectorPart = DetAttribute::Monitor;
		}

		cmap[gchan] = this_chan;
	}

	input.close();
	is_valid = true;
	return is_valid;
}
