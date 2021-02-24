#include<iostream>
#include<string.h>
#include<vector>
#include<algorithm>
#include<set>
#include<map>

using namespace std;

char epsilon= 'e';

set<char> find_terminals(vector<string>& grammar)
{
    set<char> terminals;
    for(auto& rule: grammar)
    {
        for(auto& ch: rule)
        {
            if(islower(ch))
                terminals.insert(ch);
        }
    }
    return terminals;
}

set<char> find_non_terminals(vector<string>& grammar)
{
    set<char> non_terminals;
    for(auto& rule: grammar)
    {
        for(auto& ch: rule)
        {
            if(isupper(ch))
                non_terminals.insert(ch);
        }
    }
    return non_terminals;
}

vector<string> filter_grammar(vector<string>& grammar, char non_terminal)
{
    vector<string> relevant_rules;

    for(auto& rule: grammar)
    {
        if(rule[0]==non_terminal)
        {
            relevant_rules.push_back(rule);
        }
    }
    return relevant_rules;
}

void print_firsts(map<char, set<char>>& first_elems)
{
    for(auto& elem: first_elems)
    {
        cout<<"First("<<elem.first<<")= { ";
        for(auto& ch: elem.second)
        {
            cout<<ch<<", ";
        }
        cout<<"}\n";
    }
}

void first(vector<string>& grammar)
{
    set<char> terminals = find_terminals(grammar);
    set<char> non_terminals = find_non_terminals(grammar);

    map<char, set<char>> first_elems;

    for(auto& ch : terminals)
    {
        first_elems[ch] = set<char>({ch}); 
        //cout<<"FIRST("<<ch<<") = "<<'{'<<ch<<"}\n";
    }

    for(auto& ch: non_terminals)
    {
        first_elems[ch] = set<char>({});
    }
    
    for(size_t j=0; j<grammar.size(); ++j)
    {
        for(auto& rule: grammar)
        {
            char ch=rule[0];

            for(size_t i=0; i<rule.length(); ++i)
            {   
                if(i<2)
                    continue;
                
                if( find(terminals.begin(), terminals.end(), rule[i]) != terminals.end() )
                {
                    first_elems[ch].insert(rule[2]);
                    break;
                }
                else
                {
                    
                    first_elems[ch].insert(first_elems[rule[i]].begin(), first_elems[rule[i]].end());
                    
                    if(first_elems[ch].find(epsilon)==first_elems[ch].end())
                    {
                        break;
                    }
                }
            }
        }       
    }

    print_firsts(first_elems);    
}

int main()
{
    vector<string> grammar;
    bool enter_rules=true;
    string st;
    while(enter_rules)
    {
        cout<<"Enter Grammar Rule: ";
        getline(cin, st);
        grammar.push_back(st);
        cout<<"Do you want to add more rules?(y/n) ";
        getline(cin, st);
        if(char(st[0])!='y')
            enter_rules=false;
    }
    first(grammar);
}