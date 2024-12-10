#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <vector>
#include <sstream>
#include <stdexcept>
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
// Symbol Table Class
class SymbolTable
{
public:
    void declareVariable(const string &name, const string &type)
    {
        if (symbolTable.find(name) != symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is already declared.");
        }
        symbolTable[name] = type;
    }

    void declareType(const string &name, const string &category)
    {
        if (typeTable.find(name) != typeTable.end())
        {
            throw runtime_error("Semantic error: Type '" + name + "' is already declared.");
        }
        typeTable[name] = category;
    }

    string getVariableType(const string &name)
    {
        if (symbolTable.find(name) == symbolTable.end())
        {
            throw runtime_error("Semantic error: Variable '" + name + "' is not declared.");
        }
        return symbolTable[name];
    }

    bool isDeclared(const string &name) const
    {
        return symbolTable.find(name) != symbolTable.end();
    }

    bool isType(const string &name) const
    {
        auto it = typeTable.find(name);
        if (it == typeTable.end())
            return false;
        return (it->second == "struct" || it->second == "class");
    }

private:
    map<string, string> symbolTable; // name -> type
    map<string, string> typeTable;   // typeName -> category
};

// Intermediate Code Generator Class
class IntermediateCodeGenerator
{
public:
    vector<string> instructions;
    int tempCount = 0;

    string newTemp()
    {
        return "t" + to_string(tempCount++);
    }

    void addInstruction(const string &instr)
    {
        instructions.push_back(instr);
    }

    void printInstructions() const
    {
        for (const auto &instr : instructions)
        {
            cout << instr << endl;
        }
    }

    vector<string> getInstructionsAsVector() const
    {
        return instructions;
    }
};
// Parser Class
class Parser
{
public:
    Parser(const vector<Token> &tokens, SymbolTable &symTable, IntermediateCodeGenerator &icg)
        : tokens(tokens), pos(0), symTable(symTable), icg(icg) {}

    void parseProgram()
    {
        while (tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
    }

private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable &symTable;
    IntermediateCodeGenerator &icg;
    stack<int> switchEndLabels; // Stack to keep track of current switch end labels
    stack<int> loopEndLabels;   // Stack to keep track of loop end labels

    void parseStatement()
    {
        if (isDeclarationStart())
        {
            parseDeclaration();
        }
        else if (tokens[pos].type == T_STRUCT || tokens[pos].type == T_CLASS)
        {
            parseTypeDeclaration();
        }
        else if (tokens[pos].type == T_IF)
        {
            parseIfStatement();
        }
        else if (tokens[pos].type == T_WHILE)
        {
            parseWhileStatement();
        }
        else if (tokens[pos].type == T_FOR)
        {
            parseForStatement();
        }
        else if (tokens[pos].type == T_SWITCH)
        {
            parseSwitchStatement();
        }
        else if (tokens[pos].type == T_FUNC)
        {
            parseFunction();
        }
        else if (tokens[pos].type == T_RETURN)
        {
            parseReturnStatement();
        }
        else if (tokens[pos].type == T_BREAK)
        {
            parseBreakStatement();
        }
        else if (tokens[pos].type == T_LBRACE)
        {
            parseBlock();
        }
        else if (tokens[pos].type == T_ID)
        {
            parseAssignmentOrStructAccess();
        }
        else if (tokens[pos].type == T_SEMICOLON)
        {
            pos++; // Empty statement
        }
        else
        {
            error("Unexpected token in parseStatement");
        }
    }

    bool isDeclarationStart() const
    {
        return (tokens[pos].type == T_INT ||
                tokens[pos].type == T_BOOL ||
                tokens[pos].type == T_STRING_TYPE);
    }

    void parseTypeDeclaration()
    {
        if (tokens[pos].type == T_STRUCT)
        {
            parseStructDeclaration();
        }
        else if (tokens[pos].type == T_CLASS)
        {
            parseClassDeclaration();
        }
        else
        {
            error("Unknown type declaration");
        }
    }

    void parseFunction()
    {
        expect(T_FUNC);
        string funcName = expectAndReturnValue(T_ID);
        expect(T_LPAREN);
        expect(T_RPAREN);
        expect(T_LBRACE);
        icg.addInstruction("FUNC " + funcName + ":");
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseStatement();
        }
        expect(T_RBRACE);
        icg.addInstruction("END FUNC " + funcName);
    }

