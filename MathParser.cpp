#include "MathParser.h"

bool MathExpression::IsDigit(const size_t &position) { return isdigit(expression[position]); }

bool MathExpression::IsPoint(const size_t &position) { return expression[position] == '.'; }

bool MathExpression::IsLetter(const size_t &position) { return isalpha(expression[position]); }

bool MathExpression::IsBracketSequenceCorrect() {
    // переменная для подсчета открывающих и закрывающих скобок
    int count = 0;

    for (char &iter: expression) {
        // каждая открывающая скобка увеличивает счетчик
        if (iter == '(') count++;
            // каждая закрывающая скобка уменьшает счетчик
        else if (iter == ')') count--;

        // если количество станет меньше нуля, значит скобки расставлены неправильно
        if (count < 0) return false;
    }

    // если count = 0, то открывающих и закрывающих скобок одинаковое количество
    return count == 0;
}

MathExpression::TypeOfTokens MathExpression::GetTokenType(size_t position, const string &name) {
    TypeOfTokens type = unknown;

    if (operations.IsFunction(name)) {
        while (position < expression.size() && isspace(expression[position])) position++;
        if (expression[position] == '(') type = func;
        else throw runtime_error("Ошибка. После функции " + name + " ожидается '('");
    }
        // Если операция является и бинарной, и унарной
    else if (operations.IsUnaryOperation(name) && operations.IsBinaryOperation(name)) {
        // Получаем позицию символа перед операцией, -1 - означает, что операция находится в начале строки
        position -= name.size() + 1;
        while (position > 0 && isspace(expression[position])) position--;
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

    while (index < expression.size() && isspace(expression[index]))
        index++;

    // Если встречаем цифру, получаем полностью число
    while (index < expression.size() && (IsDigit(index) || IsPoint(index)))
        tokenName += expression[index++];
    if (!tokenName.empty())
        token.type = number;
        // Если встречаем букву, получаем полностью слово
    else if (IsLetter(index))
        while (index < expression.size() && IsLetter(index))
            tokenName += (char) tolower(expression[index++]);
        // Иначе получаем символ
    else if (index < expression.size()) tokenName += expression[index++];

    if (tokenName == ",") token.type = comma;

    // Устанавливаем тип операции
    if (token.type == unknown) token.type = GetTokenType(index, tokenName);

    if (index < expression.size() && tokenName.empty()) throw runtime_error("Ошибка. Непредвиденный символ");
    token.name = tokenName;

    return token;
}

void MathExpression::BuildPostfixNotation() {
    stack<Token> tokens;

    while (index < expression.size()) {
        Token token = GetToken();

        switch (token.type) {
            case unknown:
                if (index == expression.size()) break;
                throw runtime_error("Ошибка. Такой операции нет");

            case comma:
            case number:
                postfixNotationExpression.push_back(token);
                break;

            case closeBracket:
                // Пока не дойдем до открывающей скобки, добавляем токен в обратную нотацию
                while (tokens.top().type != openBracket) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }
                tokens.pop();

                // Когда дошли до открывающей скобки, проверяем на наличие функции перед ней
                if (!tokens.empty() && tokens.top().type == func) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }

                break;

            case binaryOperation:
                // Пока на вершине стека унарная операция или бинарная с большим или равным приоритетом, добавляем токен в обратную нотацию
                while (!tokens.empty() &&
                       (tokens.top().type == binaryOperation || tokens.top().type == unaryOperation) &&
                       operations.priorities[token.name] <=
                       operations.priorities[tokens.top().name]) {
                    postfixNotationExpression.push_back(tokens.top());
                    tokens.pop();
                }
            case openBracket:
            case unaryOperation:
            case func:
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
    vector<Fraction> args;
    Fraction a, b, x;

    BuildPostfixNotation();

    for (const auto &iter: postfixNotationExpression) {
        switch (iter.type) {
            case comma:
                if (numbers.empty()) throw runtime_error("Ошибка. Ожидается операнд");

                args.push_back(numbers.top());
                numbers.pop();

                break;

            case number:
                numbers.push(Fraction(iter.name));
                break;

            case unaryOperation:
                if (numbers.empty()) throw runtime_error("Ошибка вычисления. Пропущен операнд");

                x = numbers.top();
                numbers.pop();

                numbers.push(operations.unaryOperations[iter.name](x));
                break;

            case binaryOperation:
                b = numbers.top();
                numbers.pop();

                if (numbers.empty()) throw runtime_error("Ошибка вычисления. Пропущен операнд");

                a = numbers.top();
                numbers.pop();

                numbers.push(operations.binaryOperations[iter.name](a, b));
                break;

            case func:
                if (numbers.empty()) throw runtime_error("Ошибка. Пропущен аргумент функции");

                args.push_back(numbers.top());
                numbers.pop();

                int numberOfArguments = operations.GetNumberOfFunctionArguments(iter.name);

                if (numberOfArguments != 0 && args.size() > numberOfArguments)
                    throw runtime_error("Ошибка вычисления. Функция " + iter.name +
                                        " принимает количество аргументов = " + to_string(numberOfArguments));

                numbers.push(operations.functions[iter.name](args));
                args.clear();
                break;
        }
    }

    if (numbers.size() > 1) throw runtime_error("Ошибка вычисления. Пропущен оператор или функция");

    return numbers.top();
}
