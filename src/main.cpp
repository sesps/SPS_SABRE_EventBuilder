#include "EventBuilder.h"
#include "spsdict/DataStructs.h"
#include "evb/EVBApp.h"
#include "evb/Stopwatch.h"

int main(int argc, char** argv) 
{
	EnforceDictionaryLinked();
	if(argc != 3) 
	{
		std::cerr<<"Incorrect number of command line arguments!"<<std::endl;
		std::cerr<<"Need to specify type of operation (buildSlow, buildFast, etc.) and input file."<<std::endl;
		return 1;
	}

	std::string filename = argv[2];
	std::string operation = argv[1];


	/* DEFAULT Operation Types:
		Convert (convert binary archive to root data)
		ConvertSlow (convert binary arhcive to event slow data)
		ConvertFast (convert binary archive to event fast data)
		ConvertSlowA (convert binary archive to analyzed slow event data)
		ConvertFastA (convert binary archive to analyzed fast event data)
		Merge (combine root files)
		Plot (generate a default histogram file from analyzed data)
	*/

	EventBuilder::EVBApp theBuilder;

	theBuilder.ReadConfigFile(filename);

	EventBuilder::Stopwatch timer;
	timer.Start();
	if(operation == "Convert")
		theBuilder.Convert2RawRoot();
	else if(operation == "Merge")
		theBuilder.MergeROOTFiles();
	else if(operation == "Plot")
		theBuilder.PlotHistograms();
	else if (operation == "ConvertSlow")
		theBuilder.Convert2SortedRoot();
	else if (operation == "ConvertFast")
		theBuilder.Convert2FastSortedRoot();
	else if (operation == "ConvertSlowA")
		theBuilder.Convert2SlowAnalyzedRoot();
	else if (operation == "ConvertFastA")
		theBuilder.Convert2FastAnalyzedRoot();
	else 
	{
		std::cerr<<"Unidentified type of operation! Check your first argument."<<std::endl;
		return 1;
	}
	
	timer.Stop();
	std::cout<<"Elapsed time (ms): "<<timer.GetElapsedMilliseconds()<<std::endl;

	return 0;
}