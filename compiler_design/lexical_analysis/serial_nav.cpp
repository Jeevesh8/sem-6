#include<iostream>
#include<utility>
#include<vector>
#include<string>
#include<stdio.h>
#include<ctype.h>

using namespace std;

enum TOKEN_TYPE {UNK, IF, WHILE, Variable, NUM, RelOp, Parentheses};
enum RELOP {unk_relop, eq, lt, gt, le, ge, ne};
enum PAREN {unk_paren, OPEN, CLOSE};

struct TOKEN
{
    TOKEN_TYPE tt;
    int val;
    char* ptr;

    TOKEN(): ptr(nullptr), val(0), tt(UNK) {}
    TOKEN(TOKEN_TYPE t, int v) : tt(t), val(v), ptr(nullptr) {}
    TOKEN(TOKEN_TYPE t) : tt(t), val(0), ptr(nullptr) {}
    TOKEN(TOKEN_TYPE t, char* p) : tt(t), ptr(p), val(0)     {}
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
    if(*ptr=='\0')  return make_pair(nullptr, unk_relop);
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
            switch(*(ptr+1))
            {
                case '=':
                    return make_pair(ptr+2, eq);
                default:
                    return make_pair(ptr+1, unk_relop);
            }
            
    }
    return make_pair(ptr, unk_relop);
}

char* run_relop_nfa(char* ptr, vector<TOKEN>& v)
{
    while(true)
    {
       pair<char*, RELOP> nfa_out = relop_nfa(ptr);
       if(nfa_out.first==ptr|| *ptr=='\0' || ptr==nullptr)
       {
           TOKEN tok = {RelOp, RELOP::unk_relop};
           v.push_back(tok);
           break;
       }
       ptr = nfa_out.first;
       TOKEN tok = {RelOp, nfa_out.second};
       v.push_back(tok);
    }
    return ptr;
}

pair<char*, char*> num_nfa(char* ptr)
{
    char* init_ptr = ptr;
    
    while(isdigit(*ptr))
    {   
        ptr+=1;
    }
    
    if(ptr==init_ptr)
    {
        return make_pair(ptr, nullptr);
    }

    char* table_entry = new char(ptr-init_ptr+1);
    copy(init_ptr, ptr, table_entry);
    table_entry[ptr-init_ptr] = '\0';
    return make_pair(ptr, table_entry);
}

char* run_num_nfa(char* ptr, vector<TOKEN>& v)
{
    while(true)
    {
        pair<char*, char*> nfa_out = num_nfa(ptr);
        TOKEN tok = {NUM, nfa_out.second};
        v.push_back(tok);
        if(ptr==nfa_out.first || *ptr=='\0' || ptr==nullptr)
            break;
        ptr = nfa_out.first;
    }
    return ptr;
}

pair<char*, char*> variable_nfa(char* ptr)
{
    char* init_ptr = ptr;
    if(!isalpha(*ptr) && *ptr!='_')
    {
        return make_pair(ptr, nullptr);
    }
    while(isalnum(*ptr))
    {
        ptr+=1;
    }
    char* var_name = new char(ptr-init_ptr+1);
    copy(init_ptr, ptr, var_name);
    var_name[ptr-init_ptr] = '\0';
    return make_pair(ptr, var_name);
}

char* run_var_nfa(char* ptr, vector<TOKEN>& v)
{
    while(true)
    {
        pair<char*, char*> nfa_out = variable_nfa(ptr);
        TOKEN tok = {Variable, nfa_out.second};
        v.push_back(tok);
        if(ptr==nfa_out.first || *ptr=='\0' || ptr==nullptr)
            break;
        ptr = nfa_out.first;
    }
    return ptr;
}

