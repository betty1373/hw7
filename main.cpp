#include "src/printip.h"
using namespace printip;
/// @file

/// @brief Main function. Print Ip's from various types.
/// @author btv<example@example.com>
int main(int, char *[]) {
   print_ip(char{-1});
   std::cout<<std::endl;
   
   print_ip(short{0});
   std::cout<<std::endl;

   print_ip(int{2130706433});
   std::cout<<std::endl;

   print_ip(long{8875824491850138409});
   std::cout<<std::endl;

   print_ip(std::string{"Hello, world!"});
   std::cout<< std::endl;

   print_ip(std::vector<int>{100,200,300,400});
   std::cout<<std::endl;

   print_ip(std::list<short>{400,300,200,100});
   std::cout<<std::endl;

   print_ip(std::make_tuple(123,456,789,0));
   std::cout<<std::endl;
   return 0;
}  
