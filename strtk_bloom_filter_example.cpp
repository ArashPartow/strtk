/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Bloom Filter Example                                          *
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
   Description: This example  demonstrates the  usage pattern  for the bloom
                filter  data  structure.  The example  goes  through  all 26
                -choose-14 unique combinations of letters(a-z), adds each of
                them as a string representation to the bloom filter in  both
                all upper and  all lower case,  then proceeds to  test their
                existence within the filter, noting any errors.

                Once that is  complete, another set  of strings composed  of
                sub-strings of  length 7  representing the  first and second
                halves  of  the original  set  of strings  is  then iterated
                through, and subsequently  their existence within  the bloom
                filter is tested, in the event a result of true is  returned
                the false positive count is incremented.

                At the end of the  process the bloom filter is  persisted to
                disk, read back from disk into a new bloom filter, then each
                element from the  original set is  tested against the  newly
                constructed bloom  filter, noting  any errors.  It should be
                noted that the  entropy of the  persisted bloom filter  is a
                good indicator as to the overall performance of the filter.
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
   static const std::size_t k = 14;
   static const std::size_t element_count = 2 * static_cast<std::size_t>(strtk::n_choose_k(n,k));
   static const double false_positive_probability = 0.0001;

   typedef strtk::combination_iterator<char*> iterator_type;

   strtk::bloom::parameters parameters;

   parameters.projected_element_count    = element_count;
   parameters.false_positive_probability = false_positive_probability;
   parameters.random_seed                = strtk::magic_seed;
   parameters.maximum_number_of_hashes   = 7;

   if (!parameters)
   {
      std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
      return 1;
   }

   parameters.compute_optimal_parameters();

   strtk::bloom::filter filter(parameters);

   printf("Filter Size: %7.3fKB "
          "Data Size: %8.3fKB "
          "Hash Count: %llu\n",
          filter.size() / (8.0 * strtk::one_kilobyte),
          (1.0 * element_count * k) / strtk::one_kilobyte,
          static_cast<unsigned long long>(filter.hash_count()));

   static const std::size_t buffer_size = k * element_count;
   unsigned char* buffer = new unsigned char[buffer_size];

   {
      iterator_type itr(k,letters);
      const iterator_type end(letters);

      unsigned char* buf_itr = buffer;

      unsigned int real_elem = 0;

      while (end != itr)
      {
         iterator_type::range_type range = *itr;
         std::string s(range.first,range.second);
         std::copy(s.data(),s.data() + s.size(),buf_itr);
         buf_itr+= s.size();
         strtk::convert_to_uppercase(s);
         std::copy(s.data(),s.data() + s.size(),buf_itr);
         buf_itr+= s.size();
         real_elem += 2;
         ++itr;
      }
   }

   {
      unsigned char* buf_itr = buffer;
      const unsigned char* buf_end = buffer + buffer_size;

      strtk::util::timer timer;
      timer.start();

      while (buf_end != buf_itr)
      {
         filter.insert(buf_itr,k);
         buf_itr += k;
      }

      timer.stop();

      printf("[insert ] Element Count: %llu\tTotal Time: %5.3fsec\tRate: %10.3felem/sec\n",
             static_cast<unsigned long long>(filter.element_count()),
             timer.time(),
             element_count / timer.time());
   }

   {
      unsigned char* buf_itr = buffer;
      const unsigned char* buf_end = buffer + buffer_size;

      strtk::util::timer timer;
      timer.start();

      while (buf_end != buf_itr)
      {
         if (!filter.contains(buf_itr,k))
         {
            std::cout << "Error: Failed to find: " << std::string(buf_itr,buf_itr + k) << std::endl;
         }

         buf_itr += k;
      }

      timer.stop();

      printf("[contain] Element Count: %llu\tTotal Time: %5.3fsec\tRate: %10.3felem/sec\n",
             static_cast<unsigned long long>(filter.element_count()),
             timer.time(),
             element_count / timer.time());
   }

   {
      static const std::size_t small_size = k / 2;

      unsigned int false_positive_count = 0;
      unsigned int small_element_count = 0;

      unsigned char* buf_itr = buffer;
      const unsigned char* buf_end = buffer + buffer_size;

      strtk::util::timer timer;
      timer.start();

      while (buf_end != buf_itr)
      {
         if (filter.contains(buf_itr,small_size))
         {
            ++false_positive_count;
         }

         ++small_element_count;
         buf_itr += small_size;
      }

      timer.stop();

      printf("[FPC    ] Element Count: %llu\tFalse Positive Count: %d\tFalse Positive Probability: %9.8f\tTotal Time: %5.3fsec\tRate: %10.3felem/sec\n",
             static_cast<unsigned long long>(small_element_count),
             false_positive_count,
             (1.0 * false_positive_count)/small_element_count,
             timer.time(),
             small_element_count / timer.time());
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
      else
      {
         unsigned char* buf_itr = buffer;
         const unsigned char* buf_end = buffer + buffer_size;
         unsigned int failures = 0;

         while (buf_end != buf_itr)
         {
            if (!secondary_filter.contains(buf_itr,k))
            {
               std::cout << "Error: Failed to find: " << std::string(buf_itr,buf_itr + k) << " in secondary filter!" << std::endl;
               ++failures;
            }

            buf_itr += k;
         }

         if (0 == failures)
         {
            std::cout << "Successfully replicated bloom filter." << std::endl;
         }
      }
   }

   delete [] buffer;

   return 0;
}
