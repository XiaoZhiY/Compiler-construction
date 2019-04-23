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
	返回值0没有更新
	返回值1有更新
	返回值-1为错误
	*/
	int updateNode(int i, int j);

	/*
	对节点 i,j 测试使用 R1
	返回0 则表示R1不适用
	返回1 则表示成功
	返回-1 有错;
	*/
	int testR1(int i, int j);
	int testR2(int i, int j);
	int testR3(int i, int j);

	// 测试该字符串开头结尾是不是特殊字符.和|
	bool preTestString(string s);
	string removeRedundantBracket(string s);
	void updateCharset();
};


class DFA :public FA
{
public:
	bool _parseNFA(NFA &obj);
private:
	// 用于添加从fromstate 通过edge到或者'0'(空边) 到的集合
	void addState(vector<vector<Edge>> &sG, int fromState, char edge, set<int>& stateSet);
};

class MFA :public FA
{
public:
	bool _parseDFA(DFA &obj);
	bool updateSet(int index, vector<vector<Edge>> &sG, vector<set<int>> &q, map<int, int> &mmap);
};