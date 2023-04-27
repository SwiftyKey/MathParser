#ifndef MATHPARSER_H
#define MATHPARSER_H

#include <stack>
#include <vector>
#include <string>

#include "Operations.h"
#include "Fraction.h"

using namespace std;

/*
 * TODO: добавить разделительную запятую
 * TODO: скомпилировать библиотеку (в объектный файл)
 */

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
        unknown, number, openBracket, closeBracket, binaryOperation, unaryOperation
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
     * GetOperationType - возвращает тип операции
     */
    TypeOfTokens GetOperationType(size_t position, const string &name);

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
        if (expr.empty()) throw runtime_error("Пустое выражение");
        expression = expr;
        // убираем пробельные символы
        expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());

    }

    /**
     * Функция-член класса MathExpression
     * Eval - возвращает вычисленное значение математического выражения
     */
    Fraction Eval();
};

bool MathExpression::IsDigit(const size_t &position) { return isdigit(expression[position]); }

bool MathExpression::IsPoint(const size_t &position) { return expression[position] == '.'; }

bool MathExpression::IsLetter(const size_t &position) { return isalpha(expression[position]); }

bool MathExpression::IsBracketSequenceCorrect() {
    // если count = 0, то открывающих и закрывающих скобок одинаковое количество
    int count = 0;

    for (char &iter: expression) {
        // подсчет скобок
        if (iter == '(') count++;
        else if (iter == ')') count--;

        if (count < 0) return false;
    }

    return count == 0;
}

MathExpression::TypeOfTokens MathExpression::GetOperationType(size_t position, const string &name) {
    TypeOfTokens type = unknown;


    // Если операция является и бинарной, и унарной
    if (operations.IsUnaryOperation(name) && operations.IsBinaryOperation(name)) {
        // Получаем позицию символа перед операцией, -1 - означает, что операция находится в начале строки
        position -= name.size() + 1;
        type = ((position != -1 && (IsDigit(position) || IsPoint(position) || expression[position] == ')'))
                ? binaryOperation : unaryOperation);
    } else if (operations.IsUnaryOperation(name)) type = unaryOperation;
    else if (operations.IsBinaryOperation(name))type = binaryOperation;
    else if (name == "(") type = openBracket;
    else if (name == ")") type = closeBracket;

    return type;
}

MathExpression::Token MathExpression::GetToken() {
    string tokenName;
    Token token;

    if (!IsBracketSequenceCorrect()) throw runtime_error("Некорректная скобочная последовательность");

    // Если встречаем цифру, получаем полностью число
    if (IsDigit(index) || IsPoint(index)) {
        while (index < expression.size() && (IsDigit(index) || IsPoint(index))) {
            // Так как разделитель между дробной и целой части числа может быть '.' и ',', заменяем на точку
            if (IsPoint(index)) {
                tokenName += '.';
                index++;
            } else tokenName += expression[index++];
        }
        token.type = number;
        // Если встречаем букву, получаем полностью слово
    } else if (IsLetter(index))
        while (index < expression.size() && IsLetter(index)) tokenName += (char) tolower(expression[index++]);
        // Иначе получаем символ
    else tokenName += expression[index++];

    // Устанавливаем тип операции
    if (token.type == unknown) token.type = GetOperationType(index, tokenName);
    if (token.type == unknown) throw runtime_error("Такой операции нет");

    if (tokenName.empty()) throw runtime_error("Непредвиденный символ");
    token.name = tokenName;

    return token;
}

void MathExpression::BuildPostfixNotation() {
    stack<Token> tokens;

    while (index < expression.size()) {
        Token token = GetToken();

        switch (token.type) {
            case number:
                postfixNotationExpression.push_back(token);
                break;

            case openBracket:
                tokens.push(token);
                break;

            case closeBracket:
                // Пока не дойдем до открывающей скобки, добавляем токен в обратную нотацию
                while (tokens.top().type != openBracket) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }
                tokens.pop();
                break;

            case binaryOperation:
                // Пока на вершине стека унарная операция или бинарная с большим или равным приоритетом, добавляем токен в обратную нотацию
                while (!tokens.empty() &&
                       (tokens.top().type == binaryOperation || tokens.top().type == unaryOperation) &&
                       operations.operationsPriority[token.name] <=
                       operations.operationsPriority[tokens.top().name]) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }
                tokens.push(token);
                break;

            case unaryOperation:
                // Пока на вершине стека унарная операция, добавляем токен в обратную нотацию
                while (!tokens.empty() && tokens.top().type == unaryOperation &&
                       operations.operationsPriority[token.name] <=
                       operations.operationsPriority[tokens.top().name]) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }
                tokens.push(token);
                break;
        }
    }

    // Если дошли до конца выражения, то добавляем оставшиеся токены в обратную нотацию
    if (index == expression.size()) {
        while (!tokens.empty()) {
            postfixNotationExpression.push_back(tokens.top());
            tokens.pop();
        }
    }
}

Fraction MathExpression::Eval() {
    stack<Fraction> numbers;
    Fraction a, b, x;

    BuildPostfixNotation();

    for (auto &iter: postfixNotationExpression) {
        switch (iter.type) {
            case number:
                numbers.push(Fraction(iter.name));
                break;
            case unaryOperation:
                if (numbers.empty()) throw runtime_error("Ошибка вычисления. Проверьте выражение");

                x = numbers.top();
                numbers.pop();

                numbers.push(operations.unaryOperations[iter.name](x));
                break;
            case binaryOperation:
                if (numbers.empty()) throw runtime_error("Ошибка вычисления. Проверьте выражение");

                b = numbers.top();
                numbers.pop();

                a = numbers.top();
                numbers.pop();

                numbers.push(operations.binaryOperations[iter.name](a, b));
                break;
        }
    }

    return numbers.top();
}

#endif //MATHPARSER_H