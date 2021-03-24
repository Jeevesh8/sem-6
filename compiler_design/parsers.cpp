#include<iostream>
#include<string.h>
#include<stdexcept>
#include<vector>
#include<algorithm>
#include<set>
#include<map>
#include<stack>

#include "parsers.h"
#include "first_n_follow.cpp"

using namespace std;

int check_and_insert(map<pair<char,char>, string>& parsing_table, char non_terminal, char terminal, string entry)
{
    pair<char, char> idx = make_pair(non_terminal, terminal);
    if(!parsing_table.count(idx))
    {
        parsing_table[idx]=entry;
    }
    else{
        throw domain_error(string("Grammar not suitable for generating LL-(1) parser. Double entries: ")+parsing_table[idx]+" and "+entry+
                                  " for location: ("+non_terminal+","+terminal+")");
    }
    return 0;
}

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
                check_and_insert(parsing_table, rule[0], terminal, rule);
            }
            else
            {
                for(auto& terminal : follows[rule[0]])
                {
                    check_and_insert(parsing_table, rule[0], terminal, rule);
                }
            }
        }
    }
    return parsing_table;
}

vector<string> ll1_parse(string seq, map<pair<char, char>, string> parse_table)
{
    /*
    This function performs an LL(1)-parse of the sequence "seq" using the parse table given.
    */
   seq+=right_end_marker;
   
   vector<string> seq_of_moves;
   stack<char> st;
   
   st.push(right_end_marker);
   st.push(start_symbol);
   
   char top_of_stack ;
   size_t cur_parse_pos = 0;

   while(top_of_stack!=right_end_marker)
   {
       top_of_stack = st.top();
       pair<char, char> cur_state = make_pair(top_of_stack, seq[cur_parse_pos]);
       
       if(top_of_stack==seq[cur_parse_pos])
       {
           st.pop();
           cur_parse_pos++;
       }
       
       else if(is_terminal(top_of_stack))
       {
           throw domain_error(string("Syntax Error: Can't parse the given sequence using LL-1 parser.")+
                                     "Expected "+top_of_stack+" instead of the first character encountered in: "+seq.substr(cur_parse_pos, 10));
       }
       
       else if(parse_table.count(cur_state)==0)
       {
           string s;
           for(auto& entry: parse_table)
           {
                if(entry.first.first==top_of_stack)
                {    
                    s += entry.first.second;
                    s += " ,";
                }
           }
           throw domain_error(string("Syntax Error: Can't parse the given sequence using LL-1 parser.")+
                              "Expected one of: { "+s+" } instead of the first character encounterd in: "+seq.substr(cur_parse_pos, 10));
       }

       else{
           string applied_rule = parse_table[cur_state];
           seq_of_moves.push_back(applied_rule);
           st.pop();
           
           for(size_t j=applied_rule.size()-1; j>1; j--)
           {
               st.push(applied_rule[j]);
           }
       }
   }
   return seq_of_moves;
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
    cout<<endl;
    return 0;
}