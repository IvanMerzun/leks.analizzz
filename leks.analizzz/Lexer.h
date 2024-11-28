#pragma once
#include "Token.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>

using namespace std;

// ����������� ���������� � �������� ���������
class Lexer {
private:
    std::ifstream inputFile;
    char currentChar;
    bool endOfFile;
    int currentLine; // ������� ����� ������

    void nextChar() 
    {
        if (!inputFile.get(currentChar)) 
        {
            endOfFile = true;
        }
        //std::cout << "Read char: " << currentChar << " at line " << currentLine << std::endl; // ��� �������

        if (currentChar == '\n') {
            currentLine++; // ����������� ����� ������
            
        }
    }

    void skipWhitespace() 
    {
        while (std::isspace(currentChar)) 
        {
            nextChar();
        }
    }

    bool isLetter(char c) 
    {
        return std::isalpha(c); // �������� �� ����� (������� ��������� � ��������)
    }

    bool isDigit(char c) 
    {
        return std::isdigit(c);
    }

    Token collectWord() 
    {
        string lexeme;
        // ������� ��������� ���������
        if (currentChar == '+' || currentChar == '=' || currentChar == '-')
        {
            lexeme += currentChar;
            nextChar(); // ��������� � ���������� ������� ����� ���������
            if (currentChar == ';' || currentChar == ')' || currentChar == '(' || currentChar == '{' ||
                currentChar == '}' || currentChar == ' ')
            {
                return Token(OPERATOR, lexeme); // ���������� ����� ���������
            }
            else
            {
                //�������� ��������� �������
                while (!endOfFile && !isspace(currentChar) &&
                    currentChar != ';' && currentChar != ')' &&
                    currentChar != '(' && currentChar != '{' &&
                    currentChar != '}' && currentChar != ',')
                {
                    lexeme += currentChar;
                    nextChar(); // ��������� � ���������� ������� ����� ���������
                }
                return Token(ERROR, lexeme);
            }
           
        }

        // ��������� ������ ������
        if (isLetter(currentChar))
        {
            // ���� ������ ������ � �����, �������� �������� ������������� �������� ����� ��� �������������
            bool isPotentialKeyword = true;   // ����, ����������� �� ����������� ������������� �������� ����
            int keywordStep = 0;              // ������� ����� ��� �������� �������� ����

            // ����� ��� ������������� ����������� ����
            bool isCheckingInt = true;
            bool isCheckingFloat = true;
            bool isCheckingMain = true;
            bool isCheckingReturn = true;
            bool isCheckingItof = true;
            bool isCheckingFtoi = true;
            //bool isCheckingConst = true;

            // �������� ������� �����������
            while (!endOfFile && !isspace(currentChar) &&
                currentChar != ';' && currentChar != ')' &&
                currentChar != '(' && currentChar != '{' &&
                currentChar != '}' && currentChar != ',')
            {
                

                // ��������� �������� ��� "int"
                if (isCheckingInt) 
                {
                    if (keywordStep == 0 && currentChar == 'i') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 1 && currentChar == 'n') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 2 && currentChar == 't') 
                    {
                        keywordStep++;
                        lexeme += currentChar;
                        nextChar();
                        if (currentChar == ' ' || currentChar == ';' || currentChar == '(' || currentChar == ')') 
                        {
                            return Token(TYPE, "int"); // ����� "int", ��������� �� �����������
                        }
                        else
                        {
                            
                            isCheckingInt = false;
                        }
                    }
                    else 
                    {
                        isCheckingInt = false;
                    }
                }

                // ��������� �������� ��� "float"
                if (isCheckingFloat) 
                {
                    if (keywordStep == 0 && currentChar == 'f') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 1 && currentChar == 'l') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 2 && currentChar == 'o') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 3 && currentChar == 'a') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 4 && currentChar == 't') 
                    {
                        keywordStep++;
                        lexeme += currentChar;
                        nextChar();
                        if (currentChar == ' ' || currentChar == ';' || currentChar == '(' || currentChar == ')') 
                        {
                            return Token(TYPE, "float");
                        }
                        else
                        {
                            isCheckingFloat = false;
                        }
                    }
                    else 
                    {
                        isCheckingFloat = false;
                    }
                }

                // ��������� �������� ��� "main"
                if (isCheckingMain) 
                {
                    if (keywordStep == 0 && currentChar == 'm') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 1 && currentChar == 'a') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 2 && currentChar == 'i') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 3 && currentChar == 'n') 
                    {
                        keywordStep++;
                        lexeme += currentChar;
                        nextChar();
                        if (currentChar == ' ' || currentChar == ';' || currentChar == '(' || currentChar == ')') 
                        {
                            return Token(ID_NAME, "main");
                        }
                        else
                        {
                            isCheckingMain = false;
                        }
                    }
                    else 
                    {
                        isCheckingMain = false;
                    }
                }

                // ��������� �������� ��� "return"
                if (isCheckingReturn) 
                {
                    if (keywordStep == 0 && currentChar == 'r') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 1 && currentChar == 'e') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 2 && currentChar == 't') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 3 && currentChar == 'u') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 4 && currentChar == 'r') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 5 && currentChar == 'n') 
                    {
                        keywordStep++;
                        lexeme += currentChar;
                        nextChar();
                        if (currentChar == ' ' || currentChar == ';' || currentChar == '(' || currentChar == ')') 
                        {
                            return Token(END, "return");
                        }
                        else
                        {
                            isCheckingReturn = false;
                        }
                    }
                    else {
                        isCheckingReturn = false;
                    }
                }

                // ��������� �������� ��� "itof"
                if (isCheckingItof) 
                {
                    if (keywordStep == 0 && currentChar == 'i') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 1 && currentChar == 't') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 2 && currentChar == 'o') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 3 && currentChar == 'f') 
                    {
                        keywordStep++;
                        lexeme += currentChar;
                        nextChar();
                        if (currentChar == ' ' || currentChar == ';' || currentChar == '(' || currentChar == ')') 
                        {
                            return Token(ID_NAME, "itof");
                        }
                        else
                        {
                            isCheckingItof = false;
                        }
                    }
                    else 
                    {
                        isCheckingItof = false;
                    }
                }

                // ��������� �������� ��� "ftoi"
                if (isCheckingFtoi) 
                {
                    if (keywordStep == 0 && currentChar == 'f') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 1 && currentChar == 't') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 2 && currentChar == 'o') 
                    {
                        keywordStep++;
                    }
                    else if (keywordStep == 3 && currentChar == 'i') 
                    {
                        keywordStep++;
                        lexeme += currentChar;
                        nextChar();
                        if (currentChar == ' ' || currentChar == ';' || currentChar == '(' || currentChar == ')') 
                        {
                            return Token(ID_NAME, "ftoi");
                        }
                        else
                        {
                            isCheckingFtoi = false;
                        }
                    }
                    else 
                    {
                        isCheckingFtoi = false;
                    }
                }

                // ���� �� ���� �������� ����� �� ����������, ���������� ���� �������
                if (!isCheckingInt && !isCheckingFloat && !isCheckingMain &&
                    !isCheckingReturn && !isCheckingItof && !isCheckingFtoi)
                {
                    if (!isLetter(currentChar)) 
                    { 
                        //�������� ��������� �������
                        while (!endOfFile && !isspace(currentChar) &&
                            currentChar != ';' && currentChar != ')' &&
                            currentChar != '(' && currentChar != '{' &&
                            currentChar != '}' && currentChar != ',')
                        {
                            lexeme += currentChar;
                            nextChar(); // ��������� � ���������� ������� ����� ���������
                        }
                        return Token(ERROR, lexeme);
                    }
                }

                lexeme += currentChar;
                nextChar();
            }

            // ��� ����� ����, ������ ������� ������� ������ �� ���� � ��� �������������
            return Token(ID_NAME, lexeme);
        }


        else if (isDigit(currentChar))
        {
            bool ffloat = false;
            int k = 0;
            while (!endOfFile && !isspace(currentChar) &&
                currentChar != ';' && currentChar != ')' &&
                currentChar != '(' && currentChar != '{' &&
                currentChar != '}' && currentChar != ',')
            {
                if (!isDigit(currentChar))
                {
                    if (currentChar == '.' && k == 0)
                    {
                        ffloat = true;
                        ++k;
                    }
                    else
                    {
                        while (!endOfFile && !isspace(currentChar) &&
                            currentChar != ';' && currentChar != ')' &&
                            currentChar != '(' && currentChar != '{' &&
                            currentChar != '}' && currentChar != ',')
                        {

                            lexeme += currentChar;
                            nextChar();
                        }
                        return Token(ERROR, lexeme);
                    }
                }

                lexeme += currentChar;
                nextChar();
            }

            if (!ffloat)
            {
                return Token(CONST, lexeme);
            }
            else
            {
                return Token(CONST, lexeme);
            }
        }

        else
        {
           
            
            //�������� ��������� �������
            while (!endOfFile && !isspace(currentChar) &&
                currentChar != ';' && currentChar != ')' &&
                currentChar != '(' && currentChar != '{' &&
                currentChar != '}' && currentChar != ',')
            {

                lexeme += currentChar;
                nextChar(); // ��������� � ���������� ������� ����� ���������
            }
            return Token(ERROR, lexeme);
        }

       
    }

    
    

public:
    Lexer(const std::string& fileName) : endOfFile(false), currentLine(1)
    {
        inputFile.open(fileName);
        //inputFile << " $";
        nextChar();  // ��������� ������ ������
    }

    int getLineNumber() const 
    {
        return currentLine;
    }

    Token getNextToken() 
    {
        skipWhitespace();

        if (endOfFile) 
        {
            return Token(END_OF_FILE, "$"); // ������ $ ������ UNKNOWN
        }

        if (currentChar == ';' || currentChar == ')' || currentChar == '(' || currentChar == '{' ||
            currentChar == '}' || currentChar == ',')
        {
            string lexeme(1, currentChar);
            nextChar();
            return Token(DELIMITER, (lexeme));
            
        }
        

        else
        {
            return collectWord();
        }

        
    }

    ~Lexer() {
        if (inputFile.is_open()) 
        {
            inputFile.close();
        }
    }
};






