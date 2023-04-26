#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include <cmath>
#include <map>
#include <functional>

#include "Fraction.h"

using namespace std;

/**
 * Класс операций
 * Реализует шаблон проектирования - Singleton
 */
class Operations {
private:

    /**
     * Дружественный класс Expression
     * Expression имеет доступ к скрытым переменным-членам класса Operations без геттеров и сеттеров
     */
    friend class Expression;

    /**
     * Словарь бинарных операций
     * Ключ - имя операции типа string
     * Значение - лямбда-выражение от двух переменных типа Fraction
     */
    map<string, function<Fraction(const Fraction &, const Fraction &)>> binaryOperations = {
            {"+", [](const Fraction &a, const Fraction &b) { return a + b; }},
            {"-", [](const Fraction &a, const Fraction &b) { return a - b; }},
            {"*", [](const Fraction &a, const Fraction &b) { return a * b; }},
            {"/", [](const Fraction &a, const Fraction &b) { return a / b; }},
            {"^", [](const Fraction &a, const Fraction &b) { return Fraction::Power(a, b); }},
            {"e", [](const Fraction &a, const Fraction &b) { return a * Fraction::Power(Fraction(10.0), b); }}
    };

    /**
     * Словарь унарных операций
     * Ключ - имя операции типа string
     * Значение - лямбда-выражение от одной переменной типа Fraction
     */
    map<string, function<Fraction(const Fraction &)>> unaryOperations = {
            {"+",      [](const Fraction &a) { return a; }},
            {"-",      [](const Fraction &a) { return -a; }},
            {"sin",    [](const Fraction &a) { return Fraction(sin((long double) a)); }},
            {"cos",    [](const Fraction &a) { return Fraction(cos((long double) a)); }},
            {"tg",     [](const Fraction &a) { return Fraction(tan((long double) a)); }},
            {"tan",    [](const Fraction &a) { return Fraction(tan((long double) a)); }},
            {"ctg",    [](const Fraction &a) { return Fraction(cos((long double) a) / sin((long double) a)); }},
            {"arcsin", [](const Fraction &a) { return Fraction(asin((long double) a)); }},
            {"arccos", [](const Fraction &a) { return Fraction(acos((long double) a)); }},
            {"arctg",  [](const Fraction &a) { return Fraction(atan((long double) a)); }},
            {"arctan", [](const Fraction &a) { return Fraction(atan((long double) a)); }},
            {"arcctg", [](const Fraction &a) { return Fraction(M_PI_2 - atan((long double) a)); }},
            {"asin",   [](const Fraction &a) { return Fraction(asin((long double) a)); }},
            {"acos",   [](const Fraction &a) { return Fraction(acos((long double) a)); }},
            {"atg",    [](const Fraction &a) { return Fraction(atan((long double) a)); }},
            {"atan",   [](const Fraction &a) { return Fraction(atan((long double) a)); }},
            {"actg",   [](const Fraction &a) { return Fraction(M_PI_2 - atan((long double) a)); }},
            {"abs",    [](const Fraction &a) { return Fraction(abs((long double) a)); }},
            {"int",    [](const Fraction &a) { return Fraction(floor((long double) a)); }},
            {"sqrt",   [](const Fraction &a) { return Fraction::Power(a, Fraction(0.5)); }}
    };

    /**
     * Словарь приоритетов бинарных операций
     * Ключ - имя операции типа string
     * Значение - приоритет операции типа int
     */
    map<string, int> operationsPriority = {
            {"+",      1},
            {"-",      1},
            {"*",      2},
            {"/",      2},
            {"^",      3},
            {"e",      3},
            {"sin",    3},
            {"cos",    3},
            {"tg",     3},
            {"tan",    3},
            {"ctg",    3},
            {"arcsin", 3},
            {"arccos", 3},
            {"arctg",  3},
            {"arctan", 3},
            {"arcctg", 3},
            {"asin",   3},
            {"acos",   3},
            {"atg",    3},
            {"atan",   3},
            {"actg",   3},
            {"abs",    3},
            {"int",    3},
            {"sqrt",   3}
    };

    Operations() {}; // Закрытый конструктор по умолчанию

    Operations(const Operations &); // Закрытый копирующий конструктор

    const Operations &operator=(const Operations &); // Закрытое присваивание

public:

    /**
     * Статическая функция-член
     * Возвращает один единственный доступный экземпляр класс Operations
     */
    static Operations &GetInstance() {
        /**
         * Статическая переменная для гарантии наличия только одного экземпляра класса Operations
         */
        static Operations onlyInstance;
        return onlyInstance;
    }

    /**
     * Функция-член для добавления бинарной операции
     */
    void AddBinaryOperation(const string &name,
                            const function<Fraction(const Fraction &, const Fraction &)> &func, int priority = 3) {
        if (IsBinaryOperation(name)) throw runtime_error("Такая операция уже есть");
        binaryOperations[name] = func;
        operationsPriority[name] = priority;
    }

    /**
     * Функция-член для добавления унарной операции
     */
    void AddUnaryOperation(const string &name, const function<Fraction(const Fraction &)> &func, int priority = 3) {
        if (IsUnaryOperation(name)) throw runtime_error("Такая операция уже есть");
        unaryOperations[name] = func;
        operationsPriority[name] = priority;
    }

    /**
     * Функция-член для проверки того, является ли операция бинарной
     */
    bool IsBinaryOperation(const string &name) { return (binaryOperations.find(name) != binaryOperations.end()); }

    /**
     * Функция-член для проверки того, является ли операция унарной
     */
    bool IsUnaryOperation(const string &name) { return (unaryOperations.find(name) != unaryOperations.end()); }
};

#endif //OPERATIONS_H