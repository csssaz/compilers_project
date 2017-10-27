
#include "hparser.h"

using namespace std;

int HParser::parse() {
  set_AST(program());
  return 0;
}

ProgramNode* HParser::program() {
  match(decaf::token_type::kwClass);
  string name = token_.lexeme;
  match(decaf::token_type::Identifier);
  match(decaf::token_type::ptLBrace);
  auto list_vdn = variable_declarations();
  auto list_mdn = method_declarations();
  match(decaf::token_type::ptRBrace);
  match(decaf::token_type::EOI);
  return new ProgramNode(name, list_vdn, list_mdn);
}

list<VariableDeclarationNode*>* HParser::variable_declarations() {
  auto list_vdn = new list<VariableDeclarationNode*>();
  while (token_.type == decaf::token_type::kwInt ||
         token_.type == decaf::token_type::kwReal) {
    ValueType type = this->type();
    auto list_v = variable_list();
    list_vdn->push_back(new VariableDeclarationNode(type, list_v));
  }
  return list_vdn;
}

ValueType HParser::type() {
  ValueType valuetype = ValueType::VoidVal;
  if (token_.type == decaf::token_type::kwInt) {
    match(decaf::token_type::kwInt);
    valuetype = ValueType::IntVal;
  } else if (token_.type == decaf::token_type::kwReal) {
    match(decaf::token_type::kwReal);
    valuetype = ValueType::RealVal;
  } else {
    error(decaf::token_type::kwInt);
  }
  return valuetype;
}

list<VariableExprNode*>* HParser::variable_list() {
  auto list_v = new list<VariableExprNode*>();
  list_v->push_back(variable());
  while (token_.type == decaf::token_type::ptComma) {
    match(decaf::token_type::ptComma);
    list_v->push_back(variable());
  }
  match(decaf::token_type::ptSemicolon);
  return list_v;
}

VariableExprNode* HParser::variable() {
  auto node = new VariableExprNode(token_.lexeme);
  match(decaf::token_type::Identifier);
  return node;
}

list<MethodNode*>* HParser::method_declarations() {
  list<MethodNode*>* list_mdn = new list<MethodNode*>();
  list_mdn->push_back(method_declaration());
  while (token_.type == decaf::token_type::kwStatic) {
    list_mdn->push_back(method_declaration());
  }
  return list_mdn;
}

MethodNode* HParser::method_declaration() {
  match(decaf::token_type::kwStatic);
  auto type = method_return_type();
  string method_name = token_.lexeme;
  match(decaf::token_type::Identifier);
  match(decaf::token_type::ptLParen);
  auto params = parameters();
  match(decaf::token_type::ptRParen);
  match(decaf::token_type::ptLBrace);
  auto list_vdn = variable_declarations();
  auto list_stm = statement_list();
  match(decaf::token_type::ptRBrace);
  return new MethodNode(type, method_name, params, list_vdn, list_stm);
}

ValueType HParser::method_return_type() {
  if (token_.type == decaf::token_type::kwVoid) {
    match(decaf::token_type::kwVoid);
    return ValueType::VoidVal;
  }
  return type();
}

list<ParameterNode*>* HParser::parameters() {
  if (token_.type == decaf::token_type::kwInt ||
      token_.type == decaf::token_type::kwReal) {
    return parameter_list();
  }
  return new list<ParameterNode*>();
}

list<ParameterNode*>* HParser::parameter_list() {
  auto param_list = new list<ParameterNode*>();
  param_list->push_back(new ParameterNode(type(), variable()));
  while (token_.type == decaf::token_type::ptComma) {
    match(decaf::token_type::ptComma);
    param_list->push_back(new ParameterNode(type(), variable()));
  }
  return param_list;
}

list<StmNode*>* HParser::statement_list() {
  auto stm_list = new list<StmNode*>();
  while (token_.type == decaf::token_type::kwIf ||
         token_.type == decaf::token_type::kwFor ||
         token_.type == decaf::token_type::kwReturn ||
         token_.type == decaf::token_type::kwBreak ||
         token_.type == decaf::token_type::kwContinue ||
         token_.type == decaf::token_type::ptLBrace ||
         token_.type == decaf::token_type::Identifier) {
    stm_list->push_back(statement());
  }
  return stm_list;
}

