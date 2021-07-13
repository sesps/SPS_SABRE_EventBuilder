#ifndef CUTHANDLER_H
#define CUTHANDLER_H

#include "DataStructs.h"

class CutHandler {
public:
	CutHandler();
	CutHandler(const std::string& filename);
	~CutHandler();
	void SetCuts(const std::string& filename);
	bool IsValid() { return validFlag; };
	bool IsInside(ProcessedEvent* eaddress);
	std::vector<TCutG*> GetCuts() { return cut_array; };

private:
	void InitVariableMap();

	std::vector<TCutG*> cut_array;
	std::vector<TFile*> file_array;
	std::unordered_map<std::string, double*> varmap;
	bool validFlag;
	ProcessedEvent m_event;

};

#endif