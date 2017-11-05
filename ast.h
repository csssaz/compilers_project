#ifndef DECAFPARSER_AST_H
#define DECAFPARSER_AST_H

#include <algorithm>
#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <sstream>
#include "symbol_table.h"
#include "tac.h"

/////////////////////////////////////////////////////////////////////////////////

struct Data {
  Data(SymbolTable& st)
      : sym_table(st),
        variable_no(0),
        label_no(0),
        expr_return_type(ValueType::VoidVal) {}

  SymbolTable& sym_table;  // Reference to the symbol-table; do not delete.

  int variable_no;  // Counters for increasing the numbers of
  int label_no;     // variables and labels.

  std::string method_name;       // Method name, needed for return.
  std::stack<int> for_label_no;  // The label_no of enclosing for loops,
                                 // needed for continue/break statements.

  std::string expr_return_var;  // Variable used to store expr value.
  ValueType expr_return_type;   // Type of expression evaluated.

  // You may add/remove data members to this structure as you see fit.
};

inline void warning_msg(std::string message) {
  std::cout << "WARNING: " << message << std::endl;
}

inline void error_msg(std::string message) {
  std::cout << "ERROR: " << message << std::endl;
  throw;
}

inline void add_to_symbol_table(SymbolTable& st, EntryType entry_type,
                                std::string scope, std::string name,
                                ValueType value_type, std::string signature) {
  SymbolTable::Entry* entry = st.lookup(scope, name);
  if (entry == nullptr) {
    SymbolTable::Entry entry;
    entry.name = name;
    entry.scope = scope;
    entry.entry_type = entry_type;
    entry.value_type = value_type;
    entry.signature = signature;
    st.add(entry);
  } else {
    error_msg("Identifier '" + name + "' already declared in scope.");
  }
}

/////////////////////////////////////////////////////////////////////////////////

class Node {
 public:
  virtual const std::string str() const = 0;

  virtual void icg(Data& data, TAC& tac) const = 0;

  virtual ~Node() = default;
};

inline std::string tostr(const Node* node) {
  return (node == nullptr ? "(null)" : node->str());
}

/////////////////////////////////////////////////////////////////////////////////

class ExprNode : public Node {};

class NumberExprNode : public ExprNode {
 public:
  explicit NumberExprNode(const std::string value) : value_(value) {}

  virtual const std::string str() const override {
    return std::string("(NUM ") + value_ + ')';
  }

  virtual void icg(Data& data, TAC&) const override {
    // Provided.
    data.expr_return_var = value_;
    data.expr_return_type =
        (std::all_of(value_.begin(), value_.end(), ::isdigit)
             ? ValueType::IntVal
             : ValueType::RealVal);
  }

 protected:
  std::string value_;
};

class AndExprNode : public ExprNode {
 public:
  AndExprNode(ExprNode* lhs, ExprNode* rhs) : lhs_(lhs), rhs_(rhs) {}

