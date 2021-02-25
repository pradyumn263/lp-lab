#include <bits/stdc++.h>
using namespace std;

vector<pair<char, string> > productions; //initializing productions as vector of pairs

void input_func()
{
	cout << "Number of productions";
	int n;
	cin >> n;

	cout << "Productions\n";

	for (int i = 1; i <= n; i++)
	{
		cout << "Production " << i << endl;

		char lhs;
		string rhs;

		cout << "LHS:";
		cin >> lhs;
		cout << "RHS:";
		cin >> rhs;

		productions.push_back(make_pair(lhs, rhs));
	}
}

void removeUselessProductions(pair<char, string> temp, vector<char> &useful_prod)
{
	for (int i = 0; i < temp.second.size(); i++)
	{
		for (int j = 0; j < productions.size(); j++)
		{
			if (productions[j].first == temp.second[i]) //if you can derive the RHS non terminal from LHS
			{
				if (find(useful_prod.begin(), useful_prod.end(), temp.second[i]) == useful_prod.end()) //if not already present
				{
					useful_prod.push_back(temp.second[i]);				   //insert
					removeUselessProductions(productions[j], useful_prod); //recursion
				}
			}
		}
	}
}
void useless_utility(vector<char> useful_prod)
{
	int i = 0;

	while (i < productions.size())
	{
		if (find(useful_prod.begin(), useful_prod.end(), productions[i].first) == useful_prod.end())
		{
			productions.erase(productions.begin() + i);
		}
		else
		{
			i++;
		}
	}

	for (i = 0; i < productions.size(); i++)
	{
		cout << productions[i].first << "->" << productions[i].second << endl;
	}
}

void removeNullProductions()
{
	vector<char> null_prod;

	int i = 0;
	int null_len;
	while (i < productions.size())
	{
		if (productions[i].second[0] == '0') //if RHS has a null value
		{
			null_prod.push_back(productions[i].first);	//push LHS into null vector
			productions.erase(productions.begin() + i); //removing it from productions
		}
		else
		{
			i++;
		}
	}

	i = 0;

	while (1)
	{
		null_len = null_prod.size();

		while (i < productions.size())
		{
			int flag = 0;
			for (int j = 0; j < productions[i].second.size(); j++)
			{
				if (find(null_prod.begin(), null_prod.end(), productions[i].second[j]) == null_prod.end()) //if the jth non terminal in RHS of productions elements is not in LHS of null vector
				{
					flag = 1;
					break;
				}
			}

			if (flag == 0) // If the whole RHS is in LHS of null vector
			{
				if (find(null_prod.begin(), null_prod.end(), productions[i].first) == null_prod.end()) //if the LHS of the found productions is not already present in null vaector
				{
					null_prod.push_back(productions[i].first);
				}
			}

			i++;
		}

		if (null_len == null_prod.size())
			break; //keep going on until there is no other productions found that can be inserted into null vector,i.e the null vector before and after is same
	}

	i = 0;
	int k = 0;

	while (i < productions.size()) //go along for all productions
	{
		string s = productions[i].second; //storing RHS of productions

		for (int j = 0; j < s.size(); j++)
		{
			if (find(null_prod.begin(), null_prod.end(), s[j]) != null_prod.end()) //if found a non terminal from RHS in null vector
			{
				string temp2;

				for (int k = 0; k < s.size(); k++) //susbtituting the non terminal found in RHS by null-S->ASA-AS|SA|S
				{
					if (k != j)
					{
						string temp3(1, s[k]);
						temp2 += temp3;
					}
				}

				pair<char, string> temp = make_pair(productions[i].first, temp2); //make a production pair

				productions.insert(productions.begin() + i + 1, temp); //insert into vector
			}
		}

		i++;
	}

	i = 0;
	while (i < productions.size())
	{
		if (productions[i].second.size() == 0) //if any empty production
		{
			productions.erase(productions.begin() + i);
		}
		else
		{
			i++;
		}
	}

	i = 0;

	while (i < productions.size())
	{
		int flag = 0;

		for (int j = i + 1; j < productions.size(); j++)
		{
			if (productions[i] == productions[j]) //if any productions repeating
			{
				flag = 1;
				break;
			}
		}

		if (flag == 1)
		{
			productions.erase(productions.begin() + i);
		}
		else
			i++;
	}

	for (i = 0; i < productions.size(); i++)
	{
		cout << productions[i].first << "->" << productions[i].second << endl;
	}
}

