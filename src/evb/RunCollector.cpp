#include "EventBuilder.h"
#include "RunCollector.h"
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TCollection.h>
#include <TList.h>
#include <cstdlib>
#include <cstdio>

namespace EventBuilder {

	RunCollector::RunCollector():
		m_initFlag(false), m_directory(""), m_prefix(""), m_suffix(""), m_minRun(0), m_maxRun(0)
	{
	}
	
	RunCollector::RunCollector(const std::string& dirname, const std::string& prefix, const std::string& suffix) :
		m_initFlag(true), m_directory(dirname), m_prefix(prefix), m_suffix(suffix), m_minRun(0), m_maxRun(0)
	{
	}
	
	RunCollector::RunCollector(const std::string& dirname, const std::string& prefix, const std::string& suffix, int min, int max) :
		m_initFlag(true), m_directory(dirname), m_prefix(prefix), m_suffix(suffix), m_minRun(min), m_maxRun(max)
	{
	}
	
	RunCollector::~RunCollector() {}
	
	void RunCollector::SetSearchParams(const std::string& dirname, const std::string& prefix, const std::string& suffix, int min, int max)
	{
		m_directory = dirname.c_str();
		m_prefix = prefix.c_str();
		m_suffix = suffix.c_str();
		m_minRun = min; m_maxRun = max;
		m_initFlag = true;
	}
	
	bool RunCollector::GrabAllFiles() 
	{
		if(!m_initFlag)
			return false;
	
		TSystemDirectory sysdir(m_directory.c_str(), m_directory.c_str());
		TList *flist = sysdir.GetListOfFiles();
		m_filelist.clear();
	
		if(!flist)  //Make sure list is real. If not, means no directory
		{
			EVB_WARN("RunCollector::GrabAllFiles() unable to find any files in directory {0}",m_directory);
			return false;
		}
	
		TSystemFile *file;
		std::string fname, temp;
		TIter next_element(flist); //List iterator
		while((file = (TSystemFile*)next_element())) 
		{
			temp = file->GetName();
			if(temp.size() < m_prefix.size() || temp.size() < m_suffix.size())
				continue;
			else if(!file->IsDirectory() && !temp.compare(0,m_prefix.size(),m_prefix) && 
					!temp.compare(temp.size()-m_suffix.size(), m_suffix.size(), m_suffix)) 
			{
		  		fname = m_directory+temp;
		  		m_filelist.push_back(fname);
			} 
		}
	
		delete flist;
		if(m_filelist.size()>0) 
			return true;
		else 
		{
			EVB_WARN("RunCollector::GrabAllFiles() unable to find any files in directory {0} which match run pattern",m_directory);
			return false;
		}
	}
	
	std::string RunCollector::GrabFile(int runNum) {
		if(!m_initFlag) 
			return "";
		TSystemDirectory sysdir(m_directory.c_str(), m_directory.c_str());
		TList* flist = sysdir.GetListOfFiles();
	
		if(!flist)
			return "";
	
		TSystemFile *file;
		std::string fname = "", temp;
		std::string runno = "_"+std::to_string(runNum)+m_suffix;
		TIter next_element(flist);
		while((file = (TSystemFile*)next_element())) 
		{
			temp = file->GetName();
			if(temp.size() < m_prefix.size() || temp.size() < runno.size())
				continue;
			else if(!file->IsDirectory() && !temp.compare(0,m_prefix.size(),m_prefix) && 
					!temp.compare(temp.size()-runno.size(),runno.size(), runno)) 
			{
				fname = m_directory+temp;
				break;
			}
		}
	
		delete flist;
		return fname;
	}
	
	/*Grabs all files within a specified run range*/
	bool RunCollector::GrabFilesInRange() 
	{
		if(!m_initFlag)
			return false;
	
		TSystemDirectory sysdir(m_directory.c_str(), m_directory.c_str());
		TList *flist = sysdir.GetListOfFiles();
		m_filelist.clear();
	
		if(!flist) 
		{
			EVB_WARN("RunCollector::GrabFilesInRange() unable to find any files in directory {0}",m_directory);
			return false;
		}
	
		TSystemFile *file;
		std::string fname, temp;
		std::string runno;
		for(int i=m_minRun; i<=m_maxRun; i++) //loop over range
		{
			TIter next_element(flist);//list iterator
			runno = "_"+std::to_string(i) + m_suffix; //suffix is now _#.suffix
			while((file = (TSystemFile*)next_element())) //look through directory until file found
			{
				temp = file->GetName();
				if(temp.size() < m_prefix.size() || temp.size() < runno.size())
					continue;
				else if(!file->IsDirectory() && !temp.compare(0,m_prefix.size(),m_prefix) && 
						!temp.compare(temp.size()-runno.size(),runno.size(), runno)) 
				{
					fname = m_directory+temp;
					m_filelist.push_back(fname);
					break; //if we find the file, break out of iterator loop
				}
			}
		}
	
		delete flist;
		if(m_filelist.size()>0)
			return true;
		else 
		{
			EVB_WARN("RunCollector::GrabAllFiles() unable to find any files in directory {0} which match run pattern and were in run range",m_directory);
			return false;
		}
	
	}
	
	bool RunCollector::Merge_hadd(const std::string& outname) 
	{
		int sys_return;
		if(!m_initFlag) 
			return false;
	
		if(m_maxRun == 0) 
		{
			if(GrabAllFiles()) 
			{ 
				std::string clump = "hadd "+outname;
				for(unsigned int i=0; i<m_filelist.size(); i++) 
					clump += " "+m_filelist[i];
				sys_return = std::system(clump.c_str());
				return true;
			} 
			else
				return false;
		} 
		else 
		{
			if(GrabFilesInRange()) 
			{
				std::string clump = "hadd "+outname;
				for(unsigned int i=0; i<m_filelist.size(); i++)
					clump += " "+m_filelist[i];
				sys_return = std::system(clump.c_str());
				return true;
			} 
			else 
				return false;
		}

		if(!sys_return)
			EVB_ERROR("How did i even get here?");
		return false;
	}
	
	bool RunCollector::Merge_TChain(const std::string& outname) 
	{
		if(!m_initFlag)
			return false;
		TFile *output = new TFile(outname.c_str(), "RECREATE");
		TChain *chain = new TChain("SPSTree", "SPSTree");
		
		if(m_maxRun == 0) 
		{
			if(GrabAllFiles()) 
			{ 
				for(unsigned int i=0; i<m_filelist.size(); i++)
					chain->Add(m_filelist[i].c_str());
				chain->Merge(output,0,"fast");
				return true;
			} 
			else
				return false;
		} 
		else 
		{
			if(GrabFilesInRange()) 
			{
				for(unsigned int i=0; i<m_filelist.size(); i++)
					chain->Add(m_filelist[i].c_str());
				chain->Merge(output,0,"fast");
				return true;
			} else
				return false;
		}
	
		if(output->IsOpen()) 
			output->Close();
		return false;
	}

}