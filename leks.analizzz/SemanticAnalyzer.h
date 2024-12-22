#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "Node.h"

class SemanticAnalyzer
{
private:
    std::ofstream outFile; // ���� ��� ������
    string typemain, idret; 
    struct VariableInfo {
        std::string type;  // ��� ����������
    };

    // ������� �������� (��������� ���� ����������� ����������)
    std::unordered_map<std::string, VariableInfo> declaredVariables;

    // ����������� ��������� ������
    void processNode(const Node& node) {

        if (node.getData() == "Begin") {
            processBegin(node);
        }

        if (node.getData() == "Descriptions") {
            processDescriptions(node);
        }

        if (node.getData() == "Operators") {
            processOperators(node); // ����� ���������� ��� ����������
        }

        if (node.getData() == "End") {
            processEnd(node);
        }

        // ���������� ������������ ��� �������� ����
        for (int i = 0; i < node.getChildCount(); ++i) {
            processNode(node.getSon(i));
        }
    }

    void processBegin(const Node& node)
    {
        typemain = node.getSon(0).getSon(0).getData();
    }

    void processEnd(const Node& node)
    {
        idret = node.getSon(1).getSon(0).getData();

        // ���������, ���������� �� ��� ���������� � ������� ��������
        if (declaredVariables.find(idret) == declaredVariables.end()) {
            throw std::runtime_error("Variable '" + idret + "' used before declaration.");
        }

        // ���������, ��� ��� ������������ ���������� ��������� � ����� �������
        if (declaredVariables[idret].type != typemain) {
            throw std::runtime_error("Expected in return " + typemain + ", but got " + declaredVariables[idret].type + ".");
        }
    }


    // ��������� Descriptions
    void processDescriptions(const Node& node) {
        for (int i = 0; i < node.getChildCount(); ++i) {
            if (node.getSon(i).getData() == "Descr") {
                processDescr(node.getSon(i));
            }
        }
    }

    // ��������� Descr
    void processDescr(const Node& node) {
        std::string type = node.getSon(0).getSon(0).getData(); // ��� ���������� (int, float)

        std::vector<std::string> variables;
        if (node.getSon(1).getData() == "VarList")
        {
            collectVariables(node.getSon(1), variables);
        }

        // ���������, ��� ���������� ��� �� ���� ���������
        for (const auto& var : variables) {
            if (declaredVariables.find(var) != declaredVariables.end()) {
                // ������: ���������� ��� ���������
                throw std::runtime_error("Variable '" + var + "' is already declared.");
            }
            else {
                // ��������� ���������� � ������� �������� � � �����
                declaredVariables[var] = { type };
            }
        }

        // ��������� ������ ��� ����������� ������
        std::string result = type; // �������� � ���� ������
        if (variables.size() == 1)
        {
            for (const auto& var : variables) {
                result += " " + var; // ��������� ����� ����������
            }
            result += " DECL"; // ��������� ���������� � DECL
        }
        else
        {
            for (const auto& var : variables)
            {
                result += " " + var; // ��������� ����� ����������
            }
            result += " " + std::to_string(variables.size() + 1) + " DECL"; // ��������� ���������� � DECL
        }

        // ���������� ������ � ����
        outFile << result << std::endl;
    }

    // ����������� ���� ���������� �� VarList
    void collectVariables(const Node& node, std::vector<std::string>& variables)
    {
        for (int i = 0; i < node.getChildCount(); ++i)
        {
            if (node.getSon(i).getData() == "Id")
            {
                variables.push_back(node.getSon(i).getSon(0).getData());
            }
        }
    }

    void processOperators(const Node& node)
    {
        for (int i = 0; i < node.getChildCount(); ++i)
        {
            if (node.getSon(i).getData() == "Op")
            {
                processOp(node.getSon(i));
            }
        }
    }

    void processOp(const Node& node)
    {
        std::string varName = node.getSon(0).getSon(0).getData();  // �������� ��� ���������� (Id)

        // ��������� ��� ��������� ������ �� ���������
        std::string exprType = getExprType(node.getSon(2)); //�������� Expr

        // ��������� ��� ���������� ����� � ������ �� ���������
        if (declaredVariables.find(varName) == declaredVariables.end()) {
            // ������: ���������� ������������ �� ����������
            throw std::runtime_error("Variable '" + varName + "' used before declaration.");
        }

        std::string varType = declaredVariables[varName].type;

        // ���������, ��� ��� ���������� � ��������� ���������
        if (varType != exprType) {
            throw std::runtime_error("Type mismatch: variable '" + varName + "' is of type '" + varType +
                "' but expression is of type '" + exprType + "'.");
        }

        string exprPostfix = processExpr(node.getSon(2));  // �������� ����������� ������ ��� ���������
        string result = varName + " " + exprPostfix + " =";

        outFile << result << std::endl;

       
    }

