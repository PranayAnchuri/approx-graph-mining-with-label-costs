#include "stats.hpp"

Stats::Stats() {
    delim = '\t';
}

void Stats::start(const string& param){
    if(!present(timers, param)) {
        add_timer(param);
    }
    timers[param].start();
}

void Stats::stop(const string& param){
    timers[param].stop();
    //INFO(*stat_logger, to_string());
}

void Stats::store(const string& param) {
    timers[param].store();
}

void Stats::add_timer(string param) {
    timers[param] = Timer();
}

void Stats::add_counter(string param) {
    counters[param] = Counter(0);
}

void Stats::inc(string param, const int& val) {
    if(!present(counters, param))
        add_counter(param);
    counters[param].add(val);
}

std::string Stats::to_string() {
    // get the string representation of all the timers and the counters
	stringstream ss;
	tr(timers, it) {
		if(!it->first.compare("Walk"))
			ss << endl << it->first << delim << it->second.to_string(true);
		else
			ss << endl << it->first << delim << it->second.to_string(false);
    }
    tr(counters, it) {
        ss << endl << it->first << delim <<  it->second.to_string();
    }
    /*tr(mem_usage, types) {
        int s = accumulate(all(types->second), 0);
        ss << "\n" << types->first << "\tAvg\t" << s/float(types->second.size()) << "\t";
	for(int i=0; i< types->second.size() ; i+= 10)
		ss << types->second[i] << "\t";
        tr(types->second, mem) {
            ss << *mem << "\t";
        }
    }*/
    return ss.str();
}

void Stats::push_mem(const string& memtype, const int& usage) {
    mem_usage[memtype].push_back(usage);
}

