// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <map>

#include "FA.h"
using namespace std;


int main()
{
	/*
	((a|b)*|(c.d*)*).s*
	((a|b)*|(c.d*)*).s
	*/
	NFA FA;
	if (FA._parseRegex("((a|b)*|(c.d*)*).s*"))
	{
		cout << "((a|b)*|(c.d*)*).s*" << endl;
		cout << "FA:" << endl;
		FA.print();
	}
	else
	{
		cout << "error" << endl;
	}
	cout << endl;
	cout << "DFA:" << endl;
	DFA dfa;
	dfa._parseNFA(FA);
	dfa.print();
	cout << endl;
	cout << "MFA:" << endl;
	MFA mfa;
	mfa._parseDFA(dfa);

	mfa.print();
	return 0;
}

