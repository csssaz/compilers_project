//
// Compiler project -- main driver for part 1
//
#include "HLexer.h"

using namespace std;

HLexer::HLexer(std::istream& is, SymbolTable& symbol_table)
    : Lexer(is, symbol_table), line_no_(1) {
  is_.get(c_);
}

void HLexer::get_next(Token& token) {
  token.lexeme.clear();
  token.entry = nullptr;

  while (is_.good() && isspace(c_)) {
    if (c_ == '\n') {
      ++line_no_;
    }
    is_.get(c_);
  }

  token.line = line_no_;

  if (!is_.good()) {
    token.type = Tokentype::EOI;
    return;
  }

  switch (c_) {
    case '{':
      token.type = Tokentype::ptLBrace;
      token.lexeme.push_back(c_);
      is_.get(c_);
      break;
    case '}':
      token.type = Tokentype::ptRBrace;
      token.lexeme.push_back(c_);
      is_.get(c_);
      break;
    default:
      token.type = Tokentype::ErrUnknown;
      token.lexeme.push_back(c_);
      is_.get(c_);
      break;
  }
}

std::string HLexer::get_name() const { return "handmade"; }

HLexer::~HLexer()
{
}
