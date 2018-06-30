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
    double startingBankroll;
    double startingBet;
    uint failureLimit;
    bool debug = false;
    uint8_t verbosity = 0;
    double bet;
    double roundNumber = 0;
    double maxBankrollBalance = 0;
    string unmatchedArgument = "";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            verbosity = 1;
        } else if (strcmp(argv[i], "-vv") == 0) {
            verbosity = 2;
        } else if (strcmp(argv[i], "-h") == 0) {
            cout << "Diese Software simuliert eine beliebte Roulette Strategie." << endl;
            cout << "Folgende Argumente können an das Programm übergeben werden:" << endl;
            cout << "\t-v\t\tDas Programm gibt mehr Informationen aus." << endl;
            cout << "\t-vv\t\tDas Programm gibt sämtliche Informationen aus." << endl;
            cout << "\t-d\t\tDas Programm startet ohne Konfigurationsmöglichkeiten.";

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
        startingBankroll = 200.0;
        startingBet = bet = 10.0;
        failureLimit = 5;
    } else {
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
        cout << "fängst du wieder mit deinem Grundeinsatz an. So riskierst du in einer Runde nicht zuviel..." << endl;
        cout << endl;

        cout << "Mit wie viel Geld in der Bankroll möchtest du starten?" << endl << "\t";
        cin >> startingBankroll;

        cout << "Mit welchem Grundeinsatz möchtest du spielen?" << endl << "\t";
        cin >> startingBet;
        bet = startingBet;

        cout << "Wie viele Fehlversuche möchtest du als Obergrenze festlegen? (0 = Spielen ohne Limit)" << endl << "\t";
        cin >> failureLimit;

        cout << endl;
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
