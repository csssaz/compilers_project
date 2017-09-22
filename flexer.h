// Compilers Fall 2017. Project -- part I.
#ifndef LEXER_FLEXER_H
#define LEXER_FLEXER_H

#include <FlexLexer.h>
#include "Lexer.h"

class FLexer : public Lexer {
 public:
  FLexer(std::istream& is, SymbolTable& symbol_table)
      : Lexer(is, symbol_table), lexer_(&is) {}

  virtual void get_next(Token& token) {
    int token_no = lexer_.yylex();
    token.type =
        (token_no == 0) ? Tokentype::EOI : static_cast<Tokentype>(token_no);
    token.lexeme = lexer_.YYText();
    token.line = lexer_.lineno();
    if (token.type == Tokentype::Identifier ||
        token.type == Tokentype::Number) {
      token.entry = symbol_table_.lookup(lexer_.YYText());
      if (token.entry == nullptr) {
        SymbolTable::Entry entry{lexer_.YYText()};
        token.entry = symbol_table_.add(entry);
      }
    } else {
      token.entry = nullptr;
    }
  }

  virtual std::string get_name() const { return "flex"; }

  virtual ~FLexer() {}

 private:
  yyFlexLexer lexer_;
};

#endif  // LEXER_FLEXER_H
