// PL homework: hw1
// fsa.cc

#include <iostream>

#include "fsa.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

bool RunFSA(const FiniteStateAutomaton& fsa, const string& str) {
  // Implement this function.
  return false;
}

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
  // Implement this function. 

  for(elem_iter = elements.begin(); elem_iter != elements.end(); elem_iter++){
        if(elem_iter->str.empty() == 1){
            nfa = 1;
            break;
        }
  }

  /* DFA */
  if(!nfa){
        for(int i = 0; i < elements.size(); i++){
            if(elem_iter->str.size() != 1){
                string temp;

                for(int j = 0; j < elem_iter->str.size(); j++){
                    temp = elem_iter->str.substr(j,1);

                    fsa->states[elements[i].state][temp].insert(elements[i].next_state);
                }
            }
            else
                fsa->states[elements[i].state][elements[i].str].insert(elements[i].next_state);
        }
        for(int i = 0; i < accept_states.size(); i++){
            f->isfinal.insert(accept_states[i];
        }
  }
  /* NFA */
  else{
    // Build NFA first
    

    LOG << "num_elements: " << elements.size()
        << ", accept_states: " << accept_states.size() << endl;
    return false;
  }
}
