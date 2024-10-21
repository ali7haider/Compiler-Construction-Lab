#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <fstream>

using namespace std;

enum TokenType
{
    T_INT,
    T_ID,
    T_NUM,
    T_IF,
    T_ELSE,
    T_RETURN,
    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_MUL,
    T_DIV,
    T_LPAREN,
    T_RPAREN,
    T_LBRACE,
    T_RBRACE,
    T_SEMICOLON,
    T_GT,
    T_EOF,
};

struct Token
{
    TokenType type;
    string value;
    int lineNumber;
};

class Lexer
{
private:
    string src;
    size_t pos;
    int lineNumber;

public:
    Lexer(const string &src)
    {
        this->src = src;
        this->pos = 0;
        this->lineNumber = 1;
    }

    vector<Token> tokenize()
    {
        vector<Token> tokens;
        while (pos < src.size())
        {
            char current = src[pos];

            if (current == '\n')
            {
                lineNumber++;
                pos++;
                continue;
            }
            if (isspace(current))
            {
                pos++;
                continue;
            }
            if (isdigit(current))
            {
                tokens.push_back(Token{T_NUM, consumeNumber(), lineNumber});
                continue;
            }
            if (isalpha(current))
            {
                string word = consumeWord();
                if (word == "int")
                    tokens.push_back(Token{T_INT, word, lineNumber});
                else if (word == "if")
                    tokens.push_back(Token{T_IF, word, lineNumber});
                else if (word == "else")
                    tokens.push_back(Token{T_ELSE, word, lineNumber});
                else if (word == "return")
                    tokens.push_back(Token{T_RETURN, word, lineNumber});
                else
                    tokens.push_back(Token{T_ID, word, lineNumber});
                continue;
            }

            switch (current)
            {
            case '=':
                tokens.push_back(Token{T_ASSIGN, "=", lineNumber});
                break;
            case '+':
                tokens.push_back(Token{T_PLUS, "+", lineNumber});
                break;
            case '-':
                tokens.push_back(Token{T_MINUS, "-", lineNumber});
                break;
            case '*':
                tokens.push_back(Token{T_MUL, "*", lineNumber});
                break;
            case '/':
                tokens.push_back(Token{T_DIV, "/", lineNumber});
                break;
            case '(':
                tokens.push_back(Token{T_LPAREN, "(", lineNumber});
                break;
            case ')':
                tokens.push_back(Token{T_RPAREN, ")", lineNumber});
                break;
            case '{':
                tokens.push_back(Token{T_LBRACE, "{", lineNumber});
                break;
            case '}':
                tokens.push_back(Token{T_RBRACE, "}", lineNumber});
                break;
            case ';':
                tokens.push_back(Token{T_SEMICOLON, ";", lineNumber});
                break;
            case '>':
                tokens.push_back(Token{T_GT, ">", lineNumber});
                break;
            default:
                cout << "Unexpected character: " << "'" << current << "' at line " << lineNumber << endl;
                exit(1);
            }
            pos++;
        }
        tokens.push_back(Token{T_EOF, "", lineNumber});
        return tokens;
    }

    string consumeNumber()
    {
        size_t start = pos;
        while (pos < src.size() && isdigit(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }

    string consumeWord()
    {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos]))
            pos++;
        return src.substr(start, pos - start);
    }
};

class Parser
{
public:
    Parser(const vector<Token> &tokens)
    {
        this->tokens = tokens;
        this->pos = 0;
    }

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        cout << "Parsing completed successfully! No Syntax Error" << endl;
    }

private:
    vector<Token> tokens;
    size_t pos;

    // New error function to print message with line number
    void error(const string &message)
    {
        cout << "Syntax error at line " << tokens[pos].lineNumber << ": " << message << endl;
        exit(1);
    }

    void parseStatement()
    {
        if (tokens[pos].type == T_INT)
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignment();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else
        {
            error("unexpected token " + tokens[pos].value);
        }
    }

    void parseBlock()
    {
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
    }

    void parseDeclaration()
    {
        expect(T_INT);
        expect(T_ID);
        expect(T_SEMICOLON);
    }

    void parseAssignment()
    {
        expect(T_ID);
        expect(T_ASSIGN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        parseExpression();
        expect(T_RPAREN);
        parseStatement();
        if (tokens[pos].type == T_ELSE)
        {
            expect(T_ELSE);
            parseStatement();
        }
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        parseExpression();
        expect(T_SEMICOLON);
    }

    void parseExpression()
    {
        parseTerm();
        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS)
        {
            pos++;
            parseTerm();
        }
        if (tokens[pos].type == T_GT)
        {
            pos++;
            parseExpression(); // After relational operator, parse the next expression
        }
    }

    void parseTerm()
    {
        parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            pos++;
            parseFactor();
        }
    }

    void parseFactor()
    {
        if (tokens[pos].type == T_NUM || tokens[pos].type == T_ID)
        {
            pos++;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            expect(T_LPAREN);
            parseExpression();
            expect(T_RPAREN);
        }
        else
        {
            error("unexpected token " + tokens[pos].value);
        }
    }

    void expect(TokenType type)
    {
        if (tokens[pos].type == type)
        {
            pos++;
        }
        else
        {
            error("expected " + tokenTypeToString(type) + " but found " + tokens[pos].value);
        }
    }

    // A helper function to convert TokenType to string for better error messages
    string tokenTypeToString(TokenType type)
    {
        switch (type)
        {
        case T_INT:
            return "int";
        case T_ID:
            return "identifier";
        case T_NUM:
            return "number";
        case T_IF:
            return "if";
        case T_ELSE:
            return "else";
        case T_RETURN:
            return "return";
        case T_ASSIGN:
            return "=";
        case T_PLUS:
            return "+";
        case T_MINUS:
            return "-";
        case T_MUL:
            return "*";
        case T_DIV:
            return "/";
        case T_LPAREN:
            return "(";
        case T_RPAREN:
            return ")";
        case T_LBRACE:
            return "{";
        case T_RBRACE:
            return "}";
        case T_SEMICOLON:
            return ";";
        case T_GT:
            return ">";
        case T_EOF:
            return "end of file";
        default:
            return "unknown";
        }
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_name>\n";
        return 1;
    }

    ifstream file(argv[1]);

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << argv[1] << "\n";
        return 1;
    }

    string line;
    string content;
    while (getline(file, line))
    {
        content += line + "\n";
    }

    file.close();

    Lexer lexer(content);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    return 0;
}
