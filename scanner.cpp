#include "scanner.h"
#include "calcex.h"
#include <iostream>
#include <string>
#include <cctype>
#include <cstdio>

using namespace std;

//Uncomment this to get debug information
// #define debug

const int numberOfKeywords = 6;

const string keywd[numberOfKeywords] = {
  string("S"), string("R"), string("P"), string("M"), string("C"), string("set")
};


Scanner::Scanner(istream* in):
  inStream(in),
  lineCount(1),
  colCount(-1),
  needToken(true),
  lastToken(0)
{}

Scanner::~Scanner() {
   try {
      delete inStream;
   } catch (...) {}
}

void Scanner::putBackToken() {
   needToken = false;
}

Token* Scanner::getToken() {
   if (!needToken) {
      needToken=true;
      return lastToken;
   }

   Token* t;
   int state=0;
   bool foundOne=false;
   int c;
   string lex;
   TokenType type;
   int k;
   int column, line, bitControl;

   c = inStream->get();

   while (!foundOne) {
      colCount++;
      switch (state) {
         case 0 :
            bitControl = 0;
            lex = "";
            column=colCount;
            line = lineCount;
            if (isalpha(c)) state = 1;
            else if (isdigit(c)) state = 2;
            else if (c == '+') state = 3;
            else if (c == '-') state = 4;
            else if (c == '*') state = 5;
            else if (c == '/') state = 6;
            else if (c == '(') state = 7;
            else if (c == ')') state = 8;
            else if (c == '%') state = 9;
            else if (c == '=') state = 10;
            else if (c == '<') {state = 11; ++bitControl;}
            else if (c == '>') {state = 12; ++bitControl;}
            else if (c == '\n') {colCount=-1; lineCount++;}
            else if (isspace(c));
            else if (inStream->eof() || c == EOF) {
               foundOne=true;
               type=eof;
            }
            else {
               cout << "Unrecognized Token found at line " << line <<
                  " and column " << column << endl;
               throw UnrecognizedToken;
            }
            break;
         case 1 :
            if (isalnum(c) || isalpha(c)) {
              if (c == 'S' || c == 'R' ||c == 'C' || c ==  'P' ||c == 'M') {
                type = identifier;
                colCount = colCount - 1;
                foundOne = true;
              }else state=1;
            }
            else {

               for (k=0;k<numberOfKeywords;k++)
                  if (lex == keywd[k]) {
                     foundOne = true;
                     type = keyword;
                  }
               if (!foundOne) {
                  type = identifier;
                  foundOne = true;
               }
            }
            break;
         case 2 :
            if (isdigit(c)) state=2;
            else {
               type = number;
               foundOne=true;
            }
            break;
         case 3 :
            type = add;
            foundOne = true;
            break;
         case 4 :
            type = sub;
            foundOne = true;
            break;
         case 5 :
            type = times;
            foundOne=true;
            break;
         case 6 :
            type = divide;
            foundOne=true;
            break;
         case 7 :
            type = lparen;
            foundOne=true;
            break;
         case 8 :
            type = rparen;
            foundOne=true;
            break;
        case 9 :
          type = mod;
          foundOne = true;
          break;
        case 10 :
          type = equals;
          foundOne = true;
          break;
        case 11 :
          if (c == '<' && bitControl != 2) {
            state = 11;
            ++bitControl;
          }else if (bitControl == 2){
            type = shiftL;
            foundOne = true;
            bitControl = 0;
          }else {
           cout << "Second < not found for left shift bit " << '\n';
            throw ParseError;
          }
          break;
        case 12 :
          if (c == '>' && bitControl != 2) {
            state = 12;
            ++bitControl;
          }else if (bitControl == 2){
            type = shiftR;
            foundOne = true;
            bitControl = 0;
          }else {
            cout << "Second > not found for right shift bit " << '\n';
            throw ParseError;
          }
          break;
        }

      if (!foundOne) {
	lex = lex + static_cast<char>(c);
         c = inStream->get();
      }
   }

   inStream->putback(c);
   colCount--;
   if (type == number || type == identifier || type == keyword) {
      t = new LexicalToken(type,new string(lex), line, column);
   }
   else {
      t = new Token(type, line, column);
   }

#ifdef debug
   cout << "just found " << lex << " with type " << type << endl;
#endif

   lastToken = t;
   return t;

}