pair<char*, TOKEN_TYPE> keyword_nfa(char* ptr)
{
    char* state = ptr;
    switch(*state)
    {
        case 'i':
            state+=1;
            switch(*state)
            {
                case 'f':
                    state+=1;
                    switch(*state)
                    {
                        default:
                            if(!isalnum(*state))
                                return make_pair(state-1, IF);
                            else
                                return make_pair(ptr, UNK);
                    }
                default:
                    return make_pair(ptr, UNK);
            }
        
        case 'w':
            state+=1;
            switch(*state)
            {
                case 'h':
                    state+=1;
                    switch(*state)
                    {
                        case 'i':
                            state+=1;
                            switch(*state)
                            {
                                case 'l':
                                    state+=1;
                                    switch(*state)
                                    {
                                        case 'e':
                                            state+=1;
                                            switch(*state)
                                            {
                                                default:
                                                    if(!isalnum(*state) && *state!='_')
                                                        return make_pair(state, WHILE);
                                                    else
                                                        return make_pair(ptr, UNK);
                                            }
                                        default:
                                            return make_pair(ptr, UNK);                                    
                                    }
                                default:
                                    return make_pair(ptr, UNK);
                            }
                        default:
                            return make_pair(ptr, UNK);            
                    }
                default:
                    return make_pair(ptr, UNK);    
            }
        
        default:
            return make_pair(ptr, UNK);
    }
}

char* run_keyword_nfa(char* ptr, vector<TOKEN>& v)
{
    while(true)
    {
        pair<char*, TOKEN_TYPE> nfa_out = keyword_nfa(ptr);
        TOKEN tok = {nfa_out.second};
        v.push_back(tok);
        if(ptr==nfa_out.first || *ptr=='\0' || ptr==nullptr)
            break;
        ptr = nfa_out.first;
    }
    return ptr;
}

pair<char*, PAREN> parentheses_nfa(char* ptr)
{
    switch(*ptr)
    {
        case '(':
            return make_pair(ptr+1, OPEN);
        case ')':
            return make_pair(ptr+1, CLOSE);
        default:
            return make_pair(ptr, unk_paren);
    }
}

char* run_parentheses_nfa(char* ptr, vector<TOKEN>& v)
{
    while(true)
    {
        pair<char*, PAREN> nfa_out = parentheses_nfa(ptr);
        TOKEN tok = {Parentheses, nfa_out.second};
        v.push_back(tok);
        if(ptr==nfa_out.first || *ptr=='\0' || ptr==nullptr)
            break;
        ptr = nfa_out.first;
    }
    return ptr;
}

vector<TOKEN> compile(char* ptr)
{
    /*
    This function parses the string, calling all 
    NFA's and will decide which one to call when.
    Today, we only implement lexical analysis.
    */
   vector<TOKEN> v;
   char* new_ptr;

   while(*ptr!='\0' && ptr!=nullptr)
   {
        new_ptr = run_keyword_nfa(ptr, v);
        v.pop_back();                                           //Remove <unk> token put in by run_keyword_nfa
        
        if(new_ptr==ptr)
        {
            new_ptr = run_var_nfa(ptr, v);
            v.pop_back();                                           //Remove <unk> token put in by run_var_nfa

            if(new_ptr==ptr)
            {                
                new_ptr = run_num_nfa(ptr, v);
                v.pop_back();                                           //Remove <unk> token put in by run_num_nfa
                
                if(new_ptr==ptr)
                {
                    new_ptr = run_relop_nfa(ptr, v);
                    v.pop_back();                                           //Remove <unk> token put in by run_relop_nfa
                    
                    if(new_ptr==ptr)
                    {
                        new_ptr = run_parentheses_nfa(ptr, v);
                        v.pop_back();                                       //Remove <unk> token put in by run_parentheses_nfa
                        
                        if(new_ptr==ptr)
                        {
                            //Show error or call next NFA
                            cout<<"Lexical Error : Can't recognise token beginnning from "<<*ptr;
                            exit(1);
                        }
                    }
                }
            }
        }
        ptr = new_ptr ;
   }
   return v;
}

/*Test function*/
int main()
{
    char arr[] = "while(abc==32)";
    vector<TOKEN> tokens = compile(arr);
    for(auto tok: tokens)
    {
        if(tok.ptr!=nullptr)
        {   
            cout<<"<"+to_string(tok.tt)+",";
            cout<<tok.ptr<<">";
        }
        else
            cout<<"<"+to_string(tok.tt)+","+to_string(tok.val)+">";
    }
}