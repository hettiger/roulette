//
// Created by Martin Hettiger on 30.06.18.
//

#include "Bankroll.h"

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