void removeUnitProductions(pair<char, string> temp, int index)
{
	if (temp.second.size() == 1 && isupper(temp.second[0])) //if a unit production in RHS
	{
		for (int i = 0; i < productions.size(); i++)
		{
			if (productions[i].first == temp.second[0]) //if any production has the non terminal in their LHS
			{
				if (productions[i].second.size() > 1 || !isupper(productions[i].second[0])) //and if the non terminal does not give a unit production again
				{
					productions[index].second = productions[i].second; //then replace the IN production with this
				}
				else
				{
					removeUnitProductions(productions[i], i); //recursion
					productions[index].second = productions[i].second;
				}
			}
		}
	}
}

void remove_immediate_left_recursion()
{
	for (int i = 0; i < productions.size(); i++)
	{
		if (productions[i].first == productions[i].second[0]) //If LHS matches first element of RHS
		{
			string temp2;

			for (int j = 1; j < productions[i].second.size(); j++)
			{
				temp2 += productions[i].second[j]; //copying everythinf except first element
			}

			string temp3 = productions[i + 1].second; //temp3=RHS of the next production

			string nont(1, productions[i].first); //copying the repeated nonterminal to string
			temp3 += nont;
			temp3 += "'";
			productions[i].second = temp3;

			productions[i + 1].first = nont[0];

			temp2 += nont;
			temp2 += "'"; //makingnew non terminal
			temp2 += "|0";
			productions[i + 1].second = temp2; //adding null production
		}
	}

	for (int i = 0; i < productions.size(); i++)
	{
		if (productions[i].second[productions[i].second.size() - 2] == '|')
			cout << productions[i].first << "'->" << productions[i].second << endl;
		else
			cout << productions[i].first << "->" << productions[i].second << endl;
	}
}

int main()
{
	input_func();

	cout << "Grammar after removing null productions\n"; //first should be null
	removeNullProductions();

	cout << "Grammar after removing unit productions\n"; //then unit
	for (int i = 0; i < productions.size(); i++)
	{
		removeUnitProductions(productions[i], i);
	}
	for (int i = 0; i < productions.size(); i++)
	{
		cout << productions[i].first << "->" << productions[i].second << endl;
	}

	cout << "Grammar after removing useless symbols\n"; //then uselss
	vector<char> useful_prod;
	useful_prod.push_back(productions[0].first);
	removeUselessProductions(productions[0], useful_prod);
	useless_utility(useful_prod);

	//  useful_prod.clear();
	// useful_prod.push_back(productions[0].first);
	// removeUselessProductions(productions[0], useful_prod); useless_utility(useful_prod);

	cout << "Grammar after removing immediate left recursion\n";
	remove_immediate_left_recursion();

	return 0;
}
// 5
// S AB
// A aAA
// A 0
// B bBB
// B 0

/*

	3
S -> abS | abA 
A -> cd
C -> dc

4
S -> ABCd     
A -> BC                            
B -> bB | $                            
C -> cC | $

3
S -> Aa | B | c
B -> A | bb
A -> a | bc | B

2
S -> SAd | Sa | a
A -> Ae | b

4
S -> ACE | B
A -> a 
C -> Cc | BC | b 
E -> aA | e | $


9
S ACE
S B
A a
C Cc
C BC
C b
E aA
E e
E 0
*/