//
// Created by Martin Hettiger on 01.07.18.
//

#include "Samplesize.h"

void Samplesize::setSize(uint size) {
    Samplesize::size = size;
}

uint Samplesize::getSize() const {
    return size;
}

Samplesize::Samplesize(uint size, std::mutex &default_mutex) {
    this->size = size;
    this->default_mutex = &default_mutex;
}

bool Samplesize::decrease() {
    default_mutex->lock();
    bool isDecreased = false;

    if (size > 0) {
        size--;
        isDecreased = true;
    }

    default_mutex->unlock();

    return isDecreased;
}
