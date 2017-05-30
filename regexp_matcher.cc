// PL homework: hw2
// regexp_matcher.cc

#include <iostream>
#include "regexp_matcher.h"

using namespace std;

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
    // Implement this function. 
    int nfa = 0;
    int cur_state = 1;
    int next_state = cur_state + 1;
    int index = 0;
    vector<int> accept_states;

    /* Convert regular expression to NFA */
    // Single characters  - ex) abc
    // Any character .
    // Set of characters [] - ex) [abc]
    // OR | - ex) a|b
    // Zero or more repitition * - ex) a*
    // Group () - ex) (abc)

    while(regexp[index] != '\0'){
        elem new_elem;

        switch(regexp[index]){
            /* Any character */
            case('.') :

                break;

            /* Set of characters */
            case('[') :

                break;
            case(']') :
                index++;
                break;
            
            /* OR */
            case('|') :

                break;

            /* Zero or more repition */
            case('*') :

                break;

            /* Group */
            case('(') :

                break;
            case(')') :
                index++;
                break;

            /* Normal character input */
            default :
                new_elem.state = cur_state; 
                new_elem.next_state = next_state;
                new_elem.input = regexp[index];

                cur_state++;
                next_state++;
                index++;
                        
                regexp_matcher->storage.push_back(new_elem);

                break;
        }
    }

    accept_states.push_back(cur_state);

    /* Decide whether it's NFA or DFA */

    // When epsilon appears.
    for(int i = 0; i < regexp_matcher->storage.size(); i++){
        if(regexp_matcher->storage[i].input == 0){
            nfa = 1;
            break;
        }
    }
    // When one state has two or more next_state.
    for(int i = 0; i < regexp_matcher->storage.size(); i++){
        for(int j = i+1; j < regexp_matcher->storage.size(); j++){
            // Same current state, same input.
            if(regexp_matcher->storage[i].state == regexp_matcher->storage[j].state && regexp_matcher->storage[i].input == regexp_matcher->storage[j].input){
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
        for(int i = 0; i < regexp_matcher->storage.size(); i++){
            transition = Integer_to_string(regexp_matcher->storage[i].state) + regexp_matcher->storage[i].input;
            next = Integer_to_string(regexp_matcher->storage[i].next_state);
            regexp_matcher->fsa.table[transition] =  next;
        }

        // Put accept states.
        for(int i = 0; i < accept_states.size(); i++){
            regexp_matcher->fsa.accept.push_back(accept_states[i]);
        }

    }
    /* NFA */
    else{
        for(int i = 0; i < regexp_matcher->storage.size(); i++){
            // Initialize input list except epsilon.
            if(regexp_matcher->storage[i].input != '0')
                regexp_matcher->fsa.input_list.insert(regexp_matcher->storage[i].input);
        }
        Make_nfa_table(regexp_matcher);

        // Initialize accept states.
        for(int i = 0; i < accept_states.size(); i++)
            regexp_matcher->fsa.accept.push_back(accept_states[i]);
    }

    return true;
}

bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
  // Implement this function.
  map<string, string>::const_iterator iter;
  string transition,isfinal;
  int index = 0;
  char input = str[0];

  // Start state : 1
  string cur_state = Integer_to_string(1);

  // Loop until input string ends.
  while(str[index] != '\0'){
        // Create transition : cur_state + input
        transition = cur_state + input;
        // Find if tansition is in table.
        iter = (regexp_matcher.fsa.table).find(transition);

        if(iter != regexp_matcher.fsa.table.end())
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

  for(int i = 0; i < regexp_matcher.fsa.accept.size(); i++){
        for(int j = 0; j < isfinal.size(); j++){
            if(regexp_matcher.fsa.accept[i] == (int)(isfinal[j]-'0'))
                return true;
        }
  }
  return false;
}
/* This function makes cluster by finding epsilons in given state.*/
std::string Check_epsilon(const RegExpMatcher& regexp_matcher, int state){
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

        for(int i = 0; i < regexp_matcher.storage.size(); i++){
            // When finds epsilon.
            if(regexp_matcher.storage[i].state == top && regexp_matcher.storage[i].input == 0){
                cluster_size = available_state.size();
                // Put available states by epsilon.
                available_state.insert(regexp_matcher.storage[i].next_state);

                // When can't find every available states yet.
                // Comparing two size to avoid pushing duplicated states. : set
                if(available_state.size() != cluster_size)
                    state_stack.push(regexp_matcher.storage[i].next_state);
            }
         }
     }
     
     // Append available states to cluster.
     for(iter = available_state.begin(); iter != available_state.end(); iter++)
         cluster.append(Integer_to_string(*iter));
         
     return cluster;   
}
/* This function finds next pieces when current states and inputs are given. */
std::string Check_next(const RegExpMatcher& regexp_matcher, int state, char input){
    string cur_state;
    string next_cluster,next_string;
    set<int> next_set;
    set<int>::iterator iter;
    int cur_piece,next_state;

    // First, find current state's cluster.
    cur_state = Check_epsilon(regexp_matcher, state);

    for(int i = 0; i < cur_state.size(); i++){
        // Split cluster in to pieces.
        cur_piece = (int)(cur_state.at(i) - '0');
        for(int j = 0; j < regexp_matcher.storage.size(); j++){
            // Find next pieces and put those in to set. 
            if(cur_piece == regexp_matcher.storage[j].state && input == regexp_matcher.storage[j].input){
                next_set.insert(regexp_matcher.storage[j].next_state);
            }
        }
    }

    for(iter = next_set.begin(); iter != next_set.end(); iter++){
        // Check if next pieces have epsilon.
        next_cluster = Check_epsilon(regexp_matcher, *iter);
        // Spilit cluster in to pieces.
        for(int i = 0; i < next_cluster.size(); i++){
            next_state = (int)(next_cluster[i] - '0');
            next_set.insert(next_state);
        }
    }
    // Convert set to string
    for(iter = next_set.begin(); iter != next_set.end(); iter++){
        next_string.append(Integer_to_string(*iter));
    }
    return next_string;
}
/* This function finds next states when current state and input are given. */
std::string Find_next(const RegExpMatcher& regexp_matcher, std::string state, char input){
    int cur_piece,next_piece;
    string next_cluster, next_string;
    set<int> next_set;
    set<int>::iterator iter;

    // Current state can be cluster.
    // Split cluster in to pieces.
    // Find all next states by changing current states.
    for(int i = 0; i < state.size(); i++){
        cur_piece = (int)(state[i] - '0');
        next_cluster = Check_next(regexp_matcher,cur_piece,input);

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
        next_string.append(Integer_to_string(*iter));

    return next_string;
}
/* This function makes NFA table */
// Find transition and next stats.
// Then put this pair(Transition + next states) in to map.
void Make_nfa_table(RegExpMatcher *regexp_matcher){
    stack<string> state_stack;
    set<char>::iterator iter;
    set<string> exist;
    // State starts with 1.
    string cur_state = Integer_to_string(1);
    string transition,next_state;

    state_stack.push(cur_state);
    exist.insert(cur_state);

    while(!state_stack.empty()){
        cur_state = state_stack.top();

        state_stack.pop();
        for(iter = regexp_matcher->fsa.input_list.begin(); iter != regexp_matcher->fsa.input_list.end(); iter++){
            /* Initialize transition and next state */
            // transition = current state + input
            // next state : Using another function. -> next state can be multiple
            next_state = Find_next(*regexp_matcher,cur_state,*iter);
            transition = cur_state + *iter;

            // Put transition and next state in to fsa
            regexp_matcher->fsa.table[transition] =  next_state;

            // Loop until next state is not in exist and not same as current state(Recursive case)
            if(cur_state != next_state && exist.find(next_state) == exist.end()){
                state_stack.push(next_state);
                exist.insert(next_state);
            }
        }
    }
}
/* This function change integer to string. */
string Integer_to_string(int num){
    string str;
    char tmp_char;

    // Change number to character.
    tmp_char = (char)num + '0';
    // Change character to string.
    str = tmp_char;

    return str;
}
