#pragma once

#include "config.hpp"
#include "store.hpp"

/*
 * Read the db file, sim file,
 * get frequent level patterns
 * from the arguments specified in
 * conf
 */
extern Logger* logger;
extern Logger* stat_logger;
void read_inp(Config& conf, Store& st);
void preprocess(Config& conf, Store& st);
