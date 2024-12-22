#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

class Node 
{
private:
    string data;
    vector<Node> children;

public:
    Node(string s) : data(s) {}

    Node& addSon(const string& s) {
        children.emplace_back(s); // ��������� ���� � ������
        return children.back();   // ���������� ������ �� ��������� ����������� ����
    }

    Node& getSon(int i) 
    {
        return children.at(i); // ���������� ������ � ��������� ����
    }

    int getChildCount() const { // ����� ��� ��������� ����� �������� �����
        return children.size();
    }

    void print(ofstream& outFile, int lvl = 0) const 
    {
        for (int i = 0; i < lvl; ++i) 
        {
            outFile << "  ";
        }
        outFile << data << endl;
        for (const auto& child : children) 
        {
            child.print(outFile, lvl + 1);
        }
    }
};

