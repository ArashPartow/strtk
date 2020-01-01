/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Combination Iterator                                          *
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
   Description: This example demonstrates the use of the combination_iterator
                which is based on the next_combination routine.
                The example will iterate through all n-choose-k combinations
                based on a sequence of unsigned integers. Essentially a
                similar usage pattern to that of next_combination but based
                on an iterator facade.
*/


#include <cstddef>
#include <iostream>
#include <vector>

#include "strtk.hpp"


int main()
{
   typedef std::vector<unsigned int> list_type;
   typedef strtk::combination_iterator<list_type::iterator> iterator_type;
   list_type uint_list;

   // n choose k
   static const std::size_t n = 6;
   static const std::size_t k = 4;

   strtk::iota(uint_list,n,static_cast<unsigned int>(0));

   iterator_type itr(k, uint_list.begin(), uint_list.end());
   const iterator_type end(uint_list.end());

   std::size_t count = 0;
   while (end != itr)
   {
      iterator_type::range_type range = *itr;

      std::cout << strtk::text::right_align(4, '0', count++) << "\t" << strtk::join(" ", range) << std::endl;

      ++itr;
   }

   return 0;
}
