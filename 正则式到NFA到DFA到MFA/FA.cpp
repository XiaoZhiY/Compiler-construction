
#include "stdafx.h"
#include "FA.h"

void FA::print()
{
	cout << "start:";
	for (auto it = startState.begin(); it != startState.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << "end:";
	for (auto it = endState.begin();it != endState.end();it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	for (int i = 0; i < G.size(); i++)
	{
		for (int j = 0; j < G[i].size(); j++)
		{
			cout << G[i][j].u << " "
				<< G[i][j].v << " " << G[i][j].val << endl;
		}
	}
}

void FA::init()
{
	G.clear();
	vertexnum = 0;
	edgenum = 0;
	endState.clear();
	successFlag = false;
	charset.clear();
}


vector<vector<Edge>> FA::getMap()
{
	return G;
}

int FA::getVertexnum()
{
	return vertexnum;
}

int FA::getEdgenum()
{
	return edgenum;
}

set<int> FA::getStartState()
{
	return startState;
}

set<int> FA::getEndState()
{
	return endState;
}

int FA::getSuccessFlag()
{
	return successFlag;
}

set<char> FA::getCharset()
{
	return charset;
}



void NFA::parseRegex(string val)
{
	if (_parseRegex(val))
	{
		successFlag = true;
	}
	else
	{
		init();
	}
}

bool NFA::_parseRegex(string val)
{
	G.clear();
	// �ռ�
	if (0)
	{
	}
	// �ձ�
	else if (0)
	{

	}
	//������ֻ��a
	else if (0)
	{
	}
	else
	{

		startState.insert(0);
		endState.insert(1);
		vertexnum = 2;

		G.resize(2);
		G[0].push_back(Edge{ 0,1,val });

		while (true)
		{
			bool changeflag = 0;
			for (int i = 0; i < G.size(); i++)
			{
				for (int j = 0; j < G[i].size(); j++)
				{
					int state = updateNode(i, j);
					if (state == 1)
					{
						changeflag = 1;
						break;
					}
					else if (state == -1)
					{
						successFlag = false;
						return false;
					}
				}
				if (changeflag == 1)
					break;
			}

			if (changeflag == 0)
				break;
		}
	}
	updateCharset();
	successFlag = true;
	return true;
}




int NFA::updateNode(int i, int j)
{
	if (G[i][j].val.size() == 1)
		return 0;
	// R1
	// deal with A | B | C
	// a.(b|c)|(c|a)
	// �Լ������� ��ͷ���߽�βΪ'|'Ϊ�����
	int t1 = testR1(i, j);
	if (t1 == 1)
		return 1;
	else if (t1 == -1)
		return -1;
	//R2
	// deal with A . B . C
	// a.(b|c).(c|a)
	// �Լ������� ��ͷ���߽�βΪ'.'Ϊ�����
	int t2 = testR2(i, j);
	if (t2 == 1)
		return 1;
	else if (t2 == -1)
		return -1;

	//R2
	// deal with (a|b)* , (a.b)* , a*
	// �Լ������� ��ͷΪ'*'Ϊ�����
	// ���ʱ������|��.��������������ֻ����*
	int t3 = testR3(i, j);
	if (t3 == 1)
		return 1;
	else if (t3 == -1)
		return -1;

	// ʲôrule��������˵���д�
	return -1;
}


int NFA::testR1(int i, int j)
{
	string s = G[i][j].val;
	int v = G[i][j].v;
	vector<string> vec;
	// ����ͷ�᲻����|������.
	if (!preTestString(s))
	{
		// ���ش���
		return -1;
	}
	bool useR1 = false;
	int bracketCnt = 0, left = 0;
	for (int k = 0; k < s.size(); k++)
	{
		if (s[k] == '(')
			bracketCnt++;
		else if (s[k] == ')')
			bracketCnt--;
		if (s[k] == '|' && bracketCnt == 0)
		{
			useR1 = true;
			// ���� ������� (a..b)||b
			if (k - left == 0)
				return -1;
			vec.push_back(s.substr(left, k - left));
			left = k + 1;
		}
	}
	if (bracketCnt != 0)
	{
		//������ ���Ų�ƥ�� ����
		return -1;
	}

	if (useR1)
	{
		vec.push_back(s.substr(left, s.size() - left));
		G[i].erase(G[i].begin() + j);
		edgenum--;
		for (int k = 0; k < vec.size(); k++)
		{
			edgenum++;
			G[i].push_back(Edge{ i, v, removeRedundantBracket(vec[k]) });
		}
		return 1;
	}
	return 0;
}


int NFA::testR2(int i, int j)
{
	string s = G[i][j].val;
	int v = G[i][j].v;
	vector<string> vec;
	// ����ͷ�᲻����|������.
	if (!preTestString(s))
	{
		// ���ش���
		return -1;
	}
	bool useR2 = false;
	int bracketCnt = 0, left = 0;
	for (int k = 0; k < s.size(); k++)
	{
		if (s[k] == '(')
			bracketCnt++;
		else if (s[k] == ')')
			bracketCnt--;
		if (s[k] == '.' && bracketCnt == 0)
		{
			useR2 = true;
			// ���� ������� (a..b)||b
			if (k - left == 0)
				return -1;
			vec.push_back(s.substr(left, k - left));
			left = k + 1;
		}
	}
	if (bracketCnt != 0)
	{
		//������ ���Ų�ƥ�� ����
		return -1;
	}

	if (useR2)
	{
		vec.push_back(s.substr(left, s.size() - left));
		G[i].erase(G[i].begin() + j);
		edgenum--;
		int from = i;
		int to;
		for (int k = 0; k < vec.size() - 1; k++)
		{
			edgenum++;
			G.push_back(vector<Edge>{});
			to = vertexnum;
			vertexnum++;
			G[from].push_back(Edge{ from, to, removeRedundantBracket(vec[k]) });
			from = to;
		}
		G[from].push_back(Edge{ from, v, removeRedundantBracket(vec[vec.size() - 1]) });
		return 1;
	}
	return 0;
}


int NFA::testR3(int i, int j)
{
	string s = G[i][j].val;
	int v = G[i][j].v;
	// ����ͷ�᲻����|������.
	if (!preTestString(s))
	{
		// ���ش���
		return -1;
	}
	if (s[s.size() - 1] != '*')
		return -1;

	string newS = removeRedundantBracket(s.substr(0, s.size() - 1));
	G[i].erase(G[i].begin() + j);
	G.push_back(vector<Edge>{});

	// ��ʱ�� 0 ����ձ�
	G[i].push_back(Edge{ i, vertexnum, "0" });
	G[vertexnum].push_back(Edge{ vertexnum, v, "0" });
	G[vertexnum].push_back(Edge{ vertexnum, vertexnum, newS });
	vertexnum++;
	return 1;
}


bool NFA::preTestString(string s)
{
	int t = s.size() - 1;
	if (s[0] == '|' || s[t] == '|')
		return false;
	if (s[0] == '.' || s[t] == '.')
		return false;
	return true;
}

string NFA::removeRedundantBracket(string s)
{
	if (s.size() == 1)
		return s;
	if (s[0] == '(' && s[s.size() - 1] == ')')
		return s.substr(1, s.size() - 2);
	return s;
}

void NFA::updateCharset()
{
	for (int i = 0; i < G.size(); i++)
	{
		for (int j = 0; j < G[i].size(); j++)
		{
			char c = G[i][j].val[0];
			if (c != '0')
				charset.insert(c);
		}
	}
}

bool DFA::_parseNFA(NFA & obj)
{
	if (!obj.getSuccessFlag())
	{
		successFlag = false;
		return false;
	}
	FA::init();

	vector<vector<Edge>> sG = obj.getMap();
	set<int> nfaStartState = obj.getStartState();
	set<char> character = obj.getCharset();
	for (auto it = character.begin(); it != character.end(); it++)
	{
		charset.insert(*it);
	}
	
	map<set<int>, int> stateMap;
	set<int> state;
	state.clear();

	// init start state

	for (auto it = nfaStartState.begin(); it != nfaStartState.end(); it++)
	{
		// ʵ����ֻ����һ����ʼ״̬
		state.insert(*nfaStartState.begin());
		// ��ʼ���ϵĿձ߱հ�
		addState(sG, *nfaStartState.begin(), '0', state);
	}
	
	
	stateMap[state] = 0;
	startState.insert(0);
	G.push_back(vector<Edge>{});
	vertexnum++;
	
	queue<set<int>> q;
	q.push(state);
	while (!q.empty())
	{
		set<int> curState = q.front();
		q.pop();
		

		// ���Ǵ������е�ͨ����ĸ��
		for (auto it1 = character.begin(); it1 != character.end(); it1++)
		{
			set<int> newState;
			for (auto it2 = curState.begin(); it2 != curState.end(); it2++)
			{
				addState(sG, *it2, *it1, newState);
			}

			if (newState.empty())
				continue;

			if (stateMap.find(newState) == stateMap.end())
			{
				stateMap[newState] = vertexnum;
				vertexnum++;
				q.push(newState);
				G.push_back(vector<Edge>{});
			}
			G[stateMap[curState]].push_back(Edge{ stateMap[curState] , stateMap[newState],string{*it1} });
			edgenum++;
		}
	}


	// ���Ҫ�����е���ֹ״̬ �ŵ�set����ȥ
	set<int> oldEndState = obj.getEndState();
	for (auto it1 = stateMap.begin();it1 != stateMap.end();it1++)
	{
		for (auto it2 = it1->first.begin();it2 != it1->first.end();it2++)
		{
			if (oldEndState.find(*it2) != oldEndState.end())
			{
				endState.insert(it1->second);
				break;
			}
		}
	}
	successFlag = true;
	return true;

}

void DFA::addState(vector<vector<Edge>> &sG,int fromState, char edge, set<int>& stateSet)
{
	//  ��Ҫע��������ı��Ѿ�������stateSet������ �Ͳ�Ҫ�ظ�����ӣ���Ȼ����ѭ��Ŷ
	queue<int> unfinistedState;
	unfinistedState.push(fromState);
	while (!unfinistedState.empty())
	{
		int curState = unfinistedState.front();
		unfinistedState.pop();
		for (int i = 0; i < sG[curState].size(); i++)
		{
			int to = sG[curState][i].v;
			if (stateSet.find(to) != stateSet.end())
			{	
				// ����Ѿ���stateSet������
				continue;
			}
			if (sG[curState][i].val[0] == '0')
			{
				// �����bug���������� ��ΪҪ���� 0 �ߵ����
				if (edge == '0')
				{
					stateSet.insert(to);
				}
				unfinistedState.push(to);
			}
			else if (sG[curState][i].val[0] == edge && edge != '0')
			{
				stateSet.insert(to);
				// �����to��ʼͨ��'0'���ܵ�state
				addState(sG, to, '0', stateSet);
			}
				
		}
	}

}

bool MFA::_parseDFA(DFA & obj)
{
	init();

	set<char> character= obj.getCharset();
	vector<vector<Edge>> sG = obj.getMap();
	for (auto it = character.begin(); it != character.end(); it++)
	{
		charset.insert(*it);
	}
	vector<set<int>> q;
	set<int> unfinishedSet;
	set<int> dfaEndState = obj.getEndState();

	for (int i = 0; i < obj.getVertexnum(); i++)
	{
		if (dfaEndState.find(i) == dfaEndState.end())
		{
			unfinishedSet.insert(i);
		}
	}
	if (!unfinishedSet.empty())
		q.push_back(unfinishedSet);
	q.push_back(dfaEndState);
	while (true)
	{
		map<int, int> mmap;
		for (int i = 0; i < q.size(); i++)
		{
			for (auto it = q[i].begin(); it != q[i].end(); it++)
			{
				mmap[*it] = i;
			}
		}
		bool flag = false;
		for (int i = 0; i < q.size(); i++)
		{
			if (q[i].size() == 1)
				continue;
			flag = updateSet(i ,sG, q, mmap);
			if (flag == true)
				break;
		}
		if (flag == false)
			break;
	}

	
	// ����{1,2} = 0,{3,4} = 1
	map<int, int> finalSet;
	for (int i = 0; i < q.size(); i++)
	{
		for (auto it = q[i].begin(); it != q[i].end(); it++)
		{
			finalSet[*it] = i;
		}
	}

	G.resize(q.size());
	vertexnum = q.size();
	for (int i = 0; i < q.size(); i++)
	{
		if (q[i].size() == 0)
			return false;
		int node = *q[i].begin();
		for (int j = 0; j < sG[node].size(); j++)
		{
			G[i].push_back(Edge{ i,finalSet[sG[node][j].v], sG[node][j].val });
			edgenum++;
		}
	}

	set<int> dfaStartState = obj.getStartState();
	for (int i = 0; i < q.size(); i++)
	{
		for (auto it = q[i].begin(); it != q[i].end(); it++)
		{
			if (dfaStartState.find(*it) != dfaStartState.end())
			{
				startState.insert(i);
			}
			if (dfaEndState.find(*it) != dfaEndState.end())
			{
				endState.insert(i);
			}
		}
	}

	return true;
}

bool MFA::updateSet(int index, vector<vector<Edge>>& sG, vector<set<int>>& q, map<int, int>& mmap)
{
	map<map<char, int>, set<int>> res;
	for (auto it = q[index].begin(); it != q[index].end(); it++)
	{
		map<char, int> cim;
		int pp = *it;
		for (int i = 0; i < sG[pp].size(); i++)
		{
			cim[sG[pp][i].val[0]] = sG[pp][i].v;
		}
		res[cim].insert(pp);
	}
	if (res.size() == 1)
		return false;
	
	q.erase(q.begin() + index);
	for (auto it = res.begin(); it != res.end(); it++)
	{
		q.push_back(it->second);
	}
	return true;
}
