#include<iostream>
#include<string.h>
#include<vector>
#include<algorithm>
#include<set>
#include<map>

#include "../parsers.h"
#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>

using namespace std;


BOOST_AUTO_TEST_CASE( parse_table_generation )
{
    vector<string> grammar = {"S=AB", "A=aC", "B=bC", "C=e"};
    map<pair<char,char>, string> parse_table = {{make_pair('S', 'a'), "S=AB"},
                                                {make_pair('A', 'a'), "A=aC"},
                                                {make_pair('B', 'b'), "B=bC"},
                                                {make_pair('C', 'b'), "C=e"},
                                                {make_pair('C', '$'), "C=e"},};
    map<pair<char, char>, string> generated_parse_table = generate_parsing_table(grammar);
    BOOST_TEST(generated_parse_table==parse_table);
    BOOST_TEST(print_parse_table(generated_parse_table)==0);
}

BOOST_AUTO_TEST_CASE( parse_the_sequence )
{
    vector<string> grammar = {"S=AB", "A=+i", "B=i"};
    map<pair<char,char>, string> parse_table = {{make_pair('S', '+'), "S=AB"},
                                                {make_pair('A', '+'), "A=+i"},
                                                {make_pair('B', 'i'), "B=i"},};
    map<pair<char, char>, string> generated_parse_table = generate_parsing_table(grammar);
    BOOST_TEST(generated_parse_table==parse_table);
    BOOST_TEST(print_parse_table(generated_parse_table)==0);
    
    BOOST_TEST( ll1_parse("+ii", parse_table)==vector<string>({"S=AB", "A=+i", "B=i"}) );

    vector<string> seq_of_moves = ll1_parse("+ii", parse_table);
    
    for(auto& move: seq_of_moves)
    {
        cout<<move<<endl;
    }
}
