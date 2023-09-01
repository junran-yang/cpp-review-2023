#include "sumof.h"

double sumOf(const std::vector<double>& data) {
    // This is deliberately complicated! Distill what you have to use
    // and ignore what you don't.

    double result = 0;
    for (std::vector<double>::const_iterator it = data.begin();
                                        it != data.end(); ++it) {
        result += *it;
    }

    return result;
}
