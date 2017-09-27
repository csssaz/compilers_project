#include "regex.h"
#include "token.h"
#include <cassert>

namespace regex {

// we escape the original operators with 
// non printable characters.
namespace operators {
const char STAR = 20;
const char UNION = 21;
const char CONCATENATION = 22;
const char PLUS = 23;
const char ONE_OR_ZERO = 24;
const char LPAR = 25;
const char RPAR = 26;
}

std::map<char, char> operator_to_hidden = {
  {'*', operators::STAR},
  {'|', operators::UNION},
  {'+', operators::PLUS},
  {'?', operators::ONE_OR_ZERO},
  {'(', operators::LPAR},
  {')', operators::RPAR}
};

std::map<char, char> hidden_to_operator = {
  {operators::STAR, '*'},
  {operators::UNION, '|'},
  {operators::LPAR, '('},
  {operators::RPAR, ')'},
  {operators::CONCATENATION, '.'},
  {operators::PLUS, '+'},
  {operators::ONE_OR_ZERO, '?'}
};

std::map<char, int> operator_precedence = {
  {operators::STAR, 0},
  {operators::PLUS, 0},
  {operators::ONE_OR_ZERO, 0},
  {operators::CONCATENATION, 1},
  {operators::UNION, 2}
};

// convert operators to special symbols
// and add explicit concatenation.
std::string PreProcessRegex(std::string infix_regex) {
  std::string fixed;
  int n = static_cast<int>(infix_regex.size());
  bool escape = false;
  for (int i = 0; i < n; i++) {
    char c = infix_regex[i];
    if (escape) {
      fixed.push_back(c);
    } else if (operator_to_hidden.count(c)) {
      fixed.push_back(operator_to_hidden[c]);
    } else if (c == '\\') {
      escape = true;
      continue;
    } else {
      fixed.push_back(c);
    }

    // add concatenation
    if (i + 1 < n &&
        (c != '|' || (c == '|' && escape)) &&
        (c != '(' || (c == '(' && escape)) &&
        infix_regex[i + 1] != '|' &&
        infix_regex[i + 1] != ')' &&
        infix_regex[i + 1] != '+' &&
        infix_regex[i + 1] != '?' &&
        infix_regex[i + 1] != '*') {
      fixed.push_back(operators::CONCATENATION);
    }
    escape = false;
  }
#ifdef DEBUG
  std::cerr << "after pre-process" << std::endl;
  for (char c : fixed) {
    int hidden = hidden_to_operator.count(c);
    std::cerr << (hidden ? hidden_to_operator[c] : c) << " " << (hidden ? "hidden" : "normal")  << std::endl;
  }
  std::cerr << std::endl;
#endif
  return fixed;
}

// convert a regular expresion from infix to
// postfix form using the shunting yard algorithm.
// e.g. "a.(b|c).d" -> "abc|.d."
// this assumes all continuous input symbols are joined
// by the concatenation operator.
std::string InfixToPostfix(std::string &infix_regex) {
  std::string escaped_infix = regex::PreProcessRegex(infix_regex);
  std::string postfix;
  std::stack<char> s;
  for (char c : escaped_infix) {
    bool is_operator = (operator_precedence.count(c) != 0);
    if (!is_operator && c != operators::LPAR && c != operators::RPAR) {
      postfix.push_back(c);
    } else if (is_operator) {
      while (!s.empty() && s.top() != operators::LPAR &&
             operator_precedence[s.top()] <= operator_precedence[c]) {
        postfix.push_back(s.top());
        s.pop();
      }
      s.push(c);
    } else if (c == operators::LPAR) {
      s.push(c);
    } else {
      assert(c == operators::RPAR);
      while (!s.empty() && s.top() != operators::LPAR) {
        postfix.push_back(s.top());
        s.pop();
      }
      assert(!s.empty() && s.top() == operators::LPAR);
      s.pop();
    }
  }
  while (!s.empty()) {
    postfix.push_back(s.top());
    s.pop();
  }
#ifdef DEBUG
  std::cerr << "infix: " << infix_regex << " " << postfix << std::endl;
  std::cerr << "after postfix" << std::endl;
  for (char c : postfix) std::cerr << (hidden_to_operator.count(c) ? hidden_to_operator[c] : c); 
  std::cerr << std::endl;
#endif
  return postfix;
}

Node::Node()
    : node_index_(-1),
      visited_(false),
      token_type_(-1) {}

Node::Node(int index)
    : node_index_(index),
      visited_(false),
      token_type_(-1) {}

void Node::AddEdge(int to, char symbol) {
  edges_[symbol].push_back(to);
}

void Node::AddEpsilonEdge(int to) {
  epsilon_edges_.push_back(to);
}

// constructor for custom regex (testing purposes)
RegexMatcher::RegexMatcher(std::string infix_regex)
    : postfix_regex_(regex::InfixToPostfix(infix_regex)) {
  ConstructPostfix(postfix_regex_);
  int accept_state;
  std::tie(start_state_, accept_state) = build_stack_.top();
  accepting_states_.insert(accept_state);
  build_stack_.pop();
  assert(build_stack_.empty());
}

// Constructor to read rules from a file, similar to
// what flex does.
// Receives as parameter the input stream to be used to parse
RegexMatcher::RegexMatcher(std::istream &is)
    : forward_(nullptr), line_no_(1), eoi_(false) {
  std::string filename = "hregex.in";
  std::ifstream fin(filename);
  if (!fin.good()) {
    std::cerr << "no input file" << std::endl;
    throw;
  }

  // read buffer
  is.read(buffer_, BUFF_SIZE);
  forward_ = buffer_;
  buffer_[is.gcount()] = '\0';

  Node start_state;
  std::string regex;
  int token_type;
  while (fin >> regex >> token_type) {
    if (regex == "whitespace") {
      regex = "(\n|\t|\r| )";
    }
    if (regex == "comments") {
      CommentRegex();
    } else if (regex == "unclosed") {
      UnclosedCommentRegex();
    } else {
      std::string postfix_regex = regex::InfixToPostfix(regex);
      ConstructPostfix(postfix_regex);
    }
    int initial, accept;
    std::tie(initial, accept) = build_stack_.top();
    states_[accept].token_type_ = token_type;
    accepting_states_.insert(accept);
    build_stack_.pop();
    assert(build_stack_.empty());
    start_state.AddEpsilonEdge(initial);
  }

  fin.close();
  start_state_ = static_cast<int>(states_.size());
  states_.push_back(start_state);
}


// constructs the NFA to represent the
// regex passed as parameter, that should be in
// postfix mode.
void RegexMatcher::ConstructPostfix(std::string postfix_regex) {
  for (char c : postfix_regex) {
    if (c == operators::UNION)
      AddUnion();
    else if (c == operators::CONCATENATION)
      AddConcatenation();
    else if (c == operators::STAR)
      AddKleeneStar();
    else if (c == operators::PLUS)
      AddPlusOperator();
    else if (c == operators::ONE_OR_ZERO)
      AddOneOrZero();
    else
      AddSymbol(c);
  }
}

std::string RegexMatcher::postfix_regex() {
  std::string out;
  for (char c : postfix_regex_)
    out.push_back(hidden_to_operator.count(c) ? hidden_to_operator[c] : c);
  return out;
}

std::tuple<Node, Node> RegexMatcher::GetStartEndNodes() {
  int num_states = static_cast<int>(states_.size());
  return std::make_tuple(Node(num_states), Node(num_states + 1));
}

// method for special comment regex because why not
void RegexMatcher::CommentRegex() {
  AddSymbol('/');
  AddSymbol('*');
  AddConcatenation();
  AddSymbol('*');
  AddSymbol('/');
  AddConcatenation();

  int s_a, e_a, s_b, e_b;
  std::tie(s_b, e_b) = build_stack_.top();
  build_stack_.pop();
  std::tie(s_a, e_a) = build_stack_.top();
  build_stack_.pop();

  Node absorb(static_cast<int>(states_.size()));
  // wildcard transitions. very ugly.
  for (char c = 9; c <= 126; c++) {
    absorb.AddEdge(absorb.node_index_, c);
  } 

  states_[e_a].AddEpsilonEdge(absorb.node_index_);
  absorb.AddEpsilonEdge(s_b);
  states_.push_back(absorb);

  build_stack_.push({s_a, e_b});
}

void RegexMatcher::UnclosedCommentRegex() {
  AddSymbol('/');
  AddSymbol('*');
  AddConcatenation();
  int s, e;
  std::tie(s, e) = build_stack_.top();
  // wildcard transitions. very ugly.
  for (char c = 9; c <= 126; c++) {
    states_[e].AddEdge(e, c);
  }
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

// Creates an NFA for the Plus operator.
// This is esentially the same as Kleene star
// but we need at least one match
void RegexMatcher::AddPlusOperator() {
  int start_a, end_a;
  std::tie(start_a, end_a) = build_stack_.top();
  build_stack_.pop();

  Node start_state, end_state;
  std::tie(start_state, end_state) = GetStartEndNodes();

  start_state.AddEpsilonEdge(start_a);
  states_[end_a].AddEpsilonEdge(start_a);
  states_[end_a].AddEpsilonEdge(end_state.node_index_);

  states_.push_back(start_state);
  states_.push_back(end_state);
  build_stack_.push(
      std::make_tuple(start_state.node_index_, end_state.node_index_));
}

// NFA for ? operator: this means we allow
// either one or zero matches of the NFA that is
// popped.
void RegexMatcher::AddOneOrZero() {
  int start_a, end_a;
  std::tie(start_a, end_a) = build_stack_.top();
  build_stack_.pop();

  Node start_state, end_state;
  std::tie(start_state, end_state) = GetStartEndNodes();

  start_state.AddEpsilonEdge(end_state.node_index_);
  start_state.AddEpsilonEdge(start_a);
  states_[end_a].AddEpsilonEdge(end_state.node_index_);

  states_.push_back(start_state);
  states_.push_back(end_state);
  build_stack_.push(
      std::make_tuple(start_state.node_index_, end_state.node_index_));
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
    if (accepting_states_.count(v)) {
      return true;
    }
  }
  return false;
}

int RegexMatcher::NextToken() {
  if (eoi_) {
    return Tokentype::EOI;
  }
  for (auto &state : states_) {
    state.visited_ = false;
  }
  std::vector<int> current_state;
  DfsEpsilon(start_state_, current_state);

  int final_state = -1; 
  char* lexeme_start = forward_;
  char* lexeme_end = forward_;

  for (;; forward_++) {
    char c = *forward_;
    if (c == '\0') {
      break;
    }
    for (auto &state : states_) state.visited_ = false;
    std::vector<int> next_state;
    for (int from : current_state) {
      for (int to : states_[from].edges_[c]) {
        DfsEpsilon(to, next_state);
      }
    }
    // can't advance anymore so we are done
    if (next_state.empty()) {
      break;
    }
    // pick accepting state
    int chosen = static_cast<int>(states_.size()) + 1;
    bool at_least_one = false, non_greedy = false;
    for (int reached : next_state) {
      //std::cerr << reached << ", ";
      if (accepting_states_.count(reached)) {
        lexeme_end = forward_;
        at_least_one = true;
        chosen = std::min(chosen, reached);
        // TODO: fix magic number
        // non greedy match for comments
        if (states_[reached].token_type_ == -3) {
          final_state = reached;
          non_greedy = true;
          break;
        }
      }
    }
    final_state = at_least_one ? chosen : final_state;
    if (non_greedy) break;
    std::swap(current_state, next_state);
  }

  forward_ = lexeme_end;
  forward_++;
  matched_lexeme_.clear();
  for (; lexeme_start != forward_; lexeme_start++) {
    matched_lexeme_.push_back(*lexeme_start);
    line_no_ += static_cast<int>(matched_lexeme_.back() == '\n');
  }
  //std::cerr << matched_lexeme_ << std::endl;

  if (*forward_ == '\0') {
    eoi_ = true;
  }

  return final_state == -1 ? static_cast<int>(Tokentype::ErrUnknown)
                           : (states_[final_state].token_type_ <= -2
                                  ? NextToken()
                                  : states_[final_state].token_type_);
}

std::string RegexMatcher::GetLexeme() { return matched_lexeme_; }

int RegexMatcher::line_no() { return line_no_; }
}
