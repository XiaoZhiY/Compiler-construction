#pragma once

#ifndef _FA_H
#define _FA_H
#endif // !_FA_H


#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <queue>
using namespace std;

struct Edge
{
	int u, v;
	string val;
};


class FA
{
public:
	FA(int e = 0, int v = 0)
	{
		vertexnum = v;
		edgenum = e;
	}
	void init();
	void print();
	vector<vector<Edge>> getMap();
	int getVertexnum();
	int getEdgenum();
	set<int> getStartState();
	set<int> getEndState();
	int getSuccessFlag();
	set<char> getCharset();
protected:
	vector<vector<Edge>> G;
	int vertexnum;
	int edgenum;
	set<int> startState;
	set<int> endState;
	set<char> charset;
	int successFlag;
};

class NFA :public FA
{
public:
	NFA(int e = 0, int v = 0) :FA(e, v) {}
	void parseRegex(string val);
	bool _parseRegex(string val);




private:
	/*
	����ֵ0û�и���
	����ֵ1�и���
	����ֵ-1Ϊ����
	*/
	int updateNode(int i, int j);

	/*
	�Խڵ� i,j ����ʹ�� R1
	����0 ���ʾR1������
	����1 ���ʾ�ɹ�
	����-1 �д�;
	*/
	int testR1(int i, int j);
	int testR2(int i, int j);
	int testR3(int i, int j);

	// ���Ը��ַ�����ͷ��β�ǲ��������ַ�.��|
	bool preTestString(string s);
	string removeRedundantBracket(string s);
	void updateCharset();
};


class DFA :public FA
{
public:
	bool _parseNFA(NFA &obj);
private:
	// ������Ӵ�fromstate ͨ��edge������'0'(�ձ�) ���ļ���
	void addState(vector<vector<Edge>> &sG, int fromState, char edge, set<int>& stateSet);
};

class MFA :public FA
{
public:
	bool _parseDFA(DFA &obj);
	bool updateSet(int index, vector<vector<Edge>> &sG, vector<set<int>> &q, map<int, int> &mmap);
};