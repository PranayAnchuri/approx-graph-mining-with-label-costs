#include "Counter.hpp"

Counter::Counter() {
    count = 0;
}

Counter::Counter(int init){
    count = init;
}

void Counter::add(int val) {
    count+=val;
}

int Counter::get() {
    return count;
}

std::string Counter::to_string() {
    std::stringstream ss;
    ss <<count;
    return ss.str();
}
