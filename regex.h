#ifndef REGEX_H
#define REGEX_H

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

namespace regex {
const int BUFF_SIZE = 10000;
// Node used in NFA directed graph.
class Node {
 public:
  // index in Node vector
  int node_index_;
  // to use in dfs
  bool visited_;
  // if its terminal
  int token_type_;
  std::vector<int> epsilon_edges_;
  // letter -> [node_1, node_2, ...]
  std::map<char, std::vector<int>> edges_;

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
  // read regex from file
  RegexMatcher(std::istream& is);

  int NextToken();
  std::string GetLexeme();
  int line_no();

  bool Matches(const std::string &input);
  std::string postfix_regex();

 private:
  int start_state_;
  std::set<int> accepting_states_;
  std::vector<Node> states_;
  std::stack<std::tuple<int, int>> build_stack_;
  std::string postfix_regex_;
  std::string matched_lexeme_;
  
  char* forward_;
  char buffer_[BUFF_SIZE + 3];
  int line_no_;
  bool eoi_;

  std::tuple<Node, Node> GetStartEndNodes();

  void ConstructPostfix(std::string postfix_regex);
  void DfsEpsilon(int current_state, std::vector<int> &visited);

  void AddSymbol(char symbol);
  
  // |
  void AddUnion();
  // .
  void AddConcatenation();
  // *
  void AddKleeneStar();
  // +
  void AddPlusOperator();
  // ?
  void AddOneOrZero();

  // custom constructions for tricky cases
  void CommentRegex();
  void UnclosedCommentRegex();
};

}
#endif
