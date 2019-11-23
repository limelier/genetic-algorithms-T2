#include <cmath>
#include "../config.h"
#include "methods.h"
#include "../util/bitset.h"
#include "../util/random.h"

using Clock = std::chrono::high_resolution_clock;

double Lower;
double Upper;
double Dimensions;

size_t BITS;

std::vector<std::vector<bool>> generatePopulation() {
    std::vector<std::vector<bool>> population;
    population.reserve(POP_SIZE);

    const auto CHUNK = bitsetSize(Lower, Upper, PRECISION);
    BITS = CHUNK * Dimensions;

    auto candidate = generateBitset(BITS);

    for (int i = 0; i < POP_SIZE; i++) {
        population.push_back(generateBitset(BITS));
    }

    return population;
}

std::vector<std::vector<bool>> mutate(std::vector<std::vector<bool>> population) {
    for (std::vector<bool>& individual : population) {
        for (auto bit : individual) {
            if (randomSubunitary() < 0.1) {
                bit = !bit;
            }
        }
    }
    return population;
}

std::vector<std::vector<bool>> crossover(std::vector<std::vector<bool>> population) {
    return population;
}

std::vector<std::vector<bool>> select(std::vector<std::vector<bool, std::allocator<bool>>> population, const testFunction function) {
    return population;
}

std::vector<bool> bestIndividual(std::vector<std::vector<bool>> vector, const testFunction &function) {
    return std::vector<bool>();
}

result geneticSearch(const testFunction &function, double lower, double upper, int dimensions) {
    Lower = lower;
    Upper = upper;
    Dimensions = dimensions;

    const auto startTime = Clock::now();

    auto population = generatePopulation();
    int gens = 1;

    while (gens < GEN_LIMIT) {
        population = mutate(population);
        population = crossover(population);
        population = select(population, function);

        auto crtTime = Clock::now();
        auto crtDuration = std::chrono::duration_cast<std::chrono::milliseconds>(crtTime - startTime).count();
        if (crtDuration > MS_PER_ATTEMPT) {
            break;
        }
    }

    auto best = bestIndividual(population, function);

    const auto endTime = Clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    return result(); // TODO
}
