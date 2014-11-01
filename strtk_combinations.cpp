/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Combinations                                                  *
 * Author: Arash Partow (2002-2014)                              *
 * URL: http://www.partow.net/programming/strtk/index.html       *
 *                                                               *
 * Copyright notice:                                             *
 * Free use of the String Toolkit Library is permitted under the *
 * guidelines and in accordance with the most current version of *
 * the Common Public License.                                    *
 * http://www.opensource.org/licenses/cpl1.0.php                 *
 *                                                               *
 *****************************************************************
*/


/*
   Description: This example  demonstrates the  use of  the next_combination
                and for_each combination routines.  Input is taken from  the
                command line as one positive  integer (k) and a series  of N
                unique  strings.  Then   all  n-choose-k  combinations   are
                streamed one per line to stdout.

                Example usage:
                strtk_combination 3 abc 123 def 456

                Output:
                123     456     abc
                123     456     def
                123     abc     def
                456     abc     def
*/


#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "strtk.hpp"


void usage(const std::string& msg = "")
{
   std::cout << "usage: strtk_combination <r> <s_1> <s_2> .... <s_k>" << std::endl;
   if (!msg.empty())
      std::cout << "error message: " << msg << std::endl;
}

void print(std::vector<std::string>::iterator begin,
           std::vector<std::string>::iterator end)
{
   std::cout << strtk::join("\t",begin,end) << std::endl;
}

int main(int argc, char* argv[])
{
   if (argc < 3) return (usage(),1);

   std::size_t r = 0;

   if (!strtk::string_to_type_converter(std::string(argv[1]),r))
      return (usage("Invalid 'r' value."),1);

   if (0 == r) return 0;

   std::vector<std::string> str_lst;
   strtk::parse(argc - 2, argv + 2, str_lst);
   std::sort(str_lst.begin(),str_lst.end());

   if (r > str_lst.size()) return (usage("r > k"),1);

   strtk::for_each_combination(str_lst.begin(),str_lst.end(),r,&print);

   return 0;
}
