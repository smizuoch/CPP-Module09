#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <string>

class RPN {
public:
    static bool evaluate(const std::string &expr, long &result);

private:
    class Stack;
    static bool isOperator(const std::string &tok);
};

#endif // RPN_HPP