#ifndef T1_PROJECT_SRC_METHODS_H_
#define T1_PROJECT_SRC_METHODS_H_

#include <functional>
#include "../util/result.h"
#include "../util/functions.h"

result geneticSearch(const testFunction &function,
        double lower,
        double upper,
        int dimensions);

typedef std::function<result(testFunction, double, double, int)> testMethod;

#endif //T1_PROJECT_SRC_METHODS_H_
