
#include "utils1.hpp"

float RangeFloat(float min, float max)
{
    // generate a float between min and maximum
    float random = ((float) rand() ) / (float) RAND_MAX;
    float range = max - min;
    return (random*range) + min;
}

void split(const std::string &s,std::vector<std::string>& words, char delim ) {
    words.clear();
    std::stringstream iss(s);
    std::string item;
    while(std::getline(iss, item, delim)) {
            words.push_back(item);
        }
}

std::string convertInt(int number)
{
    std::stringstream ss;//create a stringstream
    ss << number;//add number to the stream
    return ss.str();//return a string with the contents of the stream
}

int convertToInt(std::string num) {
    // Convert the string representation of a number to integer
    std::stringstream str(num); 
    int x;  
    str >> x;  
    if (!str) {
        std::cerr << "Conversion to int failed " << num <<std::endl;
    } 
    return x;
}

double convertToDouble(std::string num) {
    // Convert the string representation of a number to integer
    std::stringstream str(num); 
    double x;  
    str >> x;  
    if (str.fail()) {
        std::cerr << "Conversion to double failed " << num <<std::endl;
    } 
    return x;
}
