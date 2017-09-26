// Compilers Fall 2017. Project -- part I.
#ifndef LEXER_HLEXER_H
#define LEXER_HLEXER_H

#include "lexer.h"
#include "regex.h"

class HLexer : public Lexer {
 public:
  HLexer(std::istream& is, SymbolTable& symbol_table);
  virtual void get_next(Token& token);
  virtual std::string get_name() const;
  virtual ~HLexer();

 private:
  regex::RegexMatcher lexer_;
};

#endif //LEXER_HLEXER_H
