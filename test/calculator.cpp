#include <cmath>
#include <string>
#include "stack.h"
#include <iomanip>
#include <iostream>

using namespace std;

bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool isOperator(char c) {
    switch (c) {
        case '(':
        case ')':
        case '^':
        case '/':
        case '*':
        case '+':
        case '-':
            return true;

        default: return false;
    }
}

int getPrecedence(char c) {
    switch (c) {
        case '(':
        case ')': return 1;
        
        case '+':
        case '-': return 2;

        case '*': 
        case '/': return 3;

        case '^': return 4;

        default: return -1;
    }
}

void fixExpression(StackNode<char>** operators, char currentOperator, string& expression) {
    char prev = top(*operators);
    char operatorOut = ' ';

    if(currentOperator == ')') {
        while(operatorOut != '(') {
            operatorOut = pop(operators);

            if(operatorOut != '(')
                expression = expression + " " + operatorOut;
        }
    }
    else if(getPrecedence(prev) > getPrecedence(currentOperator)) {
        operatorOut = pop(operators);
        expression = expression + " " + operatorOut;
    
        push(operators, currentOperator);
    }
    else 
        push(operators, currentOperator);
}

float operate(float a, float b, char op) {
    switch (op) {
        case '+': return a + b;

        case '-': return a - b;

        case '*': return a * b;

        case '/':
            if(b == 0)
                throw runtime_error("Math error: Attempted to divide by zero.\n");
            else
                return a / b;

        case '^': return pow(a, b);

        default: return -1;
    }

    return -1;
}

int main() {
    StackNode<char>* operatorStack = NULL;
    string output = "";

    string operation = "100.5*-90+3*2", value = "";
    int length = operation.length(), start = 0;

    for(int i = 0; i < length; i++) {
        char currentSpot = operation[i];

        if(isOperator(currentSpot)) {
            if(!(currentSpot == '-' && isOperator(operation[i - 1]))) {
                value = value + " " + operation.substr(start, i - start) + " " + currentSpot;
                start = i + 1;
            }          
        }
        else if(i + 1 >= length) 
            value = value + operation.substr(start, i - start) + " " + currentSpot; 
    }

    cout << "value:" << value << "\n";

    string str = "";
    stringstream ss(value);

    while(ss >> str) {
        char* isChar = NULL;
        float f = strtof(str.c_str(), &isChar);

        if(*isChar) {
            if(empty(operatorStack)) 
                push(&operatorStack, *isChar);
            else 
                fixExpression(&operatorStack, *isChar, output);
        }
        else {
            output = output + " " + str;
        }
    }

    while(!empty(operatorStack)) {
        char op = pop(&operatorStack);
        output = output + " " + op;
    }

    output.erase(0, 1);
    output.erase(output.length(), 1);

    cout << "output: " << output << endl;

    return 0;
}