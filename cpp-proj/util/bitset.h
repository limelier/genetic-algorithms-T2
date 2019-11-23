#ifndef T1_PROJECT_UTIL_BITSET_H_
#define T1_PROJECT_UTIL_BITSET_H_

#include <cstddef>
#include <vector>

std::size_t bitsetSize(double lower, double upper, int precision);
std::vector<bool> generateBitset(size_t size);
std::vector<std::vector<bool>> generateNeighbors(std::vector<bool>& bitset);
std::vector<bool> randomNeighbor(std::vector<bool>& bitset);
double bitsetToDouble(const std::vector<bool>& bitset, size_t bits, double lower, double upper);
std::vector<bool> mergeBitsets(const std::vector<std::vector<bool>>& bitsets);
std::vector<std::vector<bool>> splitBitset(std::vector<bool> bitset, size_t chunkSize);
std::vector<double> bitsetToDoubles(const std::vector<bool>& bitset, size_t chunkSize, double lower, double upper);

#endif //T1_PROJECT_UTIL_BITSET_H_
