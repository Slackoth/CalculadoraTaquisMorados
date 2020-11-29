#include <iostream>

class Token {
    public:
        enum class Type { 
            Number, Operator, 
            LeftParenthesis, RightParenthesis, 
            Unknown 
        };

        const Type type;
        std::string value;
        int precedence;
        bool rightAssociative;
        bool isUnary;

        Token(Type t, std::string v, int p = -1, bool ra = false, bool u = false):
            type(t), value(v), precedence(p), rightAssociative(ra), isUnary(u) {}
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.value;
    return os;
}