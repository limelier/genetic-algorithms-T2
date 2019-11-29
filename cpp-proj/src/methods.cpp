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
size_t CHUNK;

std::vector<std::vector<bool>> generatePopulation() {
    std::vector<std::vector<bool>> population;
    population.reserve(POP_SIZE);

    auto candidate = generateBitset(BITS);

    for (int i = 0; i < POP_SIZE; i++) {
        population.push_back(generateBitset(BITS));
    }

    return population;
}

std::vector<std::vector<bool>> mutate(std::vector<std::vector<bool>> population, double multiplier) {
    for (std::vector<bool> &individual : population) {
        for (auto bit : individual) {
            if (randomSubunitary() < BASE_MUTATION_CHANCE * multiplier / BITS) {
                bit = !bit;
            }
        }
    }
    return population;
}

std::vector<std::vector<bool>> crossover(std::vector<std::vector<bool>> population) {
    population = shufflePopulation(population);
    for (int i = 0; i < CROSSOVERS; i++) {
        auto crossoverResult = cross(population[i], population[i + 1]);
        population[i] = crossoverResult.first;
        population[i + 1] = crossoverResult.second;
    }
    return population;
}

std::vector<std::vector<bool>>
select(std::vector<std::vector<bool, std::allocator<bool>>> population, const testFunction &function, double &bEval,
       std::vector<bool> &bIndividual, double &multiplier) {
    typeof(population) newPop;
    double bCEval = 10000000;
    std::vector<bool> bCIndividual;
    std::vector<double> eval;
    double evalSum = 0;
    eval.reserve(POP_SIZE);
    std::vector<double> fitness;
    fitness.reserve(POP_SIZE);

    double maxValue = function(bitsetToDoubles(population[0], CHUNK, Lower, Upper));
    for (const auto &individual : population) {
        auto value = function(bitsetToDoubles(individual, CHUNK, Lower, Upper));
        eval.push_back(value);
        if (value > maxValue) maxValue = value;
        if (value < bCEval) {
            bCEval = value;
            bCIndividual = individual;
        }
        evalSum += value;
    }

    if (bCEval < bEval) {
        bEval = bCEval;
        bIndividual = bCIndividual;
    }

    double evalAvg = evalSum / double(POP_SIZE);
    if (evalAvg - bCEval <= DIVERSITY_TRESHHOLD) {
        multiplier += MULTIPLIER_INCREMENT;
    } else {
        multiplier = 1.0;
    }

    auto base = maxValue + fabs(maxValue) * BASE_MULT;
    for (auto value : eval) {
        auto fit = base - value;
        fitness.push_back(fit);
    }

    std::vector<double> wheel(POP_SIZE);
    wheel[0] = fitness[0];
    for (int i = 1; i < POP_SIZE; i++) {
        wheel[i] = wheel[i - 1] + fitness[i];
    }

    auto wheelEnd = wheel[POP_SIZE - 1];
    for (int i = 0; i < POP_SIZE; i++) {
        auto needle = randomSubunitary() * wheelEnd;

        int which = 0;
        while (needle > wheel[which]) {
            which++;
        }
        newPop.push_back(population[which]);
    }

    return newPop;
}

result geneticSearch(const testFunction &function, double lower, double upper, int dimensions) {
    double multiplier = 1.0;
    Lower = lower;
    Upper = upper;
    Dimensions = dimensions;
    double bEval = 100000000;
    std::vector<bool> bIndividual;

    CHUNK = bitsetSize(Lower, Upper, PRECISION);
    BITS = CHUNK * Dimensions;

    const auto startTime = Clock::now();

    auto population = generatePopulation();
    int generation = 1;

    while (generation < GEN_LIMIT) {
        population = mutate(population, multiplier);
        population = crossover(population);
        population = select(population, function, bEval, bIndividual, multiplier);

        generation++;

        auto crtTime = Clock::now();
        auto crtDuration = std::chrono::duration_cast<std::chrono::milliseconds>(crtTime - startTime).count();
        if (crtDuration > MS_PER_ATTEMPT) {
            break;
        }
    }

    const auto endTime = Clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    return {bEval, bitsetToDoubles(bIndividual, CHUNK, Lower, Upper), duration};
}
