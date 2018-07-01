//
// Created by Martin Hettiger on 01.07.18.
//

#ifndef ROULETTE_SAMPLESIZE_H
#define ROULETTE_SAMPLESIZE_H

#include <mutex>
#include <ntsid.h>

class Samplesize {

private:
    uint size;
    std::mutex *default_mutex;

public:
    void setSize(uint size);

    uint getSize() const;

    Samplesize(uint size, std::mutex &default_mutex);

    bool decrease();

};


#endif //ROULETTE_SAMPLESIZE_H
