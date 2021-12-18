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
		m_rmin(0), m_rmax(0), m_ZT(0), m_AT(0), m_ZP(0), m_AP(0), m_ZE(0), m_AE(0), m_ZR(0), m_AR(0),
		m_B(0), m_Theta(0), m_BKE(0), m_progressFraction(0.1), m_workspace("none"), m_mapfile("none"), m_shiftfile("none"),
		m_cutList("none"), m_scalerfile("none"), m_SlowWindow(0), m_FastWindowIonCh(0), m_FastWindowSABRE(0)
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
		input>>junk>>m_workspace;
		input>>junk;
		std::getline(input, junk);
		std::getline(input, junk);
		input>>junk>>m_mapfile;
		input>>junk>>m_scalerfile;
		input>>junk>>m_cutList;
		input>>junk>>m_ZT>>junk>>m_AT;
		input>>junk>>m_ZP>>junk>>m_AP;
		input>>junk>>m_ZE>>junk>>m_AE;
		input>>junk>>m_B;
		input>>junk>>m_BKE;
		input>>junk>>m_Theta;
		input>>junk;
		std::getline(input, junk);
		std::getline(input, junk);
		input>>junk>>m_shiftfile;
		input>>junk>>m_SlowWindow;
		input>>junk>>m_FastWindowIonCh;
		input>>junk>>m_FastWindowSABRE;
		input>>junk;
		std::getline(input, junk);
		std::getline(input, junk);
		input>>junk>>m_rmin;
		input>>junk>>m_rmax;
	
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
		output<<"WorkspaceDirectory: "<<m_workspace<<std::endl;
		output<<"-------------------------------"<<std::endl;
		output<<"------Experimental Inputs------"<<std::endl;
		output<<"ChannelMapFile: "<<m_mapfile<<std::endl;
		output<<"ScalerFile: "<<m_scalerfile<<std::endl;
		output<<"CutListFile: "<<m_cutList<<std::endl;
		output<<"ZT: "<<m_ZT<<std::endl;
		output<<"AT: "<<m_AT<<std::endl;
		output<<"ZP: "<<m_ZP<<std::endl;
		output<<"AP: "<<m_AP<<std::endl;
		output<<"ZE: "<<m_ZE<<std::endl;
		output<<"AE: "<<m_AE<<std::endl;
		output<<"BField(G): "<<m_B<<std::endl;
		output<<"BeamKE(MeV): "<<m_BKE<<std::endl;
		output<<"Theta(deg): "<<m_Theta<<std::endl;
		output<<"-------------------------------"<<std::endl;
		output<<"-------Timing Information------"<<std::endl;
		output<<"BoardOffsetFile: "<<m_shiftfile<<std::endl;
		output<<"SlowCoincidenceWindow(ps): "<<m_SlowWindow<<std::endl;
		output<<"FastCoincidenceWindow_IonCh(ps): "<<m_FastWindowIonCh<<std::endl;
		output<<"FastCoincidenceWindow_SABRE(ps): "<<m_FastWindowSABRE<<std::endl;
		output<<"-------------------------------"<<std::endl;
		output<<"--------Run Information--------"<<std::endl;
		output<<"MinRun: "<<m_rmin<<std::endl;
		output<<"MaxRun: "<<m_rmax<<std::endl;
		output<<"-------------------------------"<<std::endl;
	
		output.close();
	
		EVB_INFO("Successfully wrote config to file.");
	
	}
	
	void EVBApp::PlotHistograms() 
	{
		std::string analyze_dir = m_workspace+"/analyzed/";
		std::string plot_file = m_workspace+"/histograms/run_"+std::to_string(m_rmin)+"_"+std::to_string(m_rmax)+".root";
		SFPPlotter grammer;
		grammer.SetProgressCallbackFunc(m_progressCallback);
		grammer.SetProgressFraction(m_progressFraction);
		grammer.ApplyCutlist(m_cutList);
		EVB_INFO("Generating histograms from analyzed runs [{0}, {1}] with Cut List {2}...", m_rmin, m_rmax, m_cutList);
		EVB_INFO("Output file will be named {0}",plot_file);
	
		grabber.SetSearchParams(analyze_dir, "", ".root", m_rmin, m_rmax);
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
		std::string rawroot_dir = m_workspace+"/raw_root/";
		std::string unpack_dir = m_workspace+"/temp_binary/";
		std::string binary_dir = m_workspace+"/raw_binary/";
		EVB_INFO("Converting binary archives to ROOT files over run range [{0}, {1}]",m_rmin,m_rmax);
	
		grabber.SetSearchParams(binary_dir, "", ".tar.gz",0,1000);
	
		std::string rawfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(unpack_dir);
		converter.SetShiftMap(m_shiftfile);
		converter.SetScalerInput(m_scalerfile);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		for(int i=m_rmin; i<=m_rmax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...", binfile);
			rawfile = rawroot_dir + "compass_run_"+ std::to_string(i) + ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.bin";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2RawRoot(rawfile);
			sys_return = system(wipe_command.c_str());
	
		}
		EVB_INFO("Conversion complete.");
	}
	
	void EVBApp::MergeROOTFiles() 
	{
		std::string merge_file = m_workspace+"/merged/run_"+std::to_string(m_rmin)+"_"+std::to_string(m_rmax)+".root";
		std::string file_dir = m_workspace+"/analyzed/";
		EVB_INFO("Merging ROOT files into single file for runs in range [{0}, {1}]", m_rmin, m_rmax);
		EVB_INFO("Merged file will be named {0}", merge_file);
		std::string prefix = "";
		std::string suffix = ".root";
		grabber.SetSearchParams(file_dir, prefix, suffix,m_rmin,m_rmax);
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
		std::string sortroot_dir = m_workspace+"/sorted/";
		std::string unpack_dir = m_workspace+"/temp_binary/";
		std::string binary_dir = m_workspace+"/raw_binary/";
		EVB_INFO("Converting binary archives to event built ROOT files over run range [{0}, {1}]",m_rmin,m_rmax);
	
		grabber.SetSearchParams(binary_dir,"",".tar.gz",m_rmin,m_rmax);
	
		std::string sortfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(unpack_dir);
		converter.SetShiftMap(m_shiftfile);
		converter.SetScalerInput(m_scalerfile);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
	
		int count=0;
		for(int i=m_rmin; i<= m_rmax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...",binfile);
	
			sortfile = sortroot_dir +"run_"+std::to_string(i)+ ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.bin";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2SortedRoot(sortfile, m_mapfile, m_SlowWindow);
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
		std::string sortroot_dir = m_workspace+"/fast/";
		std::string unpack_dir = m_workspace+"/temp_binary/";
		std::string binary_dir = m_workspace+"/raw_binary/";
		EVB_INFO("Converting binary archives to fast event built ROOT files over run range [{0}, {1}]",m_rmin,m_rmax);
	
		grabber.SetSearchParams(binary_dir,"",".tar.gz",m_rmin,m_rmax);
	
		std::string sortfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(unpack_dir);
		converter.SetShiftMap(m_shiftfile);
		converter.SetScalerInput(m_scalerfile);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		int count=0;
		for(int i=m_rmin; i<=m_rmax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...",binfile);
	
			sortfile = sortroot_dir + "run_" + std::to_string(i) + ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.bin";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2FastSortedRoot(sortfile, m_mapfile, m_SlowWindow, m_FastWindowSABRE, m_FastWindowIonCh);
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
		std::string sortroot_dir = m_workspace+"/analyzed/";
		std::string unpack_dir = m_workspace+"/temp_binary/";
		std::string binary_dir = m_workspace+"/raw_binary/";
		EVB_INFO("Converting binary archives to analyzed event built ROOT files over run range [{0}, {1}]",m_rmin,m_rmax);

		grabber.SetSearchParams(binary_dir,"",".tar.gz",m_rmin, m_rmax);
	
		std::string sortfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(unpack_dir);
		converter.SetShiftMap(m_shiftfile);
		converter.SetScalerInput(m_scalerfile);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		int count=0;
		for(int i=m_rmin; i<=m_rmax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...",binfile);
	
			sortfile = sortroot_dir + "run_" + std::to_string(i) + ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.bin";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2SlowAnalyzedRoot(sortfile, m_mapfile, m_SlowWindow, m_ZT, m_AT, m_ZP, m_AP, m_ZE, m_AE, m_BKE, m_B, m_Theta);
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
		std::string sortroot_dir = m_workspace+"/analyzed/";
		std::string unpack_dir = m_workspace+"/temp_binary/";
		std::string binary_dir = m_workspace+"/raw_binary/";
		EVB_INFO("Converting binary archives to analyzed fast event built ROOT files over run range [{0}, {1}]",m_rmin,m_rmax);
		
		grabber.SetSearchParams(binary_dir,"",".tar.gz",m_rmin,m_rmax);
	
		std::string sortfile, binfile;
		std::string unpack_command, wipe_command;
	
		CompassRun converter(unpack_dir);
		converter.SetShiftMap(m_shiftfile);
		converter.SetScalerInput(m_scalerfile);
		converter.SetProgressCallbackFunc(m_progressCallback);
		converter.SetProgressFraction(m_progressFraction);
	
		EVB_INFO("Beginning conversion...");
		int count=0;
		for(int i=m_rmin; i<=m_rmax; i++) 
		{
			binfile = grabber.GrabFile(i);
			if(binfile == "") 
				continue;
			converter.SetRunNumber(i);
			EVB_INFO("Converting file {0}...",binfile);
	
			sortfile = sortroot_dir + "run_" + std::to_string(i) + ".root";
			unpack_command = "tar -xzf "+binfile+" --directory "+unpack_dir;
			wipe_command = "rm -r "+unpack_dir+"*.bin";
	
			sys_return = system(unpack_command.c_str());
			converter.Convert2FastAnalyzedRoot(sortfile, m_mapfile, m_SlowWindow, m_FastWindowSABRE, m_FastWindowIonCh, m_ZT, m_AT, m_ZP, m_AP, m_ZE, m_AE, m_BKE, m_B, m_Theta);
			sys_return = system(wipe_command.c_str());
			count++;
		}
		if(count==0)
			EVB_WARN("Conversion failed, no archives were found!");
		else
			EVB_INFO("Conversion complete.");
	}
	
	bool EVBApp::SetKinematicParameters(int zt, int at, int zp, int ap, int ze, int ae, double b, double theta, double bke) 
	{
	
		if((at + ap - ae) < 0 || (zt + zp - ze) < 0) 
		{
			EVB_WARN("Invalid kinematic parameters, nucleon number not conserved! Parameters not set");
			return false;
		}
		m_ZT = zt; m_AT = at; m_ZP = zp; m_AP = ap; m_ZE = ze; m_AE = ae;
		m_B = b; m_Theta = theta; m_BKE = bke;
	
		m_ZR = (zt + zp - ze);
		m_AR = (at + ap - ae);

		EVB_TRACE("Kinematic paramters updated.");
	
		return true;
	}


	void EVBApp::SetRunRange(int rmin, int rmax) { EVB_TRACE("Min Run, max run set to [{0}, {1}]", rmin, rmax); m_rmin = rmin; m_rmax = rmax; }
	void EVBApp::SetWorkDirectory(const std::string& fullpath) { EVB_TRACE("Workspace set to {0}", fullpath); m_workspace = fullpath; }
	void EVBApp::SetChannelMap(const std::string& name) { EVB_TRACE("Channel map set to {0}",name); m_mapfile = name; }
	void EVBApp::SetBoardShiftFile(const std::string& name) { EVB_TRACE("Shift file set to {0}", name); m_shiftfile = name; }
	void EVBApp::SetSlowCoincidenceWindow(double window) { EVB_TRACE("Slow Coincidence Window set to {0}",window); m_SlowWindow = window; }
	void EVBApp::SetFastWindowIonChamber(double window) { EVB_TRACE("Fast Coinc. Window Ion Ch. set to {0}",window); m_FastWindowIonCh = window; }
	void EVBApp::SetFastWindowSABRE(double window) { EVB_TRACE("Fast Coinc. Window SABRE set to {0}",window); m_FastWindowSABRE = window; }
	void EVBApp::SetCutList(const std::string& name) { EVB_TRACE("Cut List set  to {0}", name); m_cutList = name; }
	void EVBApp::SetScalerFile(const std::string& fullpath) { EVB_TRACE("Scaler file set to {0}", fullpath); m_scalerfile = fullpath; }

}