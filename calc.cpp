#include <iostream>
#include <sstream>
#include <string>
#include "calcex.h"
#include "calculator.h"
using namespace std;

Calculator* calc;

int main(int argc, char* argv[]) {

   string line;
   try {
     calc = new Calculator();

   while (true) {

      cout << "> ";

      getline(cin, line);
      // line + '\n';

      if(!cin) break;

      int result = calc->eval(line);

      cout << "= " << result << endl;

}

      delete calc;

   }
   catch(Exception ex) {
      cout << "Program Aborted due to exception!" << endl;
   }
}