StmNode* HParser::statement() {
  switch (token_.type) {
    case (decaf::token_type::kwIf): {
      match(decaf::token_type::kwIf);
      match(decaf::token_type::ptLParen);
      ExprNode* expr = expr_or();
      match(decaf::token_type::ptRParen);
      BlockStmNode* stm_if = statement_block();
      BlockStmNode* stm_else = nullptr;
      // optional_else added here:
      if (token_.type == decaf::token_type::kwElse) {
        match(decaf::token_type::kwElse);
        stm_else = statement_block();
      }
      return new IfStmNode(expr, stm_if, stm_else);
    }
    case (decaf::token_type::kwFor): {
      match(decaf::token_type::kwFor);
      match(decaf::token_type::ptLParen);
      VariableExprNode* var_new = variable();
      match(decaf::token_type::OpAssign);
      ExprNode* value = expr_or();
      match(decaf::token_type::ptSemicolon);
      ExprNode* condition = expr_or();
      match(decaf::token_type::ptSemicolon);
      VariableExprNode* var2 = variable();
      IncrDecrStmNode* incr_decr;
      if (token_.type == decaf::token_type::OpArtInc) {
        match(decaf::token_type::OpArtInc);
        incr_decr = new IncrStmNode(var2);
      } else if (token_.type == decaf::token_type::OpArtDec) {
        match(decaf::token_type::OpArtDec);
        incr_decr = new DecrStmNode(var2);
      } else {
        error(decaf::token_type::OpArtInc);
      }
      match(decaf::token_type::ptRParen);
      BlockStmNode* block = statement_block();
      return new ForStmNode(new AssignStmNode(var_new, value), condition,
                            incr_decr, block);
    }
    case (decaf::token_type::kwReturn): {
      match(decaf::token_type::kwReturn);
      // optional_expr handeled here
      ExprNode* opt_expr;
      if (token_.type != decaf::token_type::ptSemicolon) {
        opt_expr = expr_or();
      }
      match(decaf::token_type::ptSemicolon);
      return new ReturnStmNode(opt_expr);
    }
    case (decaf::token_type::kwBreak): {
      match(decaf::token_type::kwBreak);
      match(decaf::token_type::ptSemicolon);
      return new BreakStmNode();
    }
    case (decaf::token_type::kwContinue): {
      match(decaf::token_type::kwContinue);
      match(decaf::token_type::ptSemicolon);
      return new ContinueStmNode();
    }
    case (decaf::token_type::ptLBrace): {
      return statement_block();
    }
    case (decaf::token_type::Identifier): {
      return id_start_stm();
    }
    default:
      break;
  }
  return new ReturnStmNode(nullptr);
}

StmNode* HParser::id_start_stm() {
  string id = token_.lexeme;
  match(decaf::token_type::Identifier);
  switch (token_.type) {
    case decaf::token_type::ptLParen: {
      match(decaf::token_type::ptLParen);
      auto ex_list = expr_list();
      match(decaf::token_type::ptRParen);
      match(decaf::token_type::ptSemicolon);
      return new MethodCallExprStmNode(id, ex_list);
    }
    case decaf::token_type::OpAssign: {
      match(decaf::token_type::OpAssign);
      auto expr = expr_or();
      match(decaf::token_type::ptSemicolon);
      return new AssignStmNode(new VariableExprNode(id), expr);
    }
    case decaf::token_type::OpArtInc: {
      match(decaf::token_type::OpArtInc);
      match(decaf::token_type::ptSemicolon);
      return new IncrStmNode(new VariableExprNode(id));
    }
    case decaf::token_type::OpArtDec: {
      match(decaf::token_type::OpArtDec);
      match(decaf::token_type::ptSemicolon);
      return new DecrStmNode(new VariableExprNode(id));
    }
    default:
      break;
  }
}

BlockStmNode* HParser::statement_block() {
  match(decaf::token_type::ptLBrace);
  auto stm_list = statement_list();
  match(decaf::token_type::ptRBrace);
  return new BlockStmNode(stm_list);
}

// expr_list and more_expressions are in the same method.
list<ExprNode*>* HParser::expr_list() {
  list<ExprNode*>* expr_list = new list<ExprNode*>();
  expr_list->push_front(expr_or());
  while (token_.type == decaf::token_type::ptComma) {
    match(decaf::token_type::ptComma);
    expr_list->push_back(expr_or());
  }
  return expr_list;
}

ExprNode* HParser::expr_or() {
  ExprNode* lhs = expr_and();
  return expr_or_(lhs);
}

ExprNode* HParser::expr_or_(ExprNode* lhs) {
  if (token_.type == decaf::token_type::OpLogOr) {
    match(decaf::token_type::OpLogOr);
    ExprNode* rhs = expr_and();
    OrExprNode* node = new OrExprNode(lhs, rhs);
    return expr_or_(node);
  }
  return lhs;
}

ExprNode* HParser::expr_and() {
  ExprNode* lhs = expr_eq();
  return expr_and_(lhs);
}

ExprNode* HParser::expr_and_(ExprNode* lhs) {
  if (token_.type == decaf::token_type::OpLogAnd) {
    match(decaf::token_type::OpLogAnd);
    ExprNode* rhs = expr_eq();
    AndExprNode* node = new AndExprNode(lhs, rhs);
    return expr_and_(node);
  }
  return lhs;
}

ExprNode* HParser::expr_eq() {
  ExprNode* lhs = expr_rel();
  return expr_eq_(lhs);
}

