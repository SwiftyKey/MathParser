#ifndef MATHPARSER_H
#define MATHPARSER_H

#include <stack>
#include <vector>
#include <string>

#include "Operations.h"
#include "Fraction.h"

using namespace std;

/**
 * Класс математических выражений
 * Все операции проводятся над переменными типа Fraction
 */
class MathExpression {
private:

    /**
     * Поле класса MathExpression
     * operations - хранит экземпляр класса Operations
     */
    Operations &operations = Operations::GetInstance();
    /**
     * Поле класса MathExpression
     * expression - хранит введенное математическое выражение
     */
    string expression;
    /**
     * Поле класса MathExpression
     * index - хранит текущий индекс в строке expression
     */
    size_t index = 0;

    /**
     * Поле класса MathExpression
     * TypeOfTokens - перечисление типов токенов
     */
    enum TypeOfTokens {
        unknown, number, comma, openBracket, closeBracket, binaryOperation, unaryOperation, func
    };

    /**
     * Поле класса MathExpression
     * Token - структура токена
     */
    struct Token {

        /**
         * Поле структуры Token
         * name - хранит имя токена
         */
        string name;
        /**
         * Поле структуры Token
         * type - хранит тип токена
         */
        TypeOfTokens type;

        /**
         * Конструктор по умолчанию структуры Token
         */
        Token() : type(unknown) {}
    };

    /**
     * Поле класса MathExpression
     * postfixNotationExpression - хранит expression в виде обратной польской нотации
     */
    vector<Token> postfixNotationExpression;

    /**
     * Закрытая функция-член класса MathExpression
     * GetTokenType - возвращает тип операции
     */
    TypeOfTokens GetTokenType(size_t position, const string &name);

    /**
     * Закрытая функция-член класса MathExpression
     * Функция-член для получения следующего токена
     */
    Token GetToken();

    /**
     * Закрытая функция-член класса MathExpression
     * Функция-член для построения обратной польской нотации
     */
    void BuildPostfixNotation();

    /**
     * Закрытая функция-член класса MathExpression
     * IsDigit - возвращает true, если символ является цифрой, иначе - false
     */
    bool IsDigit(const size_t &position);

    /**
     * Закрытая функция-член класса MathExpression
     * IsPoint - возвращает true, если символ является точкой, иначе - false
     */
    bool IsPoint(const size_t &position);

    /**
     * Закрытая функция-член класса MathExpression
     * IsLetter - возвращает true, если символ является буквой, иначе - false
     */
    bool IsLetter(const size_t &position);

    /**
     * Закрытая функция-член класса MathExpression
     * IsBracketSequenceCorrect - возвращает true, если скобочная последовательность корректна, иначе - false
     */
    bool IsBracketSequenceCorrect();


public:
    explicit MathExpression(const string &expr) {
        if (expr.empty()) throw runtime_error("Ошибка. Пустое выражение");

        expression = expr;

        if (!IsBracketSequenceCorrect()) throw runtime_error("Ошибка. Некорректная скобочная последовательность");
    }

    /**
     * Функция-член класса MathExpression
     * Eval - возвращает вычисленное значение математического выражения
     */
    Fraction Eval();
};

#endif //MATHPARSER_H