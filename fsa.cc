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
  std::vector<FSATableElement>::const_iterator elem_iter;
  std::vector<int>::const_iterator fin_iter = accept_states.begin();
  std::vector<int>::iterator duplicate;
  Cluster first_elem;
  int cur_size,next_size,add;

  /* Create first cluster. */

  // Initialize first_elem.
  first_elem.isfinal = 0;

  // Start with 1 state.
  first_elem.cur_state.push_back(1);
  // Check if it's final state.
  for(; fin_iter != accept_states.end(); fin_iter++){
    if((*fin_iter) == 1){
        first_elem.isfinal = 1;
    }
  }

  do{
    cur_size = next_size = first_elem.cur_state.size();

    for(elem_iter = elements.begin(); elem_iter != elements.end(); elem_iter++){
      if(elem_iter->state == 1 && elem_iter->str.empty() == 1){
        add = 1;
        // Check if it's duplicated.
        for(duplicate = first_elem.cur_state.begin(); duplicate != first_elem.cur_state.end(); duplicate++){
            if((*duplicate) == elem_iter->next_state){
                add = 0;
                break;
            }
        }
        if(add){
            first_elem.cur_state.push_back(elem_iter->next_state);
            next_size++;
        }

        // Check if it's final state.
        for(fin_iter = accept_states.begin(); fin_iter != accept_states.end(); fin_iter++){
          if((*fin_iter) == elem_iter->next_state){
            first_elem.isfinal = 1;
          }       
        }     
      }
    }

    for(unsigned i = 0; i < first_elem.cur_state.size(); i++){
        for(elem_iter = elements.begin(); elem_iter != elements.end(); elem_iter++){
            if(first_elem.cur_state[i] == elem_iter->state && elem_iter->str.empty() == 1){
                add = 1;
                // Check if it's duplicated.
                for(duplicate = first_elem.cur_state.begin(); duplicate != first_elem.cur_state.end(); duplicate++){
                    if((*duplicate) == elem_iter->next_state){
                        add = 0;
                        break;
                    }
                }
                if(add){
                    first_elem.cur_state.push_back(elem_iter->next_state);
                    next_size++;
                }

                // Check if it's final state.
                for(fin_iter = accept_states.begin(); fin_iter != accept_states.end(); fin_iter++){
                    if((*fin_iter) == elem_iter->next_state){
                        first_elem.isfinal = 1;
                    }       
                }
            }
        }
    }
  }while(cur_size != next_size);

  cout << first_elem.cur_state.size() << first_elem.next_state.size() << first_elem.input << first_elem.isfinal << endl;
  




/*
  while(1){
      Cluster new_elem;
      

      fsa->dfa.push_back(new_elem);
  }
*/
  LOG << "num_elements: " << elements.size()
      << ", accept_states: " << accept_states.size() << endl;
  return false;
}
