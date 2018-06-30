//
// Created by Martin Hettiger on 30.06.18.
//

#include "Round.h"
#include "random"

bool Round::execute() {
    uint winningSpaces = (totalSpaces - 1) / 2; // black or red, excluding zero
    double chance = (double) winningSpaces / (double) totalSpaces;
    double randomNumber = makeRandomNumber(0.0, 1.0);

    return randomNumber <= chance;
}

double Round::makeRandomNumber(double start, double stop) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(start, stop);

    return dist(mt);
}
