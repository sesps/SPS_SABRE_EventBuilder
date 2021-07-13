#ifndef FLAGHANDLER_H
#define FLAGHANDLER_H

#include <map>

struct FlagCount {
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

class FlagHandler {
public:
	FlagHandler();
	FlagHandler(const std::string& filename);
	~FlagHandler();
	void CheckFlag(int board, int channel, int flag);

	const int DEAD_TIME = 0x00000001;
	const int TIME_ROLLOVER = 0x00000002;
	const int TIME_RESET = 0x00000004;
	const int FAKE_EVENT = 0x00000008;
	const int MEM_FULL = 0x00000010;
	const int TRIG_LOST = 0x00000020;
	const int N_TRIG_LOST = 0x00000040;
	const int SATURATING_IN_GATE = 0x00000080;
	const int TRIG_1024_COUNTED = 0x00000100;
	const int SATURATING_INPUT = 0x00000400;
	const int N_TRIG_COUNTED = 0x00000800;
	const int EVENT_NOT_MATCHED = 0x00001000;
	const int FINE_TIME  = 0x00004000;
	const int PILE_UP = 0x00008000;
	const int PLL_LOCK_LOSS = 0x00080000;
	const int OVER_TEMP = 0x00100000;
	const int ADC_SHUTDOWN = 0x00200000;

private:
	std::ofstream log;
	std::map<int, FlagCount> event_count_map;

	void WriteLog();
};

#endif
