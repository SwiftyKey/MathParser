#ifndef FRACTION_H
#define FRACTION_H

#include <string>
#include <cmath>
#include <limits>

using namespace std;

/**
 * Класс обыкновенных дробей
 */
class Fraction {
private:

    const long long precision = 1000000000; // точность, также изначально является знаменателем

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

Fraction::Fraction(const long double &number) { ConvertDoubleToFraction(number); }

Fraction::Fraction(const string &str) { ConvertStringToFraction(str); }


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

    if (number > numeric_limits<long long>::max()) throw runtime_error("Ошибка. Слишком большое число");

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

void Fraction::ConvertStringToFraction(const string &str) {
    size_t index = str.find('.');

    try {
        // Если точка есть
        if (index != string::npos) {
            // Получаем целую часть числа (находится до точки)
            string integral = str.substr(0, index);
            if (integral.empty()) integral = "0";
            // Получаем дробную часть числа (находится после точки)
            string decimal = str.substr(index + 1, (short) log10(precision));
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
    } catch (out_of_range& error) {
        throw runtime_error("Ошибка. Слишком большое число");
    }
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

bool Fraction::operator<(const Fraction &fraction) const {
    return (numerator * fraction.GetDenominator()) < (fraction.GetNumerator() * denominator);
}

bool Fraction::operator<=(const Fraction &fraction) const {
    return (numerator * fraction.GetDenominator()) <= (fraction.GetNumerator() * denominator);
}

bool Fraction::operator>(const Fraction &fraction) const {
    return (numerator * fraction.GetDenominator()) > (fraction.GetNumerator() * denominator);
}

bool Fraction::operator>=(const Fraction &fraction) const {
    return (numerator * fraction.GetDenominator()) >= (fraction.GetNumerator() * denominator);
}

bool Fraction::operator==(const Fraction &fraction) const {
    return (numerator * fraction.GetDenominator()) == (fraction.GetNumerator() * denominator);
}

bool Fraction::operator!=(const Fraction &fraction) const {
    return (numerator * fraction.GetDenominator()) != (fraction.GetNumerator() * denominator);
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

    result.ConvertDoubleToFraction((long double) result);

    return result;
}


#endif //FRACTION_H