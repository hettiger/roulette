#include <iostream>
#include <cmath>
#include "src/Round.h"
#include "src/Bankroll.h"
#include "src/Samplesize.h"
#include "src/Formatting.h"
#include "src/Configuration.h"
#include <thread>

using namespace std;

std::mutex default_mutex;

void processSample(bool prependHeader, Configuration *configuration) {
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
            cout << "\t\t\t------------" << endl;
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
        default_mutex.lock();

        if (prependHeader) {
            cout << R"(Runden,Bankroll,"Nächster Einsatz","Maximale Bankroll")" << endl;
        }

        cout << roundNumber << "," << currentBankrollBalance << "," << bet << "," << maxBankrollBalance << endl;
        default_mutex.unlock();
    }
}

void worker(Samplesize *samplesize, Configuration *configuration) {
    while (samplesize->decrease()) {
        processSample(false, configuration);
    }
}

int main(int argc, char *argv[]) {
    auto *samplesize = new Samplesize(1, default_mutex);
    auto *configuration = new Configuration(argc, argv, samplesize);

    if (configuration->isDebug()) {
        cout << "#####################################" << endl;
        cout << "" << endl;
        cout << "\tDebugging Information" << endl;
        cout << "\t\tVerbosity:\t\t\t" << configuration->getVerbosity() << endl;
        cout << "\t\tDurchläufe:\t\t\t" << samplesize->getSize() << endl;
        cout << "\t\tStartbankroll:\t\t" << configuration->getStartingBankroll() << endl;
        cout << "\t\tGrundeinsatz:\t\t" << configuration->getStartingBet() << endl;
        cout << "\t\tMax. Fehlversuch:\t" << configuration->getFailureLimit() << endl;
        cout << "" << endl;
        cout << "#####################################" << endl;
    }

    if (configuration->getVerbosity() > 0 || samplesize->getSize() < configuration->getNum_threads()) {
        for (uint sampleindex = 0; sampleindex < samplesize->getSize(); sampleindex++) {
            processSample((!(bool) sampleindex), configuration);
        }
    } else {
        // The first sample should be processed single threaded to guarantee CSV header comes first.
        processSample(true, configuration);
        samplesize->decrease();
        thread threads[configuration->getNum_threads() - 1];

        // Process samples on individual threads
        for (uint threadindex = 0; threadindex < configuration->getNum_threads() - 1; threadindex++) {
            threads[threadindex] = thread(worker, samplesize, configuration);
        }

        // Put some load on the main thread as well
        worker(samplesize, configuration);

        // Wait for all threads to complete their work
        for (uint threadindex = 0; threadindex < configuration->getNum_threads() - 1; threadindex++) {
            threads[threadindex].join();
        }
    }

    return 0;
}
