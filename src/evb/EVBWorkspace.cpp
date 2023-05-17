#include "EVBWorkspace.h"
#include <filesystem>
#include <TFile.h>
#include <TChain.h>

namespace EventBuilder {

    static bool CheckSubDirectory(const std::string& path)
    {
        bool status = true;
        EVB_TRACE("Checking subdirectory {0}", path);
        if(!std::filesystem::exists(path))
        {
            status = std::filesystem::create_directory(path);
            if(!status)
            {
                EVB_ERROR("Unable to create subdirectory {0}. Please check the pathing.", path);
                return status;
            }
            EVB_INFO("Created subdirectory {0}", path);
        }
        else
            EVB_INFO("Found subdirectory {0}", path);
        return status;
    }

    EVBWorkspace::EVBWorkspace(const std::string& workspace) :
        m_isValid(false), m_workspace(workspace)
    {
        Init();
    }

    EVBWorkspace::~EVBWorkspace() {}

    void EVBWorkspace::Init()
    {
        m_isValid = true;
        if(!std::filesystem::exists(m_workspace))
        {
            EVB_TRACE("Workspace {0} does not exist. Attempting to create the workspace directory...", m_workspace);
            m_isValid = std::filesystem::create_directory(m_workspace);
            if(!m_isValid)
            {
                EVB_ERROR("Unable to create workspace {0}. Please check the pathing.", m_workspace);
                return;
            }
            EVB_INFO("Created workspace directory {0}.", m_workspace);
        }
        else
            EVB_INFO("Found workspace directory {0}.", m_workspace);

        EVB_TRACE("Looking for required workspace subdirectories...");

        m_binaryDir = m_workspace + "raw_binary/";
        m_tempDir = m_workspace + "temp_binary/";
        m_sortedDir = m_workspace + "sorted/";
        m_builtDir = m_workspace + "built/";
        m_analyzedDir = m_workspace + "analyzed/";
        m_histogramDir = m_workspace + "histograms/";
        m_cutDir = m_workspace + "cuts/";
        m_mergedDir = m_workspace + "merged/";

        //Check all subdirectories. Terminate if any of them are bad
        m_isValid = CheckSubDirectory(m_binaryDir);
        if(!m_isValid)
            return;
        m_isValid = CheckSubDirectory(m_tempDir);
        if(!m_isValid)
            return;
        m_isValid = CheckSubDirectory(m_sortedDir);
        if(!m_isValid)
            return;
        m_isValid = CheckSubDirectory(m_builtDir);
        if(!m_isValid)
            return;
        m_isValid = CheckSubDirectory(m_analyzedDir);
        if(!m_isValid)
            return;
        m_isValid = CheckSubDirectory(m_histogramDir);
        if(!m_isValid)
            return;
        m_isValid = CheckSubDirectory(m_cutDir);
        if(!m_isValid)
            return;
        m_isValid = CheckSubDirectory(m_mergedDir);
    }

    std::string EVBWorkspace::GetBinaryRun(int run)
    {
        std::string file;
        std::string runID = "run_" + std::to_string(run) + ".tar.gz";
        for(auto& entry : std::filesystem::directory_iterator(m_binaryDir))
        {
            if(entry.is_regular_file() && entry.path().filename().string() == runID)
            {
                return entry.path().string();
            }
        }
        return "";
    }

    std::string EVBWorkspace::GetAnalyzedRun(int run)
    {
        std::string file;
        std::string runID = "run_" + std::to_string(run) + ".root";
        for(auto& entry : std::filesystem::directory_iterator(m_analyzedDir))
        {
            if(entry.is_regular_file() && entry.path().filename().string() == runID)
            {
                return entry.path().string();
            }
        }
        return "";
    }

    std::vector<std::string> EVBWorkspace::GetBinaryRunRange(int runMin, int runMax)
    {
        std::vector<std::string> list;
        std::string temp;
        for(int run=runMin; run<=runMax; run++)
            temp = GetBinaryRun(run);
            if(!temp.empty())
                list.push_back(temp);

        return list;
    }

    std::vector<std::string> EVBWorkspace::GetAnalyzedRunRange(int runMin, int runMax)
    {
        std::vector<std::string> list;
        std::string temp;
        for(int run=runMin; run<=runMax; run++)
        {   
            temp = GetAnalyzedRun(run);
            if(!temp.empty())
                list.push_back(temp);
        }

        return list;
    }

    bool EVBWorkspace::UnpackBinaryRunToTemp(int run)
    {
        std::string runfile = GetBinaryRun(run);
        std::string unpack_command = "tar -xzf "+runfile+" --directory "+m_tempDir;
		int	sys_return = system(unpack_command.c_str());
        if(sys_return == 0)
            return true;
        else
            return false;
    }

    std::vector<std::string> EVBWorkspace::GetTempFiles()
    {
        std::vector<std::string> list;
        for(auto& entry : std::filesystem::directory_iterator(m_tempDir))
        {
            if(entry.is_regular_file() && entry.path().filename().extension().string() == ".BIN")
                list.push_back(entry.path().string());
        }

        return list;
    }

    bool EVBWorkspace::ClearTempDirectory()
    {
        std::vector<std::filesystem::path> files;
        for(auto& entry : std::filesystem::directory_iterator(m_tempDir))
        {
            if(entry.is_regular_file())
                files.push_back(entry.path());
            else
                EVB_WARN("Detected non-file element in temp directory {0} named {1}", m_tempDir, entry.path().string());
        }

        for(size_t i=0; i<files.size(); i++)
        {
            if(!std::filesystem::remove(files[i]))
            {
                EVB_ERROR("Unable to clear temp directory {0}! File-like entry {1} could not be deleted. Please manually clear the directory.",
                          m_tempDir, files[i].string());
                return false;
            }
        }

        return true;
    }

    bool EVBWorkspace::MergeAnalyzedFiles(const std::string& outputname, int runMin, int runMax)
    {
        auto files = GetAnalyzedRunRange(runMin, runMax);
        if(files.size() == 0)
            return false;

        TFile* output = TFile::Open(outputname.c_str(), "RECREATE");
        if(!output || !output->IsOpen())
        {
            EVB_ERROR("Could not open output file {0} for merge", outputname);
            output->Close();
            return false;
        }

        TChain* chain = new TChain("SPSTree", "SPSTree");
        for(auto& entry : files)
        {   
            EVB_INFO("Merging file: {0}", entry);
            chain->Add(entry.c_str());
        }
        //Note: TChain merge is kinda fucked. It requires a file passed in, which it then implicitly takes ownership of.
        //By passing the keep option, we tell TChain we want to keep ownership of the file (otherwise it will automatically close and delete)
        //We need ownership because the Merge function does not do a good job of handling errors explicitly.
        chain->Merge(output, 0, "fast|keep");

        output->Close();
        return true;
    }
}