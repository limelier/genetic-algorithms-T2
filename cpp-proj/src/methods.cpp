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

std::vector<std::vector<bool>> mutate(std::vector<std::vector<bool>> population) {
    for (std::vector<bool>& individual : population) {
        for (auto bit : individual) {
            if (randomSubunitary() < BIT_MUTATION_CHANCE) {
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
        population[i+1] = crossoverResult.second;
    }
    return population;
}

std::vector<std::vector<bool>> select(std::vector<std::vector<bool, std::allocator<bool>>> population, const testFunction& function) {
    typeof(population) newPop;
    std::vector<double> eval;
    std::vector<double> fitness;

    double maxValue = function(bitsetToDoubles(population[0], CHUNK, Lower, Upper));
    for (const auto& individual : population) {
        auto value = function(bitsetToDoubles(individual, CHUNK, Lower, Upper));
        eval.push_back(value);
        if (value > maxValue) maxValue = value;
    }

    auto base = maxValue * 1.1;
    for (auto value : eval) {
        auto fit = base - value;
        fitness.push_back(fit);
    }

    std::vector<double> wheel(POP_SIZE);
    wheel[0] = fitness[0];
    for (int i = 1; i < POP_SIZE; i++) {
        wheel[i] = wheel[i-1] + fitness[i];
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

std::pair<std::vector<bool>, double> bestIndividual(const std::vector<std::vector<bool>>& population, const testFunction &function) {
    double bEval;
    std::vector<bool> best;

    best = population[0];
    bEval = function(bitsetToDoubles(best, CHUNK, Lower, Upper));

    for (const auto& individual : population) {
        auto cEval = function(bitsetToDoubles(individual, CHUNK, Lower, Upper));
        if (cEval > bEval) {
            best = individual;
            bEval = cEval;
        }
    }

    return {best, bEval};
}

result geneticSearch(const testFunction &function, double lower, double upper, int dimensions) {
    Lower = lower;
    Upper = upper;
    Dimensions = dimensions;

    CHUNK = bitsetSize(Lower, Upper, PRECISION);
    BITS = CHUNK * Dimensions;

    const auto startTime = Clock::now();

    auto population = generatePopulation();
    int generation = 1;

    while (generation < GEN_LIMIT) {
        population = mutate(population);
        population = crossover(population);
        population = select(population, function);

        generation++;

        auto crtTime = Clock::now();
        auto crtDuration = std::chrono::duration_cast<std::chrono::milliseconds>(crtTime - startTime).count();
        if (crtDuration > MS_PER_ATTEMPT) {
            break;
        }
    }

    auto best = bestIndividual(population, function);
    double bEval = best.second;
    std::vector<bool> bIndividual = best.first;

    const auto endTime = Clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    return {bEval, bitsetToDoubles(bIndividual, CHUNK, Lower, Upper), duration};
}
