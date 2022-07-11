/*
	EVBApp.cpp
	Class which represents the API of the event building environment. Wraps together the core concepts
	of the event builder, from conversion to plotting. Even intended to be able to archive data.
	Currently under development.

	Written by G.W. McCann Oct. 2020
*/
#include "EventBuilder.h"
#include <cstdlib>
#include "EVBApp.h"
#include "RunCollector.h"
#include "CompassRun.h"
#include "SlowSort.h"
#include "FastSort.h"
#include "SFPAnalyzer.h"
#include "SFPPlotter.h"

namespace EventBuilder {
	
	EVBApp::EVBApp() :
		m_progressFraction(0.1)
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

	bool EVBApp::ReadConfigFile(const std::string& fullpath) 
	{
		EVB_INFO("Reading in EVB configuration from file {0}...", fullpath);
		std::ifstream input(fullpath);
		if(!input.is_open()) 
		{
			EVB_WARN("Read of EVB config failed, unable to open input file!");
			return false;
		}
		std::string junk;
	
		std::getline(input, junk);
		input>>junk>>m_params.workspaceDir;
		input>>junk;
		std::getline(input, junk);
		std::getline(input, junk);
		input>>junk>>m_params.channelMapFile;
		input>>junk>>m_params.scalerFile;
		input>>junk>>m_params.cutListFile;
		input>>junk>>m_params.ZT>>junk>>m_params.AT;
		input>>junk>>m_params.ZP>>junk>>m_params.AP;
		input>>junk>>m_params.ZE>>junk>>m_params.AE;
		input>>junk>>m_params.BField;
		input>>junk>>m_params.beamEnergy;
		input>>junk>>m_params.spsAngle;
		input>>junk;
		std::getline(input, junk);
		std::getline(input, junk);
		input>>junk>>m_params.timeShiftFile;
		input>>junk>>m_params.slowCoincidenceWindow;
		input>>junk>>m_params.fastCoincidenceWindowIonCh;
		input>>junk>>m_params.fastCoincidenceWindowSABRE;
		input>>junk;
		std::getline(input, junk);
		std::getline(input, junk);
		input>>junk>>m_params.runMin;
		input>>junk>>m_params.runMax;
	
		input.close();
	
		EVB_INFO("Successfully loaded EVB config.");
	
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
	
		output<<"-------Data Location----------"<<std::endl;
		output<<"WorkspaceDirectory: "<<m_params.workspaceDir<<std::endl;
		output<<"-------------------------------"<<std::endl;
		output<<"------Experimental Inputs------"<<std::endl;
		output<<"ChannelMapFile: "<<m_params.channelMapFile<<std::endl;
		output<<"ScalerFile: "<<m_params.scalerFile<<std::endl;
		output<<"CutListFile: "<<m_params.cutListFile<<std::endl;
		output<<"ZT: "<<m_params.ZT<<std::endl;
		output<<"AT: "<<m_params.AT<<std::endl;
		output<<"ZP: "<<m_params.ZP<<std::endl;
		output<<"AP: "<<m_params.AP<<std::endl;
		output<<"ZE: "<<m_params.ZE<<std::endl;
		output<<"AE: "<<m_params.AE<<std::endl;
		output<<"BField(G): "<<m_params.BField<<std::endl;
		output<<"BeamKE(MeV): "<<m_params.beamEnergy<<std::endl;
		output<<"Theta(deg): "<<m_params.spsAngle<<std::endl;
		output<<"-------------------------------"<<std::endl;
		output<<"-------Timing Information------"<<std::endl;
		output<<"BoardOffsetFile: "<<m_params.timeShiftFile<<std::endl;
		output<<"SlowCoincidenceWindow(ps): "<<m_params.slowCoincidenceWindow<<std::endl;
		output<<"FastCoincidenceWindow_IonCh(ps): "<<m_params.fastCoincidenceWindowIonCh<<std::endl;
		output<<"FastCoincidenceWindow_SABRE(ps): "<<m_params.fastCoincidenceWindowSABRE<<std::endl;
		output<<"-------------------------------"<<std::endl;
		output<<"--------Run Information--------"<<std::endl;
		output<<"MinRun: "<<m_params.runMin<<std::endl;
		output<<"MaxRun: "<<m_params.runMax<<std::endl;
		output<<"-------------------------------"<<std::endl;
	
		output.close();
	
		EVB_INFO("Successfully wrote config to file.");
	
	}
	
	void EVBApp::PlotHistograms() 
	{
		std::string analyze_dir = m_params.workspaceDir+"/analyzed/";
		std::string plot_file = m_params.workspaceDir+"/histograms/run_"+std::to_string(m_params.runMin)+"_"+std::to_string(m_params.runMax)+".root";
		SFPPlotter grammer;
		grammer.SetProgressCallbackFunc(m_progressCallback);
		grammer.SetProgressFraction(m_progressFraction);
		grammer.ApplyCutlist(m_params.cutListFile);
		EVB_INFO("Generating histograms from analyzed runs [{0}, {1}] with Cut List {2}...", m_params.runMin, m_params.runMax, m_params.cutListFile);
		EVB_INFO("Output file will be named {0}",plot_file);
	
		grabber.SetSearchParams(analyze_dir, "", ".root", m_params.runMin, m_params.runMax);
		if(grabber.GrabFilesInRange()) 
		{
			grammer.Run(grabber.GetFileList(), plot_file);
			EVB_INFO("Finished.");
		} 
		else 
		{
			EVB_ERROR("Unable to find analyzed run files at EVBApp::PlotHistograms()!");
		}
	}
	
