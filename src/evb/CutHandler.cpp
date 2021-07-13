#include "EventBuilder.h"
#include "CutHandler.h"

CutHandler::CutHandler() :
	validFlag(false)
 {
	InitVariableMap();
 }

CutHandler::CutHandler(const std::string& filename) :
	validFlag(false)
{
	SetCuts(filename);
	InitVariableMap();
}

CutHandler::~CutHandler() {
	for(unsigned int i=0; i<file_array.size(); i++) {
		if(file_array[i]->IsOpen()) file_array[i]->Close();
	}
}

void CutHandler::SetCuts(const std::string& filename) {
	std::ifstream cutlist(filename);

	if(!cutlist.is_open()) {
		validFlag = false;
	}

	std::string junk, name, fname, varx, vary;
	cutlist>>junk>>junk>>junk>>junk;

	cut_array.clear();
	file_array.clear();

	while(cutlist>>name) {
		cutlist>>fname>>varx>>vary;
		TFile* file = TFile::Open(fname.c_str(), "READ");
		TCutG* cut = (TCutG*) file->Get("CUTG");
		if(cut) {
			cut->SetVarX(varx.c_str());
			cut->SetVarY(vary.c_str());
			cut->SetName(name.c_str());
			cut_array.push_back(cut);
			file_array.push_back(file);
		} else {
			validFlag = false;
			std::cerr<<"CutHandler has encountered a bad cut at file: "<<file<<"."<<std::endl;
			std::cerr<<"The file either does not exist or does not contain a TCutG named CUTG"<<std::endl;
			std::cerr<<"Cuts will not be used."<<std::endl;
			return;
		}
	}

	validFlag = true;
}

/*
	ADD MORE VARIABLES HERE!
*/
void CutHandler::InitVariableMap() {
	varmap["x1"] = &m_event.x1;
	varmap["x2"] = &m_event.x2;
	varmap["xavg"] = &m_event.xavg;
	varmap["scintLeft"] = &m_event.scintLeft;
	varmap["anodeBack"]  = &m_event.anodeBack;
	varmap["cathode"] = &m_event.cathode;
}

bool CutHandler::IsInside(ProcessedEvent* eaddress) {
	m_event = *eaddress;
	std::string x, y;
	for(unsigned int i=0; i<cut_array.size(); i++) {
		TCutG* cut = cut_array[i];
		x = cut->GetVarX();
		y = cut->GetVarY();
		auto xentry = varmap.find(x);
		auto yentry = varmap.find(y);
		if(xentry == varmap.end() || yentry == varmap.end()) {
			std::cerr<<"Unmapped variable called in CutHandler::IsInside()! Var names: "<<xentry->first<<" , "<<yentry->first<<std::endl;
			return false;
		}

		if(!cut->IsInside(*(xentry->second), *(yentry->second))) {
			return false;
		}
	}

	return true;
}
