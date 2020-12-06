#include "UI.hpp"
#include <math.h>
#include <iostream>
#include "stack.hpp"
#include "queue.hpp"
#include "Token.hpp"
#include "operations.hpp"

class Calculator {
    public:
        Calculator() {}
        Queue<Token> infixToPostfix(Queue<Token> infix);
        double calculatePostfix(Queue<Token> laststack);
        Queue<Token> expressionToToken(std::string expression);
        std::string validateString(std::string insertar, std::string origin, std::string last);

    private:
        bool isUnary(char op, int charPosition, char const *cExpression);
        Token chooseOperator(char op, int charPosition, const char *cExpression);
        void matchLeftParenthesis(Node<Token> **operatorStack, Queue<Token> *postfix);
        void fixPrecedence(Node<Token> **operatorStack, Token currentOperator, Queue<Token> *postfix);
};

bool Calculator::isUnary(char op, int charPosition, char const *cExpression) {
    if (charPosition == 0)
        return true;
    else if (!isdigit(*(cExpression + charPosition - 1)))
        return true;
    else
        return false;
}

Token Calculator::chooseOperator(char op, int charPosition, const char *cExpression) {
    int p = -1;
    std::string value = "";
    bool ra = false, u = false, flag = false;
    Token::Type t = Token::Type::Unknown;

    /*
        string constructor -> Fill: 
        Fills the string with n consecutive copies of character c.
    */
    switch (op) {
        case '(':
            t = Token::Type::LeftParenthesis;
            value = std::string(1, op);
            break;
        case ')':
            t = Token::Type::RightParenthesis;
            value = std::string(1, op);
            break;
        /*
            In case of the "+" and "-" operators, we need to check if they 
            are unary or not. If they are, then the token and the postfix syntaxis
            are different from their normal way.
        */
        case '+': {
            t = Token::Type::Operator;
            flag = isUnary(op, charPosition, cExpression);
            u = flag;
            p = flag ? 4 : 1;
            value = flag ? ".+" : "+";
            break;
        }
        case '-': {
            t = Token::Type::Operator;
            flag = isUnary(op, charPosition, cExpression);
            u = flag;
            p = flag ? 4 : 1;
            value = flag ? ".-" : "-";
            break;
        }
        case '*':
        case '/':
            t = Token::Type::Operator;
            p = 2;
            value = std::string(1, op);
            break;
        case '^':
            t = Token::Type::Operator;
            p = 3;
            ra = true;
            value = std::string(1, op);
            break;
        case 'r':
            t = Token::Type::Operator;
            p = 3;
            u = true;
            value = std::string(1, op);
            break;

        default:
            break;
    }

    return Token(t, value, p, ra, u);
}

Queue<Token> Calculator::expressionToToken(std::string expression) {
    int charPos = 0;
    Queue<Token> tokenExp = Queue<Token>();
    const char *cExpression = expression.c_str();
    /*
        c_string() transforms a string to an array of chars and returns 
        the pointer of the said array:
        cc = string
        *cc = a char of the array
    */
    for (const char *cc = cExpression; *cc; ++cc) {
        if (isdigit(*cc)) {
            //A pointer that mantains the first position of the array.
            const char *pp = cc;

            /*
                Check if the number has more digits than one until
                there is an operator.
                cc++ makes the pointer advance one char at a time.
            */
            for (; isdigit(*cc) || *cc == '.'; ++cc)
                ++charPos;

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
            --cc;
        }
        else {
            //Creates a token and its properties dependes upon the operators.
            Token token = chooseOperator(*cc, charPos, cExpression);

            queuePush(&tokenExp, token);
            ++charPos;
        }
    }

    return tokenExp;
}

void Calculator::fixPrecedence(Node<Token> **operatorStack, Token currentOperator, Queue<Token> *postfix) {
    Token opPrev = stackTop(*operatorStack);
    /*
        If the operator before the current operator is leftAssociative AND its precedence is
        greater or queal than the current operator.
        Or if the operator before the current operator is rightAssociative AND its precedence is
        greater than the current operator.
    */
    if ((!opPrev.rightAssociative && opPrev.precedence >= currentOperator.precedence) || (opPrev.rightAssociative && opPrev.precedence > currentOperator.precedence)) {
        /*
                Then the previous operator needs to be taken out of the operator stack,
                be pushed inside the postfix expression and push the current operator
                to the operator stack.
            */
        Token opPopped = stackPop(operatorStack);

        queuePush(postfix, opPopped);
        stackPush(operatorStack, currentOperator);
    }
    else {
        //Else the operator can be pushed directly to the operator stack.
        stackPush(operatorStack, currentOperator);
    }
}

