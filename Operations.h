#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include <vector>
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
            {"+", [](const Fraction &a) { return a; }},
            {"-", [](const Fraction &a) { return -a; }}
    };

    /**
     * Поле класса Operations
     * functions - хранит словарь функций:
     *  Ключ - имя функции типа string
     *  Значение - лямбда-выражение от одной и более переменной типа Fraction
     * Очередность функций: слева направо
     */
    map<string, function<Fraction(const vector<Fraction> &)>> functions{
            {"sin",    [](const vector<Fraction> &a) { return Fraction(sin((long double) a[0])); }},
            {"cos",    [](const vector<Fraction> &a) { return Fraction(cos((long double) a[0])); }},
            {"tg",     [](const vector<Fraction> &a) { return Fraction(tan((long double) a[0])); }},
            {"tan",    [](const vector<Fraction> &a) { return Fraction(tan((long double) a[0])); }},
            {"ctg",    [](const vector<Fraction> &a) {
                return Fraction(cos((long double) a[0]) / sin((long double) a[0]));
            }},
            {"arcsin", [](const vector<Fraction> &a) { return Fraction(asin((long double) a[0])); }},
            {"arccos", [](const vector<Fraction> &a) { return Fraction(acos((long double) a[0])); }},
            {"arctg",  [](const vector<Fraction> &a) { return Fraction(atan((long double) a[0])); }},
            {"arctan", [](const vector<Fraction> &a) { return Fraction(atan((long double) a[0])); }},
            {"arcctg", [](const vector<Fraction> &a) { return Fraction(M_PI_2 - atan((long double) a[0])); }},
            {"asin",   [](const vector<Fraction> &a) { return Fraction(asin((long double) a[0])); }},
            {"acos",   [](const vector<Fraction> &a) { return Fraction(acos((long double) a[0])); }},
            {"atg",    [](const vector<Fraction> &a) { return Fraction(atan((long double) a[0])); }},
            {"atan",   [](const vector<Fraction> &a) { return Fraction(atan((long double) a[0])); }},
            {"actg",   [](const vector<Fraction> &a) { return Fraction(M_PI_2 - atan((long double) a[0])); }},
            {"abs",    [](const vector<Fraction> &a) { return Fraction(abs((long double) a[0])); }},
            {"int",    [](const vector<Fraction> &a) { return Fraction(floor((long double) a[0])); }},
            {"sqrt",   [](const vector<Fraction> &a) { return Fraction::Power(a[0], Fraction(0.5)); }}
    };

    /**
     * Поле класса Operations
     * numberOfFunctionArguments - хранит словарь количества аргументов функций:
     *  Ключ - имя функции типа string
     *  Значение - количество аргументов типа int
     */
    map<string, int> numberOfFunctionArguments{
            {"sin",    1},
            {"cos",    1},
            {"tg",     1},
            {"tan",    1},
            {"ctg",    1},
            {"arcsin", 1},
            {"arccos", 1},
            {"arctg",  1},
            {"arctan", 1},
            {"arcctg", 1},
            {"asin",   1},
            {"acos",   1},
            {"atg",    1},
            {"atan",   1},
            {"actg",   1},
            {"abs",    1},
            {"int",    1},
            {"sqrt",   1}
    };

    /**
     * Поле класса Operations
     * priorities - хранит словарь приоритетов для операций и функций:
     *  Ключ - имя операции/функции типа string
     *  Значение - приоритет операции типа int
     */
    map<string, int> priorities = {
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
        priorities[name] = priority;
    }

    /**
     * Функция-член класса Operations
     * AddUnaryOperation - добавляет унарную операцию
     */
    void AddUnaryOperation(const string &name, const function<Fraction(const Fraction &)> &func, int priority = 3) {
        if (IsUnaryOperation(name)) throw runtime_error("Такая операция уже есть");
        unaryOperations[name] = func;
        priorities[name] = priority;
    }

    /**
     * Функция-член класса Operations
     * AddFunction - добавляет функцию
     */
    void AddFunction(const string &name, const function<Fraction(const vector<Fraction> &)> &func, int priority = 3) {
        if (IsFunction(name)) throw runtime_error("Такая функция уже есть");
        if (IsUnaryOperation(name) || IsBinaryOperation(name))
            throw runtime_error("Нельзя задавать имя функции такое же, как у операций");
        functions[name] = func;
        priorities[name] = priority;
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

    /**
     * Функция-член класса Operations
     * IsFunction - проверяет, является ли переданный аргумент функцией
     */
    bool IsFunction(const string &name) { return (functions.find(name) != functions.end()); }
};

#endif //OPERATIONS_H