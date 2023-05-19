/*
	EVBApp.cpp
	Class which represents the API of the event building environment. Wraps together the core concepts
	of the event builder, from conversion to plotting. Even intended to be able to archive data.
	Currently under development.

	Written by G.W. McCann Oct. 2020
*/
#include <cstdlib>
#include "EVBApp.h"
#include "CompassRun.h"
#include "SFPPlotter.h"
#include "yaml-cpp/yaml.h"

namespace EventBuilder {
	
	EVBApp::EVBApp() :
		m_workspace(nullptr), m_progressFraction(0.1)
	{
		SetProgressCallbackFunc(BIND_PROGRESS_CALLBACK_FUNCTION(EVBApp::DefaultProgressCallback));
	}
	
	EVBApp::~EVBApp() 
	{
	}
	
	void EVBApp::DefaultProgressCallback(long curVal, long totalVal)
	{
		double fraction = ((double)curVal)/totalVal;
		EVB_INFO("Percent of run built: {0}", fraction*100);
	}

	void EVBApp::SetParameters(const EVBParameters& params)
	{
		if(m_params.workspaceDir != params.workspaceDir)
		{
			m_workspace.reset(new EVBWorkspace(params.workspaceDir));
			if(!m_workspace->IsValid())
				EVB_ERROR("Unable to process new parameters due to bad workspace");
		}

		m_params = params;
	}

	bool EVBApp::ReadConfigFile(const std::string& fullpath) 
	{
		EVB_INFO("Reading in EVB configuration from file {0}...", fullpath);
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(fullpath);
		}
		catch(YAML::ParserException& e)
		{
			EVB_ERROR("Read of EVB config failed, unable to open input file!");
			return false;
		}
		m_params.workspaceDir = data["WorkspaceDir"].as<std::string>();
		m_params.channelMapFile = data["ChannelMap"].as<std::string>();
		m_params.scalerFile = data["ScalerFile"].as<std::string>();
		m_params.cutListFile = data["CutListFile"].as<std::string>();
		m_params.timeShiftFile = data["TimeShiftFile"].as<std::string>();
		m_params.slowCoincidenceWindow = data["SlowCoincidenceWindow(ps)"].as<double>();
		m_params.fastCoincidenceWindowIonCh = data["FastCoincidenceWinowIonCh(ps)"].as<double>();
		m_params.fastCoincidenceWindowSABRE = data["FastCoincidenceWinowSABRE(ps)"].as<double>();
		m_params.ZT = data["ZT"].as<int>();
		m_params.AT = data["AT"].as<int>();
		m_params.ZP = data["ZP"].as<int>();
		m_params.AP = data["AP"].as<int>();
		m_params.ZE = data["ZE"].as<int>();
		m_params.AE = data["AE"].as<int>();
		m_params.BField = data["BField(kG)"].as<double>();
		m_params.beamEnergy = data["BeamEnergy(MeV)"].as<double>();
		m_params.spsAngle = data["SPSAngle(deg)"].as<double>();
		m_params.nudge = data["Nudge(cm)"].as<double>();
		m_params.runMin = data["MinRun"].as<int>();
		m_params.runMax = data["MaxRun"].as<int>();
	
		EVB_INFO("Successfully loaded EVB config.");
	
