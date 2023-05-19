/*

MassLookup.h
Generates a map for isotopic masses using AMDC data; subtracts away
electron mass from the atomic mass by default. Creates a static global instance
of this map (MASS) for use throughout code it is included into.

Written by G.W. McCann Aug. 2020

Updated for better code practices and real singleton usage GWM 2023

*/
#ifndef MASS_LOOKUP_H
#define MASS_LOOKUP_H

namespace EventBuilder {

	static constexpr uint32_t GetIsotopeID(uint32_t z, uint32_t a)
	{
		return z >= a ? z*z + z + a : a*a + z;
	}

	struct NuclearData
	{
		std::string isotopicSymbol = "";
		double isotopicMass = 0.0;
		uint32_t z = 0;
		uint32_t a = 0;
	};

	class MassLookup 
	{
	public:
		~MassLookup();
		double FindMass(uint32_t Z, uint32_t A);
		std::string FindSymbol(uint32_t Z, uint32_t A);

		static MassLookup& GetInstance() { return *s_instance; }
		static bool IsInvalidMass(double mass) { return mass == s_invalidMass; }
		static bool IsInvalidSymbol(const std::string& sym) { return sym == s_invalidSymbol; }
	
	private:
		MassLookup();
		std::unordered_map<uint32_t, NuclearData> m_dataMap;
	
		static MassLookup* s_instance;

		//constants
		static constexpr double s_u2MeV = 931.4940954; // U to MeV
		static constexpr double s_electronMass = 0.000548579909; // electron mass in u
		static constexpr double s_invalidMass = -1.0; //Invalid mass in MeV
		static constexpr char s_invalidSymbol[] = "Invalid"; //Invalid isotope symbol
	};
	
}
#endif
