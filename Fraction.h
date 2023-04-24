#ifndef MATHPARSER_FRACTION_H
#define MATHPARSER_FRACTION_H

#include <string>
#include <cmath>

#include "ParseExceptions.h"

using namespace std;

class Fraction {
private:
    long long numerator, denominator;

    static long long GCD(long long a, long long b);

    static bool IsEven(const long long &a) { return (a % 2 == 0); }

public:
    explicit Fraction();

    explicit Fraction(const long double &number);

    explicit Fraction(const string &str);

    long long GetNumerator() const;

    long long GetDenominator() const;

    void SetNumerator(const long long &number);

    void SetDenominator(const long long &number);

    void TurnOver();

    long double ConvertFractionToDouble() const;

    explicit operator long double() const;

    Fraction operator+(const Fraction &fraction) const;

    Fraction operator-(const Fraction &fraction) const;

    Fraction operator-() const;

    Fraction operator*(const Fraction &fraction) const;

    Fraction operator/(const Fraction &fraction) const;

    Fraction &operator=(const Fraction &fraction);

    static Fraction Power(const Fraction &a, const Fraction &b);
};

long long Fraction::GCD(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return abs(a);
}

Fraction::Fraction() {
    numerator = 0;
    denominator = 1;
}

Fraction::Fraction(const long double &number) {
    if (!isfinite(number)) SendError(5);

    auto integral = (long long) number;
    long double decimal = number - integral;

    const long long precision = 1000000000;

    long long gcd = GCD((long long) round(decimal * precision), precision);

    denominator = precision / gcd;
    numerator = (long long) round(decimal * precision) / gcd + integral * denominator;
}

Fraction::Fraction(const string &str) {
    size_t index = str.find('.');

    if (index != string::npos) {
        long long integral = stoll(str.substr(0, index));
        string decimal = str.substr(index + 1);
        numerator = stoll(decimal);
        denominator = (long long) pow(10, decimal.size());

        long long gcd = GCD(numerator, denominator);
        numerator /= gcd;
        denominator /= gcd;

        numerator += integral * denominator;
    } else {
        numerator = stoll(str);
        denominator = 1;
    }
}

long long Fraction::GetNumerator() const { return numerator; }

long long Fraction::GetDenominator() const { return denominator; }

void Fraction::SetNumerator(const long long &number) { numerator = number; }

void Fraction::SetDenominator(const long long &number) { denominator = number; }

void Fraction::TurnOver() { swap(numerator, denominator); }

long double Fraction::ConvertFractionToDouble() const { return (long double) numerator / denominator; }

Fraction::operator long double() const { return ConvertFractionToDouble(); }

Fraction Fraction::operator+(const Fraction &fraction) const {
    Fraction result;

    if (denominator == fraction.GetDenominator()) {
        result.SetNumerator(numerator + fraction.GetNumerator());
        result.SetDenominator(denominator);
    } else {
        result.SetNumerator(numerator * fraction.GetDenominator() + denominator * fraction.GetNumerator());
        result.SetDenominator(denominator * fraction.GetDenominator());
    }

    return result;
}

Fraction Fraction::operator-(const Fraction &fraction) const {
    Fraction result;

    if (denominator == fraction.GetDenominator()) {
        result.SetNumerator(numerator - fraction.GetNumerator());
        result.SetDenominator(denominator);
    } else {
        result.SetNumerator(numerator * fraction.GetDenominator() - denominator * fraction.GetNumerator());
        result.SetDenominator(denominator * fraction.GetDenominator());
    }

    return result;
}

Fraction Fraction::operator-() const {
    Fraction result;

    result.SetNumerator(-numerator);
    result.SetDenominator(denominator);

    return result;
}

Fraction Fraction::operator*(const Fraction &fraction) const {
    Fraction result;

    result.SetNumerator(numerator * fraction.GetNumerator());
    result.SetDenominator(denominator * fraction.GetDenominator());

    return result;
}

Fraction Fraction::operator/(const Fraction &fraction) const {
    Fraction result;

    result.SetNumerator(numerator * fraction.GetDenominator());
    result.SetDenominator(denominator * fraction.GetNumerator());

    if (!isfinite((long double) result)) SendError(5);

    return result;
}

Fraction &Fraction::operator=(const Fraction &fraction) {
    if (this != &fraction) {
        this->numerator = fraction.GetNumerator();
        this->denominator = fraction.GetDenominator();
    }
    return *this;
}

Fraction Fraction::Power(const Fraction &a, const Fraction &b) {
    Fraction result;

    bool isBaseNegative = false;
    long long baseNumerator = a.GetNumerator();
    long long baseDenominator = a.GetDenominator();

    if (baseNumerator < 0) {
        isBaseNegative = true;
        baseNumerator = abs(baseNumerator);
    }

    bool isExponentNegative = false;
    long long exponentNumerator = b.GetNumerator();
    long long exponentDenominator = b.GetDenominator();

    if (exponentNumerator < 0) {
        isExponentNegative = true;
        exponentNumerator = abs(exponentNumerator);
    }

    if (isBaseNegative && !IsEven(exponentNumerator) && IsEven(exponentDenominator))
        SendError(5);

    Fraction firstFraction(pow(pow(baseNumerator, exponentNumerator), 1 / (long double) exponentDenominator));
    Fraction secondFraction(pow(pow(baseDenominator, exponentNumerator), 1 / (long double) exponentDenominator));

    result = firstFraction / secondFraction;

    if (isBaseNegative && !IsEven(exponentNumerator)) result.SetNumerator(-result.GetNumerator());

    if (isExponentNegative) result.TurnOver();

    return result;
}

#endif //MATHPARSER_FRACTION_H