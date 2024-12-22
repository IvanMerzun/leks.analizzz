#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Node.h"

class SemanticAnalyzer
{
private:
    std::ofstream outFile; // ���� ��� ������
    std::vector<std::string> errors; // ��� �������� ������

    // ����������� ��������� ������
    void processNode(const Node& node) {
        if (node.getData() == "Descriptions") {
            processDescriptions(node);
        }

        if (node.getData() == "Operators") {
            processOperators(node); // ����� ���������� ��� ����������
        }

        // ���������� ������������ ��� �������� ����
        for (int i = 0; i < node.getChildCount(); ++i) {
            processNode(node.getSon(i));
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
        //collectVariables(node.getSon(1), variables); // ���� ���� ���������� �� VarList
        if (node.getSon(1).getData() == "VarList")
        {
            collectVariables(node.getSon(1), variables);
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

        std::string exprPostfix = processExpr(node.getSon(2));  // �������� ����������� ������ ��� ���������
        string result = varName + " " + exprPostfix + " =";
        outFile << result << std::endl;
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
        processNode(root);
    }

    const std::vector<std::string>& getErrors() const {
        return errors;
    }
};

