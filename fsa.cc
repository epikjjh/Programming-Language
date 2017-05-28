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
  set<int> state;
  bool accept;

  if(str.size() == 1){

      state.insert(fsa->states[1][str]);

  }
  else if(str.size() > 1){


  }

}

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
    // Implement this function. 

    for(int i = 0; i < elements.size(); i++){
        if(elements[i].str.size() > 1){
            string temp;

            for(int j = 0; j < elements[j].str.size(); j++){
                temp = elements[i].str.substr(j,1);

                fsa->states[elements[i].state][temp].insert(elements[i].next_state);
            }
        }
        else
            fsa->states[elements[i].state][elements[i].str].insert(elements[i].next_state);
    }
    for(int i = 0; i < accept_states.size(); i++){
        fsa->isfinal.insert(accept_states[i]);
    }

    LOG << "num_elements: " << elements.size()
        << ", accept_states: " << accept_states.size() << endl;
    if(elements.size() <= 0)
        return false;
    return true;
}
