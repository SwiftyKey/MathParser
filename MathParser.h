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
class Expression {
private:

    /**
     * Получаем экземпляр класса Operations
     */
    Operations &operations = Operations::GetInstance();
    /**
     * Строка математического выражения
     */
    string expression;
    /**
     * Текущий индекс в строке
     */
    size_t index = 0;

    /**
     * Перечисление типов токенов
     */
    enum TypeOfTokens {
        unknown, number, openBracket, closeBracket, binaryOperation, unaryOperation
    };

    /**
     * Структура токена
     */
    struct Token {

        /**
         * Имя токена
         */
        string name;
        /**
         * Тип токена
         */
        TypeOfTokens type;

        Token() : type(unknown) {}
    };

    /**
     * Введенное математическое выражение в виде обратной польской нотации
     */
    vector<Token> postfixNotationExpression;

    /**
     * Функция-член для установки типа операции
     */
    TypeOfTokens SetOperationType(size_t position, const string &name);

    /**
     * Функция-член для получения следующего токена
     */
    Token GetToken();

    /**
     * Функция-член для построения обратной польской нотации
     */
    void BuildPostfixNotation();

    /**
     * Функция-член для проверки, что символ является цифрой
     */
    bool IsDigit(const size_t &position);

    /**
     * Функция-член для проверки, что символ является точкой (запятой)
     */
    bool IsPoint(const size_t &position);

    /**
     * Функция-член для проверки, что символ является буквой
     */
    bool IsLetter(const size_t &position);

    /**
     * Функция-член для проверки корректности скобочной последовательности
     */
    bool IsBracketSequenceCorrect();


public:
    explicit Expression(const string &expr) {
        if (expr.empty()) throw runtime_error("Пустое выражение");
        expression = expr;
        // убираем пробельные символы
        expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());

    }

    /**
     * Функция-член для вычисления значения математического выражения
     */
    Fraction Eval();
};

bool Expression::IsDigit(const size_t &position) { return isdigit(expression[position]); }

bool Expression::IsPoint(const size_t &position) {
    return (expression[position] == '.' || expression[position] == ',');
}

bool Expression::IsLetter(const size_t &position) { return isalpha(expression[position]); }

bool Expression::IsBracketSequenceCorrect() {
    // если count = 0, то открывающих и закрывающих скобок одинаковое количество
    int count = 0;

    for (char &iter: expression) {
        // подсчет скобок
        if (iter == '(') count++;
        else if (iter == ')') count--;

        if (count < 0) return false;
    }

    return (count == 0);
}

Expression::TypeOfTokens Expression::SetOperationType(size_t position, const string &name) {
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

Expression::Token Expression::GetToken() {
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
        while (index < expression.size() && IsLetter(index)) tokenName += (char)tolower(expression[index++]);
    // Иначе получаем символ
    else tokenName += expression[index++];

    // Устанавливаем тип операции
    if (token.type == unknown) token.type = SetOperationType(index, tokenName);
    if (token.type == unknown) throw runtime_error("Такой операции нет");

    if (tokenName.empty()) throw runtime_error("Непредвиденный символ");
    token.name = tokenName;

    return token;
}

void Expression::BuildPostfixNotation() {
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
                while (!tokens.empty() && (tokens.top().type == binaryOperation &&
                                           operations.binaryOperationsPriority[token.name] <=
                                           operations.binaryOperationsPriority[tokens.top().name] ||
                                           tokens.top().type == unaryOperation)) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }
                tokens.push(token);
                break;

            case unaryOperation:
                // Пока на вершине стека унарная операция, добавляем токен в обратную нотацию
                while (!tokens.empty() && tokens.top().type == unaryOperation) {
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

Fraction Expression::Eval() {
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