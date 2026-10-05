#include "calc/calc.h"

#include <cstdlib>
#include <stdexcept>

namespace calc {

std::int64_t gcd(std::int64_t a, std::int64_t b)
{
    a = std::llabs(a);
    b = std::llabs(b);
    while (b != 0) {
        const std::int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

bool is_prime(std::int64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    for (std::int64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

std::int64_t fibonacci(int n)
{
    if (n < 0 || n > 92)
        throw std::out_of_range("fibonacci: n must be in [0, 92]");
    std::int64_t a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        const std::int64_t t = a + b;
        a = b;
        b = t;
    }
    return a;
}

std::vector<std::string> split(const std::string& s, char sep)
{
    std::vector<std::string> out;
    std::string::size_type start = 0;
    for (;;) {
        const auto pos = s.find(sep, start);
        out.emplace_back(s.substr(start, pos - start));
        if (pos == std::string::npos)
            return out;
        start = pos + 1;
    }
}

}  // namespace calc