		m_workspace.reset(new EVBWorkspace(m_params.workspaceDir));
		if(!m_workspace->IsValid())
		{
			EVB_ERROR("Unable to process input configuration due to bad workspace.");
			return false;
		}
		return true;
	}
	
	void EVBApp::WriteConfigFile(const std::string& fullpath) 
	{
	
		EVB_INFO("Writing EVB config to file {0}...",fullpath);
		std::ofstream output(fullpath);
		if(!output.is_open()) 
		{
			EVB_WARN("Failed to write to config to file {0}, unable to open file!", fullpath);
			return;
		}

		YAML::Emitter yamlStream;
		yamlStream << YAML::BeginMap;
		yamlStream << YAML::Key << "WorkspaceDir" << YAML::Value << m_params.workspaceDir;
		yamlStream << YAML::Key << "ChannelMap" << YAML::Value << m_params.channelMapFile;
		yamlStream << YAML::Key << "ScalerFile" << YAML::Value <<  m_params.scalerFile;
		yamlStream << YAML::Key << "CutListFile" << YAML::Value << m_params.cutListFile;
		yamlStream << YAML::Key << "TimeShiftFile" << YAML::Value << m_params.timeShiftFile;
		yamlStream << YAML::Key << "SlowCoincidenceWindow(ps)" << YAML::Value << m_params.slowCoincidenceWindow;
		yamlStream << YAML::Key << "FastCoincidenceWinowIonCh(ps)" << YAML::Value << m_params.fastCoincidenceWindowIonCh;
		yamlStream << YAML::Key << "FastCoincidenceWinowSABRE(ps)" << YAML::Value << m_params.fastCoincidenceWindowSABRE;
		yamlStream << YAML::Key << "ZT" << YAML::Value << m_params.ZT;
		yamlStream << YAML::Key << "AT" << YAML::Value << m_params.AT;
		yamlStream << YAML::Key << "ZP" << YAML::Value << m_params.ZP;
		yamlStream << YAML::Key << "AP" << YAML::Value << m_params.AP;
		yamlStream << YAML::Key << "ZE" << YAML::Value << m_params.ZE;
		yamlStream << YAML::Key << "AE" << YAML::Value << m_params.AE;
		yamlStream << YAML::Key << "BField(kG)" << YAML::Value << m_params.BField;
		yamlStream << YAML::Key << "BeamEnergy(MeV)" << YAML::Value << m_params.beamEnergy;
		yamlStream << YAML::Key << "SPSAngle(deg)" << YAML::Value << m_params.spsAngle;
		yamlStream << YAML::Key << "Nudge(cm)" << YAML::Value << m_params.nudge;
		yamlStream << YAML::Key << "MinRun" << YAML::Value << m_params.runMin;
		yamlStream << YAML::Key << "MaxRun" << YAML::Value << m_params.runMax;
		yamlStream << YAML::EndMap;

		output << yamlStream.c_str();
		
		output.close();
	
		EVB_INFO("Successfully wrote config to file.");
	
	}
	
	void EVBApp::PlotHistograms() 
	{
		if(m_workspace == nullptr || !m_workspace->IsValid())
		{
			EVB_ERROR("Unable to preform event building request due to bad workspace.");
		}

		std::string plot_file = m_workspace->GetHistogramDir()+"run_"+std::to_string(m_params.runMin)+"_"+std::to_string(m_params.runMax)+".root";
		SFPPlotter grammer;
		grammer.SetProgressCallbackFunc(m_progressCallback);
		grammer.SetProgressFraction(m_progressFraction);
		grammer.ApplyCutlist(m_params.cutListFile);
		EVB_INFO("Generating histograms from analyzed runs [{0}, {1}] with Cut List {2}...", m_params.runMin, m_params.runMax, m_params.cutListFile);
		EVB_INFO("Output file will be named {0}",plot_file);
	
		auto files = m_workspace->GetAnalyzedRunRange(m_params.runMin, m_params.runMax);
		if(files.size() > 0) 
		{
			grammer.Run(files, plot_file);
			EVB_INFO("Finished.");
		} 
		else 
		{
			EVB_ERROR("Unable to find analyzed run files at EVBApp::PlotHistograms()!");
		}
	}
	
	void EVBApp::Convert2RawRoot() 
	{
		if(m_workspace == nullptr || !m_workspace->IsValid())
		{
			EVB_ERROR("Unable to preform event building request due to bad workspace.");
		}

		std::string rawroot_dir = m_workspace->GetSortedDir();
		EVB_INFO("Converting binary archives to ROOT files over run range [{0}, {1}]", m_params.runMin, m_params.runMax);
	
		std::string rawfile;
	
		CompassRun converter(m_params, m_workspace);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		int count = 0;
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			rawfile = rawroot_dir + "compass_run_"+ std::to_string(i) + ".root";
			EVB_INFO("Converting file {0}...", rawfile);
			m_workspace->ClearTempDirectory(); //In case something weird happened
			if(m_workspace->UnpackBinaryRunToTemp(i))
			{
				converter.SetRunNumber(i);
				converter.Convert2RawRoot(rawfile);
				++count;
			}
			m_workspace->ClearTempDirectory();
		}

		if(count != 0)
			EVB_INFO("Conversion complete.");
		else
			EVB_WARN("Nothing converted, no files found in the range [{0}, {1}]", m_params.runMin, m_params.runMax);
	}
	
	void EVBApp::MergeROOTFiles() 
	{
		if(m_workspace == nullptr || !m_workspace->IsValid())
		{
			EVB_ERROR("Unable to preform event building request due to bad workspace.");
		}

		std::string merge_file = m_workspace->GetMergedDir()+"run_"+std::to_string(m_params.runMin)+"_"+std::to_string(m_params.runMax)+".root";
		EVB_INFO("Merging ROOT files into single file for runs in range [{0}, {1}]", m_params.runMin, m_params.runMax);
		EVB_INFO("Merged file will be named {0}", merge_file);
		EVB_INFO("Starting merge...");
		if(!m_workspace->MergeAnalyzedFiles(merge_file, m_params.runMin, m_params.runMax)) 
		{
			EVB_ERROR("Unable to merge at EVBApp::MergeROOTFiles()!");
			return;
		}
		EVB_INFO("Finished.");
	}
	
	void EVBApp::Convert2SortedRoot() 
	{
		if(m_workspace == nullptr || !m_workspace->IsValid())
		{
			EVB_ERROR("Unable to preform event building request due to bad workspace.");
		}

		std::string sortroot_dir = m_workspace->GetBuiltDir();
		EVB_INFO("Converting binary archives to event built ROOT files over run range [{0}, {1}]", m_params.runMin, m_params.runMax);
	
		std::string sortfile;

		CompassRun converter(m_params, m_workspace);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);

		EVB_INFO("Beginning conversion...");
		int count = 0;
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			sortfile = sortroot_dir + "run_"+ std::to_string(i) + ".root";
			EVB_INFO("Converting file {0}...", sortfile);
			m_workspace->ClearTempDirectory(); //In case something weird happened
			if(m_workspace->UnpackBinaryRunToTemp(i))
			{
				converter.SetRunNumber(i);
				converter.Convert2SortedRoot(sortfile);
				EVB_INFO("Finished converting");
				++count;
			}
			m_workspace->ClearTempDirectory();
		}

		if(count != 0)
			EVB_INFO("Conversion complete.");
		else
			EVB_WARN("Nothing converted, no files found in the range [{0}, {1}]", m_params.runMin, m_params.runMax);
	}
	
	void EVBApp::Convert2FastSortedRoot()
	{
		if(m_workspace == nullptr || !m_workspace->IsValid())
		{
			EVB_ERROR("Unable to preform event building request due to bad workspace.");
		}

		std::string sortroot_dir = m_workspace->GetBuiltDir();
		EVB_INFO("Converting binary archives to fast event built ROOT files over run range [{0}, {1}]", m_params.runMin, m_params.runMax);
	
	
		std::string sortfile;
	
		CompassRun converter(m_params, m_workspace);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		int count = 0;
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			sortfile = sortroot_dir + "run_"+ std::to_string(i) + ".root";
			EVB_INFO("Converting file {0}...", sortfile);
			m_workspace->ClearTempDirectory(); //In case something weird happened
			if(m_workspace->UnpackBinaryRunToTemp(i))
			{
				converter.SetRunNumber(i);
				converter.Convert2FastSortedRoot(sortfile);
				++count;
			}
			m_workspace->ClearTempDirectory();
		}

		if(count != 0)
			EVB_INFO("Conversion complete.");
		else
			EVB_WARN("Nothing converted, no files found in the range [{0}, {1}]", m_params.runMin, m_params.runMax);
	}
	
	void EVBApp::Convert2SlowAnalyzedRoot()
	{
		if(m_workspace == nullptr || !m_workspace->IsValid())
		{
			EVB_ERROR("Unable to preform event building request due to bad workspace.");
		}

		std::string sortroot_dir = m_workspace->GetAnalyzedDir();
		EVB_INFO("Converting binary archives to analyzed event built ROOT files over run range [{0}, {1}]",m_params.runMin,m_params.runMax);

		std::string sortfile;
	
		CompassRun converter(m_params, m_workspace);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);

		EVB_INFO("Beginning conversion...");
		int count = 0;
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			sortfile = sortroot_dir + "run_"+ std::to_string(i) + ".root";
			EVB_INFO("Converting file {0}...", sortfile);
			m_workspace->ClearTempDirectory(); //In case something weird happened
			if(m_workspace->UnpackBinaryRunToTemp(i))
			{
				converter.SetRunNumber(i);
				converter.Convert2SlowAnalyzedRoot(sortfile);
				++count;
			}
			m_workspace->ClearTempDirectory();
		}

		if(count != 0)
			EVB_INFO("Conversion complete.");
		else
			EVB_WARN("Nothing converted, no files found in the range [{0}, {1}]", m_params.runMin, m_params.runMax);
	}
	
	void EVBApp::Convert2FastAnalyzedRoot() 
	{
		if(m_workspace == nullptr || !m_workspace->IsValid())
		{
			EVB_ERROR("Unable to preform event building request due to bad workspace.");
		}

		std::string sortroot_dir = m_workspace->GetAnalyzedDir();
		EVB_INFO("Converting binary archives to analyzed fast event built ROOT files over run range [{0}, {1}]",m_params.runMin,m_params.runMax);
		
	
		std::string sortfile;
	
		CompassRun converter(m_params, m_workspace);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);

		EVB_INFO("Beginning conversion...");
		int count = 0;
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			sortfile = sortroot_dir + "run_"+ std::to_string(i) + ".root";
			EVB_INFO("Converting file {0}...", sortfile);
			m_workspace->ClearTempDirectory(); //In case something weird happened
			if(m_workspace->UnpackBinaryRunToTemp(i))
			{
				converter.SetRunNumber(i);
				converter.Convert2FastAnalyzedRoot(sortfile);
				++count;
			}
			m_workspace->ClearTempDirectory();
		}

		if(count != 0)
			EVB_INFO("Conversion complete.");
		else
			EVB_WARN("Nothing converted, no files found in the range [{0}, {1}]", m_params.runMin, m_params.runMax);
	}

}