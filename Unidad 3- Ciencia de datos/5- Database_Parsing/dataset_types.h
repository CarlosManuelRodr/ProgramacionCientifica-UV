#pragma once

#include <vector>
#include <string>

/****************************
*                           *
*      Dataset storage      *
*                           *
****************************/

struct Indicators
{
    double negativeVolumeIndexNVI;
    double negativeVolumeIndexCumAvg;
    double onBalanceVolume;
    double performance;
    double positiveVolumeIndexPVI;
    double positiveVolumeIndexCumAvg;
    double priceVolumeTrend;
    double trueRange;
    double transactionVolume;
    double williamsAccumulationDistribution;
    double parabolicStopAndReversal;
    double medianPrice;
    double typicalPrice;
    double weightedClose;
    double closePrice;
    double highPrice;
    double lowPrice;
    double openPrice;
    double aroonHigh20, aroonHigh40, aroonHigh60, aroonHigh80;
    double aroonLow20, aroonLow40, aroonLow60, aroonLow80;
    double aroonOscillator20, aroonOscillator40, aroonOscillator60, aroonOscillator80;
    double averageTrueRange20, averageTrueRange40, averageTrueRange60, averageTrueRange80;
    double detrendedPriceOscillator20, detrendedPriceOscillator40, detrendedPriceOscillator60, detrendedPriceOscillator80;
    double forceIndex20, forceIndex40, forceIndex60, forceIndex80;
    double forecastOscillator20, forecastOscillator40, forecastOscillator60, forecastOscillator80;
    double linearRegressionSlope20, linearRegressionSlope40, linearRegressionSlope60, linearRegressionSlope80;
    double mESAPhase20, mESAPhase40, mESAPhase60, mESAPhase80;
    double mESASineWaveSin20, mESASineWaveSin40, mESASineWaveSin60, mESASineWaveSin80;
    double mESASineWaveCos20, mESASineWaveCos40, mESASineWaveCos60, mESASineWaveCos80;
    double momentum20, momentum40, momentum60, momentum80;
    double qStick20, qStick40, qStick60, qStick80;
    double rateOfChange20, rateOfChange40, rateOfChange60, rateOfChange80;
    double priceStandardDeviation20, priceStandardDeviation40, priceStandardDeviation60, priceStandardDeviation80;
    double standardError20, standardError40, standardError60, standardError80;
    double timeSegmentedVolume20, timeSegmentedVolume40, timeSegmentedVolume60, timeSegmentedVolume80;
    double sma20, sma40, sma60, sma80;
    double doubleExponentialMovingAverage20, doubleExponentialMovingAverage40, doubleExponentialMovingAverage60, doubleExponentialMovingAverage80;
    double ema20, ema40, ema60, ema80;
    double weightedMovingAverage20, weightedMovingAverage40, weightedMovingAverage60, weightedMovingAverage80;
    double linearRegressionTrendlines20, linearRegressionTrendlines40, linearRegressionTrendlines60, linearRegressionTrendlines80;
    double triangularMovingAverage20, triangularMovingAverage40, triangularMovingAverage60, triangularMovingAverage80;
    double tripleExponentialMovingAverage20, tripleExponentialMovingAverage40, tripleExponentialMovingAverage60, tripleExponentialMovingAverage80;
    double priceTimeSeriesForecast20, priceTimeSeriesForecast40, priceTimeSeriesForecast60, priceTimeSeriesForecast80;
    double wildersMovingAverage20, wildersMovingAverage40, wildersMovingAverage60, wildersMovingAverage80;
    double highestHigh20, highestHigh40, highestHigh60, highestHigh80;
    double lowestLow20, lowestLow40, lowestLow60, lowestLow80;
};

struct TimedIndicators
{
    std::string time;
    Indicators indicators;
};

struct StockData
{
    std::string date;
    std::vector<TimedIndicators> timedIndicators;
};

struct Dataset
{
    std::string stockName;
    std::vector<StockData> stockData;
};