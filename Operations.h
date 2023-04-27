#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include <cmath>
#include <map>
#include <functional>

#include "Fraction.h"

using namespace std;

/*
 * TODO: добавить тип: функция
 */

/**
 * Класс операций
 * Реализует шаблон проектирования - Singleton
 */
class Operations {
private:

    /**
     * Дружественный класс MathExpression
     * Expression имеет доступ к скрытым полям класса Operations без геттеров и сеттеров
     */
    friend class MathExpression;

    /**
     * Поле класса Operations
     * binaryOperations - хранит словарь бинарных операций:
     *  Ключ - имя операции типа string
     *  Значение - лямбда-выражение от двух переменных типа Fraction
     * Очередность операций: слева направо
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
     * Поле класса Operations
     * unaryOperations - хранит словарь унарных операций:
     *  Ключ - имя операции типа string
     *  Значение - лямбда-выражение от одной переменной типа Fraction
     * Очередность операций: слева направо
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
     * Поле класса Operations
     * operationsPriority - хранит словарь приоритетов для операций и функций:
     *  Ключ - имя операции типа string
     *  Значение - приоритет операции типа int
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

    /**
     * Закрытый конструктор по умолчанию класса Operations
     */
    Operations() {};

    /**
     * Закрытый копирующий конструктор класса Operations
     */
    Operations(const Operations &);

    /**
     * Закрытое присваивание класса Operations
     */
    const Operations &operator=(const Operations &);

public:

    /**
     * Статическая функция-член класса Operations
     * GetInstance - возвращает один единственный доступный экземпляр класс Operations
     */
    static Operations &GetInstance() {
        // onlyInstance - статическая переменная для гарантии наличия только одного экземпляра класса Operations
        static Operations onlyInstance;
        return onlyInstance;
    }

    /**
     * Функция-член класса Operations
     * AddBinaryOperation - добавляет бинарную операцию
     */
    void AddBinaryOperation(const string &name,
                            const function<Fraction(const Fraction &, const Fraction &)> &func, int priority = 3) {
        if (IsBinaryOperation(name)) throw runtime_error("Такая операция уже есть");
        binaryOperations[name] = func;
        operationsPriority[name] = priority;
    }

    /**
     * Функция-член класса Operations
     * AddUnaryOperation - добавляет унарную операцию
     */
    void AddUnaryOperation(const string &name, const function<Fraction(const Fraction &)> &func, int priority = 3) {
        if (IsUnaryOperation(name)) throw runtime_error("Такая операция уже есть");
        unaryOperations[name] = func;
        operationsPriority[name] = priority;
    }

    /**
     * Функция-член класса Operations
     * IsBinaryOperation - проверяет, является ли операция бинарной
     */
    bool IsBinaryOperation(const string &name) { return (binaryOperations.find(name) != binaryOperations.end()); }

    /**
     * Функция-член класса Operations
     * IsUnaryOperation - проверяет, является ли операция унарной
     */
    bool IsUnaryOperation(const string &name) { return (unaryOperations.find(name) != unaryOperations.end()); }
};

#endif //OPERATIONS_H