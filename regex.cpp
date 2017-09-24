#include "regex.h"
#include <iostream>
#include <cassert>
namespace regex {

Node::Node() : node_index_(-1), is_accepting_(false), visited_(false) {}

Node::Node(int index)
    : node_index_(index), is_accepting_(false), visited_(false) {}

void Node::AddEdge(int to, char symbol) {
  edges_[symbol].push_back(to);
}

void Node::AddEpsilonEdge(int to) {
  epsilon_edges_.push_back(to);
}

RegexMatcher::RegexMatcher(std::string infix_regex)
    : postfix_regex_(InfixToPostfix(infix_regex)) {
  for (char c : postfix_regex_) {
    if (c == '|') {
      AddUnion();
    } else if (c == '.') {
      AddConcatenation();
    } else if (c == '*') {
      AddKleeneStar();
    } else {
      AddSymbol(c);
    }
  }
  std::tie(start_state_, accept_state_) = build_stack_.top();
  build_stack_.pop();
  assert(build_stack_.empty());
}

std::tuple<Node, Node> RegexMatcher::GetStartEndNodes() {
  int num_states = static_cast<int>(states_.size());
  return std::make_tuple(Node(num_states), Node(num_states + 1));
}

// NFA for a single symbol.
// creates two states and makes the transition
// between them to have the given symbol.
void RegexMatcher::AddSymbol(char symbol) {
  Node start_state, end_state;
  std::tie(start_state, end_state) = GetStartEndNodes();
  start_state.AddEdge(end_state.node_index_, symbol);
  states_.push_back(start_state); 
  states_.push_back(end_state);
  build_stack_.push(
      std::make_tuple(start_state.node_index_, end_state.node_index_));
}

// Creates an NFA with the union of
// the two NFAs in the top of the stack.
// Pushes the new start and end states to the stack.
void RegexMatcher::AddUnion() {
  int start_a, end_a, start_b, end_b;
  std::tie(start_b, end_b) = build_stack_.top();
  build_stack_.pop();
  std::tie(start_a, end_a) = build_stack_.top();
  build_stack_.pop();

  Node start_state, end_state;
  std::tie(start_state, end_state) = GetStartEndNodes();

  start_state.AddEpsilonEdge(start_a);
  start_state.AddEpsilonEdge(start_b);
  states_[end_a].AddEpsilonEdge(end_state.node_index_);
  states_[end_b].AddEpsilonEdge(end_state.node_index_);

  states_.push_back(start_state);
  states_.push_back(end_state);
  build_stack_.push(
      std::make_tuple(start_state.node_index_, end_state.node_index_));
}

// Creates an NFA with the concatenation of the
// two NFAs in the top of the stack.
// Pushes the new start and end indexes to the stack.
void RegexMatcher::AddConcatenation() {
  int start_a, end_a, start_b, end_b;
  std::tie(start_b, end_b) = build_stack_.top();
  build_stack_.pop();
  std::tie(start_a, end_a) = build_stack_.top();
  build_stack_.pop();

  states_[end_a].AddEpsilonEdge(start_b);
  build_stack_.push(std::make_tuple(start_a, end_b));
}

// Creates an NFA for the Kleene star of the last
// NFA in the stack.
// Pushes the new start and end indexes to the stack.
void RegexMatcher::AddKleeneStar() {
  int start_a, end_a;
  std::tie(start_a, end_a) = build_stack_.top();
  build_stack_.pop();

  Node start_state, end_state;
  std::tie(start_state, end_state) = GetStartEndNodes();

  start_state.AddEpsilonEdge(start_a);
  start_state.AddEpsilonEdge(end_state.node_index_);
  states_[end_a].AddEpsilonEdge(start_a);
  states_[end_a].AddEpsilonEdge(end_state.node_index_);

  states_.push_back(start_state);
  states_.push_back(end_state);
  build_stack_.push(
      std::make_tuple(start_state.node_index_, end_state.node_index_));
}

// convert a regular expresion from infix to
// postfix form using the shunting yard algorithm.
// e.g. "a.(b|c).d" -> "abc|.d."
// this assumes all continuous input symbols are joined
// by the concatenation operator.
std::string RegexMatcher::InfixToPostfix(std::string &infix_regex) {
  std::map<char, int> operator_precedence = {
      {'*', 0}, {'.', 1}, {'|', 2},
  };
  std::string postfix;
  std::stack<char> s;
  for (char c : infix_regex) {
    bool is_operator = (operator_precedence.count(c) != 0);
    if (!is_operator && c != '(' && c != ')') {
      postfix.push_back(c);
    } else if (is_operator) {
      while (!s.empty() && s.top() != '(' &&
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

// method to compute the epsilon closure of a given state
// it recursively visits the states reachable through
// epsilon transitions and stores the explored nodes in
// the parameter visited.
void RegexMatcher::DfsEpsilon(int current_state, std::vector<int> &visited) {
  visited.push_back(current_state);
  states_[current_state].visited_ = true;
  for (int to : states_[current_state].epsilon_edges_) {
    if (!states_[to].visited_) {
      DfsEpsilon(to, visited);
    }
  }
}

void print_state(std::vector<int> &s) {
  std::cerr << "current state: ";
  for (int v : s) std::cerr << v << ", ";
  std::cerr << std::endl;
}

// interface to check if the regex accepts an input
// string. Simulates the NFA using the DfsEpsilon method
// defined above.
bool RegexMatcher::Matches(const std::string &input) {
  for (auto &state : states_) {
    state.visited_ = false;
  }
  std::vector<int> current_state;
  DfsEpsilon(start_state_, current_state);


  for (char c : input) {
    for (auto &state : states_) {
      state.visited_ = false;
    }
    std::vector<int> next_state;
    for (int v : current_state) {
      for (int to : states_[v].edges_[c]) {
        DfsEpsilon(to, next_state);
      }
    }
    if (next_state.empty()) {
      return false;
    }
    std::swap(current_state, next_state);
  }


  for (int v : current_state) {
    if (v == accept_state_) {
      return true;
    }
  }
  return false;
}
}
