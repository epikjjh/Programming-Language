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

// This structure is in need to avoid tricky case.
// when input size is larger than 1.
// Example : "ab"
// Should convert string to character.
struct elem{
  int state;
  int next_state;
  char input;
};

struct FiniteStateAutomaton {
  // Make your own FSA struct here.
  std::vector<int> accept;                      // Final states.
  std::set<char> input_list;                    // Whole inputs. This is required while building NFA table.
  std::map<std::string, std::string> table;     // Table saves DFA's info. Key : transition(current states + input), Value : next states 
};

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);
std::string Check_epsilon(const std::vector<elem>& storage, int state);
std::string Check_next(const std::vector<elem>& storage, int state, char input);
std::string Find_next(const std::vector<elem>& storage, std::string state, char input);
void Make_nfa_table(const std::vector<elem>& storage, FiniteStateAutomaton *fsa);
std::string Change_to_string(int num);

#endif //_PL_HOMEWORK_FSA_H_

