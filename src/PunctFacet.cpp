//
// Created by Martin Hettiger on 01.07.18.
//

#include "PunctFacet.h"

char PunctFacet::do_decimal_point() const {
    return ',';
}

char PunctFacet::do_thousands_sep() const {
    return '.';
}
