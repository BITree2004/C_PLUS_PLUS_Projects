#include <iostream>
#include <fstream>
#include "expression.h"
#include "parser/expression.tab.h"
#include "parser/expression.lexer.h"

using namespace std;

extern expression::expression *result1;
extern expression::expression *result2;

void yyerror(const char *error) {
    cerr << error;
}

int yywrap() {
    return 1;
}

namespace expression {
    expression *list::to_expression() {
        if (is_it_final()) {
            return new number(0);
        }
        auto element_to_expression = [&](pair<list, _type> element) -> expression * {
            return new multiply(new pow(new limit_ordinal(), element.first.to_expression()),
                                new number(element.second));
        };
        expression *res = element_to_expression(arr[0]);
        for (size_t i = 1; i < arr.size(); ++i) {
            res = new add(res, element_to_expression(arr[i]));
        }
        return res;
    }
}


int main() {
    freopen("src/tests2/13.txt", "r", stdin);
    freopen("src/tests2/o13.txt", "w", stdout);
    std::string expression;
    std::getline(std::cin, expression);
    expression += '\n';
    yy_scan_string(expression.c_str());
    yyparse();

    /*cout << "FIRST PART PASSED AS: ";
    result1->print(std::cout);
    std::cout << std::endl;
    cout << "SECOND PART PASSED AS: ";
    result2->print(std::cout);
    std::cout << std::endl;*/
    auto x = result1->to_normal();
    auto x2 = x.to_expression();
    auto y = result2->to_normal();
    auto y2 = y.to_expression();
    // NORMAL FORM
    /*cout << "FIRST PART PASSED AS: ";
    x2->print(std::cout);
    std::cout << std::endl;
    cout << "SECOND PART PASSED AS: ";
    y2->print(std::cout);
    std::cout << std::endl;*/
    if (x.compare(y) == 0) {
        cout << "Равны";
    } else {
        cout << "Не равны";
    }
    delete result1;
    delete result2;
    delete x2;
    delete y2;
    return 0;
}
