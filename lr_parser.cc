// PL homework: hw2
// lr_parser.cc

#include <assert.h>

#include <iostream>
#include <vector>
#include <stack>

#include "lr_parser.h"

#define DISABLE_LOG false
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser) {
  for(int i = 0; i < elements.size(); i++)
      lr_parser->table.push_back(elements[i]);
  for(int i = 0; i < rules.size(); i++)
      lr_parser->rules.push_back(rules[i]);
  return true;
}

bool RunLRParser(const LRParser& lr_parser, const std::string& str) {
    stack<int> lr_stack;
    int cur = 0;
    int pop_num = 0;
    pair<LRAction, int> tmp;
    pair<LRAction, int> g_tmp;
    bool dot = false;

    lr_stack.push(0);

    while(cur != str.size()){
        tmp = Match(lr_parser.table, lr_stack.top(), str[cur]);
        if(tmp.first == INVALID)
            return false;
        else if(tmp.first == SHIFT){
            lr_stack.push(str[cur]);
            cur++;
            lr_stack.push(tmp.second);
        }
        else if(tmp.first == REDUCE){
            pop_num = lr_parser.rules.at(tmp.second-1).num_rhs;
            for(int i = 0; i < pop_num*2; i++)
                lr_stack.pop();

             g_tmp = Match(lr_parser.table, lr_stack.top(), lr_parser.rules.at(tmp.second-1).lhs_symbol);

             if(g_tmp.first == INVALID)
                 return false;

             lr_stack.push(lr_parser.rules.at(tmp.second-1).lhs_symbol);
             lr_stack.push(g_tmp.second);
         }
         else if(tmp.first == ACCEPT)
             return true;
    }

    return false;
}
pair<LRAction, int> Match(vector<LRTableElement> table, int state, int symbol){
    for(int i = 0; i < table.size(); i++){
        if(table.at(i).state == state && table.at(i).symbol == symbol){
            return make_pair(table.at(i).action, table.at(i).next_state);
        }
    }
    return make_pair(INVALID, -1);
}
