#pragma once

#include <string>
#include <cmath>
#include <limits>
#include <stdexcept>

using namespace std;

/**
 * Класс обыкновенных дробей
 */
class Fraction {
private:

    /**
     * Поле класса Fraction
     * precision - хранит точность (степень 10)
     */
    const long long precision = 1000000000;

    /**
     * Поле класса Fraction
     * numerator - хранит числитель дроби
     */
    long long numerator;
    /**
     * Поле класса Fraction
     * denominator - хранит знаменатель дроби
     */
    long long denominator;

    /**
     * Закрытая функция-член класса Fraction
     * GCD - возвращает наибольший общий делитель
     * Применяется алгоритм Евклида
     */
    static long long GCD(long long a, long long b);

    /**
     * Закрытая функция-член класса Fraction
     * IsEven - возвращает true, если число четное, иначе - false
     */
    static bool IsEven(const long long &a) { return (a % 2 == 0); }

public:

    /**
     * Конструктор по умолчанию класса Fraction
     */
    explicit Fraction();

    /**
     * Конструктор с числовым аргументом класса Fraction
     */
    explicit Fraction(const long double &number);

    /**
     * Конструктор с числовым аргументом класса Fraction
     */
    explicit Fraction(const string &str);

    /**
     * Функция-член класса Fraction
     * GetNumerator - возвращает числитель дроби
     */
    long long GetNumerator() const;

    /**
     * Функция-член класса Fraction
     * GetDenominator - возвращает знаменатель дроби
     */
    long long GetDenominator() const;

    /**
     * Функция-член класса Fraction
     * SetNumerator - изменяет числитель дроби
     */
    void SetNumerator(const long long &number);

    /**
     * Функция-член класса Fraction
     * SetDenominator - изменяет знаменатель дроби
     */
    void SetDenominator(const long long &number);

    /**
     * Функция-член класса Fraction
     * TurnOver - переворачивает дробь (числитель и знаменатель меняются местами)
     */
    void TurnOver();

    /**
     * Функция-член класса Fraction
     * ConvertFractionToDouble - переводит обыкновенную дробь в десятичную
     */
    long double ConvertFractionToDouble() const;

    /**
     * Функция-член класса Fraction
     * ConvertDoubleToFraction - переводит дробное число в обыкновенную дробь
     */
    void ConvertDoubleToFraction(const long double &number);

    /**
     * Функция-член класса Fraction
     * ConvertStringToFraction - переводит дробное число записанное в строке в обыкновенную дробь
     */
    void ConvertStringToFraction(const string &str);

    /**
     * Перегрузка операторов
     */

    explicit operator long double() const;

    Fraction operator+(const Fraction &fraction) const;

    Fraction operator-(const Fraction &fraction) const;

    Fraction operator-() const;

    Fraction operator*(const Fraction &fraction) const;

    Fraction operator/(const Fraction &fraction) const;

    Fraction &operator=(const Fraction &fraction);

    bool operator<(const Fraction &fraction) const;

    bool operator<=(const Fraction &fraction) const;

    bool operator>(const Fraction &fraction) const;

    bool operator>=(const Fraction &fraction) const;

    bool operator==(const Fraction &fraction) const;

    bool operator!=(const Fraction &fraction) const;

    /**
     * Функция-член класса Fraction
     * Power - возводит дробь в дробную степень и возвращает полученную дробь
     */
    static Fraction Power(const Fraction &a, const Fraction &b);
};
