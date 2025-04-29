#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <string>

class RPN {
public:
    static bool evaluate(const std::string &expr, long &result);
};

#endif // RPN_HPP