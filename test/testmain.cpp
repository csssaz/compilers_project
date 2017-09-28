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

TEST_CASE("number matching") {
  std::string digits = "(0|1|2|3|4|5|6|7|8|9)+";
  std::string optional_fraction = "(." + digits + ")?";
  std::string optional_exponent = "(E(\\+|-)?" + digits + ")?";
  std::string number = digits + optional_fraction + optional_exponent; 
  
  regex::RegexMatcher compiled_regex(number);
  std::vector<std::string> test_correct({
      "1234",
      "123.3",
      "0.44455667",
      "1212E441",
      "1.1212E+123",
      "123.9966E-12341",
      "100000.0000E-1923",
      "1212.664545",
      "0.4E2",
      "0.1212E-1",
      "0",
  });
  for (auto test_string : test_correct)
    REQUIRE(compiled_regex.Matches(test_string) == true);

  std::vector<std::string> test_incorrect({
      "12121.1212.1212",
      "123123E",
      "0.",
      ".1212E43",
      "1212.E31",
      "999.999e+12",
      "1000.0000EE1212",
      "567.12E1212E1212",
      "5676.1212E-+1212",
      "0000E123.5",
  });
  for (auto test_string : test_incorrect)
    REQUIRE(compiled_regex.Matches(test_string) == false);
}

TEST_CASE("whitespace") {
  std::string regex = "(\n|\t|\r| )+";
  regex::RegexMatcher compiled_regex(regex);

  std::vector<std::string> test_correct({
      "                \t",
      "\t\t\t\t\n\n\n\r\r\r      ",
      " ",
      "\t",
      "\r",
      "\n\n\t\t      \r\r \t   \n",
  });
  
  for (auto test_string : test_correct)
    REQUIRE(compiled_regex.Matches(test_string) == true);

  std::vector<std::string> test_incorrect({
      "       .      ",
      "\t\t\t\t\t a",
      "a\t\t\t\t",
      "            )",
  });

  for (auto test_string : test_incorrect)
    REQUIRE(compiled_regex.Matches(test_string) == false);
}


TEST_CASE("comments") {
  std::string test_string = "/* 12344.3 asdasd\n\n */";
  std::stringstream sf(test_string);
  regex::RegexMatcher r(sf);
  r.NextToken();
  REQUIRE(r.NextToken() == static_cast<int>(Tokentype::EOI));
  REQUIRE(r.GetLexeme() == test_string);

  test_string = "/* Not closed comment";
  std::stringstream ss(test_string);
  regex::RegexMatcher r2(ss);
  REQUIRE(r2.NextToken() == static_cast<int>(Tokentype::ErrUnknown));
  REQUIRE(r2.GetLexeme() == test_string);

  test_string = "/* /* Not closed\n\n */ var \n/*******/ mate /******/*//* sasdasd//*";
  std::stringstream ss1(test_string);
  regex::RegexMatcher r3(ss1);
  REQUIRE(static_cast<Tokentype>(r3.NextToken()) == Tokentype::Identifier);
  REQUIRE(r3.GetLexeme() == "var");
  REQUIRE(static_cast<Tokentype>(r3.NextToken()) == Tokentype::Identifier);
  REQUIRE(r3.GetLexeme() == "mate");
  REQUIRE(static_cast<Tokentype>(r3.NextToken()) == Tokentype::OpArtMult);
  REQUIRE(r3.GetLexeme() == "*");
  REQUIRE(static_cast<Tokentype>(r3.NextToken()) == Tokentype::OpArtDiv);
  REQUIRE(r3.GetLexeme() == "/");
  REQUIRE(static_cast<Tokentype>(r3.NextToken()) == Tokentype::ErrUnknown);
}

