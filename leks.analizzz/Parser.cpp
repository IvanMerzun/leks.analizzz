#include "Parser.h"
#include <stdexcept>
using namespace std;

Parser::Parser(const std::string& inputFile) : lexer(inputFile), currentToken(Token(UNKNOWN, "")) {}
            
void Parser::nextToken() {
    currentToken = lexer.getNextToken();
    
    //cout << "Next token: " << currentToken.value << " at line " << lexer.getLineNumber() << std::endl; // �������

    if (currentToken.type != END_OF_FILE) {
        symbolTable.insert(currentToken);
    }
    if (currentToken.type == ERROR)
    {
        throw runtime_error("Error lexem at line " + to_string(lexer.getLineNumber()));
    }
}

Node Parser::parse() {
    Node root("Function");
    nextToken();
    Function(root);
    if (currentToken.type != END_OF_FILE) {
        throw std::runtime_error("Extra tokens after end of parsing at line " + to_string(lexer.getLineNumber()));
    }
    return root;
}

// ������� ����������
void Parser::Function(Node& n) 
{
    n.addSon("Begin");
    Begin(n.getSon(0)); // ��������� ���� Begin

    n.addSon("Descriptions");
    Descriptions(n.getSon(1)); // ���� ��� Descriptions

    n.addSon("Operators");
    Operators(n.getSon(2)); // ���� ��� Operators

    n.addSon("End");
    End(n.getSon(3)); // ���� ��� End
} 
void Parser::Begin(Node& n) 
{  
    
    if (currentToken.type == TYPE) {
        n.addSon("Type");
        Type(n.getSon(0)); // ���� Type

        n.addSon("FunctionName");
        FunctionName(n.getSon(1)); // ���� FunctionName

        if (currentToken.value == "(") {
            n.addSon("(");
            nextToken();
            if (currentToken.value == ")") {
                n.addSon(")");
                nextToken();
                if (currentToken.value == "{") {
                    n.addSon("{");
                    nextToken();
                }
                else {
                    throw std::runtime_error("Expected '{' after function declaration at line " + to_string(lexer.getLineNumber()));
                }
            }
            else {
                throw std::runtime_error("Expected ')' after function name at line " + to_string(lexer.getLineNumber()-2));
            }
        }
        else {
            throw std::runtime_error("Expected '(' after function name at line " + to_string(lexer.getLineNumber()-1));
        }
    }
    else {
        throw std::runtime_error("Expected type for function at line " + to_string(lexer.getLineNumber()));
    }
}
void Parser::End(Node& n) {
    
    if (currentToken.value == "return") {
        n.addSon("return");
        nextToken();

        n.addSon("Id");
        Id(n.getSon(1)); // ���� ��� Id

        if (currentToken.value == ";") {
            n.addSon(";");
            nextToken();

            if (currentToken.value == "}") {
                n.addSon("}");
                nextToken();
            }
            else {
                throw std::runtime_error("Expected '}' at the end of the function at line " + to_string(lexer.getLineNumber()));
            }
        }
        else {
            throw std::runtime_error("Expected ';' after return statement at line " + to_string(lexer.getLineNumber()-1));
        }
    }
    else {
        throw std::runtime_error("Expected 'return' statement in function end at line " + to_string(lexer.getLineNumber()));
    }
}
void Parser::Descriptions(Node& n) {
    if (currentToken.type == TYPE) {
        // ��������� Descr ������ ��� ������ �������� ����������
        n.addSon("Descr");
        Descr(n.getSon(n.getChildCount() - 1)); // ���� Descr ��� �������� �������� ����������

        // ���������� �������� Descriptions ��� ��������� ���������� �������� ����������
        Descriptions(n);
    }
    
}
void Parser::Operators(Node& n) {
    if (currentToken.type == ID_NAME) {
        n.addSon("Op");
        Op(n.getSon(n.getChildCount() - 1)); // ���� Op

        Operators(n);
    }
    
}
void Parser::Descr(Node& n) {
    n.addSon("Type");
    Type(n.getSon(0)); // ���� ��� Type

    n.addSon("VarList");
    VarList(n.getSon(1)); // ���� ��� VarList

    if (currentToken.value == ";") {
        n.addSon(";");
        nextToken();
    }
    else {
        throw std::runtime_error("Expected ';' after variable description at line " + to_string(lexer.getLineNumber()-1));
    }
}
void Parser::VarList(Node& n) {
   
    // ������ � ���������� ������� �������������� � VarList
    n.addSon("Id");
    Id(n.getSon(0)); // ���� ��� ������� Id

    // ������ ������������ ������� � ��������� ��������������
    while (currentToken.value == ",") {
        n.addSon(",");
        nextToken(); // ���������� �������

        // ��������� ��������� Id
        n.addSon("Id");
        Id(n.getSon(n.getChildCount() - 1)); // �������� ��������� ����������� ���� � ��������� ����� Id
    }
}
void Parser::Type(Node& n) {
    if (currentToken.type == TYPE) {
        n.addSon(currentToken.value); // ���� ��� ���� (int ��� float)
        nextToken();
    }
    else {
        throw std::runtime_error("Expected type specifier (int or float) at line " + to_string(lexer.getLineNumber()));
    }
}
void Parser::Op(Node& n) {
    n.addSon("Id");
    Id(n.getSon(0)); // ���� Id

    if (currentToken.value == "=") {
        n.addSon("=");
        nextToken();

        // ������� ���� Expr ��� ����� ���������
        Node& exprNode = n.addSon("Expr");
        Expr(exprNode); // ��������� ��������� � ���� Expr

        if (currentToken.value == ";") {
            n.addSon(";");
            nextToken();
        }
        else {
            throw std::runtime_error("Expected ';' after assignment at line " + to_string(lexer.getLineNumber()));
        }
    }
    else {
        throw std::runtime_error("Expected '=' in assignment at line " + to_string(lexer.getLineNumber()));
    }
}
void Parser::SimpleExpr(Node& n) {

    if (currentToken.value == "ftoi" || currentToken.value == "itof") {
        n.addSon(currentToken.value); // ��������� ������� (ftoi ��� itof)
        nextToken();

        if (currentToken.value == "(") {
            n.addSon("("); // ��������� ����������� ������
            nextToken();

            Node& exprNode = n.addSon("Expr");
            Expr(exprNode); // ��������� ��������� ������ ������

            if (currentToken.value == ")") {
                n.addSon(")"); // ��������� ����������� ������
                nextToken();
            }
            else {
                throw std::runtime_error("Expected ')' after function argument at line " + to_string(lexer.getLineNumber()));
            }
        }
        else {
            throw std::runtime_error("Expected '(' after function call at line " + to_string(lexer.getLineNumber()));
        }
    }
    else if (currentToken.type == ID_NAME) {
        n.addSon("Id");
        Id(n.getSon(0)); // ���� ��� ��������������
    }
    else if (currentToken.type == CONST) {
        n.addSon("Const");
        Const(n.getSon(0)); // ���� ��� ���������
    }
    else if (currentToken.value == "(") {
        n.addSon("(");
        nextToken();

        Node& exprNode = n.addSon("Expr");
        Expr(exprNode); // ��������� ���������

        if (currentToken.value == ")") {
            n.addSon(")");
            nextToken();
        }
        else {
            throw std::runtime_error("Expected ')' after expression at line " + to_string(lexer.getLineNumber()));
        }
    }
    else {
        throw std::runtime_error("Invalid simple expression at line " + to_string(lexer.getLineNumber()));
    }
}
void Parser::Expr(Node& n) {
    // ��������� ������ �������
    n.addSon("SimpleExpr");
    SimpleExpr(n.getSon(0));

    // ������������ ������� ��������
    while (currentToken.value == "+" || currentToken.value == "-") {
        n.addSon(currentToken.value); // ���� ��� ���������
        nextToken();

        n.addSon("SimpleExpr");
        SimpleExpr(n.getSon(n.getChildCount() - 1)); // ���������� getChildCount()
    }
}
void Parser::Id(Node& n) {
    if (currentToken.type == ID_NAME) {
        n.addSon(currentToken.value); // ���� ��� ��������������
        nextToken();
    }
    else {
        throw std::runtime_error("Expected identifier an line " + to_string(lexer.getLineNumber()));
    }
}
void Parser::Const(Node& n) {
    if (currentToken.type == CONST) {
        n.addSon(currentToken.value); // ���� ��� ���������
        nextToken();
    }
    else {
        throw std::runtime_error("Expected constant at line " + to_string(lexer.getLineNumber()));
    }
}

void Parser::FunctionName(Node& n)
{
    if (currentToken.type == ID_NAME) {
        n.addSon(currentToken.value); // ��������� ������������� ��� �������� ����
        nextToken();
    }
    else {
        throw std::runtime_error("Expected function name (identifier) at line " + to_string(lexer.getLineNumber()));
    }
}
