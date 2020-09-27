#include <fstream>
#include <sstream>
#include <regex>
#include <limits>
#include "filesystem.h"
#include "dataset_loader.h"
#include "parallel_function.h"
using namespace std;

/****************************
*                           *
*      Dataset loading      *
*                           *
****************************/

string load_dataset_as_string(const string path)
{
    ifstream inFile;
    inFile.open(path);

    if (inFile.is_open())
    {
        stringstream strStream;
        strStream << inFile.rdbuf();
        string databaseAsString = strStream.str();
        return databaseAsString;
    }
    else
        return string("");
}

string get_stock_name(string& dataset)
{
    regex regexp("\"Stock\"(?:\\s|\\n)*->(?:\\s|\\n)+\"([A-Za-z.]*)\"");
    smatch matches;

    if (regex_search(dataset, matches, regexp))
    {
        if (matches.size() > 1)
            return matches[1].str();
    }

    return string("Error: Cannot parse stock name.");
}

int get_balanced_symbol_position(string& text, string symbol, string inverseSymbol, int startPosition)
{
    int openSymbol = 0;
    const unsigned symbolLength = (unsigned) symbol.length();

    for (unsigned i = startPosition; i < text.length(); i++)
    {
        string currentSymbol = text.substr(i, symbolLength);
        if (currentSymbol == symbol)
            openSymbol++;
        if (currentSymbol == inverseSymbol)
            openSymbol--;

        if (openSymbol == 0)
            return i;
    }

    return -1;
}

string get_stock_data(string& dataset)
{
    regex regexp("\"Data\"(\\s|\\n)*->(\\s|\\n)*");
    smatch matches;

    if (regex_search(dataset, matches, regexp))
    {
        if (matches.size() > 0)
        {
            int listStart = (int)(matches.position(0) + matches[0].length());
            int listEnd = get_balanced_symbol_position(dataset, "{", "}", listStart);
            if (listEnd != -1)
                return dataset.substr(listStart, listEnd - listStart + 1);
        }
    }

    return string("Error: Cannot parse stock data.");
}

vector<string> split_list(string list)
{
    vector<unsigned> separatorPositions;

    for (unsigned i = 0; i < list.length(); i++)
    {
        char currentSymbol = list[i];

        if (list.substr(i, 2) == "<|")
        {
            i = get_balanced_symbol_position(list, "<|", "|>", i);
            if (i == -1)
                return vector<string>{ "Error: Unbalanced brackets in List or Association." };
        }

        if (list[i] == ',')
            separatorPositions.push_back(i);
    }

    vector<string> listComponents;
    for (unsigned i = 0; i <= separatorPositions.size(); i++)
    {
        string listElement;
        if (i == 0)
            listElement = string(&list[1], &list[separatorPositions[i]]);
        else if (i == separatorPositions.size())
            listElement = string(&list[separatorPositions[i - 1] + 1], &list[list.length() - 1]);
        else
            listElement = string(&list[separatorPositions[i - 1] + 1], &list[separatorPositions[i]]);


        listComponents.push_back(listElement);
    }

    return listComponents;
}

string get_date_from_stock_data_element(string& stockDataElement)
{
    regex regexp("\"Date\"(?:\\s|\\n)*->(?:\\s|\\n)*\"([0-9-]+)\"");
    smatch matches;

    if (regex_search(stockDataElement, matches, regexp))
    {
        if (matches.size() > 1)
            return matches[1].str();
    }

    return string("Error: Cannot parse date from stock data.");
}

string get_timed_indicators_from_stock_data_element(string& stockDataElement)
{
    regex regexp("\"TimedIndicators\"(\\s|\\n)*->(\\s|\\n)*");
    smatch matches;

    if (regex_search(stockDataElement, matches, regexp))
    {
        if (matches.size() > 0)
        {
            int listStart = (int)(matches.position(0) + matches[0].length());
            int listEnd = get_balanced_symbol_position(stockDataElement, "{", "}", listStart);
            return stockDataElement.substr(listStart, listEnd - listStart + 1);
        }
    }

    return string("Error: Cannot parse timed indicators from stock data.");
}

string get_time_from_timed_indicator(string& timedIndicator)
{
    regex regexp("\"Time\"(?:\\s|\\n)*->(?:\\s|\\n)*\"([0-9:]+)\"");
    smatch matches;

    if (regex_search(timedIndicator, matches, regexp))
    {
        if (matches.size() > 1)
            return matches[1].str();
    }

    return string("Error: Cannot parse time from timed indicator.");
}