void Calculator::matchLeftParenthesis(Node<Token> **operatorStack, Queue<Token> *postfix) {
    bool leftExits = false;

    /*
        A left parenthesis needs to be found. Until it is found AND the stack is not empty,
        then, all the operators before the left parenthesis will be popped and pushed into
        the postfix espression.
    */
    while (!stackIsEmpty(*operatorStack) && stackTop(*operatorStack).type != Token::Type::LeftParenthesis) {
        Token opPopped = stackPop(operatorStack);
        leftExits = true;

        queuePush(postfix, opPopped);
    }

    /*
        A left parenthesis is still inside the operator stack and it needs to 
        be popped if the stack is not empty. Else, it means there is no left
        parenthesis to math the right one.
    */
    if (!stackIsEmpty(*operatorStack)) 
        stackPop(operatorStack);
}

Queue<Token> Calculator::infixToPostfix(Queue<Token> infix) {
    Node<Token> *opStack = NULL;
    Queue<Token> postfix = Queue<Token>();

    /*
        Check every token inside the postfix expression.
    */
    for (Node<Token> *aux = infix.front; aux != NULL; aux = aux->next) {
        Token token = aux->data;

        switch (aux->data.type) {
            //If it is number, then it can be pushed directly to the postfix expression.
            case Token::Type::Number:
                queuePush(&postfix, aux->data);
                break;
            /*
                    If it is operator, then it will be pushed directly inside the stack operator
                    as long as the operator stack is empty. Else, we need to check if the precedence
                    of the operators will be maintained or needs to be fixed.
                */
            case Token::Type::Operator: {
                if (stackIsEmpty(opStack))
                    stackPush(&opStack, token);
                else
                    fixPrecedence(&opStack, token, &postfix);
            }
            break;

            //If it is left parenthesis, then, push it directly inside the operator stack.
            case Token::Type::LeftParenthesis:
                stackPush(&opStack, token);
                break;
            /*
                    If it is right parenthesis, then, we need to pop every operator from the stack
                    until the left parenthesis is found and check if there is a match.
                */
            case Token::Type::RightParenthesis:
                matchLeftParenthesis(&opStack, &postfix);
                break;

            default:
                break;
        }
    }

    //The operators that remain insdie the stack, can be pushed directly into the postfix expression.
    while (!stackIsEmpty(opStack))
        queuePush(&postfix, stackPop(&opStack));

    return postfix;
}

double Calculator::calculatePostfix(Queue<Token> laststack){
    Node<double>* doublestack = NULL; 

    while(!queueIsEmpty(laststack)) {
        std::string op;
        const auto token = laststack.front->data;

        queuePop(&laststack);
        
        switch (token.type) {
            case Token::Type::Number:
                stackPush(&doublestack,std::stod(token.value));
                //traverseStack(doublestack);
                break;
            case Token::Type::Operator: {
                bool empt = false;
                //stack
                auto rhs = stackTop(doublestack);
                
                stackPop(&doublestack);               
                
                //checks if stack is empty and adds it to stack if neccesary
                auto lhs = 0.00;
                std::string opr = token.value;
                //std::cout<<"opr: "<<opr.at(0)<<std::endl;
                if(!stackIsEmpty(doublestack) && opr.at(0)!='r'){
                    lhs = stackTop(doublestack);
                    
                    stackPop(&doublestack);    
                    
                    empt = true;
                }
                //std::cout<<"rhs: "<<rhs<<std::endl;
                //std::cout<<"lhs: "<<lhs<<std::endl;
                
                //depending on opr.at() it will do the operation according to postfix string
                switch (opr.at(0)) {
                    case '^':
                        stackPush(&doublestack,static_cast<double>(pow(lhs, rhs)));
                        break;
                    case '*':
                        stackPush(&doublestack,multiply(lhs,rhs));
                        break;
                    case '/':
                        stackPush(&doublestack,divide(lhs,rhs));
                        break;
                    case '+':
                        stackPush(&doublestack,sum(lhs,rhs));
                        break;
                    case '-':
                        stackPush(&doublestack,subtraction(lhs,rhs));
                        break;
                    case '.':
                        //in the case that the token is unary operator
                        if (opr.at(1)=='-') {
                            stackPush(&doublestack,negate(rhs));
                        }
                        else if (opr.at(1)=='+') {
                            stackPush(&doublestack,rhs);
                        }
                        if (empt) {
                            stackPush(&doublestack,lhs);
                        }
                        break;
                    case 'r':
                        stackPush(&doublestack,squareroot(rhs));
                        break;
                    default:
                        printf("Operator error [%s]\n", opr);
                        exit(0);
                        break;
                }
            }
            break;
            default:
            break;
        }
    }

    return stackTop(doublestack);
}

