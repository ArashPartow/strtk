/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * IPv4 Parser                                                   *
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
   : predicate_('.'),
     tokenizer_(static_cast<char*>(0),static_cast<char*>(0),predicate_)
   {}

   inline bool operator()(const std::string& data, unsigned char octet[4])
   {
      tokenizer_.assign(data.data(),data.data() + data.size());
      tokenizer_type::iterator itr = tokenizer_.begin();
      tokenizer_type::const_iterator end = tokenizer_.end();
      std::size_t octet_count = 0;
      unsigned short tmp = 0;
      while ((octet_count < 4) && (end != itr))
      {
         tokenizer_type::iterator::value_type range = *itr;
         std::size_t token_length = std::distance(range.first,range.second);
         if (0 == token_length)
            return false;
         else if (!strtk::string_to_type_converter(range.first,range.second,tmp))
            return false;
         else if (tmp > 255)
            return false;
         octet[octet_count] = static_cast<unsigned char>(tmp);
         ++itr;
         ++octet_count;
      }
      return (4 == octet_count);
   }

   inline bool operator()(const std::string& data, unsigned int& ip)
   {
      ip = 0;
      return operator()(data,reinterpret_cast<unsigned char*>(&ip));
   }

private:

   strtk::single_delimiter_predicate<char> predicate_;
   tokenizer_type tokenizer_;
};

void print_octet(const unsigned char octet[4])
{
   strtk::ext_string s;
   s << (unsigned int)(octet[0]) << "."
     << (unsigned int)(octet[1]) << "."
     << (unsigned int)(octet[2]) << "."
     << (unsigned int)(octet[3]);
   std::cout << "IP (octet) = " << strtk::text::right_align(15,' ',s.as_string());
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
         std::cout << "IP (string):" << strtk::text::right_align(15,' ',ip[i]) << "\t";
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
