//
// Created by Martin Hettiger on 01.07.18.
//

#ifndef ROULETTE_CONFIGURATION_H
#define ROULETTE_CONFIGURATION_H


#include <iostream>
#include <cstring>
#include "Samplesize.h"

using namespace std;

class Configuration {

private:
    bool debug = false;
    double startingBankroll = 200, startingBet = 10, dynamicFactor = 0, minBet = 2, maxBet = 12000;
    uint failureLimit = 5, verbosity = 0, numThreads = 8;
    string unmatchedArgument;

public:
    bool isDebug() const;

    double getStartingBankroll() const;

    double getStartingBet() const;

    double getDynamicFactor() const;

    double getMinBet() const;

    double getMaxBet() const;

    uint getFailureLimit() const;

    uint getVerbosity() const;

    uint getNumThreads() const;

    Configuration(int argc, char *argv[], Samplesize *samplesize);

private:
    void printUnmatchedArgumentInformation(string unmatchedArgument);

    void printHelp();

    void printDebuggingInformation(uint samplesize);

};


#endif //ROULETTE_CONFIGURATION_H