    void parseDeclaration()
    {
        TokenType type = tokens[pos].type;
        string typeName;
        if (type == T_INT)
            typeName = "int";
        else if (type == T_BOOL)
            typeName = "bool";
        else if (type == T_STRING_TYPE)
            typeName = "string";
        else
            error("Unknown type in declaration");

        pos++;
        string varName = expectAndReturnValue(T_ID);
        symTable.declareVariable(varName, typeName);
        // Handle optional initialization
        if (tokens[pos].type == T_ASSIGN)
        {
            pos++; // Consume '='
            string expr = parseExpression();
            icg.addInstruction(varName + " = " + expr);
        }
        expect(T_SEMICOLON);
    }

    void parseAssignmentOrStructAccess()
    {
        string lhs = parseLValue();
        if (tokens[pos].type == T_ASSIGN)
        {
            pos++;
            string rhs = parseExpression();
            icg.addInstruction(lhs + " = " + rhs);
            expect(T_SEMICOLON);
        }
        else
        {
            error("Expected assignment after lvalue");
        }
    }

    string parseLValue()
    {
        string id = expectAndReturnValue(T_ID);
        while (tokens[pos].type == T_DOT)
        {
            pos++; // Skip '.'
            string member = expectAndReturnValue(T_ID);
            id += "." + member;
        }
        return id;
    }

    void parseIfStatement()
    {
        expect(T_IF);
        expect(T_LPAREN);
        string cond = parseExpression();
        expect(T_RPAREN);
        string temp = icg.newTemp();
        icg.addInstruction(temp + " = " + cond);
        int labelTrue = icg.tempCount++;
        int labelFalse = icg.tempCount++;
        icg.addInstruction("if " + temp + " goto L" + to_string(labelTrue));
        icg.addInstruction("goto L" + to_string(labelFalse));
        icg.addInstruction("L" + to_string(labelTrue) + ":");
        parseStatement();
        if (tokens[pos].type == T_ELSE)
        {
            int labelEnd = icg.tempCount++;
            icg.addInstruction("goto L" + to_string(labelEnd));
            icg.addInstruction("L" + to_string(labelFalse) + ":");
            expect(T_ELSE);
            parseStatement();
            icg.addInstruction("L" + to_string(labelEnd) + ":");
        }
        else
        {
            icg.addInstruction("L" + to_string(labelFalse) + ":");
        }
    }

    void parseReturnStatement()
    {
        expect(T_RETURN);
        string expr = parseExpression();
        icg.addInstruction("return " + expr);
        expect(T_SEMICOLON);
    }

