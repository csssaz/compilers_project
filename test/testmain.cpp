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
  REQUIRE(regex::RegexMatcher(regex_1).postfix_regex_ == "ab.cd.|");
  std::string regex_2 = "a.(b|c).d";
  REQUIRE(regex::RegexMatcher(regex_2).postfix_regex_ == "abc|.d.");
  std::string regex_3 = "a.b.c.d";
  REQUIRE(regex::RegexMatcher(regex_3).postfix_regex_  == "ab.c.d.");
  std::string regex_4 = "a";
  REQUIRE(regex::RegexMatcher(regex_4).postfix_regex_ == "a");
  std::string regex_5 = "a*";
  REQUIRE(regex::RegexMatcher(regex_5).postfix_regex_ == "a*");
  std::string regex_6 = "a*.b";
  REQUIRE(regex::RegexMatcher(regex_6).postfix_regex_ == "a*b.");
  std::string regex_7 = "b.z.a*.d.(a.b)*";
  REQUIRE(regex::RegexMatcher(regex_7).postfix_regex_ == "bz.a*.d.ab.*.");
  std::string regex_8 = "(a.b|c*.d)*";
  REQUIRE(regex::RegexMatcher(regex_8).postfix_regex_ == "ab.c*d.|*");
  std::string regex_9 = "(1|2|3|4)*";
  REQUIRE(regex::RegexMatcher(regex_9).postfix_regex_ == "12|3|4|*");
}

TEST_CASE("regex matching_1") {
  std::string regex = "(a.b|c*.d)*";
  regex::RegexMatcher compiled_regex(regex);
  std::vector<std::string> test_correct({
      "ab",
      "",
      "abd",
      "abcccccccccccccccd",
      "abababcccdabd",
      "abddddddddddddcccccdcdcdcdab",
      "ccccccccccccdddcdcdcdcddddcccdddccdcddcccdcdcdddd"
  });
  for (auto test_string : test_correct) {
    REQUIRE(compiled_regex.Matches(test_string) == true);
  }

  std::vector<std::string> test_incorrect({
    "a",
    "abababccccccccccccc",
    "abccccccdddccdda",
    "c",
    "ababababababccccdddccdddabz",
    "12",
    "abababababbb",
    "abccccc",
  });

  for (auto test_string : test_incorrect) {
    REQUIRE(compiled_regex.Matches(test_string) == false);
  }
}

TEST_CASE("regex matching_2") {
  std::string regex =
      "(1|2|3|4|5|6|7|8|9).(0|1|2|3|4|5|6|7|8|9)*.,.(0|1|2|3|4|5|6|7|8|9)";
  regex::RegexMatcher compiled_regex(regex);
  std::vector<std::string> test_correct({
      "12334123339990000,1",
      "121212,9",
      "1,0",
      "9,0",
      "129349404,6",
  });
  for (auto test_string : test_correct) {
    REQUIRE(compiled_regex.Matches(test_string) == true);
  }

  std::vector<std::string> test_incorrect({
      "1233412333999102",
      "1234,12",
      "0,01212",
      "121212,6666",
      "121212,",
      "0,123",
      "899",
  });
  for (auto test_string : test_incorrect) {
    REQUIRE(compiled_regex.Matches(test_string) == false);
  }
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

TEST_CASE("flex parser_2") {
  SymbolTable sym;
  std::string test_string = "a = 5;";
  std::stringstream sf(test_string);

  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
    std::make_tuple("a", 1, Tokentype::Identifier),
    std::make_tuple("=", 1, Tokentype::OpAssign),
    std::make_tuple("5", 1, Tokentype::Number),
    std::make_tuple(";", 1, Tokentype::ptSemicolon)
  };

  Lexer* lexer;
  lexer = new FLexer(sf, sym);
  Token token;
  for (auto test_tuple : expected_output) {
    lexer->get_next(token);
    REQUIRE(token.lexeme == std::get<0>(test_tuple));
    REQUIRE(token.line == std::get<1>(test_tuple));
    REQUIRE(token.type == std::get<2>(test_tuple));
  }
}
