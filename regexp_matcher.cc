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
    set<char> input_list;
    set<char>::iterator input_iter;
    string input;

    input.clear();

    /* Convert regular expression to NFA */
    // Single characters  - ex) abc
    // Any character .
    // Set of characters [] - ex) [abc]
    // OR | - ex) a|b
    // Zero or more repitition * - ex) a*
    // Group () - ex) (abc)

    while(regexp[index] != '\0'){
        elem new_elem;

        // Reset input
        if(input.size() == 0)
            input = regexp[index];

        switch(regexp[index]){
            /* Any character */
            case('.') :
                new_elem.state = cur_state;
                new_elem.next_state = next_state;
                new_elem.input = ".";

                index++;

                break;

            /* Set of characters */
            case('[') :
                index++;

                while(regexp[index] != ']'){
                    new_elem.state = cur_state;
                    new_elem.next_state = next_state;
                    new_elem.input = regexp[index];
                    
                    index++;

                    regexp_matcher->storage.push_back(new_elem);
                }
                cur_state++;
                next_state++;

                break;
            case(']') :
                index++;
                break;
            
            /* OR */
            case('|') :
                Check_or(regexp,regexp_matcher,&cur_state,&next_state,&index);

                input.clear();
                break;

            /* Zero or more repition */
            case('*') :
                Check_star(regexp,regexp_matcher,&cur_state,&next_state,&index,&accept_states);
                input.clear();
                break;

            /* Group */
            case('(') :
                index++;

                while(regexp[index] != ')'){
                    input += regexp[index];

                    index++;
                }

                break;
            case(')') :
                index++;
                break;

            /* Normal character input */
            default :
                new_elem.state = cur_state; 
                new_elem.next_state = next_state;
                new_elem.input = input;

                cur_state++;
                next_state++;
                index++;
                       
                // Store 
                regexp_matcher->storage.push_back(new_elem);
                
                // Save input in to input list
                if(input.size() == 1)
                    input_list.insert(input[0]);

                input.clear();

                break;
        }
    }

    accept_states.push_back(cur_state);

    /* Setting inputs for "." case*/
    for(int i = 0; i < regexp_matcher->storage.size(); i++){
        if(regexp_matcher->storage[i].input == "."){
            // Erase "."
            regexp_matcher->storage[i].input.clear();

            for(input_iter = input_list.begin(); input_iter != input_list.end(); input_iter++){
                string tmp;

                tmp = *input_iter;
                regexp_matcher->storage[i].input.append(tmp);
            }
        }
    }

    /* Convert string input to char input */
    // Tricky case : input - "ab"
    // Split in to "a" and "b"
    for(int i = 0; i < regexp_matcher->storage.size(); i++){
        elem_char new_elem;

        new_elem.state = regexp_matcher->storage[i].state;
        new_elem.next_state = regexp_matcher->storage[i].next_state;
        // Multiple input
        if(regexp_matcher->storage[i].input.size() > 1){
            for(int j = 0; i < regexp_matcher->storage[i].input.size(); j++){
                char tmp = regexp_matcher->storage[i].input[j];

                if(j == 0){
                    new_elem.input = tmp;   
                }
                else{
                    elem_char tmp_elem;
                    
                    tmp_elem.state = regexp_matcher->storage[i].state;
                    tmp_elem.next_state = regexp_matcher->storage[i].next_state;
                    tmp_elem.input = tmp;

                    regexp_matcher->garage.push_back(tmp_elem);
                }
            }
            regexp_matcher->garage.push_back(new_elem);
        }
        // Epsilon
        else if(regexp_matcher->storage[i].input.empty() == 1){
            new_elem.input = 0; 

            regexp_matcher->garage.push_back(new_elem);
        }
        // Normal input
        else{
            char tmp = regexp_matcher->storage[i].input[0];

            new_elem.input = tmp;

            regexp_matcher->garage.push_back(new_elem);
        }
    }

    /* Decide whether it's NFA or DFA */

    // When epsilon appears.
    for(int i = 0; i < regexp_matcher->garage.size(); i++){
        if(regexp_matcher->garage[i].input == 0){
            nfa = 1;
            break;
        }
    }
    // When one state has two or more next_state.
    for(int i = 0; i < regexp_matcher->garage.size(); i++){
        for(int j = i+1; j < regexp_matcher->garage.size(); j++){
            // Same current state, same input.
            if(regexp_matcher->garage[i].state == regexp_matcher->garage[j].state && regexp_matcher->garage[i].input == regexp_matcher->garage[j].input){
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
        for(int i = 0; i < regexp_matcher->garage.size(); i++){
            transition = Integer_to_string(regexp_matcher->garage[i].state) + regexp_matcher->garage[i].input;
            next = Integer_to_string(regexp_matcher->garage[i].next_state);
            regexp_matcher->fsa.table[transition] =  next;
        }

        // Put accept states.
        for(int i = 0; i < accept_states.size(); i++){
            regexp_matcher->fsa.accept.push_back(accept_states[i]);
        }

    }
    /* NFA */
    else{
        for(int i = 0; i < regexp_matcher->garage.size(); i++){
            // Initialize input list except epsilon.
            if(regexp_matcher->garage[i].input != '0')
                regexp_matcher->fsa.input_list.insert(regexp_matcher->garage[i].input);
        }

        // Initialize accept states.
        for(int i = 0; i < accept_states.size(); i++)
            regexp_matcher->fsa.accept.push_back(accept_states[i]);

        // Make NFA table.
        Make_nfa_table(regexp_matcher);
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
    int top;

    // Initializing by given state.
    state_stack.push(state);
    available_state.insert(state);

    // Loop until finds every available states.
    while(!state_stack.empty()){
        top = state_stack.top();
        state_stack.pop();

        for(int i = 0; i < regexp_matcher.garage.size(); i++){
            // When finds epsilon.
            if(regexp_matcher.garage[i].state == top && regexp_matcher.garage[i].input == 0){
                // Put available states by epsilon.
                // When can't find every available states yet.
                // Comparing two size to avoid pushing duplicated states. : set
                if(available_state.insert(regexp_matcher.garage[i].next_state).second)
                    state_stack.push(regexp_matcher.garage[i].next_state);
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
        cur_piece = (int)(cur_state[i] - '0');
        for(int j = 0; j < regexp_matcher.garage.size(); j++){
            // Find next pieces and put those in to set. 
            if(cur_piece == regexp_matcher.garage[j].state && input == regexp_matcher.garage[j].input){
                next_set.insert(regexp_matcher.garage[j].next_state);
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
            if(next_state.empty())
                continue;
            transition = cur_state + *iter;

            // Put transition and next state in to fsa
            regexp_matcher->fsa.table[transition] =  next_state;

            // Loop until next state is not in exist and not same as current state(Recursive case)
            if(cur_state != next_state && exist.insert(next_state).second == true){
                state_stack.push(next_state);
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
void Check_or(const char *regexp, RegExpMatcher *regexp_matcher, int *cur_state, int *next_state, int *index){
    string save_before;

    // Transform former element.
    for(int i = 0; i < regexp_matcher->storage.size(); i++){
        if(regexp_matcher->storage[i].next_state == (*cur_state)){
            elem before_epsilon;

            // Create element(Before epsilon)
            before_epsilon.state = regexp_matcher->storage[i].state;
            before_epsilon.input = "";
            before_epsilon.next_state = (*next_state) + 1;

            // Store
            regexp_matcher->storage.push_back(before_epsilon);

            // Save elements' input.
            save_before = regexp_matcher->storage[i].input;
            regexp_matcher->storage[i].input = "";
            break;
        }
    }
    // Create element(Before |)
    elem before;
                
    before.state = (*cur_state);
    before.next_state = (*next_state);
    before.input = save_before;

    // Store
    regexp_matcher->storage.push_back(before);

    (*cur_state) += 2;
    (*next_state) += 2;

    // Create element(After |)
    elem after;

    after.state = (*cur_state);
    after.next_state = (*next_state);
    after.input = regexp[(*index)+1];

    // Store
    regexp_matcher->storage.push_back(after);

    (*cur_state)++;
    (*next_state)++;

    // Create element(After epsilon 1)
    elem after_epsilon_1;

    after_epsilon_1.state = (*cur_state);
    after_epsilon_1.next_state = (*next_state);
    after_epsilon_1.input = "";

    // Store
    regexp_matcher->storage.push_back(after_epsilon_1);

    // Create element(After epsilon 2)
    elem after_epsilon_2;

    after_epsilon_2.state = before.next_state;
    after_epsilon_2.next_state = (*next_state);
    after_epsilon_2.input = "";

    // Store
    regexp_matcher->storage.push_back(after_epsilon_2);

    (*cur_state)++;
    (*next_state)++;

    (*index) += 2;
}
void Check_star(const char *regexp, RegExpMatcher *regexp_matcher, int *cur_state, int *next_state, int *index, vector<int> *accept_states){
    string save_before;
    int jump_after_cur,jump_after_next,jump_before_cur,jump_before_next;
    set<int> finals;
    set<int>::iterator iter;

    // Transform former element.
    for(int i = 0; i < regexp_matcher->storage.size(); i++){
        if(regexp_matcher->storage[i].next_state == (*cur_state)){
            // Save elements' input.
            save_before = regexp_matcher->storage[i].input;
            // Save jumping epsilon's sate(jump after)
            jump_after_next = regexp_matcher->storage[i].state;
            regexp_matcher->storage[i].input = "";
            // Save accept state
            finals.insert(regexp_matcher->storage[i].state);
            break;
        }
    }
    // Create element(before *)
    elem before;
                
    before.state = (*cur_state);
    before.next_state = (*next_state);
    before.input = save_before;
    // Save accept state
    finals.insert(before.state);

    // Store
    regexp_matcher->storage.push_back(before);

    // Save jumping epsilon's state(jump before)
    jump_before_next = (*cur_state);

    (*cur_state)++;
    (*next_state)++;

    // Create element(after epsilon)
    elem after;

    after.state = (*cur_state);
    after.next_state = (*next_state);
    after.input = "";
    // Save accept state
    finals.insert(after.state);
    finals.insert(after.next_state);

    // Store
    regexp_matcher->storage.push_back(after);

    // Save jumping epsilon's state(jump before)
    jump_before_next = (*next_state);
    // Save jumping epsilon's state(jump after)
    jump_after_cur = (*next_state);

    // Create element(jump after epsilon)
    elem jump_after;

    jump_after.state = jump_after_cur;
    jump_after.next_state = jump_after_next;
    jump_after.input = "";

    // Create element(jump before epsilon)
    elem jump_before;
    
    jump_before.state = jump_before_cur;
    jump_before.next_state = jump_before_next;
    jump_before.input = "";

    // Store
    regexp_matcher->storage.push_back(jump_after);
    regexp_matcher->storage.push_back(jump_before);

    (*cur_state)++;
    (*next_state)++;
    (*index)++;

    // If it's end of stream, set accept states.
    if(regexp[*index] == '\0'){
        for(iter = finals.begin(); iter != finals.end(); iter++){
            accept_states->push_back(*iter);
        }
    }
}
