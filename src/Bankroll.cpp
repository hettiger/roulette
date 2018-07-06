//
// Created by Martin Hettiger on 30.06.18.
//

#include "Bankroll.h"
#include "Configuration.h"

Bankroll::Bankroll(double balance) : balance(balance) {}

void Bankroll::cashIn(double sum) {
    this->balance += sum;
}

bool Bankroll::cashOut(double sum) {
    if (this->balance >= sum) {
        this->balance -= sum;

        return true;
    }

    return false;
}

double Bankroll::getBalance() const {
    return this->balance;
}

double Bankroll::getStartingBet(Configuration *configuration) {
    double bet = configuration->getStartingBet();

    if (configuration->getDynamicFactor() != 0) {
        bet = floor(this->getBalance() * configuration->getDynamicFactor());
    }

    if (bet < 2.0) {
        return 2.0;
    } else if (bet > 12000.0) {
        return 12000.0;
    }

    return bet;
}
