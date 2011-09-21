/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Bloom Filter Example                                          *
 * Author: Arash Partow (2002-2011)                              *
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
   Description: This example demonstrates the usage pattern for the
                bloom filter data structure. The example goes through
                all 26-choose-13 unique combinations of letters(a-z),
                adds each of them as a string representation to the
                bloom filter, then proceeds to test their existence
                within the filter, noting any errors.
                Once that is complete, another set of strings composed
                of 26-choose-6 combinations of letters is iterated through,
                and their existence within the bloom filter is tested, in
                the event a result of true is returned the false positive
                count is incremented.
*/


#include <cstddef>
#include <iostream>
#include <string>

#include "strtk.hpp"



int main()
{
   static std::string letters = "abcdefghijklmnopqrstuvwxyz";

   // n choose k
   static const std::size_t n = letters.size();
   static const std::size_t k = 13;
   static const std::size_t element_count = static_cast<std::size_t>(strtk::n_choose_k(n,k));
   static const double false_positive_probability = 0.001;

   typedef strtk::combination_iterator<char*> iterator_type;

   strtk::bloom::filter filter(element_count,
                               false_positive_probability,
                               strtk::magic_seed);

   std::cout << "Filter Size: " << (filter.size() / (8.0 * strtk::one_kilobyte)) << "KB" << std::endl;

   {
      iterator_type itr(k,letters);
      const iterator_type end(letters);

      strtk::util::timer timer;
      timer.start();

      while (end != itr)
      {
         iterator_type::range_type range = *itr;
         filter.insert(range.first,std::distance(range.first,range.second));
         ++itr;
      }

      timer.stop();

      printf("[insert ] Element Count: %llu\tTotal Time: %5.3fsec\tRate: %10.3felem/sec\n",
             static_cast<unsigned long long>(filter.element_count()),
             timer.time(),
             element_count / timer.time());
   }

   {
      iterator_type itr(k,letters);
      const iterator_type end(letters);

      strtk::util::timer timer;
      timer.start();

      while (end != itr)
      {
         iterator_type::range_type range = *itr;
         if (!filter.contains(range.first,std::distance(range.first,range.second)))
         {
            std::cout << "Error: Failed to find: " <<
                         std::string(range.first,std::distance(range.first,range.second)) << std::endl;
         }
         ++itr;
      }

      timer.stop();

      printf("[contain] Element Count: %llu\tTotal Time: %5.3fsec\tRate: %10.3felem/sec\n",
             static_cast<unsigned long long>(filter.element_count()),
             timer.time(),
             element_count / timer.time());
   }

   {
      iterator_type itr(k/2,letters);
      const iterator_type end(letters);

      unsigned int false_positive_count = 0;

      strtk::util::timer timer;
      timer.start();

      while (end != itr)
      {
         iterator_type::range_type range = *itr;
         if (filter.contains(range.first,std::distance(range.first,range.second)))
         {
            ++false_positive_count;
         }
         ++itr;
      }

      timer.stop();

      printf("[FPC    ] False Positive Count: %d\tTotal Time: %5.3fsec\tRate: %10.3felem/sec\n",
             false_positive_count,
             timer.time(),
             element_count / timer.time());
   }

   {
      strtk::bloom::filter secondary_filter;

      if (!filter.write_to_file("bloom_filter.bin"))
      {
         std::cout << "Error - Failed to write filter to file!" << std::endl;
         return 1;
      }
      else if (!secondary_filter.read_from_file("bloom_filter.bin"))
      {
         std::cout << "Error - Failed to read filter from file!" << std::endl;
         return 1;
      }
      else if (secondary_filter != filter)
      {
         std::cout << "Error - Persisted filter and original filter do not match!" << std::endl;
         return 1;
      }
   }

   return 0;
}
