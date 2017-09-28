//
// Compiler project -- main driver for part 1
//
#include "hlexer.h"

using namespace std;

HLexer::HLexer(std::istream& is, SymbolTable& symbol_table)
    : Lexer(is, symbol_table), lexer_(is) {
}

void HLexer::get_next(Token& token) {
  
  int token_no = lexer_.NextToken();
  token.lexeme.clear();
  token.type = static_cast<Tokentype>(token_no);
  token.line = lexer_.line_no();
  if (token.type != Tokentype::EOI)
    token.lexeme = lexer_.GetLexeme();

  if (token.type == Tokentype::Identifier || token.type == Tokentype::Number) {
    token.entry = symbol_table_.lookup(token.lexeme);
    if (token.entry == nullptr) {
      SymbolTable::Entry entry{token.lexeme};
      token.entry = symbol_table_.add(entry);
    }
  } else {
    token.entry = nullptr;
  }
}
std::string HLexer::get_name() const { return "handmade"; }

HLexer::~HLexer()
{
}
