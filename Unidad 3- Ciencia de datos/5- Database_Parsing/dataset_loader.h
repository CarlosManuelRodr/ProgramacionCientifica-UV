#pragma once
#include <vector>
#include <string>
#include "dataset_types.h"

/****************************
*                           *
*      Dataset loading      *
*                           *
****************************/

std::string load_dataset_as_string(const std::string path);
std::string get_stock_name(std::string& dataset);
int get_balanced_symbol_position(std::string& text, std::string symbol, std::string inverseSymbol, int startPosition);
std::string get_stock_data(std::string& dataset);
std::vector<std::string> split_list(std::string list);
std::string get_date_from_stock_data_element(std::string& stockDataElement);
std::string get_timed_indicators_from_stock_data_element(std::string& stockDataElement);
std::string get_time_from_timed_indicator(std::string& timedIndicator);
std::string get_indicators_from_timed_indicator(std::string& timedIndicator);
std::string get_indicator_from_indicators(std::string& indicators, std::string indicatorName);
double parse_indicator(std::string& indicatorsList, std::string indicatorName);
Dataset load_dataset(const std::string path, const bool showProgress = false);
void print_dataset_info(Dataset& dataset);