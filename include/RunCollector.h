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

#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TCollection.h>
#include <TList.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

class RunCollector {
  public:
    RunCollector();
    RunCollector(const string& dirname, const string& prefix, const string& suffix);
    RunCollector(const string& dirname, const string& prefix, const string& suffix, int min, int max);
    ~RunCollector();
    void SetSearchParams(const string& dirname, const string& prefix, const string& suffix, int min, int max);
    int Merge_hadd(const string& outname);
    int Merge_TChain(const string& outname);
    int GrabAllFiles();
    int GrabFilesInRange();
    std::string GrabFile(int runNum);
    inline std::string GetSearchDir() {return dir.Data();};
    inline std::string GetSearchPrefix() {return run.Data();};
    inline std::string GetSearchSuffix() {return end.Data();};
    inline int GetRunMin() {return MinRun;};
    inline int GetRunMax() {return MaxRun;};
    vector<TString> filelist;

  private:
    bool initFlag;
    TString dir;
    TString run;
    TString end;
    Int_t MaxRun, MinRun;  //user run limits
    const Int_t LITERALMAX = 1000; //class run limit
};

#endif
