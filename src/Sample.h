//
// Created by Martin Hettiger on 01.07.18.
//

#ifndef ROULETTE_SAMPLE_H
#define ROULETTE_SAMPLE_H


#include "Configuration.h"

using namespace std;

class Sample {

private:
    bool prependHeader;
    Configuration *configuration;
    mutex *default_mutex;

public:
    Sample(bool prependHeader, Configuration *configuration, mutex &default_mutex);

    void execute();

};


#endif //ROULETTE_SAMPLE_H
