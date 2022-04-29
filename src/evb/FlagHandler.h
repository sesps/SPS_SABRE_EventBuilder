#ifndef FLAGHANDLER_H
#define FLAGHANDLER_H

#include <map>

namespace EventBuilder {

	struct FlagCount 
	{
		long total_counts=0;
		long dead_time=0;
		long time_roll=0;
		long time_reset=0;
		long fake_event=0;
		long mem_full=0;
		long trig_lost=0;
		long n_trig_lost=0;
		long sat_in_gate=0;
		long trig_1024=0;
		long sat_input=0;
		long n_trig_count=0;
		long event_not_matched=0;
		long fine_time=0;
		long pile_up=0;
		long pll_lock_loss=0;
		long over_temp=0;
		long adc_shutdown=0;
	};
	
	class FlagHandler 
	{
	public:
		FlagHandler();
		FlagHandler(const std::string& filename);
		~FlagHandler();
		void CheckFlag(int board, int channel, int flag);
	
		const int DeadTime = 0x00000001;
		const int TimeRollover = 0x00000002;
		const int TimeReset = 0x00000004;
		const int FakeEvent = 0x00000008;
		const int MemFull = 0x00000010;
		const int TrigLost = 0x00000020;
		const int NTrigLost = 0x00000040;
		const int SaturatingInGate = 0x00000080;
		const int Trig1024Counted = 0x00000100;
		const int SaturatingInput = 0x00000400;
		const int NTrigCounted = 0x00000800;
		const int EventNotMatched = 0x00001000;
		const int FineTime  = 0x00004000;
		const int PileUp = 0x00008000;
		const int PLLLockLoss = 0x00080000;
		const int OverTemp = 0x00100000;
		const int ADCShutdown = 0x00200000;
	
	private:
		std::ofstream log;
		std::map<int, FlagCount> event_count_map;
	
		void WriteLog();
	};

}
#endif
