#ifndef MATHPARSER_OPERATIONS_H
#define MATHPARSER_OPERATIONS_H

#pragma once

#include <map>
#include <functional>

#include "Fraction.h"

using namespace std;

class Operations {
private:
    friend class Expression;

    map<string, function<Fraction(const Fraction &, const Fraction &)>> binaryOperations = {
            {"+", [](const Fraction &a, const Fraction &b) { return a + b; }},
            {"-", [](const Fraction &a, const Fraction &b) { return a - b; }},
            {"*", [](const Fraction &a, const Fraction &b) { return a * b; }},
            {"/", [](const Fraction &a, const Fraction &b) { return a / b; }},
            {"^", [](const Fraction &a, const Fraction &b) { return Fraction::Power(a, b); }},
            {"e", [](const Fraction &a, const Fraction &b) { return a * Fraction::Power(Fraction(10.0), b); }}
    };

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

    map<string, int> binaryOperationsPriority = {
            {"+", 1},
            {"-", 1},
            {"*", 2},
            {"/", 2},
            {"^", 3},
            {"e", 3}
    };

    Operations() {};

    Operations(const Operations &);

    const Operations &operator=(const Operations &);

public:
    static Operations &GetInstance() {
        static Operations onlyInstance;
        return onlyInstance;
    }

    void AddBinaryOperation(const string &name,
                            const function<Fraction(const Fraction &, const Fraction &)> &func, int priority = 3) {
        if (IsBinaryOperation(name)) SendError(6);
        binaryOperations[name] = func;
        binaryOperationsPriority[name] = priority;
    }

    void AddUnaryOperation(const string &name, const function<Fraction(const Fraction &)> &func) {
        if (IsUnaryOperation(name)) SendError(6);
        unaryOperations[name] = func;
    }

    bool IsBinaryOperation(const string &name) { return (binaryOperations.find(name) != binaryOperations.end()); }

    bool IsUnaryOperation(const string &name) { return (unaryOperations.find(name) != unaryOperations.end()); }
};

#endif //MATHPARSER_OPERATIONS_H