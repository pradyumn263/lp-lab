%{
    #include <iostream>
    #include <fstream>
    #include <set>
    #include <string>
    #include <iterator>
    #include <algorithm>
    using namespace std;

    set<string> keywords;
    void wordMatched(char *s);
    void numberMatched(char *s);
    void otherMatched(char *s);
    void writeToFile(string s);
    string previouslyWritten;
%}

alpha [a-z|A-Z]
digit [0-9]

%%
{alpha}+                wordMatched(yytext);
{digit}+                numberMatched(yytext);
[ ]+                    writeToFile(" ");
\n                      writeToFile("\n");      
.                       otherMatched(yytext);
%%

void wordMatched(char *s)
{
    string word(s);
    string output;

    int k = 4;

    if(keywords.find(s) == keywords.end())
    {
        for (int i = 0; i < word.size(); i++)
        {
            output += ((((word[i] - 'a') + k)% 26) + 'a');
        }
        reverse(output.begin(), output.end());
        writeToFile(output);
        return;
    }
    else
    {
        if (word.size() <= 2)
        {
            output = word;
            reverse(output.begin(), output.end());
        }
        else
        {
            output = word.substr(0, word.size() - 2);
            output = word[word.size() - 1] + output;
            output = word[word.size() - 2] + output;
            reverse(output.begin(), output.end());
        }
        writeToFile(output);

    }
}

void numberMatched(char *s)
{
    string number(s);
    string output;

    if (number.size() <= 2)
    {
        output = number;
        reverse(output.begin(), output.end());
    }
    else
    {
        output = number;
        if (output.size() % 2 == 0)
        {
            int mid = output.size()/2;
            string firstHalf = output.substr(0, mid);
            string secondHalf = output.substr(mid, mid);

            output = secondHalf + firstHalf;

        }
        else
        {
            int mid = output.size() / 2;
            char ch = output[mid];
            string firstHalf = output.substr(0, mid);
            string secondHalf = output.substr(mid+1, mid);

            output = secondHalf + ch;
            output += firstHalf;
        }
    }

    writeToFile(output);
}

void otherMatched(char *s)
{
    string other(s);
    string output;
    output = other;
    writeToFile(output);
}

void writeToFile(string s)
{
    ofstream fout;
    fout.open("output.txt", ios_base::app);
    fout << s;
    fout.close();
    return;
}

int main()
{
    ifstream fin;

    fin.open("keywords.txt");
    string s;
    while(fin)
    {
        getline(fin, s);
        cout << s << "\n";
        keywords.insert(s);
    }    
    fin.close();

    yyin = fopen("input.txt", "r");
    yylex();

    return 0;
}