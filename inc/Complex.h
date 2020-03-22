#pragma once
#define _USE_MATH_DEFINES
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

    // Abs * Abs
    constexpr double Abs2() const noexcept { return re * re + im * im; }
    constexpr double Abs() const noexcept { return sqrt(Abs2()); }
    constexpr double& Re() noexcept { return re; }
    constexpr double Re() const noexcept { return re; }
    constexpr double& Im() noexcept { return im; }
    constexpr double Im() const noexcept { return im; }
    
    inline Complex Sin() const noexcept { return Complex(sin(re) * cosh(im), cos(re) * sinh(im)); }
    inline Complex Cos() const noexcept { return Complex(cos(re) * cosh(im), -sin(re) * sinh(im)); }

    constexpr Complex operator+(const Complex& cpx) const noexcept { return Complex(re + cpx.re, im + cpx.im); }
    constexpr Complex operator+(double x) const noexcept { return Complex(re + x, im + x); }
    friend constexpr Complex operator+(double x, const Complex& cpx) noexcept { return cpx + x; }
    constexpr Complex operator-(const Complex& cpx) const noexcept { return Complex(re - cpx.re, im - cpx.im); }
    constexpr Complex operator-(double x) const noexcept { return Complex(re - x, im - x); }
    friend constexpr Complex operator-(double x, const Complex& cpx) noexcept { return cpx - x; }
    constexpr Complex operator*(const Complex& cpx) const noexcept { return Complex(re * cpx.re - im * cpx.im, re * cpx.im + im * cpx.re); }
    constexpr Complex operator*(double x) const noexcept { return Complex(re * x, im * x); }
    friend constexpr Complex operator*(double x, const Complex& cpx) noexcept { return cpx * x; }

    constexpr Complex& operator=(const Complex& cpx) noexcept { re = cpx.re; im = cpx.im; return *this; }
    constexpr Complex& operator+=(const Complex& cpx) noexcept { *this = *this + cpx; return *this; }
    constexpr Complex& operator+=(double x) noexcept { *this = *this + x; return *this; }
    constexpr Complex& operator-=(const Complex& cpx) noexcept { *this = *this - cpx; return *this; }
    constexpr Complex& operator-=(double x) noexcept { *this = *this - x; return *this; }
    constexpr Complex& operator*=(const Complex& cpx) noexcept { *this = *this * cpx; return *this; }
    constexpr Complex& operator*=(double x) noexcept { *this = *this * x; return *this; }
};