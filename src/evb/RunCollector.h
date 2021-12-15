/*RunCollector.h
 *Class that searches through a directory looking for files of a specified format.
 *Stores all filenames in a vector which can be accessed by other functions/classes for
 *further use. Can also use Merge() to combine all files using hadd into a single file.
 *Merge() is NOT RECOMMENDED in the analyzer program.
 *
 *Created Jan2020 by GWM
 */

#ifndef RUNCOLLECTOR_H
#define RUNCOLLECTOR_H

namespace EventBuilder {

	class RunCollector 
	{
	public:
		RunCollector();
		RunCollector(const std::string& dirname, const std::string& prefix, const std::string& suffix);
		RunCollector(const std::string& dirname, const std::string& prefix, const std::string& suffix, int min, int max);
		~RunCollector();
		void SetSearchParams(const std::string& dirname, const std::string& prefix, const std::string& suffix, int min, int max);
		bool Merge_hadd(const std::string& outname);
		bool Merge_TChain(const std::string& outname);
		bool GrabAllFiles();
		bool GrabFilesInRange();
		std::string GrabFile(int runNum);
		inline std::string GetSearchDir() { return m_directory; }
		inline std::string GetSearchPrefix() { return m_prefix; }
		inline std::string GetSearchSuffix() { return m_suffix; }
		inline int GetRunMin() { return m_minRun; }
		inline int GetRunMax() { return m_maxRun; }
		inline const std::vector<std::string>& GetFileList() { return m_filelist; }
	
	private:
		bool m_initFlag;
		std::string m_directory;
		std::string m_prefix;
		std::string m_suffix;
		int m_minRun, m_maxRun;  //user run limits
		const int m_maxAllowedRuns = 1000; //class run limit
		std::vector<std::string> m_filelist;
		
	};

}
#endif
