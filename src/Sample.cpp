//
// Created by Martin Hettiger on 01.07.18.
//

#include <iostream>
#include <cmath>
#include "Sample.h"
#include "Bankroll.h"
#include "Formatting.h"
#include "Round.h"

Sample::Sample(bool prependHeader, Configuration *configuration, mutex &default_mutex) {
    this->prependHeader = prependHeader;
    this->configuration = configuration;
    this->default_mutex = &default_mutex;
}

void Sample::execute() {
    auto *bankroll = new Bankroll();
    double bet = configuration->getStartingBet(), maxBankrollBalance = 0;
    uint roundNumber = 0;
    bankroll->cashIn(configuration->getStartingBankroll());

    if (configuration->getVerbosity() >= 2) {
        cout << "Bankroll:\t " << Formatting::formatCurrency(bankroll->getBalance()) << endl;
    }

    while (bankroll->cashOut(bet)) {
        auto attemptNumber = static_cast<uint>(log(bet / configuration->getStartingBet()) / log(2) + 1);
        roundNumber++;

        if (configuration->getVerbosity() >= 2) {
            cout << "Einsatz:\t-" << Formatting::formatCurrency(bet) << endl;
        }

        if (Round::execute()) {
            double winnings = bet * 2;
            bankroll->cashIn(winnings);
            bet = configuration->getStartingBet();

            if (configuration->getVerbosity() >= 2) {
                cout << "Gewinn:\t\t+" << Formatting::formatCurrency(winnings) << endl;
            }
        } else if (attemptNumber < configuration->getFailureLimit()) {
            bet *= 2;
        } else {
            bet = configuration->getStartingBet();
        }

        double currentBankrollBalance = bankroll->getBalance();

        if (maxBankrollBalance < currentBankrollBalance) {
            maxBankrollBalance = currentBankrollBalance;
        }

        if (configuration->getVerbosity() >= 2) {
            cout << "--------------------------------" << endl;
            cout << "Bankroll:\t " << Formatting::formatCurrency(currentBankrollBalance) << endl;
        }
    }

    if (configuration->getVerbosity() >= 2) {
        cout << endl;
    }

    double currentBankrollBalance = bankroll->getBalance();

    if (configuration->getVerbosity() > 0) {
        if (currentBankrollBalance <= 0) {
            cout << "Der Spieler ging nach " << roundNumber << " Runden pleite." << endl;
        } else {
            cout << "Der Spieler müsste nach " << roundNumber << " Runden von seiner Strategie abweichen." << endl;
            cout << "Er kann den nächsten Einsatz von " << Formatting::formatCurrency(bet);
            cout << " nicht mehr bezahlen." << endl;
        }

        cout << "Die Bankroll hat über den gesamten Zeitraum ein Maximum von ";
        cout << Formatting::formatCurrency(maxBankrollBalance) << " erreicht." << endl;
    } else {
        default_mutex->lock();

        if (prependHeader) {
            cout << R"(Runden,Bankroll,"Nächster Einsatz","Maximale Bankroll")" << endl;
        }

        cout << roundNumber << "," << currentBankrollBalance << "," << bet << "," << maxBankrollBalance << endl;
        default_mutex->unlock();
    }

}
