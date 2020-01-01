/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * String Glober                                                 *
 * Author: Arash Partow (2002-2020)                              *
 * URL: http://www.partow.net/programming/strtk/index.html       *
 *                                                               *
 * Copyright notice:                                             *
 * Free use of the String Toolkit Library is permitted under the *
 * guidelines and in accordance with the most current version of *
 * the MIT License.                                              *
 * http://www.opensource.org/licenses/MIT                        *
 *                                                               *
 *****************************************************************
*/


/*
   Description: This example demonstrates the use of the match functionality
                within  the  String  Toolkit library.  The  process  takes a
                series of inputs from the  command line and assumes them  to
                be wild-card  patterns. The  process then  proceeds to  read
                input  from  stdin one  line  at a  time,  printing out  the
                patterns that successfully match the current line.

                The  patterns  can  be  constructed  using  either  of   the
                following wild-card characters:

                * : match on zero or more characters ? : match on zero
                or one character

               Example usage:

               ls | ./strtk_glober "*.hpp" "*.cpp" "*.txt" "M?kef*le"

               M?kef*le matched Makefile
               *.txt matched readme.txt
               *.cpp matched strtk_combinations.cpp
               *.cpp matched strtk_combinator_example.cpp
               *.cpp matched strtk_converters_example.cpp
               *.cpp matched strtk_examples.cpp
               *.cpp matched strtk_glober.cpp
               *.cpp matched strtk_hexview.cpp
               *.hpp matched strtk.hpp
               *.cpp matched strtk_ipv4_parser.cpp
               *.cpp matched strtk_keyvalue_example.cpp
               *.cpp matched strtk_nth_combination_example.cpp
               *.cpp matched strtk_numstats.cpp
               *.cpp matched strtk_parse_test.cpp
               *.cpp matched strtk_period_parser.cpp
               *.cpp matched strtk_randomizer.cpp
               *.cpp matched strtk_random_line.cpp
               *.cpp matched strtk_search_trie_example.cpp
               *.cpp matched strtk_serializer_example.cpp
               *.cpp matched strtk_text_parser_example01.cpp
               *.cpp matched strtk_text_parser_example02.cpp
               *.cpp matched strtk_tokengrid_example.cpp
               *.cpp matched strtk_tokenizer_cmp.cpp
               *.cpp matched strtk_tokenizer_test.cpp
               *.cpp matched strtk_wordfreq.cpp

*/


#include <cstddef>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "strtk.hpp"


class line_glober
{
public:

   line_glober(const int argc, char* argv[])
   {
      pattern_list_.reserve(argc - 1);
      strtk::parse(argc - 1,argv + 1,pattern_list_);
      std::sort(pattern_list_.begin(),pattern_list_.end());
   }

   inline void operator()(const std::string& s) const
   {
      for (std::size_t i = 0; i < pattern_list_.size(); ++i)
      {
         if (strtk::match(pattern_list_[i],s))
         {
            std::cout << pattern_list_[i] << " matched " << s << std::endl;
         }
      }
   }

private:

   std::vector<std::string> pattern_list_;
};

int main(int argc, char* argv[])
{
   if (argc <= 1)
   {
      std::cout << "strtk_glober <pattern_0> <pattern_1> .... <pattern_n>" << std::endl;;
      return 1;
   }

   strtk::for_each_line(std::cin,line_glober(argc,argv));

   return 0;
}
