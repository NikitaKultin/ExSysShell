// error.h
#include <iostream>
int error(const char* msg)
{
   static int no_of_errors = 0;
   std::cout<< "Error: " << msg << '\n';
   return no_of_errors++;
}