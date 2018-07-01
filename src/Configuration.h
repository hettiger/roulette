//
// Created by Martin Hettiger on 01.07.18.
//

#ifndef ROULETTE_CONFIGURATION_H
#define ROULETTE_CONFIGURATION_H


#include <ntsid.h>
#include <iostream>
#include "Samplesize.h"

using namespace std;

class Configuration {

private:
    bool debug = false;
    double startingBankroll = 200, startingBet = 10;
    uint failureLimit = 5, verbosity = 0, num_threads = 8;
    string unmatchedArgument;

public:
    bool isDebug() const;

    double getStartingBankroll() const;

    double getStartingBet() const;

    uint getFailureLimit() const;

    uint getVerbosity() const;

    uint getNum_threads() const;

    Configuration(int argc, char *argv[], Samplesize *samplesize);

private:
    void printUnmatchedArgumentInformation();

    void printHelp();

    void printDebuggingInformation(uint samplesize);

};


#endif //ROULETTE_CONFIGURATION_H
