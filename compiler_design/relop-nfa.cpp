#include<iostream>
#include<utility>
#include<vector>
#include<string>

using namespace std;

enum TOKEN_TYPE {RelOp};
enum RELOP {unk, eq, lt, gt, le, ge, ne};

struct TOKEN
{
    TOKEN_TYPE tt;
    int val;
};

pair<char*, RELOP> relop_nfa(char* ptr)
{
    /*
    This function implements the NFA for RelOps.
    If a relop actually starts at ptr, 
        it will return the relop, along with the 
        position from which to read characters for next NFA. 
    
    If the string ends at ptr,
        it returns nullptr and the unk RELOP
    
    If no relop actually starts at ptr,
        it will return the unk RELOP, and ptr, unchanged.
    */
    if(*ptr=='\0')  return make_pair(nullptr, unk);
    switch(*ptr)
    {
        case '<':
            switch(*(ptr+1))
            {
                case '=':
                    return make_pair(ptr+2, le);
                case '>':
                    return make_pair(ptr+2, ne);
                default:
                    return make_pair(ptr+1,lt);
            }

        case '>':
            switch(*(ptr+1))
            {
                case '=':
                    return make_pair(ptr+2, ge);
                default:
                    return make_pair(ptr+1, gt);
            }
        
        case '=':
            return make_pair(ptr+1, eq);
    }
    return make_pair(ptr, unk);
}

vector<TOKEN> compile(char* ptr)
{
    /*
    This function parses the string, calling all 
    NFA's and will decide which one to call when.
    Today, we only implement lexical analysis.
    */
   vector<TOKEN> v;
   while(*ptr!='\0' && ptr!=nullptr)
   {
       pair<char*, RELOP> nfa_out = relop_nfa(ptr);
       if(nfa_out.first==ptr)
       {
           TOKEN tok = {RelOp, RELOP::unk};
           v.push_back(tok);
           break;
       }
       ptr = nfa_out.first;
       TOKEN tok = {RelOp, nfa_out.second};
       v.push_back(tok);
   }
   return v;
}

/*Test function*/
int main()
{
    char arr[] = ">=3";
    vector<TOKEN> tokens = compile(arr);
    for(auto tok: tokens)
    {
        cout<<"<"+to_string(tok.tt)+","+to_string(tok.val)+">";
    }
}