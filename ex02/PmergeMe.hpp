#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>

class PmergeMe {
public:
    static bool parseInput(int argc, char **argv, std::vector<int> &out);
    static void sortVector(const std::vector<int> &in, std::vector<int> &out);
    static void sortDeque (const std::vector<int> &in, std::deque<int>   &out);
};

#endif // PMERGEME_HPP