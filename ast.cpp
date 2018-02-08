#include <iostream>
#include "ast.h"
#include "calculator.h"
#include "calcex.h"

// for debug information uncomment
// #define debug

AST::AST() {}

AST::~AST() {}

BinaryNode::BinaryNode(AST* left, AST* right):
   AST(),
   leftTree(left),
   rightTree(right)
{}

BinaryNode::~BinaryNode() {
#ifdef debug
   cout << "In BinaryNode destructor" << endl;
#endif

   try {
      delete leftTree;
   } catch (...) {}

   try {
      delete rightTree;
   } catch(...) {}
}

AST* BinaryNode::getLeftSubTree() const {
   return leftTree;
}

AST* BinaryNode::getRightSubTree() const {
   return rightTree;
}

UnaryNode::UnaryNode(AST* sub):
   AST(),
   subTree(sub)
{}

UnaryNode::~UnaryNode() {
#ifdef debug
   cout << "In UnaryNode destructor" << endl;
#endif

   try {
      delete subTree;
   } catch (...) {}
}

AST* UnaryNode::getSubTree() const {
   return subTree;
}

modNode::modNode(AST* left, AST* right):
  BinaryNode(left, right)
{}

int modNode::evaluate() {
  if(getRightSubTree()->evaluate() == 0){
  cout<<"Error module, indefiniton. "<<endl;
  throw ParseError;
}else{
  return getLeftSubTree()->evaluate() % getRightSubTree()->evaluate();
 }
}

ShiftLNode::ShiftLNode(AST* left, AST* right):
   BinaryNode(left, right)
{}

int ShiftLNode::evaluate() {
     return getLeftSubTree()->evaluate() << getRightSubTree()->evaluate();
}

ShiftRNode::ShiftRNode(AST* left, AST* right):
   BinaryNode(left, right)
{}

int ShiftRNode::evaluate() {
     return getLeftSubTree()->evaluate() >> getRightSubTree()->evaluate();
  }

AddNode::AddNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int AddNode::evaluate() {
   return getLeftSubTree()->evaluate() + getRightSubTree()->evaluate();
}

SubNode::SubNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int SubNode::evaluate() {
   return getLeftSubTree()->evaluate() - getRightSubTree()->evaluate();
}

TimesNode::TimesNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int TimesNode::evaluate() {
   return getLeftSubTree()->evaluate() * getRightSubTree()->evaluate();
}

DivideNode::DivideNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int DivideNode::evaluate() {
   return getLeftSubTree()->evaluate() / getRightSubTree()->evaluate();
}

StoreNode::StoreNode(AST * sub) : UnaryNode(sub) { }

int StoreNode::evaluate() {
  calc->store(getSubTree()->evaluate());
  return calc->recall();
}

int PlusNode::evaluate() {
  calc->plus(getSubTree()->evaluate());
  return calc->recall();
}

PlusNode::PlusNode(AST * sub) : UnaryNode(sub) { }

int MinusNode::evaluate() {
  calc->minus(getSubTree()->evaluate());
  return calc->recall();
}

MinusNode::MinusNode(AST * sub) : UnaryNode(sub) { }

int ClearNode::evaluate() {
  calc->store(0);
  return calc->recall();
}

ClearNode::ClearNode() : AST() { }

NumNode::NumNode(int n) : AST(),val(n) { }

int NumNode::evaluate() {
   return val;
}

RecallNode::RecallNode() : AST() { }

int RecallNode::evaluate() {
  return calc->recall();
}

setNode::setNode(string id, AST* sub):
  UnaryNode(sub), idt(id)
{}

int setNode::evaluate() {
    calc->setId(idt, getSubTree()->evaluate());
    return getSubTree()->evaluate();
}

idNode::idNode(string str):
AST(),id(str)
{}

int idNode::evaluate() {
    return calc->getId(id);
}
