#include "timer.hpp"

Timer::Timer() {
    running=false;
    curr_time=0;
}
void Timer::start() {
    gettimeofday(&start_time, (struct timezone *)0);
    running=true;
}
void Timer::stop() {
    double st, en;
    if (!running) return;
    else {
      gettimeofday(&stop_time, (struct timezone *)0);
      st = start_time.tv_sec + (start_time.tv_usec/microsec);
      en = stop_time.tv_sec + (stop_time.tv_usec/microsec);
      running=false;
      curr_time+=en-st;
    }
}
double Timer::time() {
    return curr_time;
}

void Timer::store() {
    stop();
    stored_times.push_back(curr_time);
    //start();
}

std::string Timer::to_string() {
    std::stringstream ss;
    if(!stored_times.empty()) {
        double total = accumulate(all(stored_times), 0.0);
        ss << total/float(stored_times.size()) << "-";
    }
    for(int i=0;i<stored_times.size();i++) {
        ss << stored_times[i] << ",";
    }
    return ss.str();
}
