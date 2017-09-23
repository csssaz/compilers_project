#include <cassert>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#define DEBUG asdasd
namespace regex {

std::map<char, int> operator_precedence = {
    {'*', 0}, {'.', 1}, {'|', 2},
};

// convert a regular expresion from infix to
// postfix form using the shunting yard algorithm.
// e.g. "a.(b|c).d" -> "abc|.d."
// this assumes all continuous input symbols are joined
// by the concatenation operator.
std::string InfixToPostfix(std::string &infix_regex) {
  std::string postfix;
  std::stack<char> s;
  for (char c : infix_regex) {
    bool is_operator = (operator_precedence.count(c) != 0);
    if (!is_operator && c != '(' && c != ')') {
      postfix.push_back(c);
    } else if (is_operator) {
      while (!s.empty() &&
             s.top() != '(' &&
             operator_precedence[s.top()] <= operator_precedence[c]) {
        postfix.push_back(s.top());
        s.pop();
      }
      s.push(c);
    } else if (c == '(') {
      s.push(c);
    } else {
      assert(c == ')');
      while (!s.empty() && s.top() != '(') {
        postfix.push_back(s.top());
        s.pop();
      }
      assert(!s.empty() && s.top() == '(');
      s.pop();
    }
  }
  while (!s.empty()) {
    postfix.push_back(s.top());
    s.pop();
  }
  #ifdef DEBUG
    std::cerr << "infix: " << infix_regex << " " << postfix << std::endl;
  #endif
  return postfix;
}
}
