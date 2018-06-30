//
// Created by Martin Hettiger on 30.06.18.
//

#ifndef ROULETTE_ROUND_H
#define ROULETTE_ROUND_H

#include <iostream>

class Round {

public:
    static const uint totalSpaces = 37; // european roulette without double zero

    /**
     * Execute a roulette round betting on black or red
     *
     * @return Returns whether the round is won or not
     */
    static bool execute();

private:
    /**
     * Make a random number within a given range
     *
     * @param start Lowest possible random number
     * @param stop Highest possible random number
     * @return The generated random number
     */
    static double makeRandomNumber(double start, double stop);

};


#endif //ROULETTE_ROUND_H
