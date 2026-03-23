#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cmath>
#include <charconv>

// Pretty print a double: avoid unnecessary trailing .0
inline std::string prettyDouble(double v, int precision = 12)
{
    if (std::fabs(v - std::round(v)) < 1e-12)
    {
        return std::to_string(static_cast<long long>(std::llround(v)));
    }
    // format with maximum precision but strip trailing zeros
    char buf[64];
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), v, std::chars_format::general, precision);
    if (ec == std::errc()) return std::string(buf, ptr);
    return std::to_string(v);
}

#endif // UTILS_H