  virtual const std::string str() const override {
    return std::string("(&& ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }

  virtual void icg(Data& data, TAC& tac) const override {
    std::string result_var = tac.tmp_variable_name(data.variable_no++);
    std::string lab_and_false = tac.label_name("and_false", data.label_no);
    std::string lab_and_end = tac.label_name("and_end", data.label_no);
    data.label_no++;

    tac.append(TAC::InstrType::VAR, result_var);

    lhs_->icg(data, tac);
    auto lhs_type = data.expr_return_type;
    tac.append(TAC::InstrType::EQ, data.expr_return_var, "0", lab_and_false);

    rhs_->icg(data, tac);
    tac.append(TAC::InstrType::EQ, data.expr_return_var, "0", lab_and_false);

    if (lhs_type != ValueType::IntVal ||
        data.expr_return_type != ValueType::IntVal) {
      warning_msg(
          "Type mismatch in logical && (operands are not integer values).");
    }

    tac.append(TAC::InstrType::ASSIGN, "1", result_var);
    tac.append(TAC::InstrType::GOTO, lab_and_end);

    tac.label_next_instr(lab_and_false);
    tac.append(TAC::InstrType::ASSIGN, "0", result_var);

    tac.label_next_instr(lab_and_end);

    data.expr_return_var = result_var;
    data.expr_return_type = ValueType::IntVal;
  }

 protected:
  ExprNode *lhs_, *rhs_;
};

class OrExprNode : public ExprNode {
 public:
  OrExprNode(ExprNode* lhs, ExprNode* rhs) : lhs_(lhs), rhs_(rhs) {}

  virtual const std::string str() const override {
    return std::string("(|| ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }

  virtual void icg(Data& data, TAC& tac) const override {
    std::string result_var = tac.tmp_variable_name(data.variable_no++);
    std::string lab_or_true = tac.label_name("or_true", data.label_no);
    std::string lab_or_end = tac.label_name("or_end", data.label_no);
    data.label_no++;

    tac.append(TAC::InstrType::VAR, result_var);

    lhs_->icg(data, tac);
    tac.append(TAC::InstrType::NE, data.expr_return_var, "0", lab_or_true);
    auto lhs_type = data.expr_return_type;

    rhs_->icg(data, tac);
    tac.append(TAC::InstrType::NE, data.expr_return_var, "0", lab_or_true);

    if (lhs_type != ValueType::IntVal ||
        data.expr_return_type != ValueType::IntVal) {
      warning_msg(
          "Type mismatch in logical || (operands are not integer values).");
    }

    tac.append(TAC::InstrType::ASSIGN, "0", result_var);
    tac.append(TAC::InstrType::GOTO, lab_or_end);

    tac.label_next_instr(lab_or_true);
    tac.append(TAC::InstrType::ASSIGN, "1", result_var);

    tac.label_next_instr(lab_or_end);

    data.expr_return_var = result_var;
    data.expr_return_type = ValueType::IntVal;
  }

 protected:
  ExprNode *lhs_, *rhs_;
};

class NotExprNode : public ExprNode {
 public:
  NotExprNode(ExprNode* rhs) : rhs_(rhs) {}

  virtual const std::string str() const override {
    return std::string("(! ") + tostr(rhs_) + ')';
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    std::string var = tac.tmp_variable_name(data.variable_no++);
    std::string lab_not_true = tac.label_name("not_true", data.label_no);
    std::string lab_not_end = tac.label_name("not_end", data.label_no);
    data.label_no++;

    tac.append(TAC::InstrType::VAR, var);
    rhs_->icg(data, tac);
    ValueType type_rhs = data.expr_return_type;
    tac.append(TAC::InstrType::NE, data.expr_return_var, "0", lab_not_true);
    tac.append(TAC::InstrType::ASSIGN, "1", var);
    tac.append(TAC::InstrType::GOTO, lab_not_end);
    tac.label_next_instr(lab_not_true);
    tac.append(TAC::InstrType::ASSIGN, "0", var);
    tac.label_next_instr(lab_not_end);

    data.expr_return_var = var;
    data.expr_return_type = ValueType::IntVal;

    if (type_rhs == ValueType::RealVal) {
      warning_msg(
          "Type mismatch in logical ! operation (operand is not an integer "
          "value).");
    }
  }

 protected:
  ExprNode* rhs_;
};

class RelationalExprNode : public ExprNode {
 public:
  RelationalExprNode(TAC::InstrType instr_type, ExprNode* lhs, ExprNode* rhs)
      : instr_type_(instr_type), lhs_(lhs), rhs_(rhs) {}

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    lhs_->icg(data, tac);
    std::string var_lhs = data.expr_return_var;
    ValueType type_lhs = data.expr_return_type;

    rhs_->icg(data, tac);
    std::string var_rhs = data.expr_return_var;
    ValueType type_rhs = data.expr_return_type;

    std::string lab_rel_true = tac.label_name("rel_true", data.label_no);
    std::string lab_rel_end = tac.label_name("rel_end", data.label_no);
    data.label_no++;
    std::string var = tac.tmp_variable_name(data.variable_no++);

    tac.append(TAC::InstrType::VAR, var);
    tac.append(instr_type_, var_lhs, var_rhs, lab_rel_true);
    tac.append(TAC::InstrType::ASSIGN, "0", var);
    tac.append(TAC::InstrType::GOTO, lab_rel_end);
    tac.label_next_instr(lab_rel_true);
    tac.append(TAC::InstrType::ASSIGN, "1", var);
    tac.label_next_instr(lab_rel_end);

    data.expr_return_var = var;
    data.expr_return_type = ValueType::IntVal;

    if ((type_lhs == ValueType::IntVal && type_rhs == ValueType::RealVal) ||
        (type_lhs == ValueType::RealVal && type_rhs == ValueType::IntVal)) {
      warning_msg("Type mismatch in operation " + tac.IName[instr_type_] + ".");
    }
  }

 protected:
  TAC::InstrType instr_type_;
  ExprNode *lhs_, *rhs_;
};

class EqExprNode : public RelationalExprNode {
 public:
  EqExprNode(ExprNode* lhs, ExprNode* rhs)
      : RelationalExprNode(TAC::InstrType::EQ, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(== ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class NeqExprNode : public RelationalExprNode {
 public:
  NeqExprNode(ExprNode* lhs, ExprNode* rhs)
      : RelationalExprNode(TAC::InstrType::NE, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(!= ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class LtExprNode : public RelationalExprNode {
 public:
  LtExprNode(ExprNode* lhs, ExprNode* rhs)
      : RelationalExprNode(TAC::InstrType::LT, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(< ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class LteExprNode : public RelationalExprNode {
 public:
  LteExprNode(ExprNode* lhs, ExprNode* rhs)
      : RelationalExprNode(TAC::InstrType::LE, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(<= ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class GtExprNode : public RelationalExprNode {
 public:
  GtExprNode(ExprNode* lhs, ExprNode* rhs)
      : RelationalExprNode(TAC::InstrType::GT, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(> ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class GteExprNode : public RelationalExprNode {
 public:
  GteExprNode(ExprNode* lhs, ExprNode* rhs)
      : RelationalExprNode(TAC::InstrType::GE, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(>= ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class ArithmeticExprNode : public ExprNode {
 public:
  ArithmeticExprNode(TAC::InstrType instr_type, ExprNode* lhs, ExprNode* rhs)
      : instr_type_(instr_type), lhs_(lhs), rhs_(rhs) {}

  virtual void icg(Data& data, TAC& tac) const override {
    lhs_->icg(data, tac);
    std::string lhs_var = data.expr_return_var;
    auto lhs_type = data.expr_return_type;
    rhs_->icg(data, tac);
    std::string rhs_var = data.expr_return_var;

    if (lhs_type != data.expr_return_type) {
      warning_msg("Mixing int and real types in operation " +
                  tac.IName[instr_type_] + ".");
    }

    std::string result_var = tac.tmp_variable_name(data.variable_no++);
    tac.append(TAC::InstrType::VAR, result_var);

    tac.append(instr_type_, lhs_var, rhs_var, result_var);
    data.expr_return_var = result_var;
    data.expr_return_type = lhs_type;
  }

 protected:
  TAC::InstrType instr_type_;
  ExprNode *lhs_, *rhs_;
};

class MultiplyExprNode : public ArithmeticExprNode {
 public:
  MultiplyExprNode(ExprNode* lhs, ExprNode* rhs)
      : ArithmeticExprNode(TAC::InstrType::MULT, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(* ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class DivideExprNode : public ArithmeticExprNode {
 public:
  DivideExprNode(ExprNode* lhs, ExprNode* rhs)
      : ArithmeticExprNode(TAC::InstrType::DIVIDE, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(/ ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class ModulusExprNode : public ArithmeticExprNode {
 public:
  ModulusExprNode(ExprNode* lhs, ExprNode* rhs)
      : ArithmeticExprNode(TAC::InstrType::MOD, lhs, rhs) {}

  virtual const std::string str() const override {
    return std::string("(% ") + tostr(lhs_) + ' ' + tostr(rhs_) + ')';
  }
};

class PlusExprNode : public ArithmeticExprNode {
 public:
  PlusExprNode(ExprNode* rhs)
      : ArithmeticExprNode(TAC::InstrType::ADD, nullptr, rhs) {}

  PlusExprNode(ExprNode* lhs, ExprNode* rhs)
      : ArithmeticExprNode(TAC::InstrType::ADD, lhs, rhs) {}

  virtual const std::string str() const override {
    std::string s = std::string("(+ ");
    if (lhs_ != nullptr) {
      s += tostr(lhs_) + ' ';
    }
    s += tostr(rhs_) + ')';
    return s;
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    if (lhs_ == nullptr) {
      rhs_->icg(data, tac);
    } else {
      ArithmeticExprNode::icg(data, tac);
    }
  }
};

class MinusExprNode : public ArithmeticExprNode {
 public:
  MinusExprNode(ExprNode* rhs)
      : ArithmeticExprNode(TAC::InstrType::SUB, nullptr, rhs) {}
  MinusExprNode(ExprNode* lhs, ExprNode* rhs)
      : ArithmeticExprNode(TAC::InstrType::SUB, lhs, rhs) {}

  virtual const std::string str() const override {
    std::string s = std::string("(- ");
    if (lhs_ != nullptr) {
      s += tostr(lhs_) + ' ';
    }
    s += tostr(rhs_) + ')';
    return s;
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    if (lhs_ == nullptr) {
      rhs_->icg(data, tac);
      std::string var = tac.tmp_variable_name(data.variable_no++);
      tac.append(TAC::InstrType::VAR, var);
      tac.append(TAC::InstrType::UMINUS, data.expr_return_var, var);
      data.expr_return_var = var;
    } else {
      ArithmeticExprNode::icg(data, tac);
    }
  }
};

class VariableExprNode : public ExprNode {
 public:
  explicit VariableExprNode(const std::string& id) : id_(id) {}

  virtual const std::string str() const override {
    return std::string("(VAR ") + id_ + ')';
  }

  virtual void icg(Data& data, TAC&) const override {
    // Provided.
    data.expr_return_var = id_;
    data.expr_return_type = ValueType::VoidVal;

    SymbolTable::Entry* entry = data.sym_table.lookup(data.method_name, id_);
    if (entry == nullptr &&
        !data.method_name.empty()) {  // also look in global scope.
      entry = data.sym_table.lookup("", id_);
    }
    if (entry == nullptr) {
      error_msg("Undeclared identifier '" + id_ + "'.");
    } else {
      data.expr_return_type = entry->value_type;
    }
  }

  std::string get_id() const { return id_; }

 protected:
  std::string id_;
};

/////////////////////////////////////////////////////////////////////////////////

class VariableDeclarationNode : public Node {
 public:
  VariableDeclarationNode(ValueType type, std::list<VariableExprNode*>* vars)
      : type_(type), vars_(vars) {}

  virtual const std::string str() const override {
    std::string s = std::string("(DECLARE ") + tostr(type_);
    for (auto node : *vars_) {
      s += ' ' + tostr(node);
    }
    s += ')';
    return s;
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    for (auto e : *vars_) {
      // e->icg( data, tac );
      add_to_symbol_table(data.sym_table, EntryType::Variable, data.method_name,
                          e->get_id(), type_, "");
      tac.append(TAC::InstrType::VAR, e->get_id());
    }
  }

  ValueType get_type() const { return type_; }

  const std::list<VariableExprNode*>* get_vars() const { return vars_; }

 protected:
  ValueType type_;
  const std::list<VariableExprNode*>* vars_;
};

class ParameterNode : public Node {
 public:
  ParameterNode(ValueType type, VariableExprNode* var)
      : type_(type), var_(var) {}

  virtual const std::string str() const override {
    return std::string("(PARAM ") + tostr(type_) + tostr(var_) + ")";
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    // var_->icg( data, tac );
    add_to_symbol_table(data.sym_table, EntryType::Variable, data.method_name,
                        var_->get_id(), type_, "");
    tac.append(TAC::InstrType::FPARAM, var_->get_id());
  }

  ValueType get_type() const { return type_; }

  std::string get_id() const { return var_->get_id(); }

 protected:
  ValueType type_;
  VariableExprNode* var_;
};

/////////////////////////////////////////////////////////////////////////////////

class StmNode : public Node {};

class MethodCallExprStmNode : public ExprNode, public StmNode {
 public:
  MethodCallExprStmNode(std::string id, std::list<ExprNode*>* expr_list)
      : id_(id), expr_list_(expr_list) {}

  virtual const std::string str() const override {
    std::string s("(CALL " + id_);
    for (auto e : *expr_list_) {
      s += " " + tostr(e);
    }
    s += ')';
    return s;
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Find symbol table entry for this method
    SymbolTable::Entry* entry = data.sym_table.lookup("", id_);

    if (entry == nullptr) {
      // No entry in symbol table for this method
      if (id_ != "writeln" && id_ != "write") {
        // Undeclared method
        error_msg("Method '" + id_ + "' undeclared.");
      } else {
        // write / writeln
        if (expr_list_->size() != 1) {
          warning_msg("Method '" + id_ + "' accepts 1 argument but " + std::to_string(expr_list_->size()) + " given.");
        }

        // In the case of more than one argument, add only the first one to TAC code
        expr_list_->front()->icg(data, tac);
        tac.append(TAC::InstrType::APARAM, data.expr_return_var);
        tac.append(TAC::InstrType::CALL, id_);
        data.expr_return_var = id_;
      }
    } else {
      // Get the list of types of formal parameters
      std::vector<std::string> formal_parameters = std::vector<std::string>();
      std::stringstream ss(entry->signature);
      std::string token;
      std::string expectedParams = "";
      while (std::getline(ss, token, ':')) {
        if (token.empty()) {
          // Skip empty strings because of delimiter ::
          continue;
        }
        if (!expectedParams.empty()) {
          expectedParams += ", ";
        }
        expectedParams += token;
        formal_parameters.push_back(token);
      }

      bool wrongParameters = formal_parameters.size() != expr_list_->size();

      auto iterFormal = formal_parameters.begin();

      std::vector<std::string> expr_var_names;
      std::string givenParams = "";

      // Get the list of actual parameters
      for (auto expr : *expr_list_) {
        expr->icg(data, tac);
        expr_var_names.push_back(data.expr_return_var);

        if (!givenParams.empty()) {
          givenParams += ", ";
        }
        givenParams += tostr(data.expr_return_type);

        if (tostr(data.expr_return_type) != *iterFormal) {
          // Type of actual parameter does not match the type of formal parameter
          wrongParameters = true;
        }
        
        if (iterFormal != formal_parameters.end()) {
          ++iterFormal;
        }
      }

      for (auto expr_var : expr_var_names) {
        tac.append(TAC::InstrType::APARAM, expr_var);
      }

      if (wrongParameters) {
        warning_msg("Parameters mismatch for method call '" + id_ + "': expected " + expectedParams + "; given " + givenParams);
      }

      tac.append(TAC::InstrType::CALL, id_);
      data.expr_return_var = id_;
    }
  }

 protected:
  std::string id_;
  std::list<ExprNode*>* expr_list_;
};

class AssignStmNode : public StmNode {
 public:
  AssignStmNode(VariableExprNode* lvar, ExprNode* expr)
      : lvar_(lvar), expr_(expr) {}

  virtual const std::string str() const override {
    return std::string("(= ") + tostr(lvar_) + ' ' + tostr(expr_) + ')';
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    lvar_->icg(data, tac);
    std::string var = data.expr_return_var;
    ValueType var_type = data.expr_return_type;

    expr_->icg(data, tac);
    std::string exp_var = data.expr_return_var;
    ValueType exp_type = data.expr_return_type;

    tac.append(TAC::InstrType::ASSIGN, exp_var, var);

    if ((var_type == ValueType::IntVal && exp_type == ValueType::RealVal) ||
        (var_type == ValueType::RealVal && exp_type == ValueType::IntVal)) {
      warning_msg("Type mismatch in assigning to variable '" + var + "'.");
    }
  }

 protected:
  VariableExprNode* lvar_;
  ExprNode* expr_;
};

class IncrDecrStmNode : public StmNode {};

class IncrStmNode : public IncrDecrStmNode {
 public:
  IncrStmNode(VariableExprNode* var) : var_(var) {}

  virtual const std::string str() const override {
    return std::string("(++ ") + tostr(var_) + ')';
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    var_->icg(data, tac);
    if (data.expr_return_type == ValueType::RealVal) {
      tac.append(TAC::InstrType::ADD, data.expr_return_var, "1.0",
                 data.expr_return_var);
    } else {
      tac.append(TAC::InstrType::ADD, data.expr_return_var, "1",
                 data.expr_return_var);
    }
  }

 protected:
  VariableExprNode* var_;
};

class DecrStmNode : public IncrDecrStmNode {
 public:
  DecrStmNode(VariableExprNode* var) : var_(var) {}

  virtual const std::string str() const override {
    return std::string("(-- ") + tostr(var_) + ')';
  }

  virtual void icg(Data& data, TAC& tac) const override {
    var_->icg(data, tac);
    tac.append(TAC::InstrType::SUB, data.expr_return_var,
               data.expr_return_type == ValueType::RealVal ? "1.0" : "1",
               data.expr_return_var);
  }

 protected:
  VariableExprNode* var_;
};

class ReturnStmNode : public StmNode {
 public:
  ReturnStmNode() : expr_(nullptr) {}

  ReturnStmNode(ExprNode* expr) : expr_(expr) {}

  virtual const std::string str() const override {
    std::string s("(RET ");
    if (expr_ != nullptr) {
      s += tostr(expr_);
    }
    s += ')';
    return s;
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    if (expr_ != nullptr) {
      expr_->icg(data, tac);
      tac.append(TAC::InstrType::ASSIGN, data.expr_return_var,
                 data.method_name);
    }
    tac.append(TAC::InstrType::RETURN);

    SymbolTable::Entry* entry = data.sym_table.lookup("", data.method_name);
    if (entry != nullptr) {
      if ((expr_ != nullptr && entry->value_type == ValueType::VoidVal) ||
          (expr_ == nullptr && entry->value_type != ValueType::VoidVal)) {
        error_msg("Return statement in '" + data.method_name +
                  "' does not match return value.");
      }
      if (expr_ != nullptr && data.expr_return_type != entry->value_type) {
        warning_msg("Returned value in '" + data.method_name +
                    "' does not match return type.");
      }
    }
  }

 protected:
  ExprNode* expr_;
};

class BreakStmNode : public StmNode {
 public:
  BreakStmNode() {}

  virtual const std::string str() const override {
    return std::string("(BREAK)");
  }

  virtual void icg(Data& data, TAC& tac) const override {
    if (!data.for_label_no.empty()) {
      std::string label = tac.label_name("for_end", data.for_label_no.top());
      tac.append(TAC::InstrType::GOTO, label);
    } else {
      error_msg("Break statement used outside a loop.");
    }
  }
};

class ContinueStmNode : public StmNode {
 public:
  ContinueStmNode() {}

  virtual const std::string str() const override {
    return std::string("(CONTINUE)");
  }

  virtual void icg(Data& data, TAC& tac) const override {
    if (!data.for_label_no.empty()) {
      std::string label = tac.label_name("for_incr", data.for_label_no.top());
      tac.append(TAC::InstrType::GOTO, label);
    } else {
      error_msg("Continue statement used outside a loop.");
    }
  }
};

class BlockStmNode : public StmNode {
 public:
  BlockStmNode(std::list<StmNode*>* stms) : stms_(stms) {}

  virtual const std::string str() const override {
    std::string s("(BLOCK");
    for (auto stm : *stms_) {
      s += " " + tostr(stm);
    }
    s += ")";
    return s;
  };

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    for (auto stm : *stms_) {
      stm->icg(data, tac);
    }
  }

 protected:
  std::list<StmNode*>* stms_;
};

class IfStmNode : public StmNode {
 public:
  IfStmNode(ExprNode* expr, BlockStmNode* stm_if, BlockStmNode* stm_else)
      : expr_(expr), stm_if_(stm_if), stm_else_(stm_else) {}

  virtual const std::string str() const override {
    std::string s("(IF ");
    s += tostr(expr_) + tostr(stm_if_);
    if (stm_else_ != nullptr) {
      s += tostr(stm_else_);
    }
    s += ')';
    return s;
  };

  virtual void icg(Data& data, TAC& tac) const override {
    std::string lab_true_block = tac.label_name("true_block", data.label_no);
    std::string lab_if_end = tac.label_name("if_end", data.label_no);
    data.label_no++;

    expr_->icg(data, tac);
    if (data.expr_return_type != ValueType::IntVal) {
      warning_msg(
          "Type mismatch in if statement (conditional statement is not an "
          "integer value).");
    }
    tac.append(TAC::InstrType::NE, data.expr_return_var, "0", lab_true_block);
    if (stm_else_ != nullptr) {
      stm_else_->icg(data, tac);
    }
    tac.append(TAC::InstrType::GOTO, lab_if_end);

    tac.label_next_instr(lab_true_block);
    stm_if_->icg(data, tac);

    tac.label_next_instr(lab_if_end);
  }

 protected:
  ExprNode* expr_;
  BlockStmNode *stm_if_, *stm_else_;
};

class ForStmNode : public StmNode {
 public:
  ForStmNode(AssignStmNode* assign, ExprNode* expr, IncrDecrStmNode* inc_dec,
             BlockStmNode* stms_)
      : assign_(assign), expr_(expr), inc_dec_(inc_dec), stms_(stms_) {}

  virtual const std::string str() const override {
    return "(FOR " + tostr(assign_) + tostr(expr_) + tostr(inc_dec_) +
           tostr(stms_) + ')';
  };

  virtual void icg(Data& data, TAC& tac) const override {
    std::string lab_for_expr = tac.label_name("for_expr", data.label_no);
    std::string lab_for_incr = tac.label_name("for_incr", data.label_no);
    std::string lab_for_end = tac.label_name("for_end", data.label_no);
    data.for_label_no.push(data.label_no);

    data.label_no++;

    assign_->icg(data, tac);

    tac.label_next_instr(lab_for_expr);
    expr_->icg(data, tac);
    if (data.expr_return_type != ValueType::IntVal) {
      warning_msg(
          "Type mismatch in for statement (conditional statement is not an "
          "integer value).");
    }

    tac.append(TAC::InstrType::EQ, data.expr_return_var, "0", lab_for_end);
    stms_->icg(data, tac);

    tac.label_next_instr(lab_for_incr);
    inc_dec_->icg(data, tac);
    tac.append(TAC::InstrType::GOTO, lab_for_expr);

    tac.label_next_instr(lab_for_end);
    data.for_label_no.pop();
  }

 protected:
  AssignStmNode* assign_;
  ExprNode* expr_;
  IncrDecrStmNode* inc_dec_;
  BlockStmNode* stms_;
};

/////////////////////////////////////////////////////////////////////////////////

class MethodNode : public Node {
 public:
  MethodNode(ValueType return_type, std::string id,
             std::list<ParameterNode*>* params,
             std::list<VariableDeclarationNode*>* var_decls,
             std::list<StmNode*>* stms)
      : return_type_(return_type),
        id_(id),
        params_(params),
        var_decls_(var_decls),
        stms_(stms) {}

  virtual const std::string str() const override {
    std::string s = "(METHOD " + tostr(return_type_) + ' ' + id_ + ' ';
    for (auto p : *params_) {
      s += tostr(p);
    }
    for (auto vds : *var_decls_) {
      s += tostr(vds);
    }
    for (auto stm : *stms_) {
      s += tostr(stm);
    }
    s += ')';
    return s;
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    std::string signature;
    data.method_name = id_;
    tac.label_next_instr(data.method_name);
    for (auto pd : *params_) {
      pd->icg(data, tac);
      if (!signature.empty()) {
        signature += "::";
      }
      signature += tostr(pd->get_type());
    }
    add_to_symbol_table(data.sym_table, EntryType::Method, "", id_,
                        return_type_, signature);
    // Prevent someone from using a variable with the same name as the enclosing
    // method.
    add_to_symbol_table(data.sym_table, EntryType::Method, id_, id_,
                        return_type_, signature);

    for (auto vd : *var_decls_) {
      vd->icg(data, tac);
    }
    for (auto stm : *stms_) {
      stm->icg(data, tac);
    }
    if (tac.last_instr_type() != TAC::InstrType::RETURN) {
      tac.append(TAC::InstrType::RETURN);
    }
    data.method_name = "";
  }

 protected:
  ValueType return_type_;
  std::string id_;
  std::list<ParameterNode*>* params_;
  std::list<VariableDeclarationNode*>* var_decls_;
  std::list<StmNode*>* stms_;
};

class ProgramNode : public Node {
 public:
  ProgramNode(std::string id, std::list<VariableDeclarationNode*>* var_decls,
              std::list<MethodNode*>* method_decls)
      : id_(id), var_decls_(var_decls), method_decls_(method_decls) {}

  virtual const std::string str() const override {
    std::string s = "(CLASS " + id_;
    if (var_decls_ != nullptr) {
      for (auto v : *var_decls_) {
        s += " " + tostr(v);
      }
    }
    if (method_decls_ != nullptr) {
      for (auto m : *method_decls_) {
        s += " " + tostr(m);
      }
    }
    s += ')';
    return s;
  }

  virtual void icg(Data& data, TAC& tac) const override {
    // Provided.
    for (auto vd : *var_decls_) {
      vd->icg(data, tac);
    }
    tac.append(TAC::InstrType::GOTO, "main");
    for (auto md : *method_decls_) {
      md->icg(data, tac);
    }

    SymbolTable::Entry* entry = data.sym_table.lookup("", "main");
    if (entry == nullptr) {
      error_msg("Main method is missing.");
    }
  }

 protected:
  std::string id_;
  std::list<VariableDeclarationNode*>* var_decls_;
  std::list<MethodNode*>* method_decls_;
};

#endif  // DECAFPARSER_AST_H