	void EVBApp::Convert2RawRoot() 
	{
		int sys_return;
		std::string rawroot_dir = m_params.workspaceDir+"/raw_root/";
		std::string unpack_dir = m_params.workspaceDir+"/temp_binary/";
		std::string binary_dir = m_params.workspaceDir+"/raw_binary/";
		EVB_INFO("Converting binary archives to ROOT files over run range [{0}, {1}]",m_params.runMin,m_params.runMax);
	
		grabber.SetSearchParams(binary_dir, "", ".tar.gz",0,1000);
	
		std::string rawfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(m_params);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...", binfile);
			rawfile = rawroot_dir + "compass_run_"+ std::to_string(i) + ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.BIN";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2RawRoot(rawfile);
			sys_return = system(wipe_command.c_str());
	
		}
		EVB_INFO("Conversion complete.");
	}
	
	void EVBApp::MergeROOTFiles() 
	{
		std::string merge_file = m_params.workspaceDir+"/merged/run_"+std::to_string(m_params.runMin)+"_"+std::to_string(m_params.runMax)+".root";
		std::string file_dir = m_params.workspaceDir.string()+"/analyzed/";
		EVB_INFO("Merging ROOT files into single file for runs in range [{0}, {1}]", m_params.runMin, m_params.runMax);
		EVB_INFO("Merged file will be named {0}", merge_file);
		std::string prefix = "";
		std::string suffix = ".root";
		grabber.SetSearchParams(file_dir, prefix, suffix,m_params.runMin,m_params.runMax);
		EVB_INFO("Starting merge...");
		if(!grabber.Merge_TChain(merge_file)) 
		{
			EVB_ERROR("Unable to find files for merge at EVBApp::MergeROOTFiles()!");
			return;
		}
		EVB_INFO("Finished.");
	}
	
	void EVBApp::Convert2SortedRoot() 
	{
		int sys_return;
		std::string sortroot_dir = m_params.workspaceDir+"/sorted/";
		std::string unpack_dir = m_params.workspaceDir+"/temp_binary/";
		std::string binary_dir = m_params.workspaceDir+"/raw_binary/";
		EVB_INFO("Converting binary archives to event built ROOT files over run range [{0}, {1}]",m_params.runMin,m_params.runMax);
	
		grabber.SetSearchParams(binary_dir,"",".tar.gz",m_params.runMin,m_params.runMax);
	
		std::string sortfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(m_params);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
	
		int count=0;
		for(int i=m_params.runMin; i<= m_params.runMax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...",binfile);
	
			sortfile = sortroot_dir +"run_"+std::to_string(i)+ ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.BIN";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2SortedRoot(sortfile);
			sys_return = system(wipe_command.c_str());
			count++;
		}
		if(count==0)
			EVB_WARN("Conversion failed, no archives were found!");
		else
			EVB_INFO("Conversion complete.");
	}
	
	void EVBApp::Convert2FastSortedRoot() {
		int sys_return;
		std::string sortroot_dir = m_params.workspaceDir+"/fast/";
		std::string unpack_dir = m_params.workspaceDir+"/temp_binary/";
		std::string binary_dir = m_params.workspaceDir+"/raw_binary/";
		EVB_INFO("Converting binary archives to fast event built ROOT files over run range [{0}, {1}]",m_params.runMin,m_params.runMax);
	
		grabber.SetSearchParams(binary_dir,"",".tar.gz",m_params.runMin,m_params.runMax);
	
		std::string sortfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(m_params);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		int count=0;
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...",binfile);
	
			sortfile = sortroot_dir + "run_" + std::to_string(i) + ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.BIN";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2FastSortedRoot(sortfile);
			sys_return = system(wipe_command.c_str());
			count++;
		}
		if(count==0)
			EVB_WARN("Conversion failed, no archives were found!");
		else
			EVB_INFO("Conversion complete.");
	}
	
	void EVBApp::Convert2SlowAnalyzedRoot() {
		int sys_return;
		std::string sortroot_dir = m_params.workspaceDir+"/analyzed/";
		std::string unpack_dir = m_params.workspaceDir+"/temp_binary/";
		std::string binary_dir = m_params.workspaceDir+"/raw_binary/";
		EVB_INFO("Converting binary archives to analyzed event built ROOT files over run range [{0}, {1}]",m_params.runMin,m_params.runMax);

		grabber.SetSearchParams(binary_dir,"",".tar.gz",m_params.runMin, m_params.runMax);
	
		std::string sortfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(m_params);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		int count=0;
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...",binfile);
	
			sortfile = sortroot_dir + "run_" + std::to_string(i) + ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.BIN";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2SlowAnalyzedRoot(sortfile);
			sys_return = system(wipe_command.c_str());
			count++;
		}
		if(count==0)
			EVB_WARN("Conversion failed, no archives were found!");
		else
			EVB_INFO("Conversion complete.");
	}
	
	void EVBApp::Convert2FastAnalyzedRoot() 
	{
		int sys_return;
		std::string sortroot_dir = m_params.workspaceDir+"/analyzed/";
		std::string unpack_dir = m_params.workspaceDir+"/temp_binary/";
		std::string binary_dir = m_params.workspaceDir+"/raw_binary/";
		EVB_INFO("Converting binary archives to analyzed fast event built ROOT files over run range [{0}, {1}]",m_params.runMin,m_params.runMax);
		
		grabber.SetSearchParams(binary_dir,"",".tar.gz",m_params.runMin,m_params.runMax);
	
		std::string sortfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(m_params);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		int count=0;
		for(int i=m_params.runMin; i<=m_params.runMax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...",binfile);
	
			sortfile = sortroot_dir + "run_" + std::to_string(i) + ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.BIN";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2FastAnalyzedRoot(sortfile);
			sys_return = system(wipe_command.c_str());
			count++;
		}
		if(count==0)
			EVB_WARN("Conversion failed, no archives were found!");
		else
			EVB_INFO("Conversion complete.");
	}

}