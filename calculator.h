#pragma once

#include <string>
#include <map>

using namespace std;

class Calculator {
 public:
   Calculator();

   int eval(string expr);
   int recall();
   int getId(string id);
   void store(int val);
   void plus(int val);
   void minus(int val);
   void clear();
   void setId(string id, int val);

 private:
   int memory;
   map<string, int> set;
};

extern Calculator* calc;
