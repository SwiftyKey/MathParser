#ifndef FRACTION_H
#define FRACTION_H

#include <string>
#include <cmath>

using namespace std;

/**
 * Класс обыкновенных дробей
 */
class Fraction {
private:

    /**
     * numerator - числитель
     * denominator - знаменатель
     */
    long long numerator, denominator;

    /**
     * Функция-член для нахождения наибольшего общего делителя через Алгоритм Евклида
     */
    static long long GCD(long long a, long long b);

    /**
     * Функция-член для проверки числа на четность
     */
    static bool IsEven(const long long &a) { return (a % 2 == 0); }

public:

    /**
     * Конструкторы
     */
    explicit Fraction();

    explicit Fraction(const long double &number);

    explicit Fraction(const string &str);

    /**
     * Геттер для числителя
     */
    long long GetNumerator() const;

    /**
     * Геттер для знаменателя
     */
    long long GetDenominator() const;

    /**
     * Сеттер для числителя
     */
    void SetNumerator(const long long &number);

    /**
     * Сеттер для знаменателя
     */
    void SetDenominator(const long long &number);

    /**
     * Функция-член для переворота дроби
     */
    void TurnOver();

    /**
     * Функция-член для перевода обыкновенной дроби в десятичную
     */
    long double ConvertFractionToDouble() const;

    /**
     * Функция-член для перевода десятичной дроби в обыкновенную
     */
    void ConvertDoubleToFraction(const long double &number);

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

    /**
     * Функция-член для возведения дроби в дробную степень
     */
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

Fraction::Fraction(const long double &number) { ConvertDoubleToFraction(number);}

Fraction::Fraction(const string &str) {
    size_t index = str.find('.');

    // Если точка есть
    if (index != string::npos) {
        // Получаем целую часть числа (находится до точки)
        string integral = str.substr(0, index);
        if (integral.empty()) integral = "0";
        // Получаем дробную часть числа (находится после точки)
        string decimal = str.substr(index + 1);
        if (decimal.empty()) decimal = "0";
        // Получаем числитель
        numerator = stoll(decimal);
        // Получаем знаменатель
        denominator = (long long) pow(10, decimal.size());

        // Находим наибольший общий делитель числителя и знаменателя
        long long gcd = GCD(numerator, denominator);
        // Сокращаем знаменатель насколько возможно
        numerator /= gcd;
        // Сокращаем числитель насколько возможно
        denominator /= gcd;

        // Прибавляем оставшуюся целую часть
        numerator += stoll(integral) * denominator;
        // Если точки нет, то есть переданное число является целым
    } else {
        // Получаем числитель
        numerator = stoll(str);
        denominator = 1;
    }
}

long long Fraction::GetNumerator() const { return numerator; }

long long Fraction::GetDenominator() const { return denominator; }

void Fraction::SetNumerator(const long long &number) {
    numerator = number;
}

void Fraction::SetDenominator(const long long &number) {
    denominator = number;
}

void Fraction::TurnOver() {
    // Для того чтобы получить перевернутую дробь, нужно поменять числитель и знаменатель местами
    swap(numerator, denominator);
}

long double Fraction::ConvertFractionToDouble() const { return (long double) numerator / denominator; }

void Fraction::ConvertDoubleToFraction(const long double &number) {
    if (!isfinite(number)) throw runtime_error("Ошибка вычисления. Проверьте выражение");

    const long long precision = 1000000000; // точность, также изначально является знаменателем

    auto integral = (long long) number; // целая часть числа
    long double decimal = number - integral; // дробная часть числа

    // Округляем дробную часть числа до точности precision, также изначально является числителем без учета целой части
    auto roundedDecimal = (long long) round(decimal * precision);

    // Находим наибольший общий делитель числителя и знаменателя
    long long gcd = GCD(roundedDecimal, precision);

    // Сокращаем знаменатель насколько возможно
    denominator = precision / gcd;
    // Сокращаем числитель насколько возможно и прибавляем оставшуюся целую часть
    numerator = roundedDecimal / gcd + integral * denominator;
}

Fraction::operator long double() const { return ConvertFractionToDouble(); }

Fraction Fraction::operator+(const Fraction &fraction) const {
    Fraction result;

    // Правило сложения дробей по правилам математики
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

    // Правило вычитания дробей по правилам математики
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

    // Получаем дробь, противоположную по знаку
    result.SetNumerator(-numerator);
    result.SetDenominator(denominator);

    return result;
}

Fraction Fraction::operator*(const Fraction &fraction) const {
    Fraction result;

    // Правило умножения дробей по правилам математики
    result.SetNumerator(numerator * fraction.GetNumerator());
    result.SetDenominator(denominator * fraction.GetDenominator());

    return result;
}

Fraction Fraction::operator/(const Fraction &fraction) const {
    Fraction result;

    // Правило деления дробей по правилам математики
    result.SetNumerator(numerator * fraction.GetDenominator());
    result.SetDenominator(denominator * fraction.GetNumerator());

    if (!isfinite((long double) result)) throw runtime_error("Ошибка вычисления. Проверьте выражение");

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

    // если основание степени отрицательное
    if (baseNumerator < 0) {
        isBaseNegative = true;
        baseNumerator = abs(baseNumerator);
    }

    bool isExponentNegative = false;
    long long exponentNumerator = b.GetNumerator();
    long long exponentDenominator = b.GetDenominator();

    // если показатель степени отрицательный
    if (exponentNumerator < 0) {
        isExponentNegative = true;
        exponentNumerator = abs(exponentNumerator);
    }

    if (isBaseNegative && !IsEven(exponentNumerator) && IsEven(exponentDenominator))
        throw runtime_error("Ошибка вычисления. Извлечение четного корня из отрицательного числа");

    // возводим числитель основания степени в дробную степень, где числитель - возведение степени, а знаменатель - извлечение корня
    Fraction firstFraction(pow(pow(baseNumerator, exponentNumerator), 1 / (long double) exponentDenominator));
    // возводим знаменатель основания степени в дробную степень, где числитель - возведение степени, а знаменатель - извлечение корня
    Fraction secondFraction(pow(pow(baseDenominator, exponentNumerator), 1 / (long double) exponentDenominator));

    // Так как получившиеся числитель и знаменатель могут быть нецелыми, делим получившиеся дроби
    result = firstFraction / secondFraction;

    // Если основание степени отрицательное и оно возводится в нечетную степень, то сохраняем минус
    if (isBaseNegative && !IsEven(exponentNumerator)) result.SetNumerator(-result.GetNumerator());

    // Если показатель степени отрицательный, то переворачиваем получившуюся дробь
    if (isExponentNegative) result.TurnOver();

    result.ConvertDoubleToFraction((long double)result);

    return result;
}


#endif //FRACTION_H