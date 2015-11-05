//Copyright (c) 2004 Kurt Stutsman. All rights reserved.
#include "symbol_table.hpp"

#include <iostream>
#include <iomanip>

using namespace std;
using namespace Moove;

SymbolTable table;

void putString(const string& str)
{
   table.put(str);
}

int main()
{
   std::string in;
   unsigned total = 0;

   while(cin >> in) {
      ++total;
      putString(in);
   }

   cout << setw(20) << "Total symbols: " << total << "\n";
   cout << setw(20) << "Unique symbols: " << table.size() << "\n";
   cout << setw(20) << "Repeated symbols: " << total - table.size() 
	<< "\n";
   return 0;
}


   

   
