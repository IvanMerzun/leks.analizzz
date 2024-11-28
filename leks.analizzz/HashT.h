#pragma once
#include "Lexer.h"
#include "Token.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

// ���� ��� �������� ������ � ���-�������
struct Nodehash {
    Token token;
    Nodehash* next;

    Nodehash(Token t) : token(t), next(nullptr) {}
};

class HashT
{
private:
    static const int TABLE_SIZE = 100; // ������ ���-�������
    Nodehash* table[TABLE_SIZE];           // ������ ���������� �� ������ �������� ������� 

    // ����� ASCII-����� �������� �������
    int hash(const string& lexeme) 
    {
        int hashValue = 0;
        for (char c : lexeme) 
        {
            hashValue += c;
        }
        return hashValue % TABLE_SIZE;
    }

    // ��������, ���������� �� ������� � �������
    bool exists(const string& lexeme) 
    {
        int index = hash(lexeme);
        Nodehash* current = table[index];
        while (current != nullptr) 
        {
            if (current->token.value == lexeme) 
            {
                return true; // ������� ��� ����
            }
            current = current->next;
        }
        return false;
    }

    // �������������� ���� ������ � ������
    string tokenTypeToString(TokenType type) 
    {
        switch (type) 
        {
        case ID_NAME: return "ID_NAME";
        case FLOAT_NUM: return "FLOAT_NUM";
        case INT_NUM: return "INT_NUM";
        case OPERATOR: return "OPERATOR";
        case TYPE: return "TYPE";
        case BEGIN: return "BEGIN";
        case END: return "END";
        case DELIMITER: return "DELIMITER";
        case ERROR: return "ERROR";
        case UNKNOWN: return "UNKNOWN";
        case CONST: return "CONST";
        case FUNC_NAME: return "FUNC_NAME";
        default: return "UNKNOWN";
        }
    }

public:
    // �����������
    HashT() 
    {
        for (int i = 0; i < TABLE_SIZE; ++i) 
        {
            table[i] = nullptr;
        }
    }

    // ����� ��� ���������� ������ � ������� (��� ����������)
    void insert(const Token& token) 
    {
        if (token.value.empty()) 
        {
            return; // �� ��������� ������ � ������ �������
        }

        
        if (!exists(token.value)) 
        { // ���������, ��� �� ������ ������
            int index = hash(token.value);
            Nodehash* newNodehash = new Nodehash(token);

            // ������� � ������ ������
            if (table[index] == nullptr) 
            {
                table[index] = newNodehash;
            }
            else 
            {
                newNodehash->next = table[index];
                table[index] = newNodehash;
            }
        }
    }

    // ����� ��� ������ �������
    void print(ofstream& outputFile) 
    {
        for (int i = 0; i < TABLE_SIZE; ++i) 
        {
            if (table[i] != nullptr) 
            {
                Nodehash* current = table[i];
                while (current != nullptr) 
                {
                    outputFile << "���: " << tokenTypeToString(current->token.type)
                        << " | �������: " << current->token.value
                        << " | ������: " << i << endl;
                    current = current->next;
                }
            }
        }
    }

    
    ~HashT() 
    {
        for (int i = 0; i < TABLE_SIZE; ++i) 
        {
            Nodehash* current = table[i];
            while (current != nullptr) 
            {
                Nodehash* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
    }
};

