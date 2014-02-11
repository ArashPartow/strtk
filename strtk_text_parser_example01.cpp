/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Text Parser Example 01                                        *
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
  Description: The following will parse a text file line-by-line,
               tokenizing each line using the delimiters set as
               described blow. The objective is to populate the
               word_list with tokens derived from the text file.
*/


#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <deque>

#include "strtk.hpp"


template<typename Container, typename Predicate>
struct parse_line
{
public:

   parse_line(Container& c, Predicate& p)
   : c_(&c),
     p_(&p)
   {}

   inline void operator()(const std::string& s)
   {
      strtk::split(*p_,
                    s,
                    strtk::range_to_type_back_inserter(*c_),
                    strtk::split_options::compress_delimiters);
   }

private:

   Container* c_;
   Predicate* p_;
};

template<typename Container>
void parse_text(const std::string& file_name, Container& c)
{
   static const std::string delimiters = " ,.;:<>'[]{}()_?/\\'`~!@#$%^&*|-_\"=+\t\r\n0123456789";
   strtk::multiple_char_delimiter_predicate predicate(delimiters);
   parse_line<Container,strtk::multiple_char_delimiter_predicate> pl(c,predicate);
   strtk::for_each_line(file_name,pl);
}

int main()
{
   std::string text_file_name = "text.txt";
   std::deque< std::string > word_list;
   parse_text(text_file_name,word_list);
   std::cout << "Token Count: " << word_list.size() << std::endl;
   return 0;
}
