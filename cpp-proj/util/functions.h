#ifndef PROJECT_1__FUNCTIONS_H_
#define PROJECT_1__FUNCTIONS_H_

#include <vector>
#include <cmath>
#include <functional>

// use xi in [-5.12, 5.12]
double rastrigin(const std::vector<double>& x);
double sphere(const std::vector<double>& x);

// use xi in [-10, 10]
double dixon_price(const std::vector<double>& x);

// use xi in [0, pi]
double michalewicz(const std::vector<double>& x);

typedef std::function<double(std::vector<double>)> testFunction;

#endif //PROJECT_1__FUNCTIONS_H_
