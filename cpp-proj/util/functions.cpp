#include "functions.h"

double rastrigin(const std::vector<double> &x) {
    double n = x.size();
    double result = 10 * n;
    for (double xi : x) {
        result += (pow(xi, 2) - 10 * cos(2 * M_PI * xi));
    }
    return result;
}

double sphere(const std::vector<double> &x) {
    double result = 0.0;
    for (double xi : x) {
        result += pow(xi, 2);
    }
    return result;
}

double dixon_price(const std::vector<double> &x) {
    double x1 = x[0];
    double result = pow((x1 - 1.0), 2);
    double base;
    for (size_t i = 1; i < x.size(); i++) {
        base = (2 * pow(x[i], 2)) - x[i-1];
        result += double(i + 1) * pow(base, 2);
    }
    return result;
}

double michalewicz(const std::vector<double> &x) {
    double result = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
        result -= sin(x[i]) * pow(sin(double(i + 1) * pow(x[i], 2) / M_PI), 20);
    }
    return result;
}
