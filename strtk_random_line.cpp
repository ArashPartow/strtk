/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Random Line Selection                                         *
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
   Description: This is a solution to the problem of randomly selecting a line
                from a text file in the most efficient way possible taking into
                account time and space complexities, also ensuring that the
                probability of the line selected is exactly 1/N where N is the
                number of lines in the text file - It should be noted that the
                lines can be of varying length.
*/


#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <deque>
#include <ctime>

#include <boost/random.hpp>
//#include <random>

#include "strtk.hpp"

#ifndef strtk_enable_random
   #error This example requires random
#endif


class random_line_selector
{
public:

   random_line_selector(std::string& line, const std::size_t& seed = 0xA5A5A5A5)
   : line_count_(1),
     line_(line),
     rng_(seed)
   {}

   inline void operator()(const std::string& s)
   {
      if (rng_() < (1.0 / line_count_))
         line_ = s;
      ++line_count_;
   }

private:

   random_line_selector operator=(const random_line_selector&);

   std::size_t line_count_; // should be long long
   std::string& line_;
   strtk::uniform_real_rng rng_;
};

int main(int argc, char* argv[])
{
   if (2 != argc)
   {
      std::cout << "usage: strtk_random_line <file name>" << std::endl;
      return 1;
   }

   std::string file_name = argv[1];
   std::string line;

   strtk::for_each_line(file_name,
                        random_line_selector(line,static_cast<std::size_t>(::time(0))));

   std::cout << line << std::endl;

   return 0;
}
