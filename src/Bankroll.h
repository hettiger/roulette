//
// Created by Martin Hettiger on 30.06.18.
//

#ifndef ROULETTE_BANKROLL_H
#define ROULETTE_BANKROLL_H

#include <string>
#include <cmath>
#include "Configuration.h"

class Bankroll {

private:
    double balance = 0.0;

public:
    explicit Bankroll(double balance);

    /**
     * Add money to the bankroll
     *
     * @param sum The amount of money to add
     */
    void cashIn(double sum);

    /**
     * Remove money from the bankroll
     *
     * @param sum The amount of money to remove
     * @return Whether or not the transaction was successful
     */
    bool cashOut(double sum);

    /**
     * Get the current balance
     *
     * @return The current balance
     */
    double getBalance() const;


    /**
     * Get a dynamic starting bet or fall back to the fixed starting bet
     *
     * @param configuration
     * @return The starting bet
     */
    double getStartingBet(Configuration *configuration);

};


#endif //ROULETTE_BANKROLL_H
