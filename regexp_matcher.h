// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_
#include <vector>
#include <map>
#include <set>
#include <string>
#include <stack>
/* These structures are from fsa.h */
struct elem{
    int state;
    int next_state;
    char input;
};
struct FiniteStateAutomaton{
    std::vector<int> accept;
    std::set<char> input_list;
    std::map<std::string, std::string> table;
};

struct RegExpMatcher {
  // Design your RegExpMatcher structure.
  std::vector<elem> storage;
  FiniteStateAutomaton fsa;
};

// Homework 1.3
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 1.3
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);
std::string Check_epsilon(const RegExpMatcher& regexp_matcher, int state);
std::string Check_next(const RegExpMatcher& regexp_matcher, int state, char input);
std::string Find_next(const RegExpMatcher& regexp_matcher, std::string state, char input);
void Make_nfa_table(RegExpMatcher *regexp_matcher);
std::string Integer_to_string(int num);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

