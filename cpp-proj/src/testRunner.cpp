#include <future>
#include "testRunner.h"
#include "methods.h"
#include "../util/functions.h"
#include "../config.h"
#include <fstream>

void runTest(const std::string &filename,
             const std::function<double(std::vector<double>)> &function,
             const testMethod& method,
             double lower,
             double upper,
             double dimensions) {

    std::vector<std::future<result>> futures;
    futures.reserve(ALGORITHM_RUNS);

    for (size_t i = 0; i < ALGORITHM_RUNS; i++) {
        futures.push_back(std::async(method, function, lower, upper, dimensions));
    }

    std::ofstream fout(filename, std::ostream::ate | std::ostream::out);

    fout << "= OUTPUT START =" << std::endl;

    for (size_t i = 0; i < ALGORITHM_RUNS; i++) {
        auto result = futures[i].get();
        fout << result.fx << ' ' << result.time << ' ';
        for (auto x : result.x) {
            fout << x << ' ';
        }
        fout << std::endl;
    }

    fout.close();
}

struct funcStruct {
    std::string name;
    testFunction f;
    double lower;
    double upper;
};

void runTests() {

    std::vector<funcStruct> funcs = {
        {"rastrigin", rastrigin, -5.12, 5.12},
        {"dixon&price", dixon_price, -10, 10},
        {"michalewicz", michalewicz, 0, M_PI},
        {"sphere", sphere, -5.12, 5.12}
    };

    std::vector<std::pair<std::string, testMethod>> methodPairs = {
        {"genetic", geneticSearch}
    };

    std::vector<int> dimensions = {2, 5, 30};

    for (const auto& func : funcs) {
        for (const auto& methodPair : methodPairs) {
            for (int dim : dimensions) {
                std::string filename = "output/" + func.name + '_' + methodPair.first + '_' + std::to_string(dim) + ".txt";
                runTest(filename, func.f, methodPair.second, func.lower, func.upper, dim);
            }
        }
    }
}
