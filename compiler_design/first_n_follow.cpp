#include<iostream>
#include<string.h>
#include<vector>
#include<algorithm>
#include<set>
#include<map>

#include "first_n_follow.h"

using namespace std;

bool is_terminal(char ch)
{
    if(islower(ch) || ch==epsilon)
        return true;
    return false;
}

bool is_non_terminal(char ch)
{
    if(isupper(ch))
        return true;
    return false;
}

set<char> find_terminals(vector<string>& grammar)
{
    set<char> terminals;
    for(auto& rule: grammar)
    {
        for(auto& ch: rule)
        {
            if(is_terminal(ch))
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
            if(is_non_terminal(ch))
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

int print_map(map<char, set<char>>& elems, string prefix)
{
    for(auto& elem: elems)
    {
        cout<<prefix<<elem.first<<")= { ";
        for(auto& ch: elem.second)
        {
            cout<<ch<<", ";
        }
        cout<<"}\n";
    }
    return 0;
}


map<char, set<char>> first(vector<string>& grammar)
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

            size_t i=0;
            for(; i<rule.length(); ++i)
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
            if(i==rule.length())
            {
                first_elems[ch].insert(epsilon);
            }
        }       
    }
    return first_elems;
}


set<char> first(map<char, set<char>>& firsts, 
                string expression, 
                set<char> terminals)

{
    set<char> exp_firsts = set<char>({});
    
    for(auto& elem : expression)
    {
        if( find(terminals.begin(), terminals.end(), elem) == terminals.end() )
        {
            exp_firsts.insert(firsts[elem].begin(), firsts[elem].end());
            if(find(firsts[elem].begin(), firsts[elem].end(), epsilon)==firsts[elem].end())
            {
                return exp_firsts;
            }
        }
        else
        {
            if(elem==epsilon)
            {    
                exp_firsts.insert({epsilon});
                continue;
            }
            else if(exp_firsts.empty() || exp_firsts==set<char>({epsilon}))
            {
                exp_firsts.insert({elem});
            }
            return exp_firsts;
        }
    }
}

map<char, set<char>> follow(vector<string>& grammar, map<char, set<char>> firsts)
{
    set<char> terminals = find_terminals(grammar);
    set<char> non_terminals = find_non_terminals(grammar);

    map<char, set<char>> follow_map;

    follow_map[start_symbol] = set<char>({right_end_marker});
    
    for(size_t i=0; i<grammar.size(); ++i)
    {
        for(auto& rule: grammar)
        {
            for(size_t j=2; j<rule.size();++j)
            {
                if(is_non_terminal(rule[j]))
                {
                    
                    if(j==rule.size()-1)
                    {
                        follow_map[rule[j]].insert(follow_map[rule[0]].begin(), follow_map[rule[0]].end());
                    }
                    else
                   {
                        set<char> first_beta = first(firsts, rule.substr(j+1), terminals);
                        follow_map[rule[j]].insert(first_beta.begin(), first_beta.end());
                        follow_map[rule[j]].erase(epsilon);
                        if(find(first_beta.begin(), first_beta.end(), epsilon)!=first_beta.end())
                        {
                            follow_map[rule[j]].insert(follow_map[rule[0]].begin(), follow_map[rule[0]].end());
                        }
                    }
                }
            }
        }
    }
    return follow_map;
}

/*
//Uncomment for First
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
*/


/*
//Uncomment for Follow
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
    map<char, set<char>> firsts = first(grammar);
    map<char, set<char>> follows = follow(grammar, firsts);
    print_map(follows, "FOLLOW(");
}
*/