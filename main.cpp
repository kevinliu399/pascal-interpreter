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
        exit(1);
    }

    void advance()
    {
        this -> pos ++;
        if (this -> pos > this -> text.size() - 1)
        {
            this -> current_char = '\0'; // EOF
        }
        else
        {
            this -> current_char = this -> text[this -> pos];
        }
    }

    void skip_whitespace()
    {
        while (this -> current_char != '\0' && isspace(this -> current_char))
        {
            this -> advance();
        }
    }

    int integer()
    {
        std::string res = "";
        while (this -> current_char != '\0' && isdigit(this -> current_char))
        {
            res += this -> current_char;
            this -> advance();
        }

        return std::stoi(res);
    }

    Token get_next_token()
    {
        while (this -> current_char != '\0')
        {
            if (isspace(this -> current_char))
            {
                this -> skip_whitespace();
                continue;
            }

            if (isdigit(this -> current_char))
            {
                return Token(INTEGER, this -> integer());
            }

            if (this -> current_char == '+')
            {
                this -> advance();
                return Token(PLUS, '+');
            }

            if (this -> current_char == '-')
            {
                this -> advance();
                return Token(MINUS, '-');
            }

            this -> error();
        }

        return Token(T_EOF, 0);
    }

    // Parser / Interpreter code

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

    int term()
    {
        Token token = this -> current_token;
        this->eat(INTEGER);
        return token.getValue();
    }

    int expr()
    {
        this -> current_token = this -> get_next_token();

        int res = this -> term();
        while (this -> current_token.getType() == PLUS || this -> current_token.getType() == MINUS)
        {
            Token token = this -> current_token;
            if (token.getType() == PLUS)
            {
                this -> eat(PLUS);
                res += this -> term();
            }
            else if (token.getType() == MINUS)
            {
                this -> eat(MINUS);
                res -= this -> term();
            }
        }

        return res;
    }
};

int main()
{
    while (true)
    {
        std::string text;
        std::cout << "calc> ";
        std::getline(std::cin, text);

        if (text == "exit")
        {
            break;
        }

        Interpreter interpreter(text);
        std::cout << interpreter.expr() << "\n";
    }

    return 0;
}