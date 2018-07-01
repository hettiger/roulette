#include <iostream>
#include <cmath>
#include "src/Round.h"
#include "src/Bankroll.h"
#include "src/Samplesize.h"
#include "src/Formatting.h"
#include <thread>

using namespace std;

std::mutex default_mutex;

void processSample(bool prependHeader, double startingBet, double startingBankroll, uint verbosity, uint failureLimit) {
    auto *bankroll = new Bankroll();
    double bet = startingBet, maxBankrollBalance = 0;
    uint roundNumber = 0;
    bankroll->cashIn(startingBankroll);

    if (verbosity >= 2) {
        cout << "Bankroll:\t " << Formatting::formatCurrency(bankroll->getBalance()) << endl;
    }

    while (bankroll->cashOut(bet)) {
        auto attemptNumber = static_cast<uint>(log(bet / startingBet) / log(2) + 1);
        roundNumber++;

        if (verbosity >= 2) {
            cout << "Einsatz:\t-" << Formatting::formatCurrency(bet) << endl;
        }

        if (Round::execute()) {
            double winnings = bet * 2;
            bankroll->cashIn(winnings);
            bet = startingBet;

            if (verbosity >= 2) {
                cout << "Gewinn:\t\t+" << Formatting::formatCurrency(winnings) << endl;
            }
        } else if (attemptNumber < failureLimit) {
            bet *= 2;
        } else {
            bet = startingBet;
        }

        double currentBankrollBalance = bankroll->getBalance();

        if (maxBankrollBalance < currentBankrollBalance) {
            maxBankrollBalance = currentBankrollBalance;
        }

        if (verbosity >= 2) {
            cout << "\t\t\t------------" << endl;
            cout << "Bankroll:\t " << Formatting::formatCurrency(currentBankrollBalance) << endl;
        }
    }

    if (verbosity >= 2) {
        cout << endl;
    }

    double currentBankrollBalance = bankroll->getBalance();

    if (verbosity > 0) {
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

void worker(Samplesize samplesize, double startingBet, double startingBankroll, uint verbosity, uint failureLimit) {
    while (samplesize.decrease()) {
        processSample(false, startingBet, startingBankroll, verbosity, failureLimit);
    }
}

int main(int argc, char *argv[]) {
    bool debug = false;
    auto *samplesize = new Samplesize(1, default_mutex);
    double startingBankroll = 200, startingBet = 10;
    uint failureLimit = 5, verbosity = 0, num_threads = 8;
    string unmatchedArgument;

    for (uint i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbosity = 1;
        } else if (strcmp(argv[i], "-vv") == 0) {
            verbosity = 2;
        } else if (strncmp(argv[i], "-n=", strlen("-n=")) == 0) {
            samplesize->setSize((uint) stoi(((string) argv[i]).substr(strlen("-n="))));
        } else if (strncmp(argv[i], "-r=", strlen("-r=")) == 0) {
            startingBankroll = (uint) stoi(((string) argv[i]).substr(strlen("-r=")));
        } else if (strncmp(argv[i], "-b=", strlen("-b=")) == 0) {
            startingBet = (uint) stoi(((string) argv[i]).substr(strlen("-b=")));
        } else if (strncmp(argv[i], "-l=", strlen("-l=")) == 0) {
            failureLimit = (uint) stoi(((string) argv[i]).substr(strlen("-l=")));
        } else if (strcmp(argv[i], "-h") == 0) {
            cout << "Willkommen zur Roulette Strategie Simulation." << endl << endl;
            cout << "Grundsätzlicher Ablauf:" << endl;
            cout << "\t- Es wird immer auf Rot oder Schwarz gesetzt." << endl;
            cout << "\t- Du startest mit einem von dir festgelegten Grundeinsatz." << endl;
            cout << "\t- Verlierst du, verdoppelst du deinen Einsatz." << endl;
            cout << "\t- Gewinnst du, beginnst du wieder mit deinem Grundeinsatz." << endl;
            cout << endl;
            cout << "Du hast in dieser Simulation außerdem die Möglichkeit, ";
            cout << "eine Maximalanzahl für Fehlversuche festzulegen." << endl;
            cout << "Das heißt, wenn du z. B. nach 5 Verdopplungen des Einsatzes noch nicht gewonnen hast," << endl;
            cout << "fängst du wieder mit deinem Grundeinsatz an.";
            cout << "So riskierst du in einer Runde nicht zuviel..." << endl;
            cout << endl;
            cout << "Folgende Argumente können an das Programm übergeben werden:" << endl;
            cout << "\t-d\t\t\tDebugging Information ausgeben." << endl;
            cout << "\t-v\t\t\tDas Programm gibt mehr Informationen aus." << endl;
            cout << "\t-vv\t\t\tDas Programm gibt sämtliche Informationen aus." << endl;
            cout << "\t-n=1\t\tAnzahl der Durchläufe. (Default = 1)" << endl;
            cout << "\t-r=200.00\tStartbankroll (Default = 200.00)" << endl;
            cout << "\t-b=10.00\tGrundeinsatz (Default = 10.00)" << endl;
            cout << "\t-l=5\t\tMaximal erlaubte Fehlversuche (Default = 5, Ohne Limit = 0)";

            return 0;
        } else {
            unmatchedArgument = argv[i];
        }
    }

    if (!unmatchedArgument.empty()) {
        cout << "Das Argument \"" << unmatchedArgument << "\" wird nicht unterstützt." << endl;
        cout << "Verwenden Sie \"-h\" um die Hilfe auszugeben.";

        return 1;
    }

    if (debug) {
        cout << "#####################################" << endl;
        cout << "" << endl;
        cout << "\tDebugging Information" << endl;
        cout << "\t\tVerbosity:\t\t\t" << verbosity << endl;
        cout << "\t\tDurchläufe:\t\t\t" << samplesize->getSize() << endl;
        cout << "\t\tStartbankroll:\t\t" << startingBankroll << endl;
        cout << "\t\tGrundeinsatz:\t\t" << startingBet << endl;
        cout << "\t\tMax. Fehlversuch:\t" << failureLimit << endl;
        cout << "" << endl;
        cout << "#####################################" << endl;
    }

    if (verbosity > 0 || samplesize->getSize() < num_threads) {
        for (uint sampleindex = 0; sampleindex < samplesize->getSize(); sampleindex++) {
            processSample((!(bool) sampleindex), startingBet, startingBankroll, verbosity, failureLimit);
        }
    } else {
        // The first sample should be processed single threaded to guarantee CSV header comes first.
        processSample(true, startingBet, startingBankroll, verbosity, failureLimit);
        samplesize->decrease();
        thread threads[num_threads - 1];

        // Process samples on individual threads
        for (uint threadindex = 0; threadindex < num_threads - 1; threadindex++) {
            threads[threadindex] = thread(
                    worker, *samplesize, startingBet, startingBankroll, verbosity, failureLimit
            );
        }

        // Put some load on the main thread as well
        worker(*samplesize, startingBet, startingBankroll, verbosity, failureLimit);

        // Wait for all threads to complete their work
        for (uint threadindex = 0; threadindex < num_threads - 1; threadindex++) {
            threads[threadindex].join();
        }
    }

    return 0;
}
