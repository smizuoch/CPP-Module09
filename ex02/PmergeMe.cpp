#include "PmergeMe.hpp"
#include <sstream>
#include <iostream>
#include <sys/time.h>
#include <algorithm>
#include <cstdlib>

// ---------------------------- 入力処理 ---------------------------- //
bool PmergeMe::parseInput(int argc, char **argv, std::vector<int> &out)
{
    for (int i = 1; i < argc; ++i) {
        std::istringstream iss(argv[i]);
        long n = 0;
        if (!(iss >> n) || !iss.eof() || n <= 0 || n > 2147483647)
            return false;
        out.push_back(static_cast<int>(n));
    }
    return !out.empty();
}

// ----------------- Ford‑Johnson (コンテナ非依存) ------------------ //

template <typename RandIt>
static void fordJohnson(RandIt begin, RandIt end)
{
    typedef typename RandIt::value_type T;
    std::size_t len = end - begin;
    if (len < 2) return;

    // 1) 奇数なら末尾 1 要素を straggler として外す
    bool hasStraggler = (len % 2 != 0);
    T straggler;
    if (hasStraggler) {
        --end;
        straggler = *end;
        --len;
    }

    // 2) ペア作成 (小→pend, 大→main)
    std::vector<T> pend;       pend.reserve(len / 2);
    std::vector<T> mainChain;  mainChain.reserve(len / 2);

    for (RandIt it = begin; it < end; it += 2) {
        T a = *it;
        T b = *(it + 1);
        if (b < a) std::swap(a, b);
        pend.push_back(a);
        mainChain.push_back(b);
    }

    // 3) mainChain を挿入ソート (最長でも 1500 要素程度)
    for (std::size_t i = 1; i < mainChain.size(); ++i) {
        T key = mainChain[i];
        std::size_t j = i;
        while (j && mainChain[j-1] > key) {
            mainChain[j] = mainChain[j-1];
            --j;
        }
        mainChain[j] = key;
    }

    // 4) Jacobsthal sequence 生成
    std::vector<std::size_t> jacSeq;
    for (std::size_t n = 3;; ++n) {
        unsigned long j2 = 0, j1 = 1, j = 1;
        for (std::size_t k = 2; k <= n; ++k) { j = j1 + 2 * j2; j2 = j1; j1 = j; }
        if (j >= pend.size()) break;
        jacSeq.push_back(j);
    }

    // 5) mainChain を結果ベクタにコピーし、pend を順序通り挿入
    std::vector<T> result = mainChain;
    std::vector<bool> done(pend.size(), false);

    for (std::size_t idx = 0; idx < jacSeq.size(); ++idx) {
        std::size_t p = jacSeq[idx];
        result.insert(std::upper_bound(result.begin(), result.end(), pend[p]), pend[p]);
        done[p] = true;
    }
    for (std::size_t p = 0; p < pend.size(); ++p)
        if (!done[p])
            result.insert(std::upper_bound(result.begin(), result.end(), pend[p]), pend[p]);

    // 6) straggler を最後に挿入
    if (hasStraggler)
        result.insert(std::upper_bound(result.begin(), result.end(), straggler), straggler);

    // 7) 元コンテナへ書き戻し
    std::copy(result.begin(), result.end(), begin);
}

// ---------------- コンテナごとの公開 API ------------------------ //

void PmergeMe::sortVector(const std::vector<int> &in, std::vector<int> &out)
{
    out = in;
    fordJohnson(out.begin(), out.end());
}

void PmergeMe::sortDeque(const std::vector<int> &in, std::deque<int> &out)
{
    out.assign(in.begin(), in.end());
    fordJohnson(out.begin(), out.end());
}