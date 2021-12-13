#include "EventBuilder.h"
#include "RunCollector.h"
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TCollection.h>
#include <TList.h>
#include <cstdlib>
#include <cstdio>

RunCollector::RunCollector():
	m_initFlag(false), m_directory(""), m_prefix(""), m_suffix(""), m_maxRun(0), m_minRun(0)
{
}

RunCollector::RunCollector(const std::string& dirname, const std::string& prefix, const std::string& suffix) 
	m_initFlag(true), m_directory(dirname), m_prefix(prefix), m_suffix(suffix), m_minRun(0), m_maxRun(0)
{
}

RunCollector::RunCollector(const std::string& dirname, const std::string& prefix, const std::string& suffix, int min, int max) 
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
		cerr<<"Unable to find any files in directory; check name given to the input.txt"<<endl;
		return false;
	}

	TSystemFile *file;
	std::string fname, temp;
	TIter next_element(flist); //List iterator
	while((file = (TSystemFile*)next_element())) 
	{
		temp = file->GetName();
		if(temp.size() < prefix.size() || temp.size() < suffix.size())
			continue;
		else if(!file->IsDirectory() && !temp.compare(0,prefix.size(),prefix) && 
				!temp.compare(temp.size()-(1+suffix.size()), suffix.size(), suffix)) 
		{
	  		fname = dir+temp;
	  		m_filelist.push_back(fname);
		} 
	}

	delete flist;
	if(m_filelist.size()>0) 
		return true;
	else 
	{
		cerr<<"Unable to find files with matching run name in directory; check input.txt"<<endl;
		return false;
	}
}

std::string RunCollector::GrabFile(int runNum) {
	if(!m_initFlag) 
		return "";
	TSystemDirectory sysdir(dir.Data(), dir.Data());
	TList* flist = sysdir.GetListOfFiles();

	if(!flist)
		return "";

	TSystemFile *file;
	std::string fname = "", temp;
	std::string runno = "_"+to_string(runNum)+m_suffix;
	TIter next_element(flist);
	while((file = (TSystemFile*)next_element())) 
	{
		temp = file->GetName();
		if(temp.size() < m_prefix.size() || temp.size() < runno.size())
			continue;
		else if(!file->IsDirectory() && !temp.compare(0,m_prefix.size(),m_prefix) && 
				!temp.compare(temp.size()-(1+runno.size()),runno.size(), runno)) 
		{
			fname = dir+temp;
			break;
		}
	}

	delete flist;
	return fname;
}

/*Grabs all files within a specified run range*/
int RunCollector::GrabFilesInRange() 
{
	if(!m_initFlag)
		return false;

	TSystemDirectory sysdir(dir.Data(), dir.Data());
	TList *flist = sysdir.GetListOfFiles();
	m_filelist.clear();
	int counter = 0;

	if(!flist) 
	{
		cerr<<"Unable to find any files in directory; check name given to input.txt"<<endl;
		return false;
	}

	TSystemFile *file;
	std::string fname, temp;
	std::string runno;
	for(int i=m_minRun; i<=m_maxRun; i++) //loop over range
	{
		TIter next_element(flist);//list iterator
		runno = "_"+to_string(i) + m_suffix; //suffix is now _#.suffix
		while((file = (TSystemFile*)next_element())) //look through directory until file found
		{
			temp = file->GetName();
			if(temp.size() < m_prefix.size() || temp.size() < runno.size())
				continue;
			else if(!file->IsDirectory() && !temp.compare(0,m_prefix.size(),m_prefix) && 
					!temp.compare(temp.size()-(1+runno.size()),runno.size(), runno)) 
			{
				counter++;
				fname = dir+temp;
				m_filelist.push_back(fname);
				break; //if we find the file, break out of iterator loop
			}
		}
	}

	delete flist;
	if(counter>0)
		return true;
	else 
	{
		cerr<<"Unable to find files with matching run name in directory; check input.txt"<<endl;
		return false;
	}

}

bool RunCollector::Merge_hadd(const std::string& outname) 
{
	if(!m_initFlag) 
		false;

	if(m_maxRun == 0) 
	{
		if(GrabAllFiles()) 
		{ 
			std::string clump = "hadd "+outname;
			for(unsigned int i=0; i<filelist.size(); i++) 
				clump += " "+filelist[i];
			cout<<"Merging runs into single file..."<<endl;
			std::system(clump);
			cout<<"Finished merging"<<endl;
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
			for(unsigned int i=0; i<filelist.size(); i++)
				clump += " "+filelist[i];
			cout<<"Merging runs "<<m_minRun<<" to "<<m_maxRun<<" into a single file..."<<endl;
			system(clump);
			cout<<"Finished merging"<<endl;
			return true;
		} 
		else 
			return false;
	}
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
				chain->Add(m_filelist[i].Data());
			cout<<"Merging runs into single file..."<<endl;
			chain->Merge(output,0,"fast");
			cout<<"Finished merging"<<endl;
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
				chain->Add(m_filelist[i]);
			cout<<"Merging runs "<<m_minRun<<" to "<<m_maxRun<<" into a single file..."<<endl;
			chain->Merge(output,0,"fast");
			cout<<"Finished merging"<<endl;
			return true;
		} else
			return false;
	}

	if(output->IsOpen()) 
		output->Close();
	return false;
}
