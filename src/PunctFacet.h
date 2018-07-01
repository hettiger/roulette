//
// Created by Martin Hettiger on 01.07.18.
//

#ifndef ROULETTE_PUNCTFACET_H
#define ROULETTE_PUNCTFACET_H

#include <iomanip>

class PunctFacet : public std::numpunct<char> {

public:
    /**
     * Provide a separator char for decimal point
     *
     * @return Separator char
     */
    char do_decimal_point() const override;

    /**
     * Provide a separator char for thousands
     *
     * @return Separator char
     */
    char do_thousands_sep() const override;

};


#endif //ROULETTE_PUNCTFACET_H
