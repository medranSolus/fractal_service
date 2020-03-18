#pragma once
#include <cmath>

class Complex
{
    double re = 0.0;
    double im = 0.0;

public:
    Complex() = default;
    constexpr Complex(double re, double im) noexcept : re(re), im(im) {}
    Complex(const Complex&) = default;
    Complex(Complex&&) = default;
    ~Complex() = default;

    constexpr double GetRe() const noexcept { return re; }
    constexpr double GetIm() const noexcept { return im; }
    constexpr double GetAbs() const noexcept { return sqrt(re * re + im * im); }

    constexpr Complex operator+(const Complex& cpx) const noexcept { return Complex(re + cpx.re, im + cpx.im); }
    constexpr Complex operator-(const Complex& cpx) const noexcept { return Complex(re - cpx.re, im - cpx.im); }
    constexpr Complex operator*(const Complex& cpx) const noexcept { return Complex(re * cpx.re - im * cpx.im, re * cpx.im + im * cpx.re); }

    constexpr Complex& operator=(const Complex& cpx) noexcept { re = cpx.re; im = cpx.im; return *this; }
    constexpr Complex& operator+=(const Complex& cpx) noexcept { *this = *this + cpx; return *this; }
    constexpr Complex& operator-=(const Complex& cpx) noexcept { *this = *this - cpx; return *this; }
    constexpr Complex& operator*=(const Complex& cpx) noexcept { *this = *this * cpx; return *this; }
};