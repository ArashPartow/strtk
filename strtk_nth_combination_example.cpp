/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * N'th Combination Example                                      *
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
   Description: The objective of this example is to generate the
   n'th combination for the purpose of n-choose-k.

   Example: {ABCDEF}-choose-4

   #      Index  Combination
   0000 | 0123 | ABCD
   0001 | 0124 | ABCE
   0002 | 0125 | ABCF
   0003 | 0134 | ABDE
   0004 | 0135 | ABDF
   0005 | 0145 | ABEF
   0006 | 0234 | ACDE
   0007 | 0235 | ACDF
   0008 | 0245 | ACEF
   0009 | 0345 | ADEF
   0010 | 1234 | BCDE
   0011 | 1235 | BCDF
   0012 | 1245 | BCEF
   0013 | 1345 | BDEF
   0014 | 2345 | CDEF

   Note: This technique is far more efficient and optimal than calling
   strtk::next_combination routine n-times.

*/


#include <cstddef>
#include <iostream>
#include <vector>

#include "strtk.hpp"


// n choose k
static const std::size_t n = 6;
static const std::size_t k = 4;

void nth_combination_example01()
{
   typedef std::vector<unsigned char> list_type;
   list_type char_list;

   strtk::iota(char_list,n,static_cast<unsigned char>('A'));

   const std::size_t combination_count = static_cast<std::size_t>(strtk::n_choose_k(n,k));

   for (std::size_t i = 0; i < combination_count; ++i)
   {
      std::vector<unsigned int> index_list;
      strtk::nth_combination_sequence(i, n, k, std::back_inserter(index_list));

      list_type nth_combination;
      strtk::nth_combination_sequence(i,
                                      k,
                                      char_list.begin(), char_list.end(),
                                      std::back_inserter(nth_combination));

      std::cout << strtk::text::right_align(4, '0', i) << " | "
                << strtk::join("", index_list)         << " | "
                << strtk::join("", nth_combination)    << std::endl;
   }

   std::cout << std::endl << std::endl;
}

void nth_combination_example02()
{
   typedef std::vector<unsigned char> list_type;
   list_type char_list;
   list_type next_comb_list;

   strtk::iota(char_list,n,static_cast<unsigned char>('A'));

   next_comb_list = char_list;

   const std::size_t combination_count = static_cast<std::size_t>(strtk::n_choose_k(n,k));

   for (std::size_t i = 0; i < combination_count; ++i)
   {
      std::vector<unsigned int> index_list;
      strtk::nth_combination_sequence(i, n, k, std::back_inserter(index_list));

      list_type nth_combination;
      strtk::nth_combination_sequence(i,
                                      k,
                                      char_list.begin(), char_list.end(),
                                      std::back_inserter(nth_combination));

      std::cout << strtk::text::right_align(4, '0', i) << " | "
                << strtk::join("", index_list)         << " | "
                << strtk::join("", nth_combination)    << " | "
                << strtk::join("", next_comb_list)     << std::endl;

      strtk::next_combination(next_comb_list.begin(),
                              next_comb_list.begin() + k,
                              next_comb_list.end());
   }

   std::cout << std::endl << std::endl;
}

void nth_combination_example03()
{
   typedef std::vector<unsigned char> list_type;
   list_type char_list;

   strtk::iota(char_list,n,static_cast<unsigned char>('A'));

   // Obtain 5th combination
   list_type nth_combination;
   strtk::nth_combination_sequence(4,
                                   k,
                                   char_list.begin(), char_list.end(),
                                   std::back_inserter(nth_combination));

   //Iterate beginning from 5th combination to the end
   std::size_t i = 4;
   do
   {
      std::cout << strtk::text::right_align(4, '0', i++) << " | "
                << strtk::join("", nth_combination)      << std::endl;
   }
   while (strtk::next_combination(nth_combination.begin(),
                                  nth_combination.begin() + k,
                                  nth_combination.end()));
}

int main()
{
   nth_combination_example01();
   nth_combination_example02();
   nth_combination_example03();
   return 0;
}
