//
// Created by Martin Hettiger on 30.06.18.
//

#ifndef ROULETTE_BANKROLL_H
#define ROULETTE_BANKROLL_H

#include <string>

class Bankroll {

private:
    double balance = 0.0;

public:
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

};


#endif //ROULETTE_BANKROLL_H
