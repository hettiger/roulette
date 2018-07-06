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
    auto *bankroll = new Bankroll(configuration->getStartingBankroll());
    double bet = bankroll->getStartingBet(configuration), maxBankrollBalance = 0;
    uint numRounds = 0, numWonRounds = 0, numAttempts = 0, numBlacks = 0, numZeros = 0, numReds = 0;

    if (configuration->getVerbosity() >= 2) {
        cout << "Bankroll:\t " << Formatting::formatCurrency(bankroll->getBalance()) << endl;
    }

    while (bankroll->cashOut(bet)) {
        numAttempts++;
        numRounds++;

        if (configuration->getVerbosity() >= 2) {
            cout << "Einsatz:\t-" << Formatting::formatCurrency(bet) << endl;
        }

        auto result = Round::execute();

        if (result == 1) {
            double winnings = bet * 2;
            numAttempts = 0;
            numWonRounds++;
            numBlacks++;
            bankroll->cashIn(winnings);
            bet = bankroll->getStartingBet(configuration);

            if (configuration->getVerbosity() >= 2) {
                cout << "Gewinn:\t\t+" << Formatting::formatCurrency(winnings) << endl;
            }
        } else if (result == 0) {
            double refund = bet / 2;
            bankroll->cashIn(refund);
            numZeros++;
            bet *= 2;

            if (configuration->getVerbosity() >= 2) {
                cout << "Erstattung:\t+" << Formatting::formatCurrency(refund) << endl;
            }
        } else if (numAttempts < configuration->getFailureLimit()) {
            numReds++;
            bet *= 2;
        } else {
            numReds++;
            numAttempts = 0;
            bet = bankroll->getStartingBet(configuration);
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
            cout << "Der Spieler ging nach " << numRounds << " Runden pleite." << endl;
        } else {
            cout << "Der Spieler müsste nach " << numRounds << " Runden von seiner Strategie abweichen." << endl;
            cout << "Er kann den nächsten Einsatz von " << Formatting::formatCurrency(bet);
            cout << " nicht mehr bezahlen." << endl;
        }

        cout << "Die Bankroll hat über den gesamten Zeitraum ein Maximum von ";
        cout << Formatting::formatCurrency(maxBankrollBalance) << " erreicht." << endl;
    } else {
        default_mutex->lock();

        if (prependHeader) {
            cout << R"(Runden,Schwarz,Null,Rot,Bankroll,"Nächster Einsatz","Maximale Bankroll")" << endl;
        }

        cout << numRounds << "," << numBlacks << "," << numZeros << "," << numReds << ",";
        cout << currentBankrollBalance << "," << bet << "," << maxBankrollBalance << endl;
        default_mutex->unlock();
    }

}
