#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <string>
#include "catch.hpp"
#include "flexer.h"
#include "hlexer.h"
#include "regex.h"

TEST_CASE("infix to postfix") {
  std::string regex_1 = "ab|cd";
  REQUIRE(regex::RegexMatcher(regex_1).postfix_regex() == "ab.cd.|");
  std::string regex_2 = "a(b|c)d";
  REQUIRE(regex::RegexMatcher(regex_2).postfix_regex() == "abc|.d.");
  std::string regex_3 = "abcd";
  REQUIRE(regex::RegexMatcher(regex_3).postfix_regex()  == "ab.c.d.");
  std::string regex_4 = "a";
  REQUIRE(regex::RegexMatcher(regex_4).postfix_regex() == "a");
  std::string regex_5 = "a*";
  REQUIRE(regex::RegexMatcher(regex_5).postfix_regex() == "a*");
  std::string regex_6 = "a*b";
  REQUIRE(regex::RegexMatcher(regex_6).postfix_regex() == "a*b.");
  std::string regex_7 = "bza*d(ab)*";
  REQUIRE(regex::RegexMatcher(regex_7).postfix_regex() == "bz.a*.d.ab.*.");
  std::string regex_8 = "(ab|c*d)*";
  REQUIRE(regex::RegexMatcher(regex_8).postfix_regex() == "ab.c*d.|*");
  std::string regex_9 = "(1|2|3|4)*";
  REQUIRE(regex::RegexMatcher(regex_9).postfix_regex() == "12|3|4|*");
}

TEST_CASE("regex matching_1") {
  std::string regex = "(ab|c*d)*";
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
      "(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*.(0|1|2|3|4|5|6|7|8|9)";
  regex::RegexMatcher compiled_regex(regex);
  std::vector<std::string> test_correct({
      "12334123339990000.1",
      "121212.9",
      "1.0",
      "9.0",
      "129349404.6",
  });
  for (auto test_string : test_correct) {
    REQUIRE(compiled_regex.Matches(test_string) == true);
  }

  std::vector<std::string> test_incorrect({
      "1233412333999102",
      "1234.12",
      "0.01212",
      "121212.6666",
      "121212.",
      "0.123",
      "899",
  });
  for (auto test_string : test_incorrect) {
    REQUIRE(compiled_regex.Matches(test_string) == false);
  }
}

TEST_CASE("identifier regex") {
  std::string letter = "(a";
  for (char c = 'b'; c <= 'z'; c++) {
    letter.push_back('|');
    letter.push_back(c);
  }
  for (char c = 'A'; c <= 'Z'; c++) {
    letter.push_back('|');
    letter.push_back(c);
  }
  letter += "|_)";

  std::string digits = "(0|1|2|3|4|5|6|7|8|9)";

  std::string identifier_regex = letter + "(" + letter + "|" + digits + ")*";
  regex::RegexMatcher compiled_regex(identifier_regex);
  std::vector<std::string> test_correct({
      "identifier",
      "_Identifier",
      "_Identifi3r",
      "MyCreative_Variable_Name_9098",
      "iden_ti_fi_er__1212",
  });
  for (auto test_string : test_correct) {
    REQUIRE(compiled_regex.Matches(test_string) == true);
  }

  std::vector<std::string> test_incorrect({
      "my_variable?",
      "12why",
      "0",
      "my_var.6666",
      "my_var_,",
      "not_an identifier",
      "asasasas__asdasd^",
  });
  for (auto test_string : test_incorrect) {
    REQUIRE(compiled_regex.Matches(test_string) == false);
  }
}

TEST_CASE("escaping characters") {
  std::string digits = "(0|1|2|3|4|5|6|7|8|9)";
  std::string regex_1 = digits + "*\\*" + digits + "*";
  regex::RegexMatcher r1(regex_1);
  std::vector<std::string> test_correct({
      "11212*1212",
      "*",
      "1*",
      "1212*098",
  });
  for (auto test_string : test_correct)
    REQUIRE(r1.Matches(test_string) == true);

  std::vector<std::string> test_incorrect({
      "1212**",
      "121212",
      "9999***",
      "****",
  });
  for (auto test_string : test_incorrect)
    REQUIRE(r1.Matches(test_string) == false);


  std::string regex_2 = "(\\(|\\))*";
  regex::RegexMatcher r2(regex_2);
  test_correct = std::vector<std::string>({
      "(((())))",
      "()()()))",
      "(((((",
      "))",
  });
  for (auto test_string : test_correct)
    REQUIRE(r2.Matches(test_string) == true);

  test_incorrect = std::vector<std::string>({
      "(a",
      "(( ",
      "as",
      "())).",
  });
  for (auto test_string : test_incorrect)
    REQUIRE(r2.Matches(test_string) == false);
}

TEST_CASE("keyword matching") {
  std::string keywords = "(class|static|void|if|else|for|return|break|continue|int|real)";

  regex::RegexMatcher compiled_regex(keywords);
  std::vector<std::string> test_correct({
      "static",
      "void",
      "else",
      "for",
      "return",
      "break",
      "continue",
      "int",
      "real",
      "class",
      "if",
  });
  for (auto test_string : test_correct) 
    REQUIRE(compiled_regex.Matches(test_string) == true);


  std::vector<std::string> test_incorrect({
      "float",
      "def",
      "var",
      "foo",
      "lol",
      "uint",
      "char",
  });
  for (auto test_string : test_incorrect)
    REQUIRE(compiled_regex.Matches(test_string) == false);
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
