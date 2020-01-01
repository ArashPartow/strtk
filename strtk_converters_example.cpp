/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Converters Examples                                           *
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


#include <cstddef>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <utility>
#include <string>
#include <deque>

#include "strtk.hpp"


void example01()
{
   std::string text = "An expert is someone who knows more and more about less and "
                      "less until they know absolutely everything about nothing";
   std::string hex;
   std::string base64;
   std::string hexbin;
   std::string base64bin;

   strtk::convert_bin_to_hex(text,hex);
   strtk::convert_hex_to_bin(hex,hexbin);
   strtk::convert_bin_to_base64(text,base64);
   strtk::convert_base64_to_bin(base64,base64bin);

   std::cout << text << std::endl;
   std::cout << hex << std::endl;
   std::cout << hexbin << std::endl;
   std::cout << base64 << std::endl;
   std::cout << base64bin << std::endl;
   std::cout << std::endl;
}

void example02()
{
   static const std::size_t size = 1024;
   unsigned char buffer[size];
   unsigned char hex_buffer[2 * size];
   unsigned char b64_buffer[2 * size];
   for (std::size_t i = 0; i < size; ++i) buffer[i] = static_cast<unsigned char>(i);
   strtk::convert_bin_to_hex(buffer,buffer + size, hex_buffer);
   strtk::convert_bin_to_base64(buffer,buffer + size, b64_buffer);
}

void example03()
{
   static const std::size_t size = 1024;
   unsigned char buffer1[size];
   unsigned char buffer2[size];
   unsigned char b64_buffer[2 * size];

   for (std::size_t k = 1; k < size; ++k)
   {
      for (std::size_t i = 0; i < k; ++i) buffer1[i] = static_cast<unsigned char>(i);

      std::size_t b64_size = strtk::convert_bin_to_base64(buffer1,buffer1 + k, b64_buffer);
      std::fill_n(buffer2, k, static_cast<unsigned char>(0x00));
      strtk::convert_base64_to_bin(b64_buffer,b64_buffer + b64_size, buffer2);

      for (std::size_t i = 0; i < k; ++i)
      {
         if (buffer1[i] != buffer2[i])
         {
            std::cout << "example03 - failed bin-b64 conversions, error at position: " << i << std::endl;
            return;
         }
      }
   }
}

void bin2hex_speed_test()
{
   unsigned char* bin = new unsigned char[strtk::one_megabyte];
   unsigned char* hex = new unsigned char[2 * strtk::one_megabyte];

   strtk::range::adapter<unsigned char> r1(bin,bin + strtk::one_megabyte);
   strtk::range::adapter<unsigned char> r2(hex,hex + (2 * strtk::one_megabyte));

   strtk::iota(r1,static_cast<unsigned char>(0x00));

   static const std::size_t rounds = 2000;

   {
      strtk::util::timer t;
      t.start();

      for (std::size_t r = 0; r < rounds; ++r)
      {
         strtk::convert_bin_to_hex(r1.begin(),r1.end(),r2.begin());
      }

      t.stop();

      printf("[bin2hex] Data Size: %6lluMB  Total time:%8.4f  Rate: %6.2fMB/s\n",
             static_cast<unsigned long long>(rounds * r1.size()) / strtk::one_megabyte,
             t.time(),
             (rounds * r1.size()) / (1048576.0 * t.time()));
   }

   {
      strtk::util::timer t;
      t.start();

      for (std::size_t r = 0; r < rounds; ++r)
      {
         strtk::convert_hex_to_bin(r2.begin(),r2.end(),r1.begin());
      }

      t.stop();

      printf("[hex2bin] Data Size: %6lluMB  Total time:%8.4f  Rate: %6.2fMB/s\n",
             static_cast<unsigned long long>(rounds * r1.size()) / strtk::one_megabyte,
             t.time(),
             (rounds * r1.size()) / (1048576.0 * t.time()));
   }

   delete[] bin;
   delete[] hex;
}

void bin2b64_speed_test()
{
   unsigned char* bin = new unsigned char[strtk::one_megabyte];
   unsigned char* b64 = new unsigned char[2 * strtk::one_megabyte];

   strtk::range::adapter<unsigned char> r1(bin,bin + strtk::one_megabyte);
   strtk::range::adapter<unsigned char> r2(b64,b64 + (2 * strtk::one_megabyte));

   strtk::iota(r1,static_cast<unsigned char>(0x00));

   static const std::size_t rounds = 2000;
   std::size_t length = 0;

   {
      strtk::util::timer t;
      t.start();

      for (std::size_t r = 0; r < rounds; ++r)
      {
         length = strtk::convert_bin_to_base64(r1.begin(),r1.end(),r2.begin());
      }

      t.stop();

      printf("[bin2b64] Data Size: %6lluMB  Total time:%8.4f  Rate: %6.2fMB/s\n",
             static_cast<unsigned long long>(rounds * r1.size()) / strtk::one_megabyte,
             t.time(),
             (rounds * r1.size()) / (1048576.0 * t.time()));
   }

   {
      strtk::util::timer t;
      t.start();

      for (std::size_t r = 0; r < rounds; ++r)
      {
         strtk::convert_base64_to_bin(r2.begin(), r2.begin() + length, r1.begin());
      }

      t.stop();

      printf("[b642bin] Data Size: %6lluMB  Total time:%8.4f  Rate: %6.2fMB/s\n",
             static_cast<unsigned long long>(rounds * r1.size()) / strtk::one_megabyte,
             t.time(),
             (rounds * r1.size()) / (1048576.0 * t.time()));
   }

   delete[] bin;
   delete[] b64;
}

int main()
{
   example01();
   example02();
   example03();

   bin2hex_speed_test();
   bin2b64_speed_test();

   return 0;
}
