#include "PmergeMe.hpp"
#include <iostream>
#include <sys/time.h>
#include <iomanip>

static double timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec * 1e6 + tv.tv_usec; // μs
}

static void printSequence(const std::string &label, const std::vector<int> &v)
{
    std::cout << label;
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i > 20) { std::cout << " ..."; break; }
        std::cout << " " << v[i];
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    std::vector<int> input;
    if (!PmergeMe::parseInput(argc, argv, input)) {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    printSequence("Before:", input);

    // --- vector sort ---
    std::vector<int> vSorted;
    double t0 = timestamp();
    PmergeMe::sortVector(input, vSorted);
    double t1 = timestamp();

    // --- deque sort ---
    std::deque<int> dSorted;
    double t2 = timestamp();
    PmergeMe::sortDeque(input, dSorted);
    double t3 = timestamp();

    // Show after using vector result (identical to deque)
    printSequence("After:", vSorted);

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << input.size()
              << " elements with std::vector : " << (t1 - t0) << " us" << std::endl;
    std::cout << "Time to process a range of " << input.size()
              << " elements with std::deque  : " << (t3 - t2) << " us" << std::endl;

    return 0;
}