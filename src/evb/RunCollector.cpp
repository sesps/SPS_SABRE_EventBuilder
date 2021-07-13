#include "EventBuilder.h"
#include "RunCollector.h"

using namespace std;

RunCollector::RunCollector():
  initFlag(false), dir(""), run(""), end(""), MaxRun(0), MinRun(0)
{
}

RunCollector::RunCollector(const string& dirname, const string& prefix, const string& suffix) {
  dir = dirname.c_str();
  run = prefix.c_str();
  end = suffix.c_str();

  MinRun = 0; MaxRun = LITERALMAX;
  initFlag = true;
}

RunCollector::RunCollector(const string& dirname, const string& prefix, const string& suffix, int min, int max) {
  dir = dirname.c_str();
  run = prefix.c_str();
  end = suffix.c_str();

  MinRun = min; MaxRun = max;
  initFlag = true;
}

RunCollector::~RunCollector() {}

void RunCollector::SetSearchParams(const string& dirname, const string& prefix, const string& suffix, int min, int max) {
  dir = dirname.c_str();
  run = prefix.c_str();
  end = suffix.c_str();
  MinRun = min; MaxRun = max;
  initFlag = true;
}

int RunCollector::GrabAllFiles() {
  if(!initFlag) {return 0;}
  TSystemDirectory sysdir(dir.Data(), dir.Data());
  TList *flist = sysdir.GetListOfFiles();
  filelist.clear();
  int counter = 0;
  if(flist) { //Make sure list is real. If not, means no directory
    TSystemFile *file;
    TString fname, temp;
    TIter next_element(flist); //List iterator
    while((file = (TSystemFile*)next_element())) {
      temp = file->GetName();
      if(!file->IsDirectory() && temp.BeginsWith(run.Data()) && temp.EndsWith(end.Data())) {
        counter++;
        fname = dir+temp;
        filelist.push_back(fname);
      } 
    }
    if(counter>0) {
      delete flist;
      return 1;
    } else {
      cerr<<"Unable to find files with matching run name in directory; check input.txt"<<endl;
      delete flist;
      return 0;
    }
  } else {
    cerr<<"Unable to find any files in directory; check name given to the input.txt"<<endl;
    delete flist;
    return 0;
  } 
}

std::string RunCollector::GrabFile(int runNum) {
  if(!initFlag) return "";
  TSystemDirectory sysdir(dir.Data(), dir.Data());
  TList* flist = sysdir.GetListOfFiles();

  if(!flist) return "";

  TSystemFile *file;
  TString fname = "", temp;
  string runno = "_"+to_string(runNum)+end.Data();
  TIter next_element(flist);
  while((file = (TSystemFile*)next_element())) {
    temp = file->GetName();
    if(!file->IsDirectory() && temp.BeginsWith(run.Data()) && temp.EndsWith(runno.c_str())) {
      fname = dir+temp;
      break;
    }
  }

  return fname.Data();
}

/*Grabs all files within a specified run range*/
int RunCollector::GrabFilesInRange() {
  if(!initFlag) {return 0;}
  TSystemDirectory sysdir(dir.Data(), dir.Data());
  TList *flist = sysdir.GetListOfFiles();
  filelist.clear();
  int counter = 0;
  if(flist) {
    TSystemFile *file;
    TString fname, temp;
    string runno;
    for(int i=MinRun; i<=MaxRun; i++) {//loop over range
      TIter next_element(flist);//list iterator
      runno = "_"+to_string(i) + end.Data(); //suffix is now _#.endData
      while((file = (TSystemFile*)next_element())) {//look through directory until file found
        temp = file->GetName();
        if(!file->IsDirectory()&&temp.BeginsWith(run.Data())&&temp.EndsWith(runno.c_str())){
          counter++;
          fname = dir+temp;
          filelist.push_back(fname);
          break; //if we find the file, break out of iterator loop
        }
      }
    }
    if(counter>0) {
      delete flist;
      return 1;
    } else {
      cerr<<"Unable to find files with matching run name in directory; check input.txt"<<endl;
      delete flist;
      return 0;
    }
  } else {
    cerr<<"Unable to find any files in directory; check name given to input.txt"<<endl;
    delete flist;
    return 0;
  }
}

int RunCollector::Merge_hadd(const string& outname) {
  if(!initFlag) {return 0;}
  if(MaxRun == LITERALMAX) {
    if(GrabAllFiles()) { 
      TString clump = "hadd "+outname;
      for(unsigned int i=0; i<filelist.size(); i++) {
        clump += " "+filelist[i];
      }
      cout<<"Merging runs into single file..."<<endl;
      system(clump.Data());
      cout<<"Finished merging"<<endl;
      return 1;
    } else {
      return 0;
    }
  } else {
    if(GrabFilesInRange()) {
      TString clump = "hadd "+outname;
      for(unsigned int i=0; i<filelist.size(); i++) {
        clump += " "+filelist[i];
      }
      cout<<"Merging runs "<<MinRun<<" to "<<MaxRun<<" into a single file..."<<endl;
      system(clump.Data());
      cout<<"Finished merging"<<endl;
      return 1;
    } else {
      return 0;
    }
  }
}

int RunCollector::Merge_TChain(const string& outname) {
  if(!initFlag) {return 0;}
  TFile *output = new TFile(outname.c_str(), "RECREATE");
  TChain *chain = new TChain("SPSTree", "SPSTree");
  
  if(MaxRun == LITERALMAX) {
    if(GrabAllFiles()) { 
      for(unsigned int i=0; i<filelist.size(); i++) {
        chain->Add(filelist[i].Data());
      }
      cout<<"Merging runs into single file..."<<endl;
      chain->Merge(output,0,"fast");
      cout<<"Finished merging"<<endl;
      return 1;
    } else {
      return 0;
    }
  } else {
    if(GrabFilesInRange()) {
      for(unsigned int i=0; i<filelist.size(); i++) {
        chain->Add(filelist[i]);
      }
      cout<<"Merging runs "<<MinRun<<" to "<<MaxRun<<" into a single file..."<<endl;
      chain->Merge(output,0,"fast");
      cout<<"Finished merging"<<endl;
      return 1;
    } else {
      return 0;
    }
  }
  if(output->IsOpen()) output->Close();
  return 0;
}
