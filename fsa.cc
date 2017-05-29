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
  map<string, string>::const_iterator iter;
  string transition,isfinal;
  int index = 0;
  char input = str[0];

  // Start state : 1
  string cur_state = Change_to_string(1);

  // Loop until input string ends.
  while(str[index] != '\0'){
        // Create transition : cur_state + input
        transition = cur_state + input;
        // Find if tansition is in table.
        iter = (fsa.table).find(transition);

        if(iter != fsa.table.end())
            // When find transition. change current state.
            cur_state = iter->second;
        else
            // When can't find transition.
            return false;

        index++;
        // Reinitialize input.
        input = str[index];
  }
  // When inputs end, check current state is in accept states.
  isfinal = cur_state;

  for(int i = 0; i < fsa.accept.size(); i++){
        for(int j = 0; j < isfinal.size(); j++){
            if(fsa.accept[i] == (int)(isfinal[j]-'0'))
                return true;
        }
  }

  return false;
}

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
    // Implement this function. 
    std::vector<FSATableElement>::const_iterator elem_iter;
    std::vector<elem> storage;
    int nfa = 0;

    /* Convert string input to char input */
    // Tricky case : input - "ab"
    // Split in to "a" and "b"
    for(elem_iter = elements.begin(); elem_iter != elements.end(); elem_iter++){
        elem new_elem;

        new_elem.state = elem_iter->state;
        new_elem.next_state = elem_iter->next_state;
        // Multiple input
        if(elem_iter->str.size() > 1){
            for(int i = 0; i < elem_iter->str.size(); i++){
                char tmp = elem_iter->str[i];

                if(i == 0){
                    new_elem.input = tmp;   
                }
                else{
                    elem tmp_elem;
                    
                    tmp_elem.state = elem_iter->state;
                    tmp_elem.next_state = elem_iter->state;
                    tmp_elem.input = tmp;

                    storage.push_back(tmp_elem);
                }
            }
            storage.push_back(new_elem);
        }
        // Epsilon
        else if(elem_iter->str.empty() == 1){
            new_elem.input = 0; 

            storage.push_back(new_elem);
        }
        // Normal input
        else{
            char tmp = elem_iter->str[0];

            new_elem.input = tmp;

            storage.push_back(new_elem);
        }
    }
    
    /* Decide whether it's NFA or DFA */

    // When epsilon appears.
    for(int i = 0; i < storage.size(); i++){
        if(storage[i].input == 0){
            nfa = 1;
            break;
        }
    }
    // When one state has two or more next_state.
    for(int i = 0; i < storage.size(); i++){
        for(int j = i+1; j < storage.size(); j++){
            // Same current state, same input.
            if(storage[i].state == storage[j].state && storage[i].input == storage[j].input){
                nfa = 1;
                break;
            }
        }
        if(nfa)
            break;
    }

    /* Build the fsa */

    /* DFA */
    if(!nfa){
        string transition, next;

        // Put current state + input and next staet
        // (String) transition = current state + input
        // (String) next = next state
        for(int i = 0; i < storage.size(); i++){
            transition = Change_to_string(storage[i].state) + storage[i].input;
            next = Change_to_string(elements[i].next_state);
            fsa->table[transition] =  next;
        }

        // Put accept states.
        for(int i = 0; i < accept_states.size(); i++){
            fsa->accept.push_back(accept_states[i]);
        }

    }
    /* NFA */
    else{
        for(int i = 0; i < storage.size(); i++){
            // Initialize input list except epsilon.
            if(storage[i].input != '0')
                fsa->input_list.insert(storage[i].input);
        }
        Make_nfa_table(storage, fsa);

        // Initialize accept states.
        for(int i = 0; i < accept_states.size(); i++)
            fsa->accept.push_back(accept_states[i]);
    }

    LOG << "num_elements: " << elements.size()
        << ", accept_states: " << accept_states.size() << endl;

    if(elements.size() <= 0)
        return false;

    return true;
}
/* This function makes cluster by finding epsilons in given state.*/
std::string Check_epsilon(const std::vector<elem>& storage, int state){
    stack<int> state_stack;
    set<int> available_state;
    set<int>::iterator iter;
    string cluster;
    int top,cluster_size;

    // Initializing by given state.
    state_stack.push(state);
    available_state.insert(state);

    // Loop until finds every available states.
    while(!state_stack.empty()){
        top = state_stack.top();
        state_stack.pop();

        for(int i = 0; i < storage.size(); i++){
            // When finds epsilon.
            if(storage[i].state == top && storage[i].input == 0){
                cluster_size = available_state.size();
                // Put available states by epsilon.
                available_state.insert(storage[i].next_state);

                // When can't find every available states yet.
                // Comparing two size to avoid pushing duplicated states. : set
                if(available_state.size() != cluster_size)
                    state_stack.push(storage[i].next_state);
            }
         }
     }
     
     // Append available states to cluster.
     for(iter = available_state.begin(); iter != available_state.end(); iter++)
         cluster.append(Change_to_string(*iter));
         
     return cluster;   
}
/* This function finds next pieces when current states and inputs are given. */
std::string Check_next(const std::vector<elem>& storage, int state, char input){
    string cur_state;
    string next_cluster,next_string;
    set<int> next_set;
    set<int>::iterator iter;
    int cur_piece,next_state;

    // First, find current state's cluster.
    cur_state = Check_epsilon(storage, state);

    for(int i = 0; i < cur_state.size(); i++){
        // Split cluster in to pieces.
        cur_piece = (int)(cur_state.at(i) - '0');
        for(int j = 0; j < storage.size(); j++){
            // Find next pieces and put those in to set. 
            if(cur_piece == storage[j].state && input == storage[j].input){
                next_set.insert(storage[j].next_state);
            }
        }
    }

    for(iter = next_set.begin(); iter != next_set.end(); iter++){
        // Check if next pieces have epsilon.
        next_cluster = Check_epsilon(storage, *iter);
        // Spilit cluster in to pieces.
        for(int i = 0; i < next_cluster.size(); i++){
            next_state = (int)(next_cluster[i] - '0');
            next_set.insert(next_state);
        }
    }
    // Convert set to string
    for(iter = next_set.begin(); iter != next_set.end(); iter++){
        next_string.append(Change_to_string(*iter));
    }
    return next_string;
}
/* This function finds next states when current state and input are given. */
std::string Find_next(const std::vector<elem>& storage, std::string state, char input){
    int cur_piece,next_piece;
    string next_cluster, next_string;
    set<int> next_set;
    set<int>::iterator iter;

    // Current state can be cluster.
    // Split cluster in to pieces.
    // Find all next states by changing current states.
    for(int i = 0; i < state.size(); i++){
        cur_piece = (int)(state[i] - '0');
        next_cluster = Check_next(storage,cur_piece,input);

        // Next state can be cluster.
        // Split cluster in to pieces.
        // Then save them all in to set : set can avoid duplicated inputs/
        for(int j = 0; j < next_cluster.size(); j++){
            next_piece = (int)(next_cluster[j] - '0');
            next_set.insert(next_piece);
        }
    }
    // Convert all integers to string.
    for(iter = next_set.begin(); iter != next_set.end(); iter++)
        next_string.append(Change_to_string(*iter));

    return next_string;
}
/* This function makes NFA table */
// Find transition and next stats.
// Then put this pair(Transition + next states) in to map.
void Make_nfa_table(const std::vector<elem>& storage, FiniteStateAutomaton *fsa){
    stack<string> state_stack;
    set<char>::iterator iter;
    set<string> exist;
    // State starts with 1.
    string cur_state = Change_to_string(1);
    string transition,next_state;

    state_stack.push(cur_state);
    exist.insert(cur_state);

    while(!state_stack.empty()){
        cur_state = state_stack.top();

        state_stack.pop();
        for(iter = fsa->input_list.begin(); iter != fsa->input_list.end(); iter++){
            /* Initialize transition and next state */
            // transition = current state + input
            // next state : Using another function. -> next state can be multiple
            next_state = Find_next(storage,cur_state,*iter);
            transition = cur_state + *iter;

            // Put transition and next state in to fsa
            fsa->table[transition] =  next_state;

            // Loop until next state is not in exist and not same as current state(Recursive case)
            if(cur_state != next_state && exist.find(next_state) == exist.end()){
                state_stack.push(next_state);
                exist.insert(next_state);
            }
        }
    }
}
/* This function change integer to string. */
string Change_to_string(int num){
    string str;
    char tmp_char;

    // Change number to character.
    tmp_char = (char)num + '0';
    // Change character to string.
    str = tmp_char;

    return str;
}
