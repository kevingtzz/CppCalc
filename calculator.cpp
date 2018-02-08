#include "calculator.h"
#include "parser.h"
#include "ast.h"
#include <string>
#include <iostream>
#include <sstream>
#include "calcex.h"



Calculator::Calculator():
   memory(0)
{}

int Calculator::eval(string expr) {

   Parser* parser = new Parser(new istringstream(expr));

   AST* tree = parser->parse();

   int result = tree->evaluate();

   delete tree;

   delete parser;

   return result;
}

void Calculator::store(int val) {
   memory = val;
}

int Calculator::recall() {
   return memory;
}

void Calculator::plus(int val) {
  memory = memory + val;
}

void Calculator::minus(int val) {
  memory = memory - val;
}

void Calculator::clear() {
  memory = 0;
}

void Calculator::setId(string id, int val) {
  set[id] = val;
}

int Calculator::getId(string id) {

  return set[id];

}
