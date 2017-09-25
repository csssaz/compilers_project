 // Skeleton Flex file for the project - Part 1
%option c++
%option noyywrap
%option yylineno
%{
#include "token.h"
%}
ws [ \t\r\n]
letter_ ([A-Za-z]|"_")
digit [0-9]
digits {digit}+
identifier {letter_}({letter_}|{digit})*
optional_fraction ("."{digits})?
optional_exponent ("E"("+"|"-")?{digits})?
number {digits}{optional_fraction}{optional_exponent}
comment "/*"[^*/\/]*"*/"|"//".*\n
%%
{ws}+             /* do nothing */
{comment}         /* do nothing */

 /* Relational operators */
"=="            { return Tokentype::OpRelEQ; }
"!="            { return Tokentype::OpRelNEQ; }
"<="            { return Tokentype::OpRelLTE; }
">="            { return Tokentype::OpRelGTE; }
"<"             { return Tokentype::OpRelLT; }
">"             { return Tokentype::OpRelGT; }

 /* Arithmetic operators */
"++"            { return Tokentype::OpArtInc; }
"--"            { return Tokentype::OpArtDec; }
"+"             { return Tokentype::OpArtPlus; }
"-"             { return Tokentype::OpArtMinus; }
"*"             { return Tokentype::OpArtMult; }
"/"             { return Tokentype::OpArtDiv; }
"%"             { return Tokentype::OpArtModulus; }

 /* Logical operators */
"&&"            { return Tokentype::OpLogAnd; }
"||"            { return Tokentype::OpLogOr; }
"!"             { return Tokentype::OpLogNot; }

 /* Assign */
"="             { return Tokentype::OpAssign; }

 /* Keywords */
"class"         { return Tokentype::kwClass; }
"static"        { return Tokentype::kwStatic; }
"void"          { return Tokentype::kwVoid; }
"if"            { return Tokentype::kwIf; }
"else"          { return Tokentype::kwElse; }
"for"           { return Tokentype::kwFor; }
"return"        { return Tokentype::kwReturn; }
"break"         { return Tokentype::kwBreak; }
"continue"      { return Tokentype::kwContinue; }
"int"           { return Tokentype::kwInt; }
"real"          { return Tokentype::kwReal; }

 /* Punctuation marks */
"{"             { return Tokentype::ptLBrace; }
"}"             { return Tokentype::ptRBrace; }
"["             { return Tokentype::ptLBracket; }
"]"             { return Tokentype::ptRBracket; }
"("             { return Tokentype::ptLParen; }
")"             { return Tokentype::ptRParen; }
";"             { return Tokentype::ptSemicolon; }
","             { return Tokentype::ptComma; }

 /* Number */
{number}        { return Tokentype::Number; }

 /* Identifier */
{identifier}    { return Tokentype::Identifier; }

 /* Lexical errors */
.               { return Tokentype::ErrUnknown; }

 /* End of file */
<<EOF>>         { return Tokentype::EOI; }
%%
