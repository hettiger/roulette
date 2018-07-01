//
// Created by Martin Hettiger on 01.07.18.
//

#include "Formatting.h"
#include "PunctFacet.h"
#include <sstream>

std::string Formatting::formatCurrency(double value) {
    std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), new PunctFacet));
    ss << std::fixed << std::setprecision(2);
    ss << value << " Euro";

    return ss.str();
}
