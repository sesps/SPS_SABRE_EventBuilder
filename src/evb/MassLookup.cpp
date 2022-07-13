/*

MassLookup.h
Generates a map for isotopic masses using AMDC data; subtracts away
electron mass from the atomic mass by default. Creates a static global instance
of this map (MASS) for use throughout code it is included into.

Written by G.W. McCann Aug. 2020

*/
#include "MassLookup.h"

namespace EventBuilder {
	
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
			int Z,A;
			std::string junk, element, key;
			double atomicMassBig, atomicMassSmall, isotopicMass;
			std::getline(massfile,junk);
			std::getline(massfile,junk);
			while(massfile>>junk) 
			{
				massfile>>Z>>A>>element>>atomicMassBig>>atomicMassSmall;
				isotopicMass = (atomicMassBig + atomicMassSmall*1e-6 - Z*electron_mass)*u_to_mev;
				key = "("+std::to_string(Z)+","+A+")";
				massTable[key] = isotopicMass;
				elementTable[Z] = element;
			}
		} 
		else
	    	EVB_ERROR("Mass file could not be loaded ({0}). Crashing inbound.",filepath);
	}
	
	MassLookup::~MassLookup() {}
	
	//Returns nuclear mass in MeV
	double MassLookup::FindMass(int Z, int A) 
	{
		std::string key = "("+std::to_string(Z)+","+std::to_string(A)+")";
		auto data = massTable.find(key);
		if(data == massTable.end()) 
		{
			EVB_WARN("Invalid nucleus (Z,A) ({0},{1}) at MassLookup::FindMass; returning zero.",Z,A);
			return 0;
		}
		return data->second;
	}
	
	//returns element symbol
	std::string MassLookup::FindSymbol(int Z, int A) 
	{
		auto data = elementTable.find(Z);
		if(data == elementTable.end()) 
		{
			EVB_WARN("Invalid nucleus (Z,A) ({0},{1}) at MassLookup::FindSymbol; returning empty string.",Z,A);
			return "";
		}
		std::string fullsymbol = std::to_string(A) + data->second;
		return fullsymbol;
	}
}