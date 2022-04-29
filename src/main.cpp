#include "evb/Logger.h"
#include "spsdict/DataStructs.h"
#include "evb/EVBApp.h"
#include "evb/Stopwatch.h"

int main(int argc, char** argv) 
{
	EnforceDictionaryLinked();
	EventBuilder::Logger::Init();
	if(argc != 3) 
	{
		EVB_ERROR("Incorrcect number of commandline arguments! Need to specify type of operation and input file.");
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
		EVB_ERROR("Invalid operation {0} given to EventBuilder! Exiting.", operation);
		return 1;
	}
	
	timer.Stop();
	EVB_INFO("Elapsed time (ms): {0}", timer.GetElapsedMilliseconds());

	return 0;
}