// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

#include "lr_parser.h"
#include "fsa.h"
#include <stack>

struct RegExpMatcher {
  // Design your RegExpMatcher structure.
  std::stack <std::pair <int,int> > NFA_flake;
  FiniteStateAutomaton fsa;
  LRParser lr_parser;
};

// Homework 1.3
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 1.3
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

