//
// Created by Martin Hettiger on 01.07.18.
//

#include "Configuration.h"

bool Configuration::isDebug() const {
    return debug;
}

double Configuration::getStartingBankroll() const {
    return startingBankroll;
}

double Configuration::getStartingBet() const {
    return startingBet;
}

uint Configuration::getFailureLimit() const {
    return failureLimit;
}

uint Configuration::getVerbosity() const {
    return verbosity;
}

uint Configuration::getNum_threads() const {
    return num_threads;
}

Configuration::Configuration(int argc, char **argv, Samplesize *samplesize) {
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
            printHelp();
            exit(0);
        } else {
            unmatchedArgument = argv[i];
        }

        if (!unmatchedArgument.empty()) {
            printUnmatchedArgumentInformation();
            exit(1);
        }

        if (isDebug()) {
            printDebuggingInformation(samplesize->getSize());
        }
    }
}

void Configuration::printUnmatchedArgumentInformation() {
    cout << "Das Argument \"" << unmatchedArgument << "\" wird nicht unterstützt." << endl;
    cout << "Verwenden Sie \"-h\" um die Hilfe auszugeben." << endl;
}

void Configuration::printHelp() {
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
    cout << "fängst du wieder mit deinem Grundeinsatz an. ";
    cout << "So riskierst du in einer Runde nicht zuviel..." << endl;
    cout << endl;
    cout << "Folgende Argumente können an das Programm übergeben werden:" << endl;
    cout << "-d" << endl << "Debugging Information ausgeben." << endl << endl;
    cout << "-v" << endl << "Das Programm gibt mehr Informationen aus." << endl << endl;
    cout << "-vv" << endl << "Das Programm gibt sämtliche Informationen aus." << endl << endl;
    cout << "-n=1" << endl << "Anzahl der Durchläufe. (Default = 1)" << endl << endl;
    cout << "-r=200.00" << endl << "Startbankroll (Default = 200.00)" << endl << endl;
    cout << "-b=10.00" << endl << "Grundeinsatz (Default = 10.00)" << endl << endl;
    cout << "-l=5" << endl << "Maximal erlaubte Fehlversuche (Default = 5, Ohne Limit = 0)" << endl;
}

void Configuration::printDebuggingInformation(uint samplesize) {
    cout << "#####################################" << endl;
    cout << "" << endl;
    cout << "Debugging Information" << endl;
    cout << "\tVerbosity: " << getVerbosity() << endl;
    cout << "\tDurchläufe: " << samplesize << endl;
    cout << "\tStartbankroll: " << getStartingBankroll() << endl;
    cout << "\tGrundeinsatz: " << getStartingBet() << endl;
    cout << "\tMax. Fehlversuch: " << getFailureLimit() << endl;
    cout << "" << endl;
    cout << "#####################################" << endl;
}
