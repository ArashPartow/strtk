/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Word Frequency Calculator                                     *
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
  Description: This example demonstrates how one can calculate the
               word frequency model for a given piece of text using
               the String Toolkit library. Input is taken either
               from stdin or a user specified file. Once the text
               has been fully processed, the frequency of each word
               is then printed to stdout.
*/


#include <cstdio>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <map>

#include "strtk.hpp"


/*
   Note: For performance reasons, if TR1 is available then the
         following definition of std::map should be replaced
         with std::unordered_map.
*/
typedef std::map<std::string,unsigned int> map_t;

template<typename Predicate>
struct line_parser
{
public:

   line_parser(unsigned long long& word_count,
               map_t& map,
               Predicate& p)
   : word_count_(word_count),
     map_(map),
     p_(p)
   {
      str_.reserve(32);
   }

   inline void operator() (const std::string& s)
   {
      if (s.empty()) return;
      strtk::split(p_,s,*this,strtk::split_options::compress_delimiters);
   }

   inline void operator=(const strtk::std_string::iterator_type& r)
   {
      if (r.first == r.second) return;
      ++word_count_;
      str_.assign(r.first,r.second);
      strtk::convert_to_lowercase(str_);
      ++map_[str_];
   }

   inline line_parser& operator++()    { return (*this); }
   inline line_parser& operator++(int) { return (*this); }
   inline line_parser& operator*()     { return (*this); }

private:

   inline line_parser& operator=(const line_parser&);

   unsigned long long& word_count_;
   map_t& map_;
   Predicate& p_;
   std::string str_;
};

int main(int argc, char* argv[])
{
   typedef strtk::multiple_char_delimiter_predicate predicate_t;
   typedef line_parser<const predicate_t> lp_t;

   const std::string delimiters = strtk::ext_string::all_chars()
                                - strtk::ext_string::all_lowercase_letters()
                                - strtk::ext_string::all_uppercase_letters();

   static const predicate_t predicate(delimiters);

   map_t word_list;
   unsigned long long word_count = 0;

   switch (argc)
   {
                // Consume input from stdin
      case 1  : strtk::for_each_line(std::cin, lp_t(word_count, word_list, predicate));
                break;

                // Consume input from user specified file
      case 2  : strtk::for_each_line(argv[1], lp_t(word_count, word_list, predicate));
                break;

      default :
               {
                  std::cout << "usage: strtk_wordfreq <file name>" << std::endl;
                  std::cout << "usage: cat words.txt | strtk_wordfreq" << std::endl;
                  return 1;
               }
   }

   std::cout << "Word count: " << word_count << std::endl;
   std::cout << "Unique word count: " << word_list.size() << std::endl;

   map_t::iterator itr = word_list.begin();

   while (word_list.end() != itr)
   {
      printf("%s %10d %10.9f\n",
             strtk::text::right_align(15, ' ', itr->first).c_str(),
             itr->second,
             (1.0 * itr->second) / word_count);
      ++itr;
   }

   return 0;
}
