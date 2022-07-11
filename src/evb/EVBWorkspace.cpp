#include "EVBWorkspace.h"

namespace EventBuilder {

    static bool CheckSubDirectory(const std::filesystem::path& path)
    {
        bool status = true;
        EVB_TRACE("Checking subdirectory {0}", path);
        if(!std::filesystem::exists(path))
        {
            status = std::filesystem::create_directory(path);
            if(!status)
            {
                EVB_ERROR("Unable to create subdirectory {0}. Please check the pathing.", path.string());
                return status;
            }
            EVB_INFO("Created subdirectory {0}", path);
        }
        else
            EVB_INFO("Found subdirectory {0}", path);
        return status;
    }

    EVBWorkspace::EVBWorkspace(const std::filesystem::path& workspace) :
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
            EVB_TRACE("Workspace {0} does not exist. Attempting to create the workspace directory...", m_workspace.string());
            m_isValid = std::filesystem::create_directory(m_workspace);
            if(!m_isValid)
            {
                EVB_ERROR("Unable to create workspace {0}. Please check the pathing.", m_workspace.string());
                return;
            }
            EVB_INFO("Created workspace directory {0}.", m_workspace.string());
        }
        else
            EVB_INFO("Found workspace directory {0}.", m_workspace.string());

        EVB_TRACE("Looking for required workspace subdirectories...");

        m_binaryDir = m_workspace / "raw_binary";
        m_tempDir = m_workspace / "temp_binary";
        m_sortedDir = m_workspace / "sorted";
        m_builtDir = m_workspace / "built";
        m_analyzedDir = m_workspace / "analyzed";
        m_histogramDir = m_workspace / "histograms";
        m_cutDir = m_workspace / "cuts";

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
}