// Compilers Fall 2017. Project -- part I.
#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include "symbol_table.h"

enum Tokentype {
  Identifier = 258,
  Number,
  // Relational operators
  OpRelEQ,
  OpRelNEQ,
  OpRelLT,
  OpRelLTE,
  OpRelGT,
  OpRelGTE,
  // Arithmetic operators
  OpArtInc,
  OpArtDec,
  OpArtPlus,
  OpArtMinus,
  OpArtMult,
  OpArtDiv,
  OpArtModulus,
  // Logical operators
  OpLogAnd,
  OpLogOr,
  OpLogNot,
  // Assignment Operators
  OpAssign,
  // Keywords
  kwClass,
  kwStatic,
  kwVoid,
  kwIf,
  kwElse,
  kwFor,
  kwReturn,
  kwBreak,
  kwContinue,
  kwInt,
  kwReal,
  // Punctuation marks
  ptLBrace,
  ptRBrace,
  ptLBracket,
  ptRBracket,
  ptLParen,
  ptRParen,
  ptSemicolon,
  ptComma,
  // End of input
  EOI,
  // Lexical errors
  ErrUnknown
};

#define OUTPUT_TT(tt) case Tokentype::tt: os << #tt; break;

inline std::ostream& operator<<(std::ostream& os, Tokentype type) {
  switch (type) {
    OUTPUT_TT(Identifier)
    OUTPUT_TT(Number)
    OUTPUT_TT(OpRelEQ)
    OUTPUT_TT(OpRelNEQ)
    OUTPUT_TT(OpRelLT)
    OUTPUT_TT(OpRelLTE)
    OUTPUT_TT(OpRelGT)
    OUTPUT_TT(OpRelGTE)
    OUTPUT_TT(OpArtInc)
    OUTPUT_TT(OpArtDec)
    OUTPUT_TT(OpArtPlus)
    OUTPUT_TT(OpArtMinus)
    OUTPUT_TT(OpArtMult)
    OUTPUT_TT(OpArtDiv)
    OUTPUT_TT(OpArtModulus)
    OUTPUT_TT(OpLogAnd)
    OUTPUT_TT(OpLogOr)
    OUTPUT_TT(OpLogNot)
    OUTPUT_TT(OpAssign)
    OUTPUT_TT(kwClass)
    OUTPUT_TT(kwStatic)
    OUTPUT_TT(kwVoid)
    OUTPUT_TT(kwIf)
    OUTPUT_TT(kwElse)
    OUTPUT_TT(kwFor)
    OUTPUT_TT(kwReturn)
    OUTPUT_TT(kwBreak)
    OUTPUT_TT(kwContinue)
    OUTPUT_TT(kwInt)
    OUTPUT_TT(kwReal)
    OUTPUT_TT(ptLBrace)
    OUTPUT_TT(ptRBrace)
    OUTPUT_TT(ptLBracket)
    OUTPUT_TT(ptRBracket)
    OUTPUT_TT(ptLParen)
    OUTPUT_TT(ptRParen)
    OUTPUT_TT(ptSemicolon)
    OUTPUT_TT(ptComma)
    OUTPUT_TT(EOI)
    OUTPUT_TT(ErrUnknown)
  }
  return os;
}

// Token
struct Token {
  Tokentype type;             // Type of the token.
  std::string lexeme;         // Matched lexeme.
  int line;                   // Line number in file where token is.
  SymbolTable::Entry* entry;  // Entry in symbol table.
};

#endif //LEXER_TOKEN_H
