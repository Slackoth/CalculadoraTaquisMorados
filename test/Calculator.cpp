#include <iostream>
#include "stack.hpp"
#include "queue.hpp"
#include "Token.hpp"

class Calculator {
    public:
        Calculator() {}
        Queue<Token> expressionToToken(std::string expression);
    
    private:
        void chooseOperator(char op, Token::Type* t, int* p, bool* ra);
};

void Calculator::chooseOperator(char op, Token::Type* t, int* p, bool* ra) {
    switch (op) {
        case '(': *t = Token::Type::LeftParenthesis; break;
        case ')': *t = Token::Type::RightParenthesis; break;
        case '+':
        case '-': *t = Token::Type::Operator; *p = 1; break;
        case '*':
        case '/': *t = Token::Type::Operator; *p = 2; break;
        case '^': *t = Token::Type::Operator; *p = 3; *ra = true; break;
        
        default: break;
    }
}

Queue<Token> Calculator::expressionToToken(std::string expression) {
    Queue<Token> tokenExp = Queue<Token>();

    /*
        c_string() transforms a string to an array of chars and returns 
        the pointer of the said array:
        cc = string
        *cc = a char of the array
    */
    for(const char* cc = expression.c_str(); *cc; cc++) {
        if(isdigit(*cc)) {
            //A pointer that mantains the first position of the array.
            const char* pp = cc;

            /*
                Check if the number has more digits than one until
                there is an operator.
                cc++ makes the pointer advance one char at a time.
            */
            for(; isdigit(*cc); cc++)
                ;
            
            /*
                string constructor -> Range:
                Copies the sequence of characters in the range 
                [first,last), in the same order.
            */
            std::string value = std::string(pp, cc);
            queuePush(&tokenExp, Token(Token::Type::Number, value));
            
            /*
                *cc holds an operator, so it needs to
                move backwards one char to allow the 
                conditional isdigit() to check the operator.
            */
            *cc--;
        }
        else {
            Token::Type type = Token::Type::Unknown;
            int pre = -1; bool ra = false;

            chooseOperator(*cc, &type, &pre, &ra);

            /*
                string constructor -> Fill: 
                Fills the string with n consecutive copies of character c.
            */
            queuePush(&tokenExp, Token(type, std::string(1, *cc), pre, ra));
        }
    }

    return tokenExp;
}

int main() {
    Calculator calc = Calculator();
    Queue<Token> q = calc.expressionToToken("123+");
    traverseQueue(q.front);
    std::cout << "\n";
}