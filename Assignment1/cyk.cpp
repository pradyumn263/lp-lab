#include <iostream>
#include <map>
#include <string>
#include <set>
#include <iterator>
#include <sstream>

#define mm map<string, set<string> >
using namespace std;

int main()
{
    mm m;

    // Read the Grammar Expression and store it in map string set string
    // INPUT IS OF TYPE : " " + "->" + " ";
    int numRules;
    cout << "ENTER THE NUMBER OF RUlES: ";
    cin >> numRules;
    int t = 1;
    while (t != numRules + 1)
    {
        cout << "ENTER RULE " << t << "\n";
        string input;
        fflush(stdin);
        getline(cin, input);
        stringstream ss(input);
        int count = 0;

        string temp;
        string toInsert;
        while (getline(ss, temp, ' '))
        {
            // cout << temp << " ";
            if (count == 0)
            {
                // cout << temp << "\n";
                toInsert = temp;
                count = 1;
            }
            else if (count == 1)
            {
                count = 2;
            }
            else if (count == 2)
            {
                // cout << temp << "\n";
                stringstream sa(temp);
                string currElement;
                while (getline(sa, currElement, '|'))
                {
                    // cout << currElement << "\n";
                    m[currElement].insert(toInsert);
                }
            }
        }
        t++;
    }

    //Entering the Expression Grammar
    string word = "";
    // m["AB"].insert("S");
    // m["BC"].insert("S");
    // m["BA"].insert("A");
    // m["a"].insert("A");
    // m["CC"].insert("B");
    // m["b"].insert("B");
    // m["AB"].insert("C");
    // m["a"].insert("C");
    cout << "Enter the input String " << endl;
    fflush(stdin);
    cin >> word;
    string arr[word.length()][word.length()];
    int n = word.length();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            arr[i][j] = "";
    }
    //Base Case
    for (int i = 0; i < n; i++)
    {
        string s = "";
        s += word[i];
        for (set<string>::iterator it = m[s].begin(); it != m[s].end(); it++)
            arr[n - 1][i] += *it;
    }

    for (int i = n - 2; i >= 0; i--)
    {
        for (int j = 0; j <= i; j++)
        {
            int x1 = i + 1, x2 = n - 1, y1 = j, y2 = j + (x2 - i);
            set<string> present;
            while (x1 < n && x2 > i && y2 > j)
            {
                for (int p = 0; p < arr[x1][y1].length(); p++)
                {
                    for (int q = 0; q < arr[x2][y2].length(); q++)
                    {
                        string k = "";
                        k += arr[x1][y1][p];
                        k += arr[x2][y2][q];
                        for (auto it = m[k].begin(); it != m[k].end(); it++)
                        {
                            if (present.find(*it) == present.end())
                            {
                                present.insert(*it);
                                arr[i][j] += *it;
                            }
                        }
                    }
                }
                x1++;
                x2--;
                y2--;
            }
        }
    }
    int x;
    for (x = 0; x < arr[0][0].length(); x++)
    {
        if (arr[0][0][x] == 'S')
            break;
    }
    if (x == arr[0][0].length())
        cout << word << " is not present in the Grammar" << endl;
    else
        cout << word << " is present in the Grammar" << endl;
    return 0;
}

/*

4
S -> AB|BC
A -> BA|a
B -> CC|b
C -> AB|a
baaba

*/