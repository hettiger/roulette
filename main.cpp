#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <locale>
#include "Round.h"
#include "Bankroll.h"

using namespace std;

class punct_facet: public std::numpunct<char>
{
    char do_decimal_point() const override { return ','; }
    char do_thousands_sep() const override { return '.'; }
};

string formatCurrency(double value) {
    stringstream ss;
    ss.imbue(locale(ss.getloc(), new punct_facet));
    ss << std::fixed << setprecision(2);
    ss << value << " Euro";

    return ss.str();
}

int main(int argc, char* argv[]) {
    auto *bankroll = new Bankroll();
    double startingBankroll = 200, startingBet = 10, bet = 10, roundNumber, maxBankrollBalance = 0;
    uint failureLimit = 5, verbosity = 0, samplesize = 1;
    bool debug = false;
    string unmatchedArgument = "";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbosity = 1;
        } else if (strcmp(argv[i], "-vv") == 0) {
            verbosity = 2;
        } else if (strncmp(argv[i], "-n=", strlen("-n=")) == 0) {
            samplesize = (uint) stoi(((string) argv[i]).substr(strlen("-n=")));
        } else if (strncmp(argv[i], "-r=", strlen("-r=")) == 0) {
            startingBankroll = (uint) stoi(((string) argv[i]).substr(strlen("-r=")));
        } else if (strncmp(argv[i], "-b=", strlen("-b=")) == 0) {
            startingBet = bet = (uint) stoi(((string) argv[i]).substr(strlen("-b=")));
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
        cout << "\t\tDurchläufe:\t\t\t" << samplesize << endl;
        cout << "\t\tStartbankroll:\t\t" << startingBankroll << endl;
        cout << "\t\tGrundeinsatz:\t\t" << startingBet << endl;
        cout << "\t\tMax. Fehlversuch:\t" << failureLimit << endl;
        cout << "" << endl;
        cout << "#####################################" << endl;
    }

    bankroll->cashIn(startingBankroll);

    if (verbosity >= 2) {
        cout << "Bankroll:\t " << formatCurrency(bankroll->getBalance()) << endl;
    }

    while (bankroll->cashOut(bet)) {
        auto attemptNumber = static_cast<uint>(log(bet / startingBet) / log(2) + 1);
        roundNumber++;


        if (verbosity >= 2) {
            cout << "Einsatz:\t-" << formatCurrency(bet) << endl;
        }

        if (Round::execute()) {
            double winnings = bet * 2;
            bankroll->cashIn(winnings);
            bet = startingBet;

            if (verbosity >= 2) {
                cout << "Gewinn:\t\t+" << formatCurrency(winnings) << endl;
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
            cout << "Bankroll:\t " << formatCurrency(currentBankrollBalance) << endl;
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
            cout << "Er kann den nächsten Einsatz von " << formatCurrency(bet) << " nicht mehr bezahlen." << endl;
        }

        cout << "Die Bankroll hat über den gesamten Zeitraum ein Maximum von ";
        cout << formatCurrency(maxBankrollBalance) << " erreicht.";
    } else {
        cout << R"(Runden,Bankroll,"Nächster Einsatz","Maximale Bankroll")" << endl;
        cout << roundNumber << "," << currentBankrollBalance << "," << bet << "," << maxBankrollBalance;
    }

    return 0;
}
