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
  map<string, string>::iterator iter;
  string transition,isfinal;
  int index = 0;
  char input = str[0];

  // Start state : 1
  string cur_state = Change_to_string(1);

  while(str[index] != '\0'){
        // Create transition : cur_state + input
        transition = cur_state + input;
        // Find if tansition is in table.
        iter = fsa->table.find(transition);

        if(iter != fsa->table.end())
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

  for(int i = 0; i < fsa->accept.size(); i++){
        for(int j = 0; j < isfinal.size(); j++){
            if(fsa->accept[i] == (int)(isfinal[j]-'0'))
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

    /* Converse string input to char input */
    // Tricky case : input - "ab"
    // Split in to "a" and "b"
    for(elem_iter = elements.begin(); elem_iter != elements.end(); elem_iter++){
        elem new_elem;

        new_elem.state = elem_iter->state;
        new_elem.next_state = elem_iter->next_state;
        // Multiple input
        if(elem_iter->str.size() > 1){
            for(int i = 0; i < str.size(); i++){
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
        else if(elem_iter->str.size() == 0{
            new_elem.input = ''; 

            storage.push_back(new_elem);
        }
        // Normal input
        else{
            char tmp = elem_iter[0];

            new_elem.input = tmp;

            storage.push_back(new_elem);
        }
    }
    
    /* Decide whether it's NFA or DFA */

    // When epsilon appears.
    for(int i = 0; i < storage.size(); i++){
        if(storage[i].input == ''){
            nfa = 1;
            break;
        }
    }
    // When one state has two or more next_state.
    for(int i = 0; i < storage.size(); i++){
        for(int j = i+1; j < storage.size; j++){
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

    /* DFA*/
    if(!nfa){
        string transition, next;

        // Initial state : 1
        fsa->initial = Change_to_string(1);

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
        fsa->initial = Change_to_string(elements[0].state);
        fsa->initial = Check_epsilon(elements, elements[0].state);

        for(int i = 0; i < elements.size(); i++){
            if(elements[i].str.empty() == 0)
                fsa->alpha.insert(elements[i].str);
        }
        Make_nfa_table(elements, fsa);

        for(int i = 0; i < accept_states.size() - 1; i++)
            fsa->accept.push_back(accept_states[i]);
    }
    LOG << "num_elements: " << elements.size()
        << ", accept_states: " << accept_states.size() << endl;
    if(elements.size() <= 0)
        return false;
    return true;
}
std::string Check_epsilon(const std::vector<FSATableElement>& elements, int state){
    stack<int> tempst;
    set<int> visitable;
    set<int>::iterator iter;
    string tempstr;
    int t, setsize;


    tempst.push(state);
    visitable.insert(state);

    while(!tempst.empty()){
        t = tempst.top();
        tempst.pop();

        for(int i = 0; i < elements.size(); i++){
            if(elements[i].state == t && elements[i].str == ""){
                setsize = visitable.size();
                visitable.insert(elements[i].next_state);
                if(visitable.size() != setsize)
                    tempst.push(elements[i].next_state);
            }
         }
     }
     
     for(iter = visitable.begin(); iter != visitable.end(); iter++)
         tempstr.append(Change_to_string(*iter));
         
     return tempstr;   
}
std::string Check_input(const std::vector<FSATableElement>& elements, int state, string input){
    string cur_state = Check_epsilon(elements, state);
    string tempstr, tmpstr;
    set<int> tempset;
    set<int>::iterator iter;
    int num,next_state;

    for(int i = 0; i < cur_state.size(); i++){
        num = (int)(cur_state.at(i) = '0');
        for(int j = 0; j < elements.size(); j++){
            if(num == elements[j].state && input.compare(elements[j].str) == 0){
                tempset.insert(elements[j].next_state);
            }
        }
    }

    for(iter = tempset.begin(); iter != tempset.end(); iter++){
        tempstr = Check_epsilon(elements, *iter);
        for(int j = 0; j < tempstr.size(); j++){
            next_state = (int)(tempstr.at(j) - '0');
            tempset.insert(next_state);
        }
    }
    for(iter = tempset.begin(); iter != tempset.end(); iter++){
        tmpstr.append(Change_to_string(*iter));
    }
    return tmpstr;
}
std::string Check_state(const std::vector<FSATableElement>& elements, std::string state, std::string input){
    int cur_state,next_state;
    string next_state_str, visitable_str;
    set<int> tempset;
    set<int>::iterator iter;

    for(int i = 0; i < state.size(); i++){
        cur_state = (int)(state.at(i) - '0');
        next_state_str = Check_input(elements,cur_state,input);

        for(int j = 0; j < next_state_str.size(); j++){
            next_state = (int)(next_state_str.at(j) - '0');
            tempset.insert(next_state);
        }
    }
    for(iter = tempset.begin(); iter != tempset.end(); iter++)
        visitable_str.append(Change_to_string(*iter));

    return visitable_str;
}
void Make_nfa_table(const std::vector<FSATableElement>& elements, FiniteStateAutomaton *fsa){
    stack<string> tempst;
    set<char>::iterator iter;
    set<string> exist;
    string cur_state = fsa->initial;
    string tran_state, next_state;

    tempst.push(cur_state);
    exist.insert(cur_state);

    while(!tempst.empty()){
        cur_state = tempst.top();

        tempst.pop();
        for(iter = fsa->alpha.begin(); iter != fsa->alpha.end(); iter++){
            next_state = Check_state(elements, cur_state, *iter);
            tran_state = cur_state + *iter;
            fsa->table.insert(map<string, string>::value_type(tran_state, next_state));

            if(cur_state != next_state && exist.find(next_state) == exist.end()){
                tempst.push(next_state);
                exist.insert(next_state);
            }
        }
    }
}
/* This function change integer to string. */
string Change_to_string(int num){
    string str;
    char c;

    c = (char)num + '0';
    str = c;

    return str;
}
