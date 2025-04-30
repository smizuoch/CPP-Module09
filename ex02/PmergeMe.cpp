#include "PmergeMe.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <deque>

/* ---------- Jacobsthal utilities : UNIQUE sequence 1,3,5,11,... ---- */

// 1,3,5,11,21,... を n 以下で返す（重複なし／昇順）
static std::vector<size_t> jacobSequence(size_t n)
{
    std::vector<size_t> seq;
    if (n == 0) return seq;

    size_t a = 1;      // J(1)
    size_t b = 1;      // J(0)=0 を畳み込み、次計算用に保持
    while (a <= n) {
        seq.push_back(a);          // a は毎回一意
        size_t next = a + 2 * b;   // J(n)=J(n-1)+2*J(n-2)
        b = a;
        a = next;
    }
    return seq;
}

// i (1-origin) が Jacobsthal 数か？
static bool isJacobIndex(size_t i)
{
    if (i == 0) return false;
    size_t a = 1, b = 1;           // J(1)=1, b は J(n-2) 用
    while (a < i) {
        size_t next = a + 2 * b;
        b = a;
        a = next;
    }
    return a == i;
}
/* ------------------------------------------------------------------- */

/* ---------------------------- 入力処理 ---------------------------- */
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

/* ---------- Ford-Johnson (merge-insertion) sort ---------- */
template <typename Cont>
void fordJohnson(Cont &C)
{
    typedef typename Cont::value_type value_type;
    const size_t n = C.size();
    if (n <= 1) return;                    // base-case

    /* 1) ペア分割 */
    Cont mainChain, pend;
    typename Cont::iterator it = C.begin();
    while (it != C.end()) {
        value_type first = *it++;          // 1 個目
        if (it == C.end()) {               // straggler
            pend.push_back(first);
            break;
        }
        value_type second = *it;           // 2 個目
        if (first > second) std::swap(first, second);
        mainChain.push_back(second);       // 大 → main
        pend.push_back(first);             // 小 → pend
        ++it;
    }

    /* 2) mainChain を再帰的にソート */
    fordJohnson(mainChain);

    /* 3) straggler を pend 挿入より前に処理 */
    if (pend.size() > mainChain.size()) {
        value_type straggler = pend.back();
        pend.pop_back();
        mainChain.insert(
            std::lower_bound(mainChain.begin(), mainChain.end(), straggler),
            straggler);
    }

    /* 4) Jacobsthal シーケンスに従って pend を挿入 */
    std::vector<size_t> jac = jacobSequence(pend.size());   // 1-origin
    for (std::vector<size_t>::const_iterator k = jac.begin();
         k != jac.end(); ++k) {
        value_type v = pend[*k - 1];                        // 0-origin
        mainChain.insert(
            std::lower_bound(mainChain.begin(), mainChain.end(), v), v);
    }

    /* 5) 残りの pend 要素を順次挿入 */
    for (size_t i = 0; i < pend.size(); ++i) {              // i:0-origin
        if (!isJacobIndex(i + 1)) {                         // 1-origin
            mainChain.insert(
                std::lower_bound(mainChain.begin(), mainChain.end(), pend[i]),
                pend[i]);
        }
    }
    C.swap(mainChain);                                      // 結果を返却
}

/* ---------------- コンテナごとの公開 API ------------------- */
void PmergeMe::sortVector(const std::vector<int> &in, std::vector<int> &out)
{
    out = in;
    fordJohnson(out);
}

void PmergeMe::sortDeque(const std::vector<int> &in, std::deque<int> &out)
{
    out.assign(in.begin(), in.end());
    fordJohnson(out);
}
