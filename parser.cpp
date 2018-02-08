#include "parser.h"
#include "calcex.h"
#include <string>
#include <sstream>

Parser::Parser(istream* in) {
   scan = new Scanner(in);
}

Parser::~Parser() {
   try {
      delete scan;
   } catch (...) {}
}

AST* Parser::parse() {
   return prog();
}

AST* Parser::prog() {
   AST* result = bitExpr();
   Token* t = scan->getToken();

   if (t->getType() != eof) {
      cout << "Syntax Error: Expected EOF, found token at column " << t->getCol() << endl;
      throw ParseError;
   }

   return result;
}

AST* Parser::bitExpr() {
  return restBitExpr(expr());
}

AST* Parser::restBitExpr(AST* e) {
  Token* t = scan->getToken();
  if (t->getType() == shiftL) {
    return restBitExpr(new ShiftLNode(e, expr()));
  }
  if (t->getType() == shiftR) {
    return restBitExpr(new ShiftRNode(e, expr()));
  }

  scan->putBackToken();

  return e;
}

AST* Parser::expr() {
   return restExpr(term());
}

AST* Parser::restExpr(AST* e) {
   Token* t = scan->getToken();

   if (t->getType() == add) {
      return restExpr(new AddNode(e,term()));
   }

   if (t->getType() == sub)
      return restExpr(new SubNode(e,term()));

   scan->putBackToken();

   return e;
}

AST* Parser::term() {
  return restTerm(storable());
}

AST* Parser::restTerm(AST* e) {
  Token *t = scan->getToken();

  if (t->getType() == times) {
    return restTerm(new TimesNode(e, storable()));
  }

  if (t->getType() == divide) {
    return restTerm(new DivideNode(e, storable()));
  }

  if (t->getType() == mod) {
    return restTerm(new modNode(e, term()));
  }

  scan->putBackToken();

  return e;
}

AST* Parser::storable() {
  AST* result = factor();

  Token *t = scan->getToken();

  if (t->getType() == keyword) {
    if (t->getLex() == "S") {
      return new StoreNode(result);
    }
    else if(t->getLex() == "P") {
      return new PlusNode(result);
    }
    else if(t->getLex() == "M") {
      return new MinusNode(result);
    }
    else {
      cout << "Parser error: Expected S found "
	   << t->getLex() << endl;
      throw ParseError;
    }
  }

  scan->putBackToken();

  return result;
}

AST* Parser::factor() {
  Token* t = scan->getToken();

  if (t->getType() == number) {
    istringstream in(t->getLex());
    int val;
    in >> val;
    return new NumNode(val);
  }
  if(t->getType()== identifier){

  return new idNode(t->getLex());
}


  if (t->getType() == keyword) {
    if (t->getLex() == "R") {
      return new RecallNode();
    }
    else if (t->getLex() == "C") {
      return new ClearNode();
    }
   else if (t->getLex() == "set") {                                             //implementacion set
      t = scan->getToken();

      if(t->getType() == identifier) {
        string var = t->getLex();
        t = scan->getToken();
        if (t->getType() == equals) {
          AST* result = bitExpr();

          return new setNode(var, result);
        }else{
          cout<<"Error: Expected equals '=' not found " << t->getLex()<<endl;
          throw ParseError;
         }
        }else{
             cout<<"Error: Expected identifier not found"<< t->getLex()<<endl;
             throw ParseError;
         }
      }else {
      cout << "Expected R, C or set found: "
	   << t->getLex()
	   << endl;
      throw ParseError;
    }
  }

  if (t->getType() == lparen) {
    AST* result = expr();
    t = scan->getToken();
    if (t->getType() != rparen) {
      cout << "Expected )"
	   << endl;
      throw ParseError;
    }
    return result;
  }

  cout << "Expected Number, 'R', '('" << endl;
  throw ParseError;
}
