////////////////////////////////////////////////////////////////////////////////////

%skeleton "lalr1.cc"
%require "3.0.4"
%defines
%define parser_class_name {parser_decaf}

%define api.token.constructor
%define api.value.type variant

%define parse.assert
%define parse.trace
%define parse.error verbose

%code requires
{
#include <string>
#include "ast.h"
class Parser;
}

%parse-param { Parser& driver }
%locations
%code
{
#include "parser.h"
}

%token EOI 0

%token kwClass
%token kwStatic
%token kwVoid
%token kwInt
%token kwReal
%token kwReturn
%token kwBreak
%token kwContinue
%token kwIf
%token kwElse
%token kwFor

%token ptLBrace
%token ptRBrace
%token ptComma
%token ptSemicolon
%token ptLParen
%token ptRParen

%token OpAssign
%token OpArtPlus
%token OpArtMinus
%token OpArtMult
%token OpArtDiv
%token OpArtModulus
%token OpArtInc
%token OpArtDec
%token OpRelEQ
%token OpRelNEQ
%token OpRelLT
%token OpRelLTE
%token OpRelGT
%token OpRelGTE
%token OpLogAnd
%token OpLogOr
%token OpLogNot

%token <std::string> Identifier
%token <std::string> Number
%token <std::string> ErrUnknown

%left OpLogOr
%left OpLogAnd
%left OpRelEQ OpRelNEQ
%left OpRelLT OpRelLTE OpRelGT OpRelGTE
%left OpArtPlus OpArtMinus
%left OpArtMult OpArtDiv OpArtModulus
%right OpLogNot UMINUS UPLUS

%type <std::list<VariableDeclarationNode*>*> variable_declarations
%type <ValueType> type
%type <std::list<VariableExprNode*>*> variable_list
%type <VariableExprNode*> variable
%type <ExprNode*> expression

%%

////////////////////////////////////////////////////////////////////////////////////

program: kwClass Identifier ptLBrace
             variable_declarations
             expression
         ptRBrace
         { std::list<ExprNode *> *expr_list = new std::list<ExprNode *>();
           expr_list->push_back( $5 );
           MethodCallExprStmNode *stm = new MethodCallExprStmNode("methodName", expr_list);
           std::list<StmNode *> *stm_list = new std::list<StmNode *>();
           stm_list->push_back( stm );
           MethodNode *method = new MethodNode( ValueType::IntVal, "metod", new std::list<ParameterNode *>(), new std::list<VariableDeclarationNode *>(), stm_list );
           std::list<MethodNode *> *method_decls = new std::list<MethodNode *>();
           method_decls->push_back( method );
           driver.set_AST( new ProgramNode( $2, $4, method_decls ) ); }

variable_declarations: variable_declarations type variable_list ptSemicolon
                      { $$ = $1; $$->push_back( new VariableDeclarationNode($2,$3) ); }
                    | { $$ = new std::list<VariableDeclarationNode*>(); }

type: kwInt  { $$ = ValueType::IntVal; }
    | kwReal { $$ = ValueType::RealVal; }

variable_list: variable
               { $$ = new std::list<VariableExprNode*>(); $$->push_back( $1 ); }
             | variable_list ptComma variable
               { $$ = $1; $$->push_back( $3 ); }

variable:  Identifier  { $$ = new VariableExprNode($1); }

expression: expression OpLogOr expression { $$ = new OrExprNode($1, $3); }
          | expression OpLogAnd expression { $$ = new AndExprNode($1, $3); }
          | expression OpRelEQ expression { $$ = new EqExprNode($1, $3); }
          | expression OpRelNEQ expression { $$ = new NeqExprNode($1, $3); }
          | expression OpRelLT expression { $$ = new LtExprNode($1, $3); }
          | expression OpRelLTE expression { $$ = new LteExprNode($1, $3); }
          | expression OpRelGT expression { $$ = new GtExprNode($1, $3); }
          | expression OpRelGTE expression { $$ = new GteExprNode($1, $3); }
          | expression OpArtPlus expression { $$ = new PlusExprNode($1, $3); }
          | expression OpArtMinus expression { $$ = new MinusExprNode($1, $3); }
          | expression OpArtMult expression { $$ = new MultiplyExprNode($1, $3); }
          | expression OpArtDiv expression { $$ = new DivideExprNode($1, $3); }
          | expression OpArtModulus expression { $$ = new ModulusExprNode($1, $3); }
          | OpArtPlus expression %prec UPLUS { $$ = new PlusExprNode($2); }
          | OpArtMinus expression %prec UMINUS { $$ = new MinusExprNode($2); }
          | OpLogNot expression { $$ = new NotExprNode($2); }
          | variable { $$ = $1; }
          | Number { $$ = new NumberExprNode($1); }
          | ptLParen expression ptRParen { $$ = $2; }

%%

////////////////////////////////////////////////////////////////////////////////////


void yy::parser_decaf::error(const yy::location& l, const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}
