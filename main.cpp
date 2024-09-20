#include <string.h>
#include <iostream>

typedef enum TokenType {
    INTEGER,
    T_EOF,
    PLUS,
    MINUS,
} TokenType;

class Token
{
private:
    TokenType type;
    int value;

public:
    Token() : type(T_EOF), value(0) {}
    Token(TokenType t, int v) : type(t), value(v) {}

    TokenType getType()
    {
        return type;
    }

    int getValue()
    {
        return value;
    }

    void repr()
    {
        std::cout << "Token(" << type << ", " << value << ")" << "\n";
    } 
};

class Interpreter
{
private:
    std::string text;
    int pos;
    Token current_token;
    char current_char;

public:

    Interpreter(std::string t) : text(t), pos(0), current_token(Token(T_EOF, 0)), current_char(this -> text[this -> pos]) {}

    void error()
    {
        std::cout << "Error parsing input" << "\n";
    }

    void advance()
    {
        this -> pos ++;
        if (this -> pos > this -> text.size() - 1)
        {
            this -> current_char = '\0';
        }
        else
        {
            this -> current_char = this -> text[this -> pos];
        }
    }

    Token get_next_token()
    {
        if (this -> pos > this -> text.size() - 1)
        {
            return Token(T_EOF, 0);
        }

        char curr_char = this -> text[this -> pos];

        if (isdigit(curr_char))
        {
            this -> pos++;
            return Token(INTEGER, curr_char - '0');
        }

        if (curr_char == '+')
        {
            this -> pos++;
            return Token(PLUS, '+');
        }

        this -> error();
    }

    void eat(TokenType t_type)
    {
        if (this -> current_token.getType() == t_type)
        {
            this -> current_token = this -> get_next_token();
        }
        else
        {
            this -> error();
        }
    }

    int expr()
    {
        this -> current_token = this -> get_next_token();

        Token t_left = this -> current_token;
        this -> eat(INTEGER);

        Token t_op = this -> current_token;
        this -> eat(PLUS);

        Token t_right = this -> current_token;
        this -> eat(INTEGER);

        return t_left.getValue() + t_right.getValue();
    }
};

int main()
{
    std::string text = "4+6+2";
    Interpreter interpreter(text);
    std::cout << interpreter.expr() << "\n";
    return 0;
}