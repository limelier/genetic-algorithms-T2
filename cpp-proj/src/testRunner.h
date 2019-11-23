#ifndef T1_PROJECT_SRC_TESTRUNNER_H_
#define T1_PROJECT_SRC_TESTRUNNER_H_

#include <string>
#include <functional>
#include "../util/functions.h"

void runTest(const std::string &filename,
             const testFunction &function,
             double lower,
             double upper,
             double dimensions);

void runTests();

#endif //T1_PROJECT_SRC_TESTRUNNER_H_
