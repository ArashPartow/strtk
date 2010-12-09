/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Key-Value Pair Parser Example                                 *
 * Author: Arash Partow (2002-2010)                              *
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


#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <deque>

#include "strtk.hpp"


template<typename Container>
struct key_value_parser
{
public:

   key_value_parser(Container& c, const std::string& delimiters)
   : c_(&c),
     delimiters_(delimiters){}

   void operator()(const strtk::std_string::tokenizer<>::iterator_type& it)
   {
      std::pair<std::string,std::string> kv;
      strtk::parse(it.first,it.second,delimiters_,kv.first,kv.second);
      c_->push_back(kv);
   }

private:

   Container* c_;
   std::string delimiters_;
};

int main()
{
   const std::string data = "int=-1|unsigned int=2345|double=6.789|string=a simple string";

   typedef strtk::std_string::tokenizer<>::type tokenizer_type;
   typedef std::deque< std::pair<std::string,std::string> > pair_list_type;
   typedef key_value_parser<pair_list_type> kvp_type;

   tokenizer_type pair_tokenizer(data,'|');
   pair_list_type keyvalue_list;
   kvp_type kvp(keyvalue_list,"=");

   strtk::for_each_token(data,pair_tokenizer,kvp);

   pair_list_type::iterator it = keyvalue_list.begin();
   while (keyvalue_list.end() != it)
   {
      std::cout << "<" << (*it).first << "," << (*it).second << ">" << std::endl;
      ++it;
   }

   return 0;
}
