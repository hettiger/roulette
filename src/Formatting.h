//
// Created by Martin Hettiger on 01.07.18.
//

#ifndef ROULETTE_FORMATTING_H
#define ROULETTE_FORMATTING_H

#include <string>

class Formatting {

public:
    /**
     * Take a double and apply the correct currency formatting for germany
     *
     * @param value The double value to be formatted
     * @return Correctly formatted currency string
     */
    static std::string formatCurrency(double value);

};


#endif //ROULETTE_FORMATTING_H