string get_indicators_from_timed_indicator(string& timedIndicator)
{
    regex regexp("\"Indicators\"(\\s|\\n)*->(\\s|\\n)*");
    smatch matches;

    if (regex_search(timedIndicator, matches, regexp))
    {
        if (matches.size() > 0)
        {
            int listStart = (int)(matches.position(0) + matches[0].length());
            int listEnd = get_balanced_symbol_position(timedIndicator, "{", "}", listStart);
            return timedIndicator.substr(listStart, listEnd - listStart + 1);
        }
    }

    return string("Error: Cannot parse indicators from timed indicator.");
}

bool replace_string(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

string get_indicator_from_indicators(string& indicators, string indicatorName)
{
    replace_string(indicatorName, "[", "\\[");
    replace_string(indicatorName, "]", "\\]");
    string regexPattern = indicatorName + "(?:\\s|\\n)*->(?:\\s|\\n)([a-zA-Z]+|(?:[-+]?[0-9]*[.]?[0-9]+))";

    regex regexp(regexPattern);
    smatch matches;

    if (regex_search(indicators, matches, regexp))
    {
        if (matches.size() > 1)
            return matches[1].str();
    }

    return string("Error: Cannot parse indicator [") + indicatorName + "] from indicators.";
}

bool is_number(string token)
{
    return regex_match(token, regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

double parse_indicator(string& indicatorsList, string indicatorName)
{
    string ind = get_indicator_from_indicators(indicatorsList, indicatorName);
    if (is_number(ind))
        return stod(ind);
    else
        return numeric_limits<double>::quiet_NaN();
}

Indicators parse_indicators_list(string& indicatorsList)
{
    Indicators indicators;

    // Zero window indicators
    indicators.negativeVolumeIndexNVI = parse_indicator(indicatorsList, "NegativeVolumeIndexNVI[]");
    indicators.negativeVolumeIndexCumAvg = parse_indicator(indicatorsList, "NegativeVolumeIndexCumAvg[]");
    indicators.onBalanceVolume = parse_indicator(indicatorsList, "OnBalanceVolume[]");
    indicators.performance = parse_indicator(indicatorsList, "Performance[]");
    indicators.positiveVolumeIndexPVI = parse_indicator(indicatorsList, "PositiveVolumeIndexPVI[]");
    indicators.positiveVolumeIndexCumAvg = parse_indicator(indicatorsList, "PositiveVolumeIndexCumAvg[]");
    indicators.priceVolumeTrend = parse_indicator(indicatorsList, "PriceVolumeTrend[]");
    indicators.trueRange = parse_indicator(indicatorsList, "TrueRange[]");
    indicators.transactionVolume = parse_indicator(indicatorsList, "TransactionVolume[]");
    indicators.williamsAccumulationDistribution = parse_indicator(indicatorsList, "WilliamsAccumulationDistribution[]");
    indicators.parabolicStopAndReversal = parse_indicator(indicatorsList, "ParabolicStopAndReversal[]");
    indicators.medianPrice = parse_indicator(indicatorsList, "MedianPrice[]");
    indicators.typicalPrice = parse_indicator(indicatorsList, "TypicalPrice[]");
    indicators.weightedClose = parse_indicator(indicatorsList, "WeightedClose[]");
    indicators.closePrice = parse_indicator(indicatorsList, "ClosePrice[]");
    indicators.highPrice = parse_indicator(indicatorsList, "HighPrice[]");
    indicators.lowPrice = parse_indicator(indicatorsList, "LowPrice[]");
    indicators.openPrice = parse_indicator(indicatorsList, "OpenPrice[]");

    // One window indicators
    indicators.aroonHigh20 = parse_indicator(indicatorsList, "AroonHigh[20]");
    indicators.aroonHigh40 = parse_indicator(indicatorsList, "AroonHigh[40]");
    indicators.aroonHigh60 = parse_indicator(indicatorsList, "AroonHigh[60]");
    indicators.aroonHigh80 = parse_indicator(indicatorsList, "AroonHigh[80]");
    indicators.aroonLow20 = parse_indicator(indicatorsList, "AroonLow[20]");
    indicators.aroonLow40 = parse_indicator(indicatorsList, "AroonLow[40]");
    indicators.aroonLow60 = parse_indicator(indicatorsList, "AroonLow[60]");
    indicators.aroonLow80 = parse_indicator(indicatorsList, "AroonLow[80]");
    indicators.aroonOscillator20 = parse_indicator(indicatorsList, "AroonLow[20]");
    indicators.aroonOscillator40 = parse_indicator(indicatorsList, "AroonLow[40]");
    indicators.aroonOscillator60 = parse_indicator(indicatorsList, "AroonLow[60]");
    indicators.aroonOscillator80 = parse_indicator(indicatorsList, "AroonLow[80]");
    indicators.averageTrueRange20 = parse_indicator(indicatorsList, "AverageTrueRange[20]");
    indicators.averageTrueRange40 = parse_indicator(indicatorsList, "AverageTrueRange[40]");
    indicators.averageTrueRange60 = parse_indicator(indicatorsList, "AverageTrueRange[60]");
    indicators.averageTrueRange80 = parse_indicator(indicatorsList, "AverageTrueRange[80]");
    indicators.detrendedPriceOscillator20 = parse_indicator(indicatorsList, "DetrendedPriceOscillator[20]");
    indicators.detrendedPriceOscillator40 = parse_indicator(indicatorsList, "DetrendedPriceOscillator[40]");
    indicators.detrendedPriceOscillator60 = parse_indicator(indicatorsList, "DetrendedPriceOscillator[60]");
    indicators.detrendedPriceOscillator80 = parse_indicator(indicatorsList, "DetrendedPriceOscillator[80]");
    indicators.forceIndex20 = parse_indicator(indicatorsList, "ForceIndex[20]");
    indicators.forceIndex40 = parse_indicator(indicatorsList, "ForceIndex[40]");
    indicators.forceIndex60 = parse_indicator(indicatorsList, "ForceIndex[60]");
    indicators.forceIndex80 = parse_indicator(indicatorsList, "ForceIndex[80]");
    indicators.forecastOscillator20 = parse_indicator(indicatorsList, "ForecastOscillator[20]");
    indicators.forecastOscillator40 = parse_indicator(indicatorsList, "ForecastOscillator[40]");
    indicators.forecastOscillator60 = parse_indicator(indicatorsList, "ForecastOscillator[60]");
    indicators.forecastOscillator80 = parse_indicator(indicatorsList, "ForecastOscillator[80]");
    indicators.linearRegressionSlope20 = parse_indicator(indicatorsList, "LinearRegressionSlope[20]");
    indicators.linearRegressionSlope40 = parse_indicator(indicatorsList, "LinearRegressionSlope[40]");
    indicators.linearRegressionSlope60 = parse_indicator(indicatorsList, "LinearRegressionSlope[60]");
    indicators.linearRegressionSlope80 = parse_indicator(indicatorsList, "LinearRegressionSlope[80]");
    indicators.mESAPhase20 = parse_indicator(indicatorsList, "MESAPhase[20]");
    indicators.mESAPhase40 = parse_indicator(indicatorsList, "MESAPhase[40]");
    indicators.mESAPhase60 = parse_indicator(indicatorsList, "MESAPhase[60]");
    indicators.mESAPhase80 = parse_indicator(indicatorsList, "MESAPhase[80]");
    indicators.mESASineWaveSin20 = parse_indicator(indicatorsList, "MESASineWaveSin[20]");
    indicators.mESASineWaveSin40 = parse_indicator(indicatorsList, "MESASineWaveSin[40]");
    indicators.mESASineWaveSin60 = parse_indicator(indicatorsList, "MESASineWaveSin[60]");
    indicators.mESASineWaveSin80 = parse_indicator(indicatorsList, "MESASineWaveSin[80]");
    indicators.mESASineWaveCos20 = parse_indicator(indicatorsList, "MESASineWaveCos[20]");
    indicators.mESASineWaveCos40 = parse_indicator(indicatorsList, "MESASineWaveCos[40]");
    indicators.mESASineWaveCos60 = parse_indicator(indicatorsList, "MESASineWaveCos[60]");
    indicators.mESASineWaveCos80 = parse_indicator(indicatorsList, "MESASineWaveCos[80]");
    indicators.momentum20 = parse_indicator(indicatorsList, "Momentum[20]");
    indicators.momentum40 = parse_indicator(indicatorsList, "Momentum[40]");
    indicators.momentum60 = parse_indicator(indicatorsList, "Momentum[60]");
    indicators.momentum80 = parse_indicator(indicatorsList, "Momentum[80]");
    indicators.qStick20 = parse_indicator(indicatorsList, "QStick[20]");
    indicators.qStick40 = parse_indicator(indicatorsList, "QStick[40]");
    indicators.qStick60 = parse_indicator(indicatorsList, "QStick[60]");
    indicators.qStick80 = parse_indicator(indicatorsList, "QStick[80]");
    indicators.rateOfChange20 = parse_indicator(indicatorsList, "RateOfChange[20]");
    indicators.rateOfChange40 = parse_indicator(indicatorsList, "RateOfChange[40]");
    indicators.rateOfChange60 = parse_indicator(indicatorsList, "RateOfChange[60]");
    indicators.rateOfChange80 = parse_indicator(indicatorsList, "RateOfChange[80]");
    indicators.priceStandardDeviation20 = parse_indicator(indicatorsList, "PriceStandardDeviation[20]");
    indicators.priceStandardDeviation40 = parse_indicator(indicatorsList, "PriceStandardDeviation[40]");
    indicators.priceStandardDeviation60 = parse_indicator(indicatorsList, "PriceStandardDeviation[60]");
    indicators.priceStandardDeviation80 = parse_indicator(indicatorsList, "PriceStandardDeviation[80]");
    indicators.standardError20 = parse_indicator(indicatorsList, "StandardError[20]");
    indicators.standardError40 = parse_indicator(indicatorsList, "StandardError[40]");
    indicators.standardError60 = parse_indicator(indicatorsList, "StandardError[60]");
    indicators.standardError80 = parse_indicator(indicatorsList, "StandardError[80]");
    indicators.timeSegmentedVolume20 = parse_indicator(indicatorsList, "TimeSegmentedVolume[20]");
    indicators.timeSegmentedVolume40 = parse_indicator(indicatorsList, "TimeSegmentedVolume[40]");
    indicators.timeSegmentedVolume60 = parse_indicator(indicatorsList, "TimeSegmentedVolume[60]");
    indicators.timeSegmentedVolume80 = parse_indicator(indicatorsList, "TimeSegmentedVolume[80]");
    indicators.sma20 = parse_indicator(indicatorsList, "SMA[20]");
    indicators.sma40 = parse_indicator(indicatorsList, "SMA[40]");
    indicators.sma60 = parse_indicator(indicatorsList, "SMA[60]");
    indicators.sma80 = parse_indicator(indicatorsList, "SMA[80]");
    indicators.doubleExponentialMovingAverage20 = parse_indicator(indicatorsList, "DoubleExponentialMovingAverage[20]");
    indicators.doubleExponentialMovingAverage40 = parse_indicator(indicatorsList, "DoubleExponentialMovingAverage[40]");
    indicators.doubleExponentialMovingAverage60 = parse_indicator(indicatorsList, "DoubleExponentialMovingAverage[60]");
    indicators.doubleExponentialMovingAverage80 = parse_indicator(indicatorsList, "DoubleExponentialMovingAverage[80]");
    indicators.ema20 = parse_indicator(indicatorsList, "EMA[20]");
    indicators.ema40 = parse_indicator(indicatorsList, "EMA[40]");
    indicators.ema60 = parse_indicator(indicatorsList, "EMA[60]");
    indicators.ema80 = parse_indicator(indicatorsList, "EMA[80]");
    indicators.weightedMovingAverage20 = parse_indicator(indicatorsList, "WeightedMovingAverage[20]");
    indicators.weightedMovingAverage40 = parse_indicator(indicatorsList, "WeightedMovingAverage[40]");
    indicators.weightedMovingAverage60 = parse_indicator(indicatorsList, "WeightedMovingAverage[60]");
    indicators.weightedMovingAverage80 = parse_indicator(indicatorsList, "WeightedMovingAverage[80]");
    indicators.linearRegressionTrendlines20 = parse_indicator(indicatorsList, "LinearRegressionTrendlines[20]");
    indicators.linearRegressionTrendlines40 = parse_indicator(indicatorsList, "LinearRegressionTrendlines[40]");
    indicators.linearRegressionTrendlines60 = parse_indicator(indicatorsList, "LinearRegressionTrendlines[60]");
    indicators.linearRegressionTrendlines80 = parse_indicator(indicatorsList, "LinearRegressionTrendlines[80]");
    indicators.triangularMovingAverage20 = parse_indicator(indicatorsList, "TriangularMovingAverage[20]");
    indicators.triangularMovingAverage40 = parse_indicator(indicatorsList, "TriangularMovingAverage[40]");
    indicators.triangularMovingAverage60 = parse_indicator(indicatorsList, "TriangularMovingAverage[60]");
    indicators.triangularMovingAverage80 = parse_indicator(indicatorsList, "TriangularMovingAverage[80]");
    indicators.tripleExponentialMovingAverage20 = parse_indicator(indicatorsList, "TripleExponentialMovingAverage[20]");
    indicators.tripleExponentialMovingAverage40 = parse_indicator(indicatorsList, "TripleExponentialMovingAverage[40]");
    indicators.tripleExponentialMovingAverage60 = parse_indicator(indicatorsList, "TripleExponentialMovingAverage[60]");
    indicators.tripleExponentialMovingAverage80 = parse_indicator(indicatorsList, "TripleExponentialMovingAverage[80]");
    indicators.priceTimeSeriesForecast20 = parse_indicator(indicatorsList, "PriceTimeSeriesForecast[20]");
    indicators.priceTimeSeriesForecast40 = parse_indicator(indicatorsList, "PriceTimeSeriesForecast[40]");
    indicators.priceTimeSeriesForecast60 = parse_indicator(indicatorsList, "PriceTimeSeriesForecast[60]");
    indicators.priceTimeSeriesForecast80 = parse_indicator(indicatorsList, "PriceTimeSeriesForecast[80]");
    indicators.wildersMovingAverage20 = parse_indicator(indicatorsList, "WildersMovingAverage[20]");
    indicators.wildersMovingAverage40 = parse_indicator(indicatorsList, "WildersMovingAverage[40]");
    indicators.wildersMovingAverage60 = parse_indicator(indicatorsList, "WildersMovingAverage[60]");
    indicators.wildersMovingAverage80 = parse_indicator(indicatorsList, "WildersMovingAverage[80]");
    indicators.highestHigh20 = parse_indicator(indicatorsList, "HighestHigh[20]");
    indicators.highestHigh40 = parse_indicator(indicatorsList, "HighestHigh[40]");
    indicators.highestHigh60 = parse_indicator(indicatorsList, "HighestHigh[60]");
    indicators.highestHigh80 = parse_indicator(indicatorsList, "HighestHigh[80]");
    indicators.lowestLow20 = parse_indicator(indicatorsList, "LowestLow[20]");
    indicators.lowestLow40 = parse_indicator(indicatorsList, "LowestLow[40]");
    indicators.lowestLow60 = parse_indicator(indicatorsList, "LowestLow[60]");
    indicators.lowestLow80 = parse_indicator(indicatorsList, "LowestLow[80]");

    return indicators;
}

StockData parse_stock_data(string stockDataTxt)
{
    StockData stockData;
    stockData.date = get_date_from_stock_data_element(stockDataTxt);

    vector<TimedIndicators> timedIndicatorsList;

    vector<string> timedIndicatorsAsString =
        split_list(get_timed_indicators_from_stock_data_element(stockDataTxt));

    for (unsigned j = 0; j < timedIndicatorsAsString.size(); j++)
    {
        // Parse timed indicators
        TimedIndicators timedIndicators;

        timedIndicators.time = get_time_from_timed_indicator(timedIndicatorsAsString[j]);

        string indicatorsList = get_indicators_from_timed_indicator(timedIndicatorsAsString[j]);
        timedIndicators.indicators = parse_indicators_list(indicatorsList);

        timedIndicatorsList.push_back(timedIndicators);
    }

    stockData.timedIndicators = timedIndicatorsList;
    return stockData;
}

Dataset load_dataset(const string path, const bool showProgress)
{
    Dataset loadedDataset;
    string dataset = load_dataset_as_string(path);

    // Parse dataset
    loadedDataset.stockName = get_stock_name(dataset);
    vector<string> listElements = split_list(get_stock_data(dataset));
    loadedDataset.stockData = parallel_function<StockData, std::string>(parse_stock_data, listElements);

    return loadedDataset;
}

void print_dataset_info(Dataset& dataset)
{
    cout << "Dataset Info" << endl;
    cout << "------------" << endl;
    cout << "Stock name: " << dataset.stockName << endl;
    cout << "Number of dates: " << dataset.stockData.size() << endl;
    cout << "Available dates: ";
    for (unsigned i = 0; i < dataset.stockData.size(); i++)
    {
        if (i == dataset.stockData.size() - 1)
            cout << dataset.stockData[i].date << endl;
        else
            cout << dataset.stockData[i].date << ", ";
    }

    cout << "Time points for each date: ";
    for (unsigned i = 0; i < dataset.stockData.size(); i++)
    {
        if (i == dataset.stockData.size() - 1)
            cout << dataset.stockData[i].timedIndicators.size() << endl;
        else
            cout << dataset.stockData[i].timedIndicators.size() << ", ";
    }
}