TEST_CASE("input file") {
  std::string test_string =
      "int x = 1345.13\n\treal my_var_name = 13.4E+9 - 13";
  std::stringstream sf(test_string);
  regex::RegexMatcher r(sf);

  std::vector<std::pair<Tokentype, std::string>> expected_output({
      {Tokentype::kwInt, "int"},
      {Tokentype::Identifier, "x"},
      {Tokentype::OpAssign, "="},
      {Tokentype::Number, "1345.13"},
      {Tokentype::kwReal, "real"},
      {Tokentype::Identifier, "my_var_name"},
      {Tokentype::OpAssign, "="},
      {Tokentype::Number, "13.4E+9"},
      {Tokentype::OpArtMinus, "-"},
      {Tokentype::Number, "13"},
  });

  for (auto test_case : expected_output) {
    Tokentype type;
    std::string lexeme;
    std::tie(type, lexeme) = test_case;
    REQUIRE(r.NextToken() == static_cast<int>(type));
    REQUIRE(r.GetLexeme() == lexeme);
  }

  REQUIRE(r.NextToken() == static_cast<int>(Tokentype::EOI));
}

TEST_CASE("compare lexers") {
  std::string test_string =
      "int my_var_num_ = 3, x = 12;\n"
      "\treal my_var_real_ = 13.134E-9;\n"
      "\t\tif (some_weird_expression) {"
      "\t\t for (int i = 0; i <= 10; i++) {"
      "\t\t\t int y = (10 + 66) % 88;"
      "/* all of this is a comment and"
      "\n\n\n\n should \n\n\t be ignored\n\n*/"
      "}\n y++;y--;}"
      "if (1 ==     2    ||3!=0&&      b !=7)      {}"
      "/* /* Not closed\n\n */ var \n/*******/ mate /******/*//* "
      "sasdasd//*";

  std::stringstream s_a(test_string);
  std::stringstream s_b(test_string);
  SymbolTable a, b;
  HLexer hand_lexer(s_a, a);
  FLexer flex_lexer(s_b, b);

  for (Token token_a, token_b; token_a.type != Tokentype::EOI;) {
    hand_lexer.get_next(token_a);
    flex_lexer.get_next(token_b);
    REQUIRE(token_a.type == token_b.type);
    REQUIRE(token_a.line == token_b.line);
    if (token_a.type != Tokentype::ErrUnknown)
      REQUIRE(token_a.lexeme == token_b.lexeme);
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
  delete lexer;
}

void test_lexer(std::string test_string, std::vector<std::tuple<std::string, int, Tokentype>> expected_output) {
  SymbolTable fsym, hsym;
  std::stringstream sf(test_string);
  std::stringstream sh(test_string);
  FLexer flexer(sf, fsym);
  HLexer hlexer(sh, hsym);
  Token ftoken, htoken;
  for (auto test_tuple : expected_output) {
    flexer.get_next(ftoken);
    if (ftoken.type != Tokentype::ErrUnknown)
      REQUIRE(ftoken.lexeme == std::get<0>(test_tuple));
    REQUIRE(ftoken.line == std::get<1>(test_tuple));
    REQUIRE(ftoken.type == std::get<2>(test_tuple));
    hlexer.get_next(htoken);
    if (htoken.type != Tokentype::ErrUnknown)
      REQUIRE(htoken.lexeme == std::get<0>(test_tuple));
    REQUIRE(htoken.line == std::get<1>(test_tuple));
    REQUIRE(htoken.type == std::get<2>(test_tuple));
  }
}

TEST_CASE("flex parser comments") {
  std::string test_string = "a = 5; /* Comment */\n\nb = 10.54E4; /* Comment */";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
    std::make_tuple("a", 1, Tokentype::Identifier),
    std::make_tuple("=", 1, Tokentype::OpAssign),
    std::make_tuple("5", 1, Tokentype::Number),
    std::make_tuple(";", 1, Tokentype::ptSemicolon),
    std::make_tuple("b", 3, Tokentype::Identifier),
    std::make_tuple("=", 3, Tokentype::OpAssign),
    std::make_tuple("10.54E4", 3, Tokentype::Number),
    std::make_tuple(";", 3, Tokentype::ptSemicolon)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("flex parser comment err") {
  std::string test_string = "/* Not closed comment";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("/* Not closed comment", 1, Tokentype::ErrUnknown)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("random") {
  std::string test_string = "int z%? = 5; /**//*_*//*****//**/ \n real[7.8] 411 = 435.5E+5\n /*dfg*/";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("int", 1, Tokentype::kwInt),
      std::make_tuple("z", 1, Tokentype::Identifier),
      std::make_tuple("%", 1, Tokentype::OpArtModulus),
      std::make_tuple("?", 1, Tokentype::ErrUnknown),
      std::make_tuple("=", 1, Tokentype::OpAssign),
      std::make_tuple("5", 1, Tokentype::Number),
      std::make_tuple(";", 1, Tokentype::ptSemicolon),
      std::make_tuple("real", 2, Tokentype::kwReal),
      std::make_tuple("[", 2, Tokentype::ptLBracket),
      std::make_tuple("7.8", 2, Tokentype::Number),
      std::make_tuple("]", 2, Tokentype::ptRBracket),
      std::make_tuple("411", 2, Tokentype::Number),
      std::make_tuple("=", 2, Tokentype::OpAssign),
      std::make_tuple("435.5E+5", 2, Tokentype::Number)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("random 2") {
  std::string test_string = "i+++j,k***9878964";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("i", 1, Tokentype::Identifier),
      std::make_tuple("++", 1, Tokentype::OpArtInc),
      std::make_tuple("+", 1, Tokentype::OpArtPlus),
      std::make_tuple("j", 1, Tokentype::Identifier),
      std::make_tuple(",", 1, Tokentype::ptComma),
      std::make_tuple("k", 1, Tokentype::Identifier),
      std::make_tuple("*", 1, Tokentype::OpArtMult),
      std::make_tuple("*", 1, Tokentype::OpArtMult),
      std::make_tuple("*", 1, Tokentype::OpArtMult),
      std::make_tuple("9878964", 1, Tokentype::Number)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("random 3") {
  std::string test_string = "!}{>====";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("!", 1, Tokentype::OpLogNot),
      std::make_tuple("}", 1, Tokentype::ptRBrace),
      std::make_tuple("{", 1, Tokentype::ptLBrace),
      std::make_tuple(">=", 1, Tokentype::OpRelGTE),
      std::make_tuple("==", 1, Tokentype::OpRelEQ),
      std::make_tuple("=", 1, Tokentype::OpAssign)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("splitted number") {
  std::string test_string = "7E\n-34";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("7", 1, Tokentype::Number),
      std::make_tuple("E", 1, Tokentype::Identifier),
      std::make_tuple("-", 2, Tokentype::OpArtMinus),
      std::make_tuple("34", 2, Tokentype::Number)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("error number") {
  std::string test_string = "125.45.8E3";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("125.45", 1, Tokentype::Number),
      std::make_tuple(".", 1, Tokentype::ErrUnknown),
      std::make_tuple("8E3", 1, Tokentype::Number)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("error number 2") {
  std::string test_string = "321E";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("321", 1, Tokentype::Number),
      std::make_tuple("E", 1, Tokentype::Identifier)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("and and and") {
  std::string test_string = "&&&";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("&&", 1, Tokentype::OpLogAnd),
      std::make_tuple("&", 1, Tokentype::ErrUnknown)
  };
}

TEST_CASE("keywords") {
  std::string test_string = "class\tstatic void if\telse     for return\nbreak\ncontinue int\nreal";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple("class", 1, Tokentype::kwClass),
      std::make_tuple("static", 1, Tokentype::kwStatic),
      std::make_tuple("void", 1, Tokentype::kwVoid),
      std::make_tuple("if", 1, Tokentype::kwIf),
      std::make_tuple("else", 1, Tokentype::kwElse),
      std::make_tuple("for", 1, Tokentype::kwFor),
      std::make_tuple("return", 1, Tokentype::kwReturn),
      std::make_tuple("break", 2, Tokentype::kwBreak),
      std::make_tuple("continue", 3, Tokentype::kwContinue),
      std::make_tuple("int", 3, Tokentype::kwInt),
      std::make_tuple("real", 4, Tokentype::kwReal)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("identifier, not keywords") {
  std::string test_string = "classstaticvoidifelseforreturnbreakcontinueintreal";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {
      std::make_tuple(test_string, 1, Tokentype::Identifier)
  };
  test_lexer(test_string, expected_output);
}

TEST_CASE("empty input") {
  std::string test_string = "";
  std::vector<std::tuple<std::string, int, Tokentype>> expected_output = {};
  test_lexer(test_string, expected_output);
}