ExprNode* HParser::expr_eq_(ExprNode* lhs) {
  if (token_.type == decaf::token_type::OpRelEQ) {
    match(decaf::token_type::OpRelEQ);
    ExprNode* rhs = expr_rel();
    EqExprNode* node = new EqExprNode(lhs, rhs);
    return expr_eq_(node);
  }
  if (token_.type == decaf::token_type::OpRelNEQ) {
    match(decaf::token_type::OpRelNEQ);
    ExprNode* rhs = expr_rel();
    NeqExprNode* node = new NeqExprNode(lhs, rhs);
    return expr_eq_(node);
  }
  return lhs;
}

ExprNode* HParser::expr_rel() {
  ExprNode* lhs = expr_add();
  return expr_rel_(lhs);
}

ExprNode* HParser::expr_rel_(ExprNode* lhs) {
  if (token_.type == decaf::token_type::OpRelLT) {
    match(decaf::token_type::OpRelLT);
    ExprNode* rhs = expr_add();
    LtExprNode* node = new LtExprNode(lhs, rhs);
    return expr_rel_(node);
  }
  if (token_.type == decaf::token_type::OpRelLTE) {
    match(decaf::token_type::OpRelLTE);
    ExprNode* rhs = expr_add();
    LteExprNode* node = new LteExprNode(lhs, rhs);
    return expr_rel_(node);
  }
  if (token_.type == decaf::token_type::OpRelGT) {
    match(decaf::token_type::OpRelGT);
    ExprNode* rhs = expr_add();
    GtExprNode* node = new GtExprNode(lhs, rhs);
    return expr_rel_(node);
  }
  if (token_.type == decaf::token_type::OpRelGTE) {
    match(decaf::token_type::OpRelGTE);
    ExprNode* rhs = expr_add();
    GteExprNode* node = new GteExprNode(lhs, rhs);
    return expr_rel_(node);
  }
  return lhs;
}

ExprNode* HParser::expr_add() {
  ExprNode* lhs = expr_mult();
  return expr_add_(lhs);
}

ExprNode* HParser::expr_add_(ExprNode* lhs) {
  if (token_.type == decaf::token_type::OpArtPlus) {
    match(decaf::token_type::OpArtPlus);
    ExprNode* rhs = expr_mult();
    PlusExprNode* node = new PlusExprNode(lhs, rhs);
    return expr_add_(node);
  }
  if (token_.type == decaf::token_type::OpArtMinus) {
    match(decaf::token_type::OpArtMinus);
    ExprNode* rhs = expr_mult();
    MinusExprNode* node = new MinusExprNode(lhs, rhs);
    return expr_add_(node);
  }
  return lhs;
}

ExprNode* HParser::expr_mult() {
  ExprNode* lhs = expr_unary();
  return expr_mult_(lhs);
}

ExprNode* HParser::expr_mult_(ExprNode* lhs) {
  if (token_.type == decaf::token_type::OpArtMult) {
    match(decaf::token_type::OpArtMult);
    ExprNode* rhs = expr_unary();
    MultiplyExprNode* node = new MultiplyExprNode(lhs, rhs);
    return expr_mult_(node);
  }
  if (token_.type == decaf::token_type::OpArtDiv) {
    match(decaf::token_type::OpArtDiv);
    ExprNode* rhs = expr_unary();
    DivideExprNode* node = new DivideExprNode(lhs, rhs);
    return expr_mult_(node);
  }
  if (token_.type == decaf::token_type::OpArtModulus) {
    match(decaf::token_type::OpArtModulus);
    ExprNode* rhs = expr_unary();
    ModulusExprNode* node = new ModulusExprNode(lhs, rhs);
    return expr_mult_(node);
  }
  return lhs;
}

ExprNode* HParser::expr_unary() {
  if (token_.type == decaf::token_type::OpArtPlus) {
    match(decaf::token_type::OpArtPlus);
    ExprNode* operand = expr_unary();
    PlusExprNode* node = new PlusExprNode(operand);
    return node;
  }
  if (token_.type == decaf::token_type::OpArtMinus) {
    match(decaf::token_type::OpArtMinus);
    ExprNode* operand = expr_unary();
    return new MinusExprNode(operand);
  }
  if (token_.type == decaf::token_type::OpLogNot) {
    match(decaf::token_type::OpLogNot);
    ExprNode* operand = expr_unary();
    return new NotExprNode(operand);
  }
  return factor();
}

ExprNode* HParser::factor() {
  if (token_.type == decaf::token_type::Number) {
    NumberExprNode* node = new NumberExprNode(token_.lexeme);
    match(decaf::token_type::Number);
    return node;
  }
  if (token_.type == decaf::token_type::ptLParen) {
    match(decaf::token_type::ptLParen);
    ExprNode* node = expr_or();
    match(decaf::token_type::ptRParen);
    return node;
  }
  string var_name = token_.lexeme;
  match(decaf::token_type::Identifier);
  if (token_.type == decaf::token_type::ptLParen) {
    match(decaf::token_type::ptLParen);
    list<ExprNode*>* expr_l = expr_list();
    match(decaf::token_type::ptRParen);
    return new MethodCallExprStmNode(var_name, expr_l);
  }
  return new VariableExprNode(var_name);
}