    string getExprType(const Node& node) 
    {
        
        if ( node.getChildCount() == 1)
        {
            if (node.getSon(0).getSon(0).getData() == "Const")
            {
                std::string value = node.getSon(0).getSon(0).getSon(0).getData();

                // ���� � �������� ���� �����, ������� ��� float, ����� int
                if (value.find('.') != std::string::npos)
                {
                    return "float";  // ����� � ��������� ������
                }
                else {
                    return "int";  // ����� �����
                }
            }

            // ���� ��� ����������, �� ���������� � ��� �� ������� ��������
            if (node.getSon(0).getSon(0).getData() == "Id") {
                std::string varName = node.getSon(0).getSon(0).getSon(0).getData();
                if (declaredVariables.find(varName) != declaredVariables.end()) {
                    return declaredVariables[varName].type;
                }
                else {
                    throw std::runtime_error("Variable '" + varName + "' used before declaration.");
                }
            }

            if (node.getSon(0).getSon(0).getData() == "itof")
            {
                string rezu = getExprType(node.getSon(0).getSon(2));
                if (rezu == "int")
                {
                    return "float";
                }
                else
                {
                    throw std::runtime_error("Expected int in itof.");
                }
            }

            if (node.getSon(0).getSon(0).getData() == "ftoi")
            {
                string rezu = getExprType(node.getSon(0).getSon(2));
                if (rezu == "float")
                {
                    return "int";
                }
                else
                {
                    throw std::runtime_error("Expected float in ftoi.");
                }
            }

        }
        //����� ��� ������� ������� ��� ������ getExprTypeNeed, ����� ��������� ������, ����� ��������� �������� ������ ���� ����� ����� ��, ������� ���� ���� �� �����, ������ ������. 

        else if (node.getChildCount() > 1)
        {

            string currentType = getExprTypeNeed(node.getSon(0)); // �������������� ��� ������� ��������

            for (int i = 1; i < node.getChildCount(); ++i)
            {
                // ���������� ���������
                if (node.getSon(i).getData() == "+" || node.getSon(i).getData() == "-") {
                    continue;
                }

                // ������������ SimpleExpr
                if (node.getSon(i).getData() == "SimpleExpr") {
                    string operandType = getExprTypeNeed(node.getSon(i));  // ����� ������� ��� ��������� ����

                    // ��������� ������������� �����
                    if (currentType != operandType) {
                        throw std::runtime_error("Type mismatch in arithmetic expression.");
                    }
                }
            }

            // ���������� �������� ��� ���������
            return currentType;
        }
        
        
    }

    string getExprTypeNeed(const Node& node)
    {
        if (node.getSon(0).getData() == "Const")
        {
            std::string value = node.getSon(0).getSon(0).getData();

            // ���� � �������� ���� �����, ������� ��� float, ����� int
            if (value.find('.') != std::string::npos)
            {
                return "float";  // ����� � ��������� ������
            }
            else {
                return "int";  // ����� �����
            }
        }

        // ���� ��� ����������, �� ���������� � ��� �� ������� ��������
        if (node.getSon(0).getData() == "Id") {
            std::string varName = node.getSon(0).getSon(0).getData();
            if (declaredVariables.find(varName) != declaredVariables.end()) {
                return declaredVariables[varName].type;
            }
            else {
                throw std::runtime_error("Variable '" + varName + "' used before declaration.");
            }
        }

        if (node.getSon(0).getData() == "itof")
        {
            string rezu = getExprType(node.getSon(2));
            if (rezu == "int")
            {
                return "float";
            }
            else
            {
                throw std::runtime_error("Expected int in itof.");
            }
        }

        if (node.getSon(0).getData() == "ftoi")
        {
            string rezu = getExprType(node.getSon(2));
            if (rezu == "float")
            {
                return "int";
            }
            else
            {
                throw std::runtime_error("Expected float in ftoi.");
            }
        }
    }


    string processExpr(const Node& node)
    {
        string rez;

        for (int i = 0; i < node.getChildCount(); ++i)
        {
            if (node.getSon(i).getData() == "SimpleExpr")
            {
                rez += " " + processSimpleExpr(node.getSon(i));  // ������������ ��������
            }
        }

        for (int i = 0; i < node.getChildCount(); ++i)
        {
            if (node.getSon(i).getData() == "+" || node.getSon(i).getData() == "-")
            {
                rez += " " + node.getSon(i).getData();  // ��������� ��������
            }
        }

        return rez;
    }

    string processSimpleExpr(const Node& node)
    {
        string result;
        if (node.getSon(0).getData() == "Const")
        {
            result += node.getSon(0).getSon(0).getData();  // ���������
        }
        else if (node.getSon(0).getData() == "Id")
        {
            result += node.getSon(0).getSon(0).getData();  // �������������
        }
        else if (node.getSon(0).getData() == "ftoi" || node.getSon(0).getData() == "itof")
        {
            result += processFunctionCall(node) + "CALL";  // ������������ �������
        }

        return result;
    }

    string processFunctionCall(const Node& node)
    {
        string result = node.getSon(0).getData();

        result += " ";
        result += processExpr(node.getSon(2));  // ������������ ��������� ������ ������
        result += " ";

        return result;
    }



public:
    SemanticAnalyzer(const std::string& outputFileName) {
        outFile.open(outputFileName);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open output file for semantic analysis.");
        }
    }

    ~SemanticAnalyzer() {
        if (outFile.is_open()) {
            outFile.close();
        }
    }

    void analyze(const Node& root) {
        try {
            processNode(root);

            // ���� ������ ���, ���������� ����������� ������
            outFile << "Postfix notation successfully generated." << std::endl;
        }
        catch (const std::runtime_error& e) {
            // ���� ������ ��������, ������� �� � ����
            outFile << "Error: " << e.what() << std::endl;
        }
    }
};