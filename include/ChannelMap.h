/*
	ChannelMap.h
	Class which acts as the go between for global compass channels (board#*16 + channel) and
	physical detector information. Used in the event builder to assign compass data to real values
	in a simple way. Takes in a definition file and parses it into an unordered_map container.

	Written by G.W. McCann Oct. 2020
*/
#ifndef CHANNELMAP_H
#define CHANNELMAP_H

struct Channel {
	int detectorType; //What kind of detector we're looking at
	int detectorID; //Which specific detector we're looking at
	int detectorPart; //Which specific part we're looking at
};

//Detector part/type identifiers for use in the code
enum DetAttribute {
	FOCALPLANE,
	SCINTLEFT,
	SCINTRIGHT,
	ANODEFRONT,
	ANODEBACK,
	DELAYFR,
	DELAYFL,
	DELAYBR,
	DELAYBL,
	CATHODE,
	MONITOR,
	SABRERING = 88, //These are offset to avoid interference at the variable mapping phase
	SABREWEDGE = 99 //Just don't add any new attributes with values greater than 88
};


class ChannelMap {
	
public:
	typedef std::unordered_map<int, Channel> Containter;
	typedef std::unordered_map<int, Channel>::iterator Iterator;

	ChannelMap();
	ChannelMap(const std::string& filename);
	~ChannelMap();
	bool FillMap(const std::string& filename);
	inline const Containter* GetCMap() { return &cmap; };
	inline Iterator FindChannel(int key) { return cmap.find(key); };
	inline Iterator End() { return cmap.end(); };
	inline bool IsValid() { return is_valid; };

private:
	Containter cmap;
	bool is_valid;
};


#endif
