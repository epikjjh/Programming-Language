// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <set>
#include <string>
#include <map>
#include <vector>
#include <stack>

// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

struct FSATableElement {
  int state;
  int next_state;
  std::string str;
};

struct FiniteStateAutomaton {
  // Make your own FSA struct here.
  std::string initial;
  std::vector<int> accept;
  std::set<char> alpha;
  std::map<std::string, std::string> table;
};

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);
std::string Check_epsilon(const std::vector<FSATableElement>& elements, int state);
std::string Check_input(const std::vector<FSATableElement>& elements, int state, std::string input);
std::string Check_state(const std::vector<FSATableElement>& elements, std::string state, std::string input);
void Make_nfa_table(const std::vector<FSATableElement>& elements, FiniteStateAutomaton *fsa);
std::string Change_to_string(int num);

#endif //_PL_HOMEWORK_FSA_H_

