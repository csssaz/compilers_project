#define CATCH_CONFIG_MAIN
#include <iostream>
#include <string>
#include "bparser.h"
#include "catch.hpp"
#include "hparser.h"

std::string get_ast(std::string filename, bool handmade = false) {
  FILE* fin = fopen(filename.c_str(), "r");
  Parser* parser = handmade ? static_cast<Parser*>(new HParser(fin, false, false))
                            : static_cast<Parser*>(new BParser(fin, false, false));
  parser->parse();
  std::string ast = parser->get_AST()->str();
  fclose(fin);
  delete parser;
  return ast;
}

TEST_CASE("compare asts") {
  for (auto filename : {"test.decaf", "test2.decaf", "demo.decaf"}) {
    std::cerr << "checking " << filename << std::endl;
    std::cerr << "pasing handmade" << std::endl;
    std::string handmade_ast = get_ast(filename, true);
    std::cerr << "parsing bison" << std::endl;
    std::string bison_ast = get_ast(filename); 
    REQUIRE(handmade_ast == bison_ast);
    std::cerr << std::endl;
  }
}
