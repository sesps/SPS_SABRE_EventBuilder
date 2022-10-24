#include "FlagHandler.h"

namespace EventBuilder {

	FlagHandler::FlagHandler() : 
		log("./event_log.txt")
	{
	}
	
	FlagHandler::FlagHandler(const std::string& filename) :
		log(filename)
	{
	}
	
	FlagHandler::~FlagHandler() 
	{
		WriteLog();
		log.close();
	}
	
	void FlagHandler::CheckFlag(int board, int channel, int flag) 
	{
	
		int gchan = channel + board*16;
		FlagCount& counter = event_count_map[gchan]; //yikes
	
		counter.total_counts++;
	
		if(flag & DeadTime) 
			counter.dead_time++;
	
		if(flag & TimeRollover) 
			counter.time_roll++;
	
		if(flag & TimeReset) 
			counter.time_reset++;
	
		if(flag & FakeEvent) 
			counter.fake_event++;
	
		if(flag & MemFull) 
			counter.mem_full++;
	
		if(flag & TrigLost) 
			counter.trig_lost++;
	
		if(flag & NTrigLost) 
			counter.n_trig_lost++;
	
		if(flag & SaturatingInGate) 
			counter.sat_in_gate++;
	
		if(flag & Trig1024Counted) 
			counter.trig_1024++;;
	
		if(flag & SaturatingInput) 
			counter.sat_input++;
	
		if(flag & NTrigCounted) 
			counter.n_trig_count++;
	
		if(flag & EventNotMatched) 
			counter.event_not_matched++;
	
		if(flag & PileUp) 
			counter.pile_up++;
	
		if(flag & PLLLockLoss) 
			counter.pll_lock_loss++;
	
		if(flag & OverTemp) 
			counter.over_temp++;
	
		if(flag & ADCShutdown) 
			counter.adc_shutdown++;
	
	}
	
	void FlagHandler::WriteLog() 
	{
		log<<"Event Flag Log"<<std::endl;
		log<<"-----------------------------"<<std::endl;
		for(auto& counter : event_count_map) 
		{
			log<<"-----------------------------"<<std::endl;
			log<<"GLOBAL CHANNEL No.: "<<counter.first<<std::endl;
			log<<"Total number of events: "<<counter.second.total_counts<<std::endl;
			log<<"Dead time incurred (only for V1724): "<<counter.second.dead_time<<std::endl;
			log<<"Timestamp rollovers: "<<counter.second.time_roll<<std::endl;
			log<<"Timestamp resets from external: "<<counter.second.time_reset<<std::endl;
			log<<"Fake events: "<<counter.second.fake_event<<std::endl;
			log<<"Memory full: "<<counter.second.mem_full<<std::endl;
			log<<"Triggers lost: "<<counter.second.trig_lost<<std::endl;
			log<<"N Triggers lost: "<<counter.second.n_trig_lost<<std::endl;
			log<<"Saturation within the gate: "<<counter.second.sat_in_gate<<std::endl;
			log<<"1024 Triggers found: "<<counter.second.trig_1024<<std::endl;
			log<<"Saturation on input: "<<counter.second.sat_input<<std::endl;
			log<<"N Triggers counted: "<<counter.second.n_trig_count<<std::endl;
			log<<"Events not matched: "<<counter.second.event_not_matched<<std::endl;
			log<<"Pile ups: "<<counter.second.pile_up<<std::endl;
			log<<"PLL lock lost: "<<counter.second.pll_lock_loss<<std::endl;
			log<<"Over Temperature: "<<counter.second.over_temp<<std::endl;
			log<<"ADC Shutdown: "<<counter.second.adc_shutdown<<std::endl;
			log<<"-----------------------------"<<std::endl;
		}
	}
}