    void parseBreakStatement()
    {
        expect(T_BREAK);
        expect(T_SEMICOLON);
        if (!switchEndLabels.empty())
        {
            int endLabel = switchEndLabels.top();
            cout << "Parsing 'break;' with switchEndLabel: " << endLabel << endl;
            icg.addInstruction("goto L" + to_string(endLabel));
        }
        else if (!loopEndLabels.empty())
        {
            int endLabel = loopEndLabels.top();
            cout << "Parsing 'break;' with loopEndLabel: " << endLabel << endl;
            icg.addInstruction("goto L" + to_string(endLabel));
        }
        else
        {
            cout << "Error: 'break;' found outside of switch or loop at line "
                 << tokens[pos].lineNumber << endl;
            exit(1);
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

    string parseExpression()
    {
        string left = parseTerm();

        while (tokens[pos].type == T_PLUS || tokens[pos].type == T_MINUS ||
               tokens[pos].type == T_GT || tokens[pos].type == T_LT ||
               tokens[pos].type == T_EQ || tokens[pos].type == T_NEQ ||
               tokens[pos].type == T_GTE || tokens[pos].type == T_LTE)
        {

            TokenType op = tokens[pos].type;
            pos++;
            string right = parseTerm();

            string temp = icg.newTemp();
            string opStr;

            if (op == T_PLUS)
                opStr = " + ";
            else if (op == T_MINUS)
                opStr = " - ";
            else if (op == T_GT)
                opStr = " > ";
            else if (op == T_LT)
                opStr = " < ";
            else if (op == T_EQ)
                opStr = " == ";
            else if (op == T_NEQ)
                opStr = " != ";
            else if (op == T_GTE)
                opStr = " >= ";
            else if (op == T_LTE)
                opStr = " <= ";

            icg.addInstruction(temp + " = " + left + opStr + right);
            left = temp;
        }
        return left;
    }

    string parseTerm()
    {
        string left = parseFactor();
        while (tokens[pos].type == T_MUL || tokens[pos].type == T_DIV)
        {
            TokenType op = tokens[pos].type;
            pos++;
            string right = parseFactor();
            string temp = icg.newTemp();
            icg.addInstruction(temp + " = " + left + (op == T_MUL ? " * " : " / ") + right);
            left = temp;
        }
        return left;
    }

    string parseFactor()
    {
        if (tokens[pos].type == T_NUM)
        {
            string val = tokens[pos].value;
            pos++;
            return val;
        }
        else if (tokens[pos].type == T_ID)
        {
            string id = tokens[pos].value;
            pos++;
            while (tokens[pos].type == T_DOT)
            {
                pos++; // Skip '.'
                string member = expectAndReturnValue(T_ID);
                id += "." + member;
            }
            return id;
        }
        else if (tokens[pos].type == T_LPAREN)
        {
            pos++;
            string expr = parseExpression();
            expect(T_RPAREN);
            return expr;
        }
        else if (tokens[pos].type == T_STRING)
        {
            string str = "\"" + tokens[pos].value + "\"";
            pos++;
            return str;
        }
        else if (tokens[pos].type == T_TRUE || tokens[pos].type == T_FALSE)
        {
            string boolVal = tokens[pos].value;
            pos++;
            return boolVal;
        }
        else
        {
            error("Unexpected token in parseFactor");
            return "";
        }
    }

    void expect(TokenType type)
    {
        if (tokens[pos].type != type)
        {
            error("Unexpected token");
        }
        pos++;
    }

    string expectAndReturnValue(TokenType type)
    {
        if (tokens[pos].type != type)
        {
            error("Unexpected token");
        }
        string value = tokens[pos].value;
        pos++;
        return value;
    }

    void parseWhileStatement()
    {
        expect(T_WHILE);
        expect(T_LPAREN);
        string cond = parseExpression();
        expect(T_RPAREN);
        int labelStart = icg.tempCount++;
        int labelEnd = icg.tempCount++;
        loopEndLabels.push(labelEnd);
        icg.addInstruction("L" + to_string(labelStart) + ":");
        string tempCond = icg.newTemp();
        icg.addInstruction(tempCond + " = " + cond);
        icg.addInstruction("if " + tempCond + " goto L" + to_string(labelStart + 1));
        icg.addInstruction("goto L" + to_string(labelEnd));
        icg.addInstruction("L" + to_string(labelStart + 1) + ":");
        parseStatement();
        icg.addInstruction("goto L" + to_string(labelStart));
        icg.addInstruction("L" + to_string(labelEnd) + ":");
        loopEndLabels.pop();
    }

    void parseForStatement()
    {
        expect(T_FOR);
        expect(T_LPAREN);
        // Handle initialization
        if (isDeclarationStart())
        {
            parseDeclaration();
        }
        else
        {
            parseAssignmentOrStructAccess();
        }
        // Handle condition
        string cond = parseExpression();
        expect(T_SEMICOLON);
        // Handle increment using parseAssignmentOrStructAccess()
        // Store current position to parse increment after the body
        size_t incrementPos = pos;
        // Temporarily create a copy of the current instruction count
        int tempCountBeforeIncrement = icg.tempCount;
        // Parse increment as an assignment
        string incrementLHS = expectAndReturnValue(T_ID);
        if (tokens[pos].type == T_DOT)
        {
            // Handle struct member assignment if needed
            while (tokens[pos].type == T_DOT)
            {
                pos++; // Skip '.'
                string member = expectAndReturnValue(T_ID);
                incrementLHS += "." + member;
            }
        }
        expect(T_ASSIGN);
        string incrementRHS = parseExpression();
        icg.addInstruction(incrementLHS + " = " + incrementRHS);
        expect(T_RPAREN);

        int labelStart = icg.tempCount++;
        int labelEnd = icg.tempCount++;
        loopEndLabels.push(labelEnd);
        icg.addInstruction("L" + to_string(labelStart) + ":");
        string tempCond = icg.newTemp();
        icg.addInstruction(tempCond + " = " + cond);
        icg.addInstruction("if " + tempCond + " goto L" + to_string(labelStart + 1));
        icg.addInstruction("goto L" + to_string(labelEnd));
        icg.addInstruction("L" + to_string(labelStart + 1) + ":");
        parseStatement();
        // Add the increment instruction after the body
        icg.addInstruction(incrementLHS + " = " + incrementRHS);
        icg.addInstruction("goto L" + to_string(labelStart));
        icg.addInstruction("L" + to_string(labelEnd) + ":");
        loopEndLabels.pop();
    }

    void parseSwitchStatement()
    {
        expect(T_SWITCH);
        expect(T_LPAREN);
        string expr = parseExpression();
        expect(T_RPAREN);
        expect(T_LBRACE);

        int switchEndLabel = icg.tempCount++;
        cout << "Pushing switchEndLabel: " << switchEndLabel << endl;
        switchEndLabels.push(switchEndLabel); // Push current switch end label

        while (tokens[pos].type == T_CASE || tokens[pos].type == T_DEFAULT)
        {
            if (tokens[pos].type == T_CASE)
            {
                expect(T_CASE);
                string caseValue = expectAndReturnValue(T_NUM);
                expect(T_COLON);

                // Generate intermediate code for case
                icg.addInstruction("if " + expr + " == " + caseValue + " goto L" + to_string(icg.tempCount));
                icg.addInstruction("goto L" + to_string(switchEndLabel));
                icg.addInstruction("L" + to_string(icg.tempCount++) + ":");

                // Parse statements within the case
                parseStatement(); // Parses `x = 30;`
            }
            else if (tokens[pos].type == T_DEFAULT)
            {
                expect(T_DEFAULT);
                expect(T_COLON);

                // Generate intermediate code for default case
                icg.addInstruction("goto L" + to_string(icg.tempCount));
                icg.addInstruction("L" + to_string(icg.tempCount++) + ":");

                // Parse statements within the default case
                parseStatement(); // Parses `x = 40;`
            }
        }

        expect(T_RBRACE);
        icg.addInstruction("L" + to_string(switchEndLabel) + ":");

        cout << "Popping switchEndLabel: " << switchEndLabel << endl;
        switchEndLabels.pop(); // Pop current switch end label
    }

    void parseStructDeclaration()
    {
        expect(T_STRUCT);
        string structName = expectAndReturnValue(T_ID);
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseDeclaration();
        }
        expect(T_RBRACE);
        expect(T_SEMICOLON);
        symTable.declareType(structName, "struct");
    }

    void parseClassDeclaration()
    {
        expect(T_CLASS);
        string className = expectAndReturnValue(T_ID);
        expect(T_LBRACE);
        while (tokens[pos].type != T_RBRACE && tokens[pos].type != T_EOF)
        {
            parseDeclaration();
        }
        expect(T_RBRACE);
        expect(T_SEMICOLON);
        symTable.declareType(className, "class");
    }

    void error(const string &message)
    {
        cout << "Syntax error at line " << tokens[pos].lineNumber << ": " << message
             << " Unexpected token: '" << tokens[pos].value << "' (type: " << tokens[pos].type << ")" << endl;
        exit(1);
    }
};
int main()
{
    string code = R"(
        int a = 10;
        if (a < 20) {
            a = a + 1;
        }
    )";
    Lexer lexer(code);
    vector<Token> tokenList = lexer.tokenize();
    SymbolTable symbolTable;
    IntermediateCodeGenerator codeGen;
    Parser parser(tokenList, symbolTable, codeGen);
    parser.parseProgram();

    return 0;
}
