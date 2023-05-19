/*

MassLookup.h
Generates a map for isotopic masses using AMDC data; subtracts away
electron mass from the atomic mass by default. Creates a static global instance
of this map (MASS) for use throughout code it is included into.

Written by G.W. McCann Aug. 2020

*/
#include "MassLookup.h"

namespace EventBuilder {

	//instantiate
	MassLookup* MassLookup::s_instance = new MassLookup();
	
	/*
	  Read in AMDC mass file, preformated to remove excess info. Here assumes that by default
	  the file is in a local directory etc/
	*/
	MassLookup::MassLookup() 
	{
		std::string filepath;
		#ifdef ETC_DIR_PATH
			filepath = ETC_DIR_PATH;
			filepath += "mass.txt";
		#else
			filepath = "./etc/mass.txt";
		#endif
		std::ifstream massfile(filepath);
		if(massfile.is_open()) 
		{
			std::string junk, element;
			NuclearData data;
			double atomicMassBig, atomicMassSmall;
			uint32_t id;
			std::getline(massfile,junk);
			std::getline(massfile,junk);
			while(massfile>>junk) 
			{
				massfile >> data.z >> data.a >> element >> atomicMassBig >> atomicMassSmall;
				data.isotopicMass = (atomicMassBig + atomicMassSmall*1e-6 - data.z*s_electronMass)*s_u2MeV;
				data.isotopicSymbol = fmt::format("{}{}", data.a, element);
				id = GetIsotopeID(data.z, data.a);
				m_dataMap[id] = data;
			}
		} 
		else
	    	EVB_ERROR("Mass file could not be loaded ({0}). Crashing inbound.",filepath);
	}
	
	MassLookup::~MassLookup() {}
	
	//Returns nuclear mass in MeV
	double MassLookup::FindMass(uint32_t Z, uint32_t A) 
	{
		uint32_t id = GetIsotopeID(Z, A);
		auto data = m_dataMap.find(id);
		if(data == m_dataMap.end()) 
		{
			EVB_WARN("Invalid nucleus (Z,A) ({0},{1}) at MassLookup::FindMass; returning invalid.",Z,A);
			return s_invalidMass;
		}
		return data->second.isotopicMass;
	}
	
	//returns element symbol
	std::string MassLookup::FindSymbol(uint32_t Z, uint32_t A) 
	{
		uint32_t id = GetIsotopeID(Z, A);
		auto data = m_dataMap.find(id);
		if(data == m_dataMap.end()) 
		{
			EVB_WARN("Invalid nucleus (Z,A) ({0},{1}) at MassLookup::FindSymbol; returning Invalid.",Z,A);
			return s_invalidSymbol;
		}
		return data->second.isotopicSymbol;
	}
}