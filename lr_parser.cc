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
  
  /* Copy grammar rules and table elements. */
  // Copy table elements.
  for(int i = 0; i < elements.size(); i++)
      lr_parser->table.push_back(elements[i]);
  // Copy grammar rules.
  for(int i = 0; i < rules.size(); i++)
      lr_parser->rules.push_back(rules[i]);

  return true;
}

bool RunLRParser(const LRParser& lr_parser, const std::string& str) {
    stack<int> lr_stack;
    int index = 0;
    int pop_num = 0;
    pair<LRAction, int> next;
    pair<LRAction, int> go_to;

    // First put zero in stack.
    lr_stack.push(0);

    // Assume that input symbol size must be 1.(By test file)
    /* Loop until EOF input comes to stack */
    while(index != str.size()){
        // Find next action and state.
        next = Match(lr_parser.table,lr_stack.top(),str[index]);

        /* INVALID action */
        // If can't find proper action or state, return false/
        if(next.first == INVALID){
            return false;
        }
        /* SHIFT action */
        // If shift action, push input symbol and next state into stack.
        else if(next.first == SHIFT){
            // Push input symbol and next state.
            lr_stack.push(str[index++]);
            lr_stack.push(next.second);
        }
        /* REDUCE action */
        else if(next.first == REDUCE){
            /* Prepare for pop */
            // Be careful of zero indexing.(When stores in vector)
            // Input of rule id starts with 1.(By test file)
            pop_num = lr_parser.rules[next.second - 1].num_rhs * 2;
            
            /* Remeber there are stats between symbols. */
            // Number to be popped = number of RHS * 2(Symbols + States)
            // Then push changed symbol into stack : LHS
            // Because RHS changes to LHS!

            // Pop : RHS * 2
            for(int i = 0; i < pop_num; i++)
                lr_stack.pop();

            /* Find goto action in goto table. */
            // G[State located before the very changed symbol, changed symbol]
            go_to = Match(lr_parser.table, lr_stack.top(), lr_parser.rules[next.second-1].lhs_symbol);

            // Push : LHS
            lr_stack.push(lr_parser.rules[next.second -1].lhs_symbol);

            // If it's not goto action, return false.
            if(go_to.first != GOTO)
                 return false;

            // Push : state from goto table
            lr_stack.push(go_to.second);
         }
         /* ACCEPT action */
         else if(next.first == ACCEPT)
             return true;
    }
    return false;
}
/* This function finds action and next state */
pair<LRAction, int> Match(vector<LRTableElement> table, int state, int symbol){
    for(int i = 0; i < table.size(); i++){
        // Find action and next state
        if(table[i].state == state && table[i].symbol == symbol){
            return make_pair(table.at(i).action, table.at(i).next_state);
        }
    }
    // If can't find proper pair, ruturn INVALID.
    return make_pair(INVALID, -1);
}
