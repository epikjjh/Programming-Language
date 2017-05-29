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

}

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
    // Implement this function. 
    std::vector<FSATableElement>::const_iterator elem_iter;
    int nfa = 0;

    for(elem_iter = elements.begin(); elem_iter != elements.end(); elem_iter++){
        if(elem_iter->str.empty() == 1){
            nfa = 1;
            break;
        }
    }
    /* DFA*/
    if(!nfa){
        string transition, next;

        fsa->initial = Change_to_string(elements[0].state);

        for(int i = 0; i < elements.size(); i++){
            transition = Change_to_string(elements[i].state) + elements[i].str;
            next = Change_to_string(elements[i].next_state);
            fsa->table.insert(map<string, string>::value_type(transition, next));
        }
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
string Change_to_string(int num){
    string tempstr;
    char tempchar;

    tempchar = (char)num + '0';
    tempstr = tempchar;

    return tempstr;
}
