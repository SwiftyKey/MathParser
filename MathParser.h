#ifndef MATHPARSER_MATHPARSER_H
#define MATHPARSER_MATHPARSER_H

#pragma once

#include <stack>
#include <vector>

#include "Operations.h"

using namespace std;


class Expression {
private:
    Operations &operations = Operations::GetInstance();
    string expression;
    size_t index = 0;

    enum TypeOfTokens {
        unknown, number, openBracket, closeBracket, binaryOperation, unaryOperation
    };

    struct Token {
        string name;
        TypeOfTokens type;

        Token() : type(unknown) {}
    };

    vector<Token> postfixNotationExpression;

    TypeOfTokens SetOperationType(size_t position, const string &name);

    Token GetToken();

    void BuildPostfixNotation();

    bool IsDigit(size_t position);

    bool IsPoint(size_t position);

    bool IsLetter(size_t position);

    bool IsBracketSequenceCorrect();


public:
    explicit Expression(const string &expr) {
        if (expr.empty()) SendError(0);
        expression = expr;
        expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());

    }

    Fraction Eval();
};

bool Expression::IsDigit(size_t position) { return isdigit(expression[position]); }

bool Expression::IsPoint(size_t position) { return (expression[position] == '.' || expression[position] == ','); }

bool Expression::IsLetter(size_t position) { return isalpha(expression[position]); }

bool Expression::IsBracketSequenceCorrect() {
    int count = 0;

    for (char &iter: expression) {
        if (iter == '(') count++;
        else if (iter == ')') count--;

        if (count < 0) return false;
    }

    return (count == 0);
}

Expression::TypeOfTokens Expression::SetOperationType(size_t position, const string &name) {
    TypeOfTokens type = unknown;

    if (operations.IsUnaryOperation(name) && operations.IsBinaryOperation(name)) {
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

    if (!IsBracketSequenceCorrect()) SendError(1);

    if (IsDigit(index) || IsPoint(index)) {
        while (index < expression.size() && (IsDigit(index) || IsPoint(index))) {
            if (IsPoint(index)) {
                tokenName += '.';
                index++;
            } else tokenName += expression[index++];
        }
        token.type = number;
    } else if (IsLetter(index))
        while (index < expression.size() && IsLetter(index)) tokenName += tolower(expression[index++]);
    else tokenName += expression[index++];

    if (token.type == unknown) token.type = SetOperationType(index, tokenName);
    if (token.type == unknown) SendError(4);

    if (tokenName.empty()) SendError(2);
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
                while (tokens.top().type != openBracket) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }
                tokens.pop();
                break;

            case binaryOperation:
                if (!tokens.empty()) {
                    while (!tokens.empty() && (tokens.top().type == binaryOperation &&
                                               operations.binaryOperationsPriority[token.name] <=
                                               operations.binaryOperationsPriority[tokens.top().name] ||
                                               tokens.top().type == unaryOperation)) {
                        postfixNotationExpression.push_back(tokens.top());
                        tokens.pop();
                    }
                }
                tokens.push(token);
                break;

            case unaryOperation:
                while (!tokens.empty() && tokens.top().type == unaryOperation) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }
                tokens.push(token);
                break;
        }
    }

    if (index == expression.size()) {
        while (!tokens.empty()) {
            postfixNotationExpression.push_back(tokens.top());
            tokens.pop();
        }
    }
}

Fraction Expression::Eval() {
    stack<Fraction> numbers;
    Fraction a, b, x, result;

    BuildPostfixNotation();

    for (auto &iter: postfixNotationExpression) {
        switch (iter.type) {
            case number:
                numbers.push(Fraction(iter.name));
                break;
            case unaryOperation:
                if (numbers.empty()) SendError(5);

                x = numbers.top();
                numbers.pop();

                result = operations.unaryOperations[iter.name](x);
                numbers.push(result);
                break;
            case binaryOperation:
                if (numbers.empty()) SendError(5);

                b = numbers.top();
                numbers.pop();

                a = numbers.top();
                numbers.pop();

                result = operations.binaryOperations[iter.name](a, b);
                numbers.push(result);
                break;
        }
    }

    result = numbers.top();
    numbers.pop();

    return result;
}

#endif //MATHPARSER_MATHPARSER_H