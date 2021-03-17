#include<iostream>
#include<string.h>
#include<vector>
#include<algorithm>
#include<set>
#include<map>

using namespace std;

map<pair<char,char>, string> generate_parsing_table(vector<string>& grammar);
int print_parse_table(map<pair<char,char>, string> table);