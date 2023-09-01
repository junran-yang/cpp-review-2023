#include <sumof.h>

bool roundDown = false;

double sumOf(const bsl::vector<double>& data) {
    // This is deliberately complicated! Distill what you have to use
    // and ignore what you don't.

    double result = 0;
    for (bsl::vector<double>::const_iterator it = data.begin();
                                                    it != data.end(); ++it) {
        result += *it;
    }

    return roundDown ? (int) result : result;
}

// Note: The style of cast that you see here is NOT how you would do
// it in production code. We have chosen to use it because it's
// closer to how people coming from Java are likely to think
