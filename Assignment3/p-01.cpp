#include <iostream>
#include <map>
#include <sstream>
#include <algorithm>
#include <set>
#include <vector>
#include <iterator>

using namespace std;
#define start_symbol 'S'

map<char, set<string> > mp; // grammar rules
int number_of_tuples = 0;
set<char> non_terminals;
set<char> terminals;
void print();

void useless_elemination(char C, set<char> &reachable, vector<bool> &visited)
{
    visited[C - 'A'] = true;
    reachable.insert(C);
    for (auto i : mp[C])
    {
        for (char c : i)
        {
            if (c >= 'A' && c <= 'Z' && !visited[c - 'A'])
            {
                if (reachable.find(c) == reachable.end())
                {
                    useless_elemination(c, reachable, visited);
                }
            }
        }
    }
}

void null_prod(map<char, set<string> > &temp, vector<char> &nullvars)
{
    for (auto i : mp)
    {
        for (string j : i.second)
        {
            for (int k = 0; k < j.length(); k++)
            {
                char c = j[k];
                if (find(nullvars.begin(), nullvars.end(), c) != nullvars.end())
                {
                    string s = j;
                    s.erase(s.begin() + k);
                    if (s.length() > 0)
                    {
                        temp[i.first].insert(s);
                    }
                    if (s.length() == 1)
                    {
                        if (find(nullvars.begin(), nullvars.end(), s[0]) != nullvars.end())
                        {
                            nullvars.push_back(i.first);
                        }
                    }
                }
            }
        }
    }
}

void eliminate_null()
{
    vector<char> nullvars;
    string s = "$";

    map<char, set<string> > dup = mp;
    for (auto i : mp)
    {
        if (i.second.find(s) != i.second.end())
        {
            nullvars.push_back(i.first);
            dup[i.first].erase(s);
        }
    }
    mp = dup;

    map<char, set<string> > temp;
    int prev = 0;
    for (auto i : mp)
    {
        prev += i.second.size();
    }

    while (1)
    {
        null_prod(temp, nullvars);
        for (auto i : temp)
        {
            for (auto j : i.second)
            {
                mp[i.first].insert(j);
            }
        }
        int sum = 0;
        for (auto i : mp)
        {
            sum += i.second.size();
        }
        if (sum == prev)
            break;
        prev = sum;
    }
}

void unit_prod(char C, vector<string> &add, vector<bool> &visited)
{
    visited[C - 'A'] = true;
    for (string i : mp[C])
    {
        if (i.length() == 1 && i[0] >= 'A' && i[0] <= 'Z' && !visited[i[0] - 'A'])
        {
            unit_prod(i[0], add, visited);
        }
        else
        {
            string s = i;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            if (s == i)
                add.push_back(i);
        }
    }
}

void eliminate_unit()
{
    map<char, set<string> > temp;
    for (auto i : mp)
    {
        for (string j : i.second)
        {
            if (j.length() == 1 && j[0] >= 'A' && j[0] <= 'Z')
            {
                continue;
            }
            temp[i.first].insert(j);
        }
    }

    vector<string> add;
    vector<bool> visited(26, false);
    unit_prod(start_symbol, add, visited);
    for (auto k : add)
    {
        temp[start_symbol].insert(k);
    }

    for (auto i : non_terminals)
    {
        if (i == start_symbol)
            continue;
        for (int j = 0; j < 26; j++)
            visited[j] = false;
        add.clear();
        unit_prod(i, add, visited);
        for (auto k : add)
        {
            temp[i].insert(k);
        }
    }

    mp = temp;
}

void eliminate_immediate_left_recur()
{
    map<char, set<string> > temp;
    for (auto i : mp)
    {
        vector<string> f, s;
        for (auto j : i.second)
        {
            if (j[0] == i.first)
            {
                s.push_back(j.substr(1, j.length() - 1));
            }
            else
            {
                f.push_back(j);
            }
        }
        if (s.size() == 0)
        {
            temp[i.first] = mp[i.first];
            continue;
        }
        char newSymbol;
        for (int k = 25; k >= 0; k--)
        {
            if (non_terminals.find(k + 'A') == non_terminals.end())
            {
                non_terminals.insert(k + 'A');
                newSymbol = k + 'A';
                break;
            }
        }

        for (auto j : f)
        {
            string str = j;
            str += newSymbol;
            temp[i.first].insert(str);
        }
        for (auto j : s)
        {
            string str = j;
            str += newSymbol;
            temp[newSymbol].insert(str);
        }
    }
    mp = temp;
}

void remove(set<char> &reachable)
{
    map<char, set<string> > dup = mp;
    for (auto i : mp)
    {
        if (reachable.find(i.first) == reachable.end())
            dup.erase(i.first);
    }
    mp = dup;
}

void print()
{
    for (auto i : mp)
    {
        if (i.second.size() == 0)
            continue;
        cout << i.first << " : ";
        for (auto j : i.second)
        {
            cout << j << ", ";
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    string s, t;
    getline(cin, s);
    number_of_tuples = stoi(s);
    for (int i = 0; i < number_of_tuples; i++)
    {
        getline(cin, s);
        stringstream x(s);
        bool m = false;
        char lhs;
        while (x >> t)
        {
            if (!m)
            {
                non_terminals.insert(t[0]);
                lhs = t[0];
                m = true;
                continue;
            }
            if (t == "->")
                continue;
            if (t == "|")
                continue;
            mp[lhs].insert(t);
            for (char c : t)
            {
                if (c >= 'A' && c <= 'Z')
                    non_terminals.insert(c);
                else if (c >= 'a' && c <= 'z')
                    terminals.insert(c);
            }
        }
    }

    cout << "Input has been taken successfully \nHere are the Grammar Rules received: \n";
    print();

    cout << "After eliminating null/lambda productions, here are the rules: \n";
    eliminate_null();
    print();

    cout << "After eliminating unit productions, here are the rules: \n";
    eliminate_unit();
    print();

    cout << "After eliminating useless productions, here are the rules \n";
    set<char> reachable;
    vector<bool> visited(26, false);
    useless_elemination(start_symbol, reachable, visited);
    remove(reachable);
    print();

    cout << "After eliminating left recursion: \n";
    eliminate_immediate_left_recur();
    print();
}

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

6
S -> XY
X -> a
Y -> Z | b
Z -> M
M -> N
N -> a

3
S -> ASA | aB | b
A -> B
B -> b | $

*/