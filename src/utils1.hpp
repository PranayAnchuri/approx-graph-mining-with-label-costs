#pragma once

#include<vector>
#include<string>
#include<sstream>
#include<iostream>

float RangeFloat(float min, float max);
void split(const std::string &s, std::vector<std::string>& words, char delim );
std::string convertInt(int number);
int convertToInt(std::string num);
double convertToDouble(std::string num);
