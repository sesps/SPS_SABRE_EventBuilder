#include "EventBuilder.h"
#include "FlagHandler.h"

FlagHandler::FlagHandler() : 
log("./event_log.txt")
{
}

FlagHandler::FlagHandler(const std::string& filename) :
log(filename)
{
}

FlagHandler::~FlagHandler() {
	WriteLog();
	log.close();
}

void FlagHandler::CheckFlag(int board, int channel, int flag) {

	int gchan = channel + board*16;
	FlagCount& counter = event_count_map[gchan]; //yikes

	counter.total_counts++;

	if(flag & DEAD_TIME) counter.dead_time++;

	if(flag & TIME_ROLLOVER) counter.time_roll++;

	if(flag & TIME_RESET) counter.time_reset++;

	if(flag & FAKE_EVENT) counter.fake_event++;

	if(flag & MEM_FULL) counter.mem_full++;

	if(flag & TRIG_LOST) counter.trig_lost++;

	if(flag & N_TRIG_LOST) counter.n_trig_lost++;

	if(flag & SATURATING_IN_GATE) counter.sat_in_gate++;

	if(flag & TRIG_1024_COUNTED) counter.trig_1024++;;

	if(flag & SATURATING_INPUT) counter.sat_input++;

	if(flag & N_TRIG_COUNTED) counter.n_trig_count++;

	if(flag & EVENT_NOT_MATCHED) counter.event_not_matched++;

	if(flag & PILE_UP) counter.pile_up++;

	if(flag & PLL_LOCK_LOSS) counter.pll_lock_loss++;

	if(flag & OVER_TEMP) counter.over_temp++;

	if(flag & ADC_SHUTDOWN) counter.adc_shutdown++;

}

void FlagHandler::WriteLog() {
	log<<"Event Flag Log"<<std::endl;
	log<<"-----------------------------"<<std::endl;
	for(auto& counter : event_count_map) {
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

