/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Randomizer                                                    *
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
   Description: This example demonstrates the use of the random
                permutation routine. Input is taken either from
                stdin or a user specified file. The input is then
                randomly permuted and passed to stdout.
*/


#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <deque>
#include <ctime>

#include "strtk.hpp"


int main(int argc, char* argv[])
{
   std::deque<std::string> str_lst;

   switch (argc)
   {
                // Take input from stdin
      case 1  : strtk::load_from_text_file(std::cin,str_lst);
                break;

                // Take input from user specified file
      case 2  : strtk::load_from_text_file(argv[1],str_lst);
                break;

      default :
               {
                  std::cout << "usage: strtk_randomizer <file name>" << std::endl;
                  std::cout << "usage: cat data.txt | strtk_randomizer" << std::endl;
                  return 1;
               }
   }

   if (!str_lst.empty())
   {
      #ifdef strtk_enable_random
      strtk::random_permutation(str_lst,
                                std::ostream_iterator<std::string>(std::cout,"\n"),
                                static_cast<std::size_t>(::time(0)));
      #else
      std::copy(str_lst.begin(),str_lst.end(),
                std::ostream_iterator<std::string>(std::cout,"\n"));
      #endif
   }
   return 0;
}
