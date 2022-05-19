/*
	CompassFile.h
	Wrapper class around a shared pointer to an ifstream. Here the shared pointer is used
	to overcome limitations of the ifstream class, namely that it is written such that ifstream
	cannot be modified by move semantics. Contains all information needed to parse a single binary
	CompassFile. Currently has a class wide defined buffer size; may want to make this user input
	in the future.

	Written by G.W. McCann Oct. 2020
*/
#ifndef COMPASSFILE_H
#define COMPASSFILE_H

#include "CompassHit.h"
#include "ShiftMap.h"
#include <memory>

namespace EventBuilder {

	class CompassFile 
	{
		
	public:
		CompassFile();
		CompassFile(const std::string& filename);
		CompassFile(const std::string& filename, int bsize);
		~CompassFile();
		void Open(const std::string& filename);
		void Close();
		bool GetNextHit();
	
		inline bool IsOpen() const { return m_file->is_open(); };
		inline CompassHit GetCurrentHit() const { return m_currentHit; }
		inline std::string GetName() const { return  m_filename; }
		inline bool CheckHitHasBeenUsed() const { return m_hitUsedFlag; } //query to find out if we've used the current hit
		inline void SetHitHasBeenUsed() { m_hitUsedFlag = true; } //flip the flag to indicate the current hit has been used
		inline bool IsEOF() const { return m_eofFlag; } //see if we've read all available data
		inline bool* GetUsedFlagPtr() { return &m_hitUsedFlag; }
		inline void AttachShiftMap(ShiftMap* map) { m_smap = map; }
		inline unsigned int GetSize() const { return m_size; }
		inline unsigned int GetNumberOfHits() const { return m_nHits; }
	
	
	private:
		void ReadHeader();
		void ParseNextHit();
		void GetNextBuffer();

		inline bool IsEnergy() { return (m_header & CoMPASSHeaders::Energy) != 0; }
		inline bool IsEnergyCalibrated() { return (m_header & CoMPASSHeaders::EnergyCalibrated) != 0; }
		inline bool IsEnergyShort() { return (m_header & CoMPASSHeaders::EnergyShort) != 0; }
		inline bool IsWaves() { return (m_header & CoMPASSHeaders::Waves) != 0; }
	
		using Buffer = std::vector<char>;
	
		using FilePointer = std::shared_ptr<std::ifstream>; //to make this class copy/movable
	
		std::string m_filename;
		Buffer m_hitBuffer;
		char* m_bufferIter;
		char* m_bufferEnd;
		ShiftMap* m_smap; //NOT owned by CompassFile. DO NOT delete
	
		bool m_hitUsedFlag;
		int m_bufsize = 200000; //size of the buffer in hits
		int m_hitsize; //size of a CompassHit in bytes (without alignment padding)
		uint16_t m_header;
		int m_buffersize;
	
		CompassHit m_currentHit;
		FilePointer m_file;
		bool m_eofFlag;
		unsigned int m_size; //size of the file in bytes
		unsigned int m_nHits; //number of hits in the file (m_size/24)

		enum CoMPASSHeaders
		{
			Energy = 0x0001,
			EnergyCalibrated = 0x0002,
			EnergyShort = 0x0004,
			Waves = 0x0008,
		};
	
	};

}

#endif
