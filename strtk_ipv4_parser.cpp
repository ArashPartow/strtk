/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * IPv4 Parser                                                   *
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
   Description: This example demonstrates how to build a simple IPv4
                parser. The parser takes a valid input and places it
                into 4 octets or the first 4 bytes of an unsigned int.
*/


#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

#include "strtk.hpp"


class ipv4_parser
{
   typedef strtk::tokenizer<const char*,strtk::single_delimiter_predicate<char> > tokenizer_type;

public:

   ipv4_parser()
   : predicate_('.')
   {}

   inline bool operator()(const std::string& data, unsigned char octet[4])
   {
      typedef std::pair<const char*,const char*> iterator_type;
      iterator_type token[4];

      if (4 != strtk::split_n(predicate_, data, 4, token))
         return false;

      if (!process_token(token[0],octet[0])) return false;
      if (!process_token(token[1],octet[1])) return false;
      if (!process_token(token[2],octet[2])) return false;
      if (!process_token(token[3],octet[3])) return false;

      return true;
   }

   inline bool operator()(const std::string& data, unsigned int& ip)
   {
      ip = 0;
      return operator()(data,reinterpret_cast<unsigned char*>(&ip));
   }

private:

   inline bool process_token(const std::pair<const char*,const char*>& token, unsigned char& octet)
   {
      unsigned int v = 0;
      if (!strtk::fast::numeric_convert(strtk::distance(token), token.first, v, true))
         return false;
      if (v > 255)
         return false;
      octet = static_cast<unsigned char>(v);
      return true;
   }

   strtk::single_delimiter_predicate<char> predicate_;
};

void print_octet(const unsigned char octet[4])
{
   strtk::ext_string s;
   s << (unsigned int)(octet[0]) << "."
     << (unsigned int)(octet[1]) << "."
     << (unsigned int)(octet[2]) << "."
     << (unsigned int)(octet[3]);
   std::cout << "IP (octet) = " << strtk::text::right_align(15, ' ', s.as_string());
}

int main()
{
   unsigned char octet[4] = {0};

   static const std::string ip[] =
                {
                   "1.1.1.1",
                   "12.12.12.12",
                   "123.123.123.123"
                };

   static const std::size_t ip_size = sizeof(ip) / sizeof(std::string);

   ipv4_parser parser;

   for (std::size_t i = 0; i < ip_size; ++i)
   {
      if (!parser(ip[i],octet))
         std::cout << "Failed to parse ip: " << ip[i] << std::endl;
      else
      {
         std::cout << "IP (string):" << strtk::text::right_align(15, ' ', ip[i]) << "\t";
         print_octet(octet);
      }

      unsigned int ip_int = 0;

      if (!parser(ip[i],ip_int))
         std::cout << "Failed to parse ip_int: " << ip[i] << std::endl;
      else
         std::cout << "\tIP(int): " << strtk::text::right_align(' ',ip_int) << std::endl;
   }

   return 0;
}
