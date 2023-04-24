#ifndef MATHPARSER_PARSEEXCEPTIONS_H
#define MATHPARSER_PARSEEXCEPTIONS_H

#pragma once

#include <exception>

using namespace std;

//TODO: Можно сделать класс ошибок
void SendError(int errorNumber) {
    switch (errorNumber) {
        case 0:
            throw runtime_error("Пустое выражение");
        case 1:
            throw runtime_error("Некорректная скобочная последовательность");
        case 2:
            throw runtime_error("Непредвиденный символ");
        case 3:
            throw runtime_error("Недостаточно операндов");
        case 4:
            throw runtime_error("Такой операции нет");
        case 5:
            throw runtime_error("Ошибка вычисления. Проверьте выражение");
        case 6:
            throw runtime_error("Такая операция уже есть");
        default:
            throw runtime_error("Непредвиденная ситуация");
    }
}

#endif //MATHPARSER_PARSEEXCEPTIONS_H