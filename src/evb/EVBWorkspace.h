#ifndef EVB_WORKSPACE_H
#define EVB_WORKSPACE_H

namespace EventBuilder {

    class EVBWorkspace
    {
    public:
        EVBWorkspace(const std::filesystem::path& workspace);
        ~EVBWorkspace();

        inline const bool IsValid() const { return m_isValid; }

        inline std::filesystem::path GetBinaryDir() const { return m_binaryDir; }
        inline std::filesystem::path GetTempDir() const { return m_tempDir; }
        inline std::filesystem::path GetSortedDir() const { return m_sortedDir; }
        inline std::filesystem::path GetBuiltDir() const { return m_builtDir; }
        inline std::filesystem::path GetAnalyzedDir() const { return m_analyzedDir; }
        inline std::filesystem::path GetHistogramDir() const { return m_histogramDir; }
        inline std::filesystem::path GetCutDir() const { return m_cutDir; }

        std::vector<std::string> GetBinaryRunRange(int runMin, int runMax);
        std::string GetBinaryRun(int run);
        std::vector<std::string> GetTempFiles();

    private:
        void Init();
        bool m_isValid;

        std::filesystem::path m_workspace;

        std::filesystem::path m_binaryDir;
        std::filesystem::path m_tempDir;
        std::filesystem::path m_sortedDir;
        std::filesystem::path m_builtDir;
        std::filesystem::path m_analyzedDir;
        std::filesystem::path m_histogramDir;
        std::filesystem::path m_cutDir;
    };
}

#endif