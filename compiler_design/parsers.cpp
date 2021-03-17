#include<iostream>
#include<string.h>
#include<vector>
#include<algorithm>
#include<set>
#include<map>

#include "parsers.h"
#include "first_n_follow.cpp"

using namespace std;

map<pair<char,char>, string> generate_parsing_table(vector<string>& grammar)
{
    map<pair<char,char>, string> parsing_table;
    map<char, set<char>> firsts = first(grammar);
    map<char, set<char>> follows = follow(grammar, firsts);
    set<char> terminals = find_terminals(grammar);

    for(auto& rule: grammar)
    {
        set<char> first_alpha = first(firsts, rule.substr(2), terminals);
        for(auto& terminal : first_alpha)
        {
            if(terminal!=epsilon)
            {
                parsing_table[make_pair(rule[0], terminal)] = rule;
            }
            else
            {
                for(auto& terminal : follows[rule[0]])
                {
                    parsing_table[make_pair(rule[0], terminal)] = rule;
                }
            }
        }
    }
    return parsing_table;
}

int print_parse_table(map<pair<char,char>, string> table)
{
    cout<<"\t";
    vector<char> terminals, non_terminals;
    for(auto& entry: table)
    {
        if(find(terminals.begin(), terminals.end(), entry.first.second)==terminals.end())
        {
            cout<<entry.first.second<<"\t";
            terminals.push_back(entry.first.second);
        }
    }
    for(auto& entry: table)
    {
        char cur_non_terminal = entry.first.first;
        if(find(non_terminals.begin(), non_terminals.end(), cur_non_terminal)==non_terminals.end())
        {
            cout<<"\n"<<cur_non_terminal<<"\t";
            for(auto& terminal :  terminals)
            {
                pair<char, char> cur_pair = make_pair(cur_non_terminal, terminal);
                if(table.count(cur_pair))
                    cout<<entry.second<<"  ";
                else
                    cout<<"\t";
            }

            non_terminals.push_back(cur_non_terminal);
        }
    }
    return 0;
}