std::string Calculator::validateString(std::string item, std::string ultimo, std::string operation) {
    //check multiple + or -
    if (item.size() >= 3) {
        for (int i = 0; i < item.size() - 2; i++) {
            if (item.at(i) == item.at(i + 1) || item.at(i) != item.at(i + 1)) {
                if ((item.at(i) == '+' || item.at(i) == '-') && (item.at(i + 1) == '+' || item.at(i + 1) == '-')) {
                    if (!std::isalpha(item.at(i + 2)) && item.at(i + 2) != '+' && item.at(i + 2) != '-') {
                        if (i != 0 || operation == "")
                            return "OK";
                        else
                            return "sintax error";
                    }
                    else
                        return "sintax error";
                }
            }
        }
    }

    /*STRING INPUT*/
    int p = 0;

    for (int i = 0; i < item.size() - 1; i++) {
        //empty parenthesis
        if (item.at(i) == '(' && item.at(i + 1) == ')')
            return "sintax error";
        
        //raiz
        if (std::isalpha(item.at(i))) {
            if (item.at(i) != 'r')
                return "sintax error";
            else if (i > 0 && item.at(i - 1) != '*')
                return "sintax error";
            else
                return "OK";
        }

        //multiple dots error
        if (item.at(i) == '.') {
            p++;
            
            if (p > 1)
                return "sintax error";
        }

        if (
            item.at(i) == '*' ||
            item.at(i) == '^' ||
            item.at(i) == '+' ||
            item.at(i) == '-' ||
            item.at(i) == '/') 
                p = 0;
        
        //double operator
        if (item.at(i) == item.at(i + 1)) {
            if (
                item.at(i) == '*' ||
                item.at(i) == '^' ||
                item.at(i) == '/' ||
                item.at(i) == '+' ||
                item.at(i) == '-')
                    return "sintax error";
        }

        //operations with parenthesis
        if (item.at(i) == '(' && i > 0) {
            if (item.at(i - 1) == '*' ||
                item.at(i - 1) == '^' ||
                item.at(i - 1) == '/' ||
                item.at(i - 1) == '+' ||
                item.at(i - 1) == '-')
                    return "OK";
            else
                return "sintax error";
        }

        //division by zero
        if (item.at(i) == '/' && item.at(i + 1) == '0')
            return "math error";

        //validate first element
        if (operation == "" && i == 0) {
            if ( item.at(i) == '*' || item.at(i) == '^' || item.at(i) == '/')
                return "sintax error";
        }
    }

    /*
        INDIVIDUAL ITEMS
    */

    //check if the last insert exist
    if (ultimo != "") {
        //block letters
        if (std::isalpha(item.at(0)) && item != "r")
            return "sintax error";

        if (item == "(") {
            if (ultimo == "*" || ultimo == "^" || ultimo == "-" || ultimo == "+" || ultimo == "/")
                return "OK";
            else
                return "sintax error";
        }
        
              //sintax error
        
        if ((ultimo == "/" && ultimo == item) ||(ultimo == "/" && item=="0")){
            if (item == "0"){
            //division by zero error
            return "math error";   }
            return "sintax error";
        }
        else   if (ultimo == "^" && item=="^"){
             return "sintax error";
        }
        else if (ultimo == "*" && item=="*")
        {
            return "sintax error";
        } else{
                    return "OK";
        }
        //first element operator
    }
    else if ( item == "*" || item == "^" || item == "/")
        return "sintax error";
    else {
        if (std::isalpha(item.at(0)) && item != "r")
            return "sintax error";
        else
            return "OK";
    }
}

int main() {
    //we need to create user input while cin=!'='
    std::string last;
    std::string infix;
    std::string valid;
    std::string insert;
    Calculator calculator = Calculator();
    
    while (true) {
        //we clear infix in case it case information from previous operations
        last = "";
        infix = "";
        valid = "";

        userInterfaceDisplay(infix);

        while (insert != "=" && insert != "OFF") {
            std::cout << "\n\nInsert Operation: " << std::endl;
            std::cin >> insert;

            if(insert!="OFF" && insert!="=") { 
                valid = calculator.validateString(insert,last,infix);
                //valid="OK";

                if(valid == "OK") {
                    infix = infix + insert;
                    last = insert;
                    
                    userInterfaceDisplay(infix);
                }
                else
                    userInterfaceDisplay(valid);
            }
            //here for each enter it will show gui!
        }

        //std::cout << "infix works?: " << infix << std::endl;
        //turn off flag
        if (insert == "OFF")
            break;

if (infix != "" && last!="/" && infix.at(infix.size()-1)!='/'
        && last!="*" && infix.at(infix.size()-1)!='*'
     && last!="^" && infix.at(infix.size()-1)!='^'
     && last!="+" && infix.at(infix.size()-1)!='+'
     && last!="-" && infix.at(infix.size()-1)!='-'
        && last!="^" && infix.at(infix.size()-1)!='^')
        {
            Queue<Token> tokenExp = calculator.expressionToToken(infix);
            //traverseQueue(tokenExp.front);
            std::cout << "\n";

            Queue<Token> postfix = calculator.infixToPostfix(tokenExp);
            //traverseQueue(postfix.front);
            std::cout << "\n";

            /*std::cout << "result: " << calculator.calculatePostfix(postfix);
            std::cout << std::endl;*/
            userInterfaceDisplay(std::to_string(calculator.calculatePostfix(postfix)));
        }else{
            userInterfaceDisplay("sintax error");
        }
        std::cout << "\n";

        insert = "";
    }

    return 0;
}
