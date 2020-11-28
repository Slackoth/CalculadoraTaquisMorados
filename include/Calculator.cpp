/*Main it is here to test the Calculator class.*/
#include <iostream>
#include "stack.hpp"
#include "queue.hpp"
#include "Token.hpp"

class Calculator {
    public:
        Calculator() {}
        Queue<Token> expressionToToken(std::string expression);
        Queue<Token> infixToPostfix(Queue<Token> infix);
    
    private:
        void chooseOperator(char op, Token::Type* t, int* p, bool* ra);
        void fixPrecedence(Node<Token>** operatorStack, Token currentOperator, Queue<Token>* postfix);
        void matchLeftParenthesis(Node<Token>** operatorStack, Queue<Token>* postfix);
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
    for(const char* cc = expression.c_str(); *cc; ++cc) {
        if(isdigit(*cc)) {
            //A pointer that mantains the first position of the array.
            const char* pp = cc;

            /*
                Check if the number has more digits than one until
                there is an operator.
                cc++ makes the pointer advance one char at a time.
            */
            for(; isdigit(*cc) || *cc == '.'; ++cc)
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
            --cc;
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

void Calculator::fixPrecedence(Node<Token>** operatorStack, Token currentOperator, Queue<Token>* postfix) {
    Token opPrev = stackTop(*operatorStack);
    /*
        If the operator before the current operator is leftAssociative AND its precedence is
        greater or queal than the current operator.
        Or if the operator before the current operator is rightAssociative AND its precedence is
        greater than the current operator.
    */
    if((!opPrev.rightAssociative && opPrev.precedence >= currentOperator.precedence)
        || (opPrev.rightAssociative && opPrev.precedence > currentOperator.precedence)) {
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

void Calculator::matchLeftParenthesis(Node<Token>** operatorStack, Queue<Token>* postfix) {
    bool leftExits = false;

    /*
        A left parenthesis needs to be found. Until it is found AND the stack is not empty,
        then, all the operators before the left parenthesis will be popped and pushed into
        the postfix espression.
    */
    while(!stackIsEmpty(*operatorStack) && stackTop(*operatorStack).type != Token::Type::LeftParenthesis) {
        Token opPopped = stackPop(operatorStack);
        leftExits = true;

        queuePush(postfix, opPopped);
    }

    /*
        A left parenthesis is still inside the operator stack and it needs to 
        be popped if the stack is not empty. Else, it means there is no left
        parenthesis to math the right one.
    */
    if(!stackIsEmpty(*operatorStack)) {
        stackPop(operatorStack);    
    }
    else {
        //TODO -> Throw error: Left parenthesis does not exists.
        return;
    }
}

Queue<Token> Calculator::infixToPostfix(Queue<Token> infix) {
    Node<Token>* opStack =  NULL;
    Queue<Token> postfix = Queue<Token>();

    /*
        Check every token inside the postfix expression.
    */
    for(Node<Token>* aux = infix.front; aux != NULL; aux = aux->next) {
        Token token = aux->data;

        switch (aux->data.type) {
            //If it is number, then it can be pushed directly to the postfix expression.
            case Token::Type::Number: queuePush(&postfix, aux->data); break;
            /*
                If it is operator, then it will be pushed directly inside the stack operator
                as long as the operator stack is empty. Else, we need to check if the precedence
                of the operators will be maintained or needs to be fixed.
            */
            case Token::Type::Operator: {
                if(stackIsEmpty(opStack))
                    stackPush(&opStack, token);
                else 
                    fixPrecedence(&opStack, token, &postfix);
            } break;

            //If it is left parenthesis, then, push it directly inside the operator stack.
            case Token::Type::LeftParenthesis: stackPush(&opStack, token); break;
            /*
                If it is right parenthesis, then, we need to pop every operator from the stack
                until the left parenthesis is found and check if there is a match.
            */
            case Token::Type::RightParenthesis: matchLeftParenthesis(&opStack, &postfix); break;
            
            default: break;
        }         
    }

    //The operators that remain insdie the stack, can be pushed directly into the postfix expression.
    while(!stackIsEmpty(opStack)) 
        queuePush(&postfix, stackPop(&opStack));

    return postfix;
}

int main() {
    Calculator calculator = Calculator();
    Queue<Token> tokenExp = calculator.expressionToToken("3+4.5*2.89/(10-9.55)^2^3");
    
    traverseQueue(tokenExp.front);
    std::cout << "\n";
    
    Queue<Token> postfix = calculator.infixToPostfix(tokenExp);
    
    traverseQueue(postfix.front);
    std::cout << "\n";
}