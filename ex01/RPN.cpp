#include "RPN.hpp"
#include <sstream>
#include <cstdlib>

/* 内部スタック: std::list<int> を使い LIFO 操作 */
class RPN::Stack {
    std::list<long> _lst;
public:
    bool empty()   const { return _lst.empty(); }
    std::size_t size() const { return _lst.size(); }
    long  top()    const { return _lst.back(); }
    void  push(long v)   { _lst.push_back(v); }
    void  pop()          { _lst.pop_back(); }
};

bool RPN::isOperator(const std::string &tok) {
    return tok == "+" || tok == "-" || tok == "*" || tok == "/";
}

bool RPN::evaluate(const std::string &expr, long &result)
{
    std::istringstream iss(expr);
    std::string tok;
    Stack st;

    while (iss >> tok) {
        if (tok.size() == 1 && std::isdigit(tok[0])) {
            st.push(tok[0] - '0');
        }
        else if (isOperator(tok)) {
            if (st.size() < 2)
                return false; // オペランド不足
            long b = st.top(); st.pop();
            long a = st.top(); st.pop();
            long res = 0;
            if (tok == "+")      res = a + b;
            else if (tok == "-") res = a - b;
            else if (tok == "*") res = a * b;
            else if (tok == "/") {
                if (b == 0) return false; // ゼロ除算
                res = a / b;
            }
            st.push(res);
        }
        else {
            return false; // 無効トークン
        }
    }

    if (st.size() != 1)
        return false; // 余剰オペランド

    result = st.top();
    return true;
}