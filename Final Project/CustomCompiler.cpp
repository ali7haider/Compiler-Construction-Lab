#include <iostream>
#include <string>
#include <vector>
#include <cctype>

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
    T_LT,
    T_EQ,
    T_NEQ,
    T_GTE,
    T_LTE,
    T_WHILE,
    T_FUNC,
    T_SWITCH,
    T_CASE,
    T_DEFAULT,
    T_BREAK,
    T_BOOL,
    T_TRUE,
    T_FALSE,
    T_STRING,
    T_COMMENT,
    T_COLON,
    T_FOR,
    T_STRUCT,
    T_CLASS,
    T_ARRAY,
    T_LBRACKET,
    T_RBRACKET,
    T_DOT,
    T_STRING_TYPE,
    T_EOF
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
    Lexer(const string &src) : src(src), pos(0), lineNumber(1) {}
    vector<Token> tokenize();

private:
    string consumeNumber();
    string consumeWord();
    string consumeString();
    TokenType identifyKeyword(const string &word);
    void error(const string &message);
};

vector<Token> Lexer::tokenize()
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
        if (isalpha(current) || current == '_')
        {
            string word = consumeWord();
            TokenType type = identifyKeyword(word);
            tokens.push_back(Token{type, word, lineNumber});
            continue;
        }
        if (current == '"')
        {
            tokens.push_back(Token{T_STRING, consumeString(), lineNumber});
            continue;
        }
        switch (current)
        {
        case '=':
            if (pos + 1 < src.size() && src[pos + 1] == '=')
            {
                tokens.push_back(Token{T_EQ, "==", lineNumber});
                pos++;
            }
            else
            {
                tokens.push_back(Token{T_ASSIGN, "=", lineNumber});
            }
            break;
        case '!':
            if (pos + 1 < src.size() && src[pos + 1] == '=')
            {
                tokens.push_back(Token{T_NEQ, "!=", lineNumber});
                pos++;
            }
            else
            {
                tokens.push_back(Token{T_ASSIGN, "!", lineNumber});
            }
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
            if (pos + 1 < src.size() && src[pos + 1] == '=')
            {
                tokens.push_back(Token{T_GTE, ">=", lineNumber});
                pos++;
            }
            else
            {
                tokens.push_back(Token{T_GT, ">", lineNumber});
            }
            break;
        case '<':
            if (pos + 1 < src.size() && src[pos + 1] == '=')
            {
                tokens.push_back(Token{T_LTE, "<=", lineNumber});
                pos++;
            }
            else
            {
                tokens.push_back(Token{T_LT, "<", lineNumber});
            }
            break;
        case ':':
            tokens.push_back(Token{T_COLON, ":", lineNumber});
            break;
        case '[':
            tokens.push_back(Token{T_LBRACKET, "[", lineNumber});
            break;
        case ']':
            tokens.push_back(Token{T_RBRACKET, "]", lineNumber});
            break;
        case '.':
            tokens.push_back(Token{T_DOT, ".", lineNumber});
            break;
        default:
            error("Unexpected character");
        }
        pos++;
    }
    tokens.push_back(Token{T_EOF, "", lineNumber});
    return tokens;
}

string Lexer::consumeNumber()
{
    size_t start = pos;
    while (pos < src.size() && isdigit(src[pos]))
        pos++;
    return src.substr(start, pos - start);
}

string Lexer::consumeWord()
{
    size_t start = pos;
    while (pos < src.size() && (isalnum(src[pos]) || src[pos] == '_'))
        pos++;
    return src.substr(start, pos - start);
}

string Lexer::consumeString()
{
    pos++; // Skip opening quote
    string str = "";
    while (pos < src.size() && src[pos] != '"')
    {
        if (src[pos] == '\\' && pos + 1 < src.size())
        {
            // Handle escape characters
            pos++;
            char escape = src[pos];
            switch (escape)
            {
            case 'n':
                str += '\n';
                break;
            case 't':
                str += '\t';
                break;
            case '\\':
                str += '\\';
                break;
            case '"':
                str += '"';
                break;
            default:
                str += escape;
                break;
            }
        }
        else
        {
            str += src[pos];
        }
        pos++;
    }
    if (pos >= src.size())
    {
        error("Unterminated string literal");
    }
    pos++; // Skip closing quote
    return str;
}

TokenType Lexer::identifyKeyword(const string &word)
{
    if (word == "int")
        return T_INT;
    else if (word == "if")
        return T_IF;
    else if (word == "else")
        return T_ELSE;
    else if (word == "return")
        return T_RETURN;
    else if (word == "while")
        return T_WHILE;
    else if (word == "func")
        return T_FUNC;
    else if (word == "switch")
        return T_SWITCH;
    else if (word == "case")
        return T_CASE;
    else if (word == "default")
        return T_DEFAULT;
    else if (word == "bool")
        return T_BOOL;
    else if (word == "true")
        return T_TRUE;
    else if (word == "false")
        return T_FALSE;
    else if (word == "for")
        return T_FOR;
    else if (word == "struct")
        return T_STRUCT;
    else if (word == "class")
        return T_CLASS;
    else if (word == "array")
        return T_ARRAY;
    else if (word == "string")
        return T_STRING_TYPE;
    return T_ID;
}

void Lexer::error(const string &message)
{
    cout << "Lexical error at line " << lineNumber << ": " << message << endl;
    exit(1);
}

int main()
{
    string code = R"(
        int a = 10;
        if (a < 20) {
            a = a + 1;
        }
    )";
    Lexer lexer(code);
    auto tokens = lexer.tokenize();

    for (const auto &token : tokens)
    {
        cout << "Token: " << token.value << ", Type: " << token.type << ", Line: " << token.lineNumber << endl;
    }
    return 0;
}
