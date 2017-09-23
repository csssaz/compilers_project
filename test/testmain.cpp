#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <string>
#include "catch.hpp"
#include "flexer.h"
#include "hlexer.h"
#include "regex.h"

TEST_CASE("infix to postfix") {
  std::string regex_1 = "a.b|c.d";
  REQUIRE(regex::InfixToPostfix(regex_1) == "ab.cd.|");
  std::string regex_2 = "a.(b|c).d";
  REQUIRE(regex::InfixToPostfix(regex_2) == "abc|.d.");
  std::string regex_3 = "a.b.c.d";
  REQUIRE(regex::InfixToPostfix(regex_3) == "ab.c.d.");
  std::string regex_4 = "a";
  REQUIRE(regex::InfixToPostfix(regex_4) == "a");
  std::string regex_5 = "a*";
  REQUIRE(regex::InfixToPostfix(regex_5) == "a*");
  std::string regex_6 = "a*.b";
  REQUIRE(regex::InfixToPostfix(regex_6) == "a*b.");
  std::string regex_7 = "b.z.a*.d.(a.b)*";
  REQUIRE(regex::InfixToPostfix(regex_7) == "bz.a*.d.ab.*.");
  std::string regex_8 = "(a.b|c*.d)*";
  REQUIRE(regex::InfixToPostfix(regex_8) == "ab.c*d.|*");
  std::string regex_9 = "(1|2|3|4)*";
  REQUIRE(regex::InfixToPostfix(regex_9) == "12|3|4|*");
}


TEST_CASE("flex parser") {
  SymbolTable sym;
  std::string test_string = "1345.13";
  std::stringstream sf(test_string);
  Lexer* lexer;
  lexer = new FLexer(sf, sym);
  Token token;
  lexer->get_next(token);
  REQUIRE(token.type == Tokentype::Number);
  REQUIRE(token.line == 1);
  REQUIRE(token.lexeme == test_string);
}
