/*
	ChannelMap.cpp
	Class which acts as the go between for global compass channels (board#*16 + channel) and
	physical detector information. Used in the event builder to assign compass data to real values
	in a simple way. Takes in a definition file and parses it into an unordered_map container.

	Written by G.W. McCann Oct. 2020
*/
#include "ChannelMap.h"

namespace EventBuilder {

	ChannelMap::ChannelMap() :
		m_validFlag(false)
	{
	}
	
	ChannelMap::ChannelMap(const std::string& name) :
		m_validFlag(false)
	{
		FillMap(name);
	}
	
	ChannelMap::~ChannelMap() {}
	
	bool ChannelMap::FillMap(const std::string& name) 
	{
		std::ifstream input(name);
		if(!input.is_open()) 
		{
			m_validFlag = false;
			return m_validFlag;
		}
		std::string junk, type, partname;
		int gchan, id;
	
		std::getline(input, junk);
		std::getline(input, junk);
		Channel this_chan;
		while(input>>gchan) 
		{
			//Set default values
			this_chan.type = DetType::NoneType;
			this_chan.local_channel = -1;
			this_chan.attribute = DetAttribute::NoneAttr;
			input>>id>>type>>partname;
			if(type == "SABRERING")
			{
				this_chan.type = DetType::Sabre;
				switch(id)
				{
					case 0: this_chan.attribute = DetAttribute::SabreRing0; break;
					case 1: this_chan.attribute = DetAttribute::SabreRing1; break;
					case 2: this_chan.attribute = DetAttribute::SabreRing2; break;
					case 3: this_chan.attribute = DetAttribute::SabreRing3; break;
					case 4: this_chan.attribute = DetAttribute::SabreRing4; break;
				}
				this_chan.local_channel = std::stoi(partname);
			} 
			else if(type == "SABREWEDGE") 
			{
				this_chan.type = DetType::Sabre;
				switch(id)
				{
					case 0: this_chan.attribute = DetAttribute::SabreWedge0; break;
					case 1: this_chan.attribute = DetAttribute::SabreWedge1; break;
					case 2: this_chan.attribute = DetAttribute::SabreWedge2; break;
					case 3: this_chan.attribute = DetAttribute::SabreWedge3; break;
					case 4: this_chan.attribute = DetAttribute::SabreWedge4; break;
				}
				this_chan.local_channel = std::stoi(partname);
			} 
			else if (type == "FOCALPLANE") 
			{
				this_chan.type = DetType::FocalPlane;
				this_chan.local_channel = id;
				if(partname == "SCINTRIGHT") this_chan.attribute = DetAttribute::ScintRight;
				else if(partname == "SCINTLEFT") this_chan.attribute = DetAttribute::ScintLeft;
				else if(partname == "DELAYFR") this_chan.attribute = DetAttribute::DelayFR;
				else if(partname == "DELAYFL") this_chan.attribute = DetAttribute::DelayFL;
				else if(partname == "DELAYBR") this_chan.attribute = DetAttribute::DelayBR;
				else if(partname == "DELAYBL") this_chan.attribute = DetAttribute::DelayBL;
				else if(partname == "CATHODE") this_chan.attribute = DetAttribute::Cathode;
				else if(partname == "ANODEFRONT") this_chan.attribute = DetAttribute::AnodeFront;
				else if(partname == "ANODEBACK") this_chan.attribute = DetAttribute::AnodeBack;
				else if(partname == "MONITOR") this_chan.attribute = DetAttribute::Monitor;
			}
	
			m_cmap[gchan] = this_chan;
		}
	
		input.close();
		m_validFlag = true;
		return m_validFlag;
	}
}