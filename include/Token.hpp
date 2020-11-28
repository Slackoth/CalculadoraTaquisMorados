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

        Token(Type t, std::string v, int p = -1, bool ra = false):
            type(t), value(v), precedence(p), rightAssociative(ra) {}
};

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.value;
    return os;
}