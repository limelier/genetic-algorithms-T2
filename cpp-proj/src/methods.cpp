#include <cmath>
#include "../config.h"
#include "methods.h"
#include "../util/bitset.h"
#include "../util/random.h"

using Clock = std::chrono::high_resolution_clock;


result geneticSearch(const testFunction &function, double lower, double upper, int dimensions) {
    const auto startTime = Clock::now();

    // TODO

    const auto endTime = Clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    return result(); // TODO
}
