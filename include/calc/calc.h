#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace calc {

// Greatest common divisor of a and b (always >= 0; gcd(0, 0) == 0).
std::int64_t gcd(std::int64_t a, std::int64_t b);

// True when n is a prime number.
bool is_prime(std::int64_t n);

// The n-th Fibonacci number, fib(0) == 0. Throws std::out_of_range for n < 0
// or when the result would overflow int64 (n > 92).
std::int64_t fibonacci(int n);

// Splits s on sep; an empty input gives one empty field, like most CSV readers.
std::vector<std::string> split(const std::string& s, char sep);

}  // namespace calc
