#ifndef REGEX_H
#define REGEX_H

#include <map>
#include <stack>
#include <string>
#include <vector>

namespace regex {
// Node used in NFA directed graph.
class Node {
 public:
  // index in Node vector
  int node_index_;
  // to use in dfs
  bool visited_;
  std::vector<int> epsilon_edges_;
  // letter -> [node_1, node_2, ...]
  std::map<char, std::vector<int>> edges_;
  bool is_accepting_;

  Node();
  Node(int index);


  void AddEdge(int to, char symbol);
  void AddEpsilonEdge(int to);
};

// Match a (simplified) regular expression
// Example:
//    regex::RegexMatcher r("(a|b)*.c");
//    std::string text = "aaaaabbbabababc";
//    r.Matches(text);
class RegexMatcher {
 public:
  RegexMatcher(std::string infix_regex);
  std::string postfix_regex_;

  bool Matches(const std::string &input);

 private:
  int start_state_, accept_state_;
  std::vector<Node> states_;
  std::stack<std::tuple<int, int>> build_stack_;

  std::tuple<Node, Node> GetStartEndNodes();

  void DfsEpsilon(int current_state, std::vector<int> &visited);

  void AddSymbol(char symbol);
  
  void AddUnion();
  
  void AddConcatenation();
  
  void AddKleeneStar();
  
  std::string InfixToPostfix(std::string &infix_regex);
};

}
#endif