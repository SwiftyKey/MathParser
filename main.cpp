#include <iostream>
#include "MathParser.h"

using namespace std;

int errors = 0;

void test(const string &input, long double expected) {
    try {
        MathExpression expression(input);
        Fraction result = expression.Eval();
        cout << input << " = " << expected << " : , got " << (long double) result << endl;
    } catch (exception &e) {
        cout << input << " : exception: " << e.what() << endl;
        ++errors;
    }
}

void tests() {
    test("0", 0);
    test("1", 1);
    test("9", 9);
    test("10", 10);
    test("+1", 1);
    test("-1", -1);
    test("(1)", 1);
    test("(-1)", -1);
    test("-(-1)", 1);
    test("-(-(-1))", -1);
    test("2-(-(-1))", 1);
    test("abs(-(-(-1)))", 1);
    test("1+20", 21);
    test("1 + 20", 21);
    test("1+20+300", 321);
    test("1+20+300+4000", 4321);
    test("-1+20", 19);
    test("(1+20)", 21);
    test("-2*3", -6);
    test("2*-3", -6);
    test("1+10*2", 21);
    test("10*2+1", 21);
    test("(1+20)*2", 42);
    test("2*(1+20)", 42);
    test("(1+2)*(3+4)", 21);
    test("2*3+4*5", 26);
    test("100+2*10+3", 123);
    test("2^3", 8);
    test("2^3^2", 64);
    test("2^3*5+2", 42);
    test("5*2^3+2", 42);
    test("2+5*2^3", 42);
    test("1+2^3*10", 81);
    test("2^3+2*10", 28);
    test("5 * 4 +           3 * 2 + 1", 27);
    test("sin(4) - cos(3)", 0.23319);
    test("-3 + 2.", -1);
    test("-3 + .3", -2.7);
    test("sin(4+3)", 0.656987);
    test("sin(3+4)", 0.656987);
    test("asin(3+4)", 0);
    test("3.21e2", 321);
    test("1+3.2e1 - 2 * 5", 23);
    test("3.21e+2", 321);
    test("1+3.2e+1 - 2 * 5", 23);
    test("3.21e-2", 0.0321);
    test("1-3.2e-1 - 2 * 5", -9.32);
    test("1e4^0.1", 2.51189);
    test("3.21E2", 321);
    test("1+3.2E1 - 2 * 5", 23);
    test("3.21E+2", 321);
    test("1+3.2E+1 - 2 * 5", 23);
    test("3.21E-2", 0.0321);
    test("1-3.2E-1 - 2 * 5", -9.32);
    test("1E4^0.1", 2.51189);
    test("int(2.99999999999997) + 34", 36);
    test("sqrt(6.25) + 34", 36.5);
    test("3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3", 3.001953125);
    test("1 + sqrt(-4) ^ 3", 2);
    test("8^(1/2)", 2.82843);
    test("(-4)^1.5", -1);
    test("8^4", 4096);
    test("0^2", 0);
    test("0^0", 1);
    test("(-8)^4", 4096);
    test("4^(-0.5)", 0.5);
    test("(-8)^(-1/2)", -1);
    test("4^(-4)", 0.00390625);
    test("(-8)^(-4)", 0.000244141);
    test("(-8)^(1/3)", -2);
    test("sqrt(2)-1/2*sin(1^2-2)", 1.83495);
    test("1/0", 0);
    test("1(e2)", 100);
    test("ctg(4)", 0.863691);
    test("arcctg(4)", 0.244979);
    test("-sin(2)^2", -0.826822);
    test("-4^2", -16);
    test("-4^(1/2)", -2);
    test("sin(2)^2-arccos(1)*((-8)^(1/3)*5)", 0.826822);
    test("acos(3/4)", 0.722734);
    test("acos(0.75) * (-10)", -7.22734);
    test("sin(2)^2", 0.826822);
    test("sin(2)^2-acos(0.75) * (-10)", 8.05416);
    test("sin(2)^2+7.22734", 8.05416);
    test("sin(cos(abs(-1)))", 0.514395);
    test("sin(cos(abs(-1)))*cos(sin(int(9.34213)))", 0.471327);
    test("sin(cos(abs(-1)))*cos(sin(int(9.34213)))+10", 10.4713);
    test("0.8845875131313131", 0.8845875131313131);
    test("0.8845875131313131 * 0.284881", 8.05416);
    test("min(1, 2)", 1);
    test("min(1, 2) * 10 - 5 / 3", 8.33333);
    test("min(min(1, 2), min(3, 5)) * 10 - 5 / 3", 8.33333);
}

void input() {
    cout << "Введите выражение (end для окончания):" << endl;

    string input;

    while (true) {
        cout << ">>";
        getline(cin, input);

        if (input == "end") break;

        try {
            MathExpression expression(input);
            Fraction result = expression.Eval();
            cout << "Ответ: " << (long double) result << endl;
        } catch (exception &e) {
            cerr << e.what() << endl;
        }
    }
}

int main() {
    system("chcp 65001");
    cout << "Курсовая работа Чернова Степана, КГУ, ИТ-0900022Б" << endl;

    Operations &operations = Operations::GetInstance();
    operations.AddFunction("min",
                           [](const vector<Fraction> &a){return Fraction(min((long double)Fraction(a[0]), (long double)Fraction(a[1])));},
                           3, 2);
    tests();
    cout << "Done with " << errors << " errors." << endl;
//    input();
}
