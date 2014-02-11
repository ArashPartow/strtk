/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Text Parser Example 02                                        *
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
               word_list with tokens derived from the text file
               that are not in the "not_of_interest_list".
*/


#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <deque>

#include "strtk.hpp"


const std::string not_of_interest_list [] = { "as", "at", "but", "by", "for",
                                              "in", "like", "next", "of", "on",
                                              "opposite", "out", "past", "to",
                                              "up", "via", ""};

const std::size_t not_of_interest_list_size = sizeof(not_of_interest_list) / sizeof(std::string);


template<typename Container, typename Predicate>
struct parse_line
{
public:

   typedef typename strtk::filter_on_match< strtk::range_to_type_back_inserter_iterator<Container> > filter_type;
   typedef typename strtk::std_string::tokenizer<Predicate>::type tokenizer_type;
   parse_line(Container& c, Predicate& p)
   : c_(c),
     p_(p),
     tmp_(""),
     tokenizer_(tmp_,p_,true),
     filter_(reinterpret_cast<const std::string*>(not_of_interest_list),
             reinterpret_cast<const std::string*>(not_of_interest_list + not_of_interest_list_size),
             strtk::range_to_type_back_inserter(c_),
             true,
             false)
   {}

   parse_line(const parse_line& pl)
   : c_(pl.c_),
     p_(pl.p_),
     tmp_(pl.tmp_),
     tokenizer_(pl.tokenizer_),
     filter_(pl.filter_)
   {}

   parse_line& operator=(const parse_line& pl)
   {
      if (this != &pl)
      {
         c_ = pl.c_;
         p_ = pl.p_;
         tmp_ = pl.tmp_;
         tokenizer_ = pl.tokenizer_;
         filter_ = pl.filter_;
      }
      return *this;
   }

   inline void operator() (const std::string& s)
   {
      const filter_type& filter = filter_;
      strtk::for_each_token(s,tokenizer_,filter);
   }

private:

   Container& c_;
   Predicate& p_;
   std::string tmp_;
   tokenizer_type tokenizer_;
   filter_type filter_;
};

template<typename Container>
void parse_text(const std::string& file_name, Container& c)
{
   static const std::string delimiters = " ,.;:<>'[]{}()_?/\\'`~!@#$%^&*|-_\"=+\t\r\n0123456789";
   strtk::multiple_char_delimiter_predicate predicate(delimiters);
   typedef parse_line<Container,strtk::multiple_char_delimiter_predicate> pl_type;
   pl_type pl(c,predicate);
   const pl_type& pl_ref = pl;
   strtk::for_each_line<const pl_type&>(file_name,pl_ref);
}

int main()
{
   std::string text_file_name = "text.txt";
   std::deque< std::string > word_list;
   parse_text(text_file_name,word_list);
   std::cout << "Token Count: " << word_list.size() << std::endl;
   return 0;
}
