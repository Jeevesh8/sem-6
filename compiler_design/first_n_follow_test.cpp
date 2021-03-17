#include<iostream>
#include<string.h>
#include<vector>
#include<algorithm>
#include<set>
#include<map>

#include "first_n_follow.cpp"
#define BOOST_TEST_MODULE MyTest
#include <boost/test/included/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE( my_test )
{
    set<char> sample = {'b','c','d'};
    map<char, set<char>> sample_map;
    sample_map['a'] = sample;
    BOOST_TEST(print_map(sample_map)==0);
}

