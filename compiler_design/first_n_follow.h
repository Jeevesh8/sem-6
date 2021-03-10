#include<string.h>
#include<vector>
#include<set>
#include<map>

using namespace std;

#ifndef __FIRST_N_FOLLOW_INCLUDED__
#define __FIRST_N_FOLLOW_INCLUDED__

bool is_terminal(char ch);
bool is_non_terminal(char ch);

set<char> find_terminals(vector<string>& grammar);
set<char> find_non_terminals(vector<string>& grammar);

vector<string> filter_grammar(vector<string>& grammar, char non_terminal);
map<char, set<char>> first(vector<string>& grammar);

void print_map(map<char, set<char>>& first_elems, string prefix);

set<char> first(map<char, set<char>>& firsts, string expression, set<char> terminals);
map<char, set<char>> follow(vector<string>& grammar, map<char, set<char>> firsts);

#endif