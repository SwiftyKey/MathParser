#include "Operations.h"

Operations& Operations::GetInstance() {
    // onlyInstance - статическая переменная для гарантии наличия только одного экземпляра класса Operations
    static Operations onlyInstance;
    return onlyInstance;
}

void Operations::AddBinaryOperation(const string &name,
                        const function<Fraction(const Fraction &, const Fraction &)> &func, int priority) {
    if (IsBinaryOperation(name)) throw runtime_error("Такая операция уже есть");
    binaryOperations[name] = func;
    priorities[name] = priority;
}


void Operations::AddUnaryOperation(const string &name, const function<Fraction(const Fraction &)> &func, int priority) {
    if (IsUnaryOperation(name)) throw runtime_error("Такая операция уже есть");
    unaryOperations[name] = func;
    priorities[name] = priority;
}


void Operations::AddFunction(const string &name, const function<Fraction(const vector<Fraction> &)> &func, int priority) {
    if (IsFunction(name)) throw runtime_error("Такая функция уже есть");
    if (IsUnaryOperation(name) || IsBinaryOperation(name))
        throw runtime_error("Нельзя задавать имя функции такое же, как у операций");
    functions[name] = func;
    priorities[name] = priority;
}

bool Operations::IsBinaryOperation(const string &name) { return (binaryOperations.find(name) != binaryOperations.end()); }

bool Operations::IsUnaryOperation(const string &name) { return (unaryOperations.find(name) != unaryOperations.end()); }

bool Operations::IsFunction(const string &name) { return (functions.find(name) != functions.end()); }