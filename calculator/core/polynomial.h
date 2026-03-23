#ifndef CORE_POLYNOMIAL_H
#define CORE_POLYNOMIAL_H

#include <vector>
#include <initializer_list>
#include <algorithm>
#include <ostream>
#include <cmath>
#include <sstream>

template<typename T>
class Polynomial
{
public:
    Polynomial() = default;
    Polynomial(std::initializer_list<T> coeffs) : coeffs_(coeffs) { normalize(); }
    explicit Polynomial(const std::vector<T>& c) : coeffs_(c) { normalize(); }

    size_t degree() const { return coeffs_.empty() ? 0 : coeffs_.size() - 1; }
    const std::vector<T>& coeffs() const { return coeffs_; }

    T evaluate(T x) const
    {
        // Horner's method
        T acc = 0;
        for (auto it = coeffs_.rbegin(); it != coeffs_.rend(); ++it) acc = acc * x + *it;
        return acc;
    }

    Polynomial<T> derivative() const
    {
        if (coeffs_.size() <= 1) return Polynomial<T>{};
        std::vector<T> out(coeffs_.size() - 1);
        for (size_t i = 1; i < coeffs_.size(); ++i) out[i - 1] = coeffs_[i] * static_cast<T>(i);
        return Polynomial<T>(out);
    }

    Polynomial<T> operator+(const Polynomial<T>& o) const
    {
        std::vector<T> out(std::max(coeffs_.size(), o.coeffs_.size()), static_cast<T>(0));
        for (size_t i = 0; i < out.size(); ++i)
        {
            if (i < coeffs_.size()) out[i] += coeffs_[i];
            if (i < o.coeffs_.size()) out[i] += o.coeffs_[i];
        }
        return Polynomial<T>(out);
    }

    Polynomial<T> operator-(const Polynomial<T>& o) const
    {
        std::vector<T> out(std::max(coeffs_.size(), o.coeffs_.size()), static_cast<T>(0));
        for (size_t i = 0; i < out.size(); ++i)
        {
            if (i < coeffs_.size()) out[i] += coeffs_[i];
            if (i < o.coeffs_.size()) out[i] -= o.coeffs_[i];
        }
        return Polynomial<T>(out);
    }

    Polynomial<T> operator*(const Polynomial<T>& o) const
    {
        if (coeffs_.empty() || o.coeffs_.empty()) return Polynomial<T>{};
        std::vector<T> out(coeffs_.size() + o.coeffs_.size() - 1, static_cast<T>(0));
        for (size_t i = 0; i < coeffs_.size(); ++i)
            for (size_t j = 0; j < o.coeffs_.size(); ++j)
                out[i + j] += coeffs_[i] * o.coeffs_[j];
        return Polynomial<T>(out);
    }

    // scalar multiplication
    Polynomial<T> operator*(T s) const
    {
        std::vector<T> out(coeffs_.size());
        for (size_t i = 0; i < coeffs_.size(); ++i) out[i] = coeffs_[i] * s;
        return Polynomial<T>(out);
    }

    T operator[](size_t i) const { return (i < coeffs_.size()) ? coeffs_[i] : static_cast<T>(0); }

    std::string toString() const
    {
        std::ostringstream ss;
        bool first = true;
        for (size_t i = 0; i < coeffs_.size(); ++i)
        {
            T c = coeffs_[i];
            if (std::fabs(static_cast<double>(c)) < 1e-14) continue;
            if (!first)
            {
                ss << (c >= 0 ? " + " : " - ");
            }
            else if (c < 0) ss << "-";
            T ac = std::fabs(c);
            if (!(std::fabs(static_cast<double>(ac) - 1.0) < 1e-14 && i != 0))
                ss << ac;
            if (i >= 1) ss << "x";
            if (i >= 2) ss << "^" << i;
            first = false;
        }
        if (first) ss << "0";
        return ss.str();
    }

private:
    void normalize()
    {
        while (!coeffs_.empty() && std::fabs(static_cast<double>(coeffs_.back())) < 1e-14) coeffs_.pop_back();
    }

    std::vector<T> coeffs_;
};

// stream insertion
template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Polynomial<T>& p)
{
    os << p.toString();
    return os;
}

#endif // CORE_POLYNOMIAL_H
