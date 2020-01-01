/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * String Toolkit Library Examples                               *
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
#include <string>
#include <iterator>
#include <utility>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <functional>

#include "strtk.hpp"


void information()
{
   std::cout << strtk::information::data() << std::endl;
}

void tokenizer_example01()
{
   std::cout << "tokenizer_example01" << std::endl;
   std::string s = "abc|123|xyz|789";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   typedef strtk::std_string::tokenizer<>::type tokenizer_type;

   tokenizer_type tokenizer(s,predicate);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << itr << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example02()
{
   std::cout << "tokenizer_example02" << std::endl;
   std::string s = "abc.123 xyz?789";
   strtk::multiple_char_delimiter_predicate predicate(" .;?");

   typedef strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer_type;

   tokenizer_type tokenizer(s,predicate,strtk::tokenize_options::compress_delimiters);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << itr << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example03()
{
   std::cout << "tokenizer_example03" << std::endl;
   std::string s = "abc||123|||||xyz|789";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');

   typedef strtk::std_string::tokenizer<>::type tokenizer_type;

   tokenizer_type tokenizer(s,predicate,strtk::tokenize_options::compress_delimiters);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << itr << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example04()
{
   std::cout << "tokenizer_example04" << std::endl;
   std::string s = "abc.;123? xyz;?789";
   strtk::multiple_char_delimiter_predicate predicate(" .;?");

   typedef strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type tokenizer_type;

   tokenizer_type tokenizer(s,predicate,strtk::tokenize_options::compress_delimiters);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << itr << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example05()
{
   std::cout << "tokenizer_example05" << std::endl;
   unsigned int data[] = {1,2,3,0,4,5,6,0,7,8,0,9};
   const std::size_t data_size = sizeof(data) / sizeof(unsigned int);
   strtk::single_delimiter_predicate<unsigned int> predicate(0);

   typedef strtk::tokenizer< unsigned int*,strtk::single_delimiter_predicate<unsigned int> > tokenizer_type;
   tokenizer_type tokenizer(data,data + data_size,predicate);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << strtk::join(" ",(*itr).first,(*itr).second) << "]";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example06()
{
   std::cout << "tokenizer_example06" << std::endl;
   unsigned int data[] = {1,2,3,0,4,5,6,10,7,8,0,9};
   const std::size_t data_size = sizeof(data) / sizeof(unsigned int);
   unsigned int delimiters[2] = {0,10};
   strtk::multiple_delimiter_predicate<unsigned int> predicate(delimiters,delimiters + 2);

   typedef strtk::tokenizer< unsigned int*,strtk::multiple_delimiter_predicate<unsigned int> > tokenizer_type;
   tokenizer_type tokenizer(data,data + data_size,predicate);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << strtk::join(" ",(*itr).first,(*itr).second) << "]";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example07()
{
   std::cout << "tokenizer_example07" << std::endl;
   double data[] = {1.1,2.2,3.3,0.0,4.4,5.5,6.6,0,7.7,8.8,0,9.9};
   const std::size_t data_size = sizeof(data) / sizeof(double);
   strtk::single_delimiter_predicate<double> predicate(0);

   typedef strtk::tokenizer< double*,strtk::single_delimiter_predicate<double> > tokenizer_type;
   tokenizer_type tokenizer(data,data + data_size,predicate);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << strtk::join(" ",(*itr).first,(*itr).second) << "]";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example08()
{
   std::cout << "tokenizer_example08" << std::endl;
   double data[] = {1.1,2.2,3.3,0.0,4.4,5.5,6.6,10.0,7.7,8.8,10.0,9.9};
   const std::size_t data_size = sizeof(data) / sizeof(double);
   double delimiters[2] = {0.0,10.0};
   strtk::multiple_delimiter_predicate<double> predicate(delimiters,delimiters + 2);

   typedef strtk::tokenizer< double*,strtk::multiple_delimiter_predicate<double> > tokenizer_type;
   tokenizer_type tokenizer(data,data + data_size,predicate);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << strtk::join(" ",(*itr).first,(*itr).second) << "]";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example09()
{
   std::cout << "tokenizer_example09" << std::endl;
   std::string s = "abc|123|xyz|789";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::std_string::tokenizer<>::type tokenizer(s,predicate);
   std::deque<std::string> token_list;
   std::copy(tokenizer.begin(),tokenizer.end(),strtk::range_to_type_back_inserter(token_list));
   std::cout << strtk::join("\t",token_list) << std::endl;
}

void tokenizer_example10()
{
   std::cout << "tokenizer_example10" << std::endl;
   std::string str_list[] = { "abc" , "delimiter" , "ijk" , "delimiter" ,
                              "lmn" , "delimiter" , "opq", "rst" ,"uvw" ,
                              "delimiter" , "xyz" , "123" };

   const std::size_t str_list_size = sizeof(str_list) / sizeof(std::string);
   strtk::range::adapter<std::string> range(str_list,str_list_size);
   strtk::single_delimiter_predicate<std::string> predicate("delimiter");

   typedef strtk::tokenizer< std::string*,strtk::single_delimiter_predicate<std::string> > tokenizer_type;
   tokenizer_type tokenizer(range.begin(),range.end(),predicate);
   tokenizer_type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << strtk::join(" ",(*itr).first,(*itr).second) << "]";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example11()
{
   std::cout << "tokenizer_example11" << std::endl;
   std::string s = "123|456|789|101112";
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');

   typedef strtk::std_string::tokenizer<strtk::single_delimiter_predicate<std::string::value_type> >::type tokenizer_type;
   tokenizer_type tokenizer(s,predicate);

   std::list<int> token_list;
   std::copy(tokenizer.begin(),tokenizer.end(),strtk::range_to_type_back_inserter(token_list));
   std::cout << strtk::join("\t",token_list) << std::endl;
}

void tokenizer_example12()
{
   std::cout << "tokenizer_example12" << std::endl;
   // tokenizer_example01 with much simpler syntax.
   std::string s = "abc|123|xyz|789";

   strtk::std_string::tokenizer<>::type tokenizer(s,strtk::std_string::tokenizer<>::predicate_type('|'));
   strtk::std_string::tokenizer<>::type::iterator itr = tokenizer.begin();

   while (tokenizer.end() != itr)
   {
      std::cout << "[" << itr << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void tokenizer_example13()
{
   std::cout << "tokenizer_example13" << std::endl;
   std::string s = "abc|123|xyz|789";
   strtk::std_string::tokenizer<>::type tokenizer(s,strtk::std_string::tokenizer<>::predicate_type('|'));
   strtk::std_string::tokenizer<>::type::iterator itr = tokenizer.begin();
   std::cout << "[" << std::string((*itr).first,(*itr).second) << "]\t"; ++itr;
   std::cout << "[" << std::string((*itr).first,(*itr).second) << "]\t"; ++itr;
   std::cout << "Remaining string: " << itr.remaining() << std::endl;
}

void split_example01()
{
   std::cout << "split_example01" << std::endl;
   std::string s = "abc|123|xyz|789";
   strtk::std_string::token_list_type token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::split(predicate,s,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void split_example02()
{
   std::cout << "split_example02" << std::endl;
   std::string s = "abc?123,xyz;789";
   strtk::std_string::token_list_type token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::split(predicate,s,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void split_example03()
{
   std::cout << "split_example03" << std::endl;
   std::string s = "abc|123|xyz|789";
   strtk::std_string::token_list_type token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   strtk::split(predicate,s,std::back_inserter(token_list),strtk::split_options::compress_delimiters);
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void split_example04()
{
   std::cout << "split_example04" << std::endl;
   std::string s = "abc?123,xyz;789";
   strtk::std_string::token_list_type token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   strtk::split(predicate,s,std::back_inserter(token_list),strtk::split_options::compress_delimiters);
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void split_example05()
{
   std::cout << "split_example05" << std::endl;
   std::string s = "abc?123,xyz;789";
   strtk::std_string::token_list_type token_list;
   strtk::split(" .;?",s,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void split_regex_example01()
{
   #ifdef strtk_enable_regex
   std::cout << "split_regex_example01" << std::endl;
   std::string s = "(12)(345)(6789)(0ijkx)(yz)";
   std::list<std::string> token_list;
   strtk::split_regex("\\(.*?\\)",
                      s,
                      strtk::range_to_type_back_inserter(token_list),
                      strtk::regex_match_mode::match_1);
   std::cout << strtk::join("\t",token_list) << std::endl;
   #endif
}

void split_regex_example02()
{
   #ifdef strtk_enable_regex
   std::cout << "split_regex_example02" << std::endl;
   {
      std::string data = "a 1^bc,0023| def?gh(4567ijk)-89 10l,m$n-op+123r@st+3u v*w2y56yz+";
      std::deque<int> int_list;
      strtk::split_regex("([+-]?([\\d]+))",
                         data,
                         strtk::range_to_type_back_inserter(int_list),
                         strtk::regex_match_mode::match_1);
      std::cout << strtk::join(" ",int_list) << std::endl;
   }

   {
      std::string data = "ab$c1.1?d-2.2ef#ghi+3.3%(123.456)!&*-7.89E+12@^=";
      std::deque<double> double_list;
      strtk::split_regex(strtk::ieee754_expression,
                         data,
                         strtk::range_to_type_back_inserter(double_list),
                         strtk::regex_match_mode::match_1);
      std::cout << strtk::join(" ",double_list) << std::endl;
   }
   #endif
}

void split_n_example01()
{
   std::cout << "split_n_example01" << std::endl;
   std::string s = "token1|token2|token3|token4|token5";
   strtk::std_string::token_list_type token_list;
   strtk::single_delimiter_predicate<std::string::value_type> predicate('|');
   const std::size_t token_count = 4;
   strtk::split_n(predicate,s,token_count,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void split_n_example02()
{
   std::cout << "split_n_example02" << std::endl;
   std::string s = "token1?token2,token3;token4,token5";
   strtk::std_string::token_list_type token_list;
   strtk::multiple_char_delimiter_predicate predicate(" .;?");
   const std::size_t token_count = 4;
   strtk::split_n(predicate,s,token_count,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void split_n_example03()
{
   std::cout << "split_n_example03" << std::endl;
   std::string s = "token1?token2,token3;token4,token5";
   strtk::std_string::token_list_type token_list;
   const std::size_t token_count = 4;
   strtk::split_n(" .;?",s,token_count,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "]\t";
      ++itr;
   }

   std::cout << std::endl;
}

void split_regex_n_example01()
{
   #ifdef strtk_enable_regex
   std::cout << "split_regex_n_example01" << std::endl;
   std::string s = "(token1)(token2)(token3)(token4)(token5)";
   std::list<std::string> token_list;
   const std::size_t token_count = 4;
   strtk::split_regex_n("\\(.*?\\)",
                        s,
                        token_count,
                        strtk::range_to_type_back_inserter(token_list));
   std::cout << strtk::join("\t",token_list) << std::endl;
   #endif
}

void split_regex_n_example02()
{
   #ifdef strtk_enable_regex
   std::cout << "split_regex_n_example02" << std::endl;
   {
      std::string data = "a 1^bc,0023| def?gh(4567ijk)-89 10l,m$n-op+123r@st+3u v*w2y56yz+";
      std::deque<int> int_list;
      strtk::split_regex_n("([+-]?([\\d]+))",
                           data,
                           3, // Extract the first 3 ints
                           strtk::range_to_type_back_inserter(int_list),
                           strtk::regex_match_mode::match_1);
      std::cout << strtk::join(" ",int_list) << std::endl;
   }

   {
      std::string data = "ab$c1.1?d-2.2ef#ghi+3.3%(123.456)!&*-7.89E+12@^=";
      std::deque<double> double_list;
      strtk::split_regex_n(strtk::ieee754_expression,
                           data,
                           4, // Extract the first 4 doubles
                           strtk::range_to_type_back_inserter(double_list),
                           strtk::regex_match_mode::match_1);
      std::cout << strtk::join(" ",double_list) << std::endl;
   }
   #endif
}

void offset_splitter_example01()
{
   std::cout << "offset_splitter_example01" << std::endl;
   std::string s = "abcdefghijklmnopqrstuvwxyz012";
   const int offset_list[] = {1,2,3,4,5,6,7};
   const strtk::offset_predicate<7> os_p(offset_list);
   strtk::std_string::token_list_type token_list;
   strtk::offset_splitter(s,os_p,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "] ";
      ++itr;
   }

   std::cout << std::endl;
}

void offset_splitter_example02()
{
   std::cout << "offset_splitter_example02" << std::endl;
   std::string s = "20000101091011123";
   const int offset_list[] = {4,2,2,2,2,2,3};
   const strtk::offset_predicate<7> os_p(offset_list);
   strtk::std_string::token_list_type token_list;
   strtk::offset_splitter(s,os_p,std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      if (1 != std::distance((*itr).first,(*itr).second))
      {
         std::cout << "[" << (*itr) << "] ";
      }

      ++itr;
   }

   std::cout << std::endl;
}

void offset_splitter_example03()
{
   std::cout << "offset_splitter_example03" << std::endl;
   std::string s = "abcdefghijklmnopqrstuvwxyz012";
   strtk::std_string::token_list_type token_list;
   strtk::offset_splitter(s,strtk::offsets(1,2,3,4,5,6,7),std::back_inserter(token_list));
   strtk::std_string::token_list_type::iterator itr = token_list.begin();

   while (token_list.end() != itr)
   {
      std::cout << "[" << (*itr) << "] ";
      ++itr;
   }

   std::cout << std::endl;
}

void construct_example()
{
   std::cout << "construct_example" << std::endl;
   std::string  i1 = "abcd";
   char         i2 = 'x';
   int          i3 = -1234;
   unsigned int i4 = 78901;
   double       i5 = 4567.8901;
   std::string output = "";
   strtk::construct(output,"|",i1,i2,i3,i4,i5);
   std::cout << output << std::endl;
}

void parse_example01()
{
   std::cout << "parse_example01" << std::endl;
   std::string input = "abcd|x|-1234|78901|4567.8901|0x75BCD15|AABB";

   std::string  o1 = "";
   char         o2 = 0x00;
   int          o3 = 0;
   unsigned int o4 = 0;
   double       o5 = 0;
   unsigned int o6 = 0;
   short int    o7 = 0;
   strtk::hex_to_number_sink<unsigned int> hns1(o6);
   strtk::hex_to_number_sink<short int> hns2(o7);
   strtk::parse(input,"|",o1,o2,o3,o4,o5,hns1,hns2);

   std::cout << o1 << "\t"
             << o2 << "\t"
             << o3 << "\t"
             << o4 << "\t"
             << o5 << "\t"
             << o6 << "\t"
             << o7 << std::endl;
}

void parse_example02()
{
   std::cout << "parse_example02" << std::endl;

   std::string int_string    = "0,-1,+2,-3,4,-5,+6,-7,8,-9";
   std::string uint_string   = "0,100,200,300,400,500,600,700,800,900";
   std::string double_string = "0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9";
   std::string string_string = "ab,cde,fghi,jklmn,opqrst,uvwxyz1,234567890";
   std::string float_string  = "1.9f,2.8f,3.7f,4.6f,5.5f,6.4f,7.3f,8.2f,9.1f,0.0f";

   std::vector<int>                  int_list;
   std::deque<double>                double_list;
   std::list<std::string>            string_list;
   std::set<float>                   float_set;
   std::multiset<float>              float_multiset;
   std::queue<unsigned int>          uint_queue;
   std::stack<unsigned int>          uint_stack;
   std::priority_queue<unsigned int> uint_prique;

   strtk::parse(int_string   , ",", int_list       );
   strtk::parse(double_string, ",", double_list    );
   strtk::parse(string_string, ",", string_list    );
   strtk::parse(float_string , ",", float_set      );
   strtk::parse(float_string , ",", float_multiset );
   strtk::parse(uint_string  , ",", uint_queue     );
   strtk::parse(uint_string  , ",", uint_stack     );
   strtk::parse(uint_string  , ",", uint_prique    );

   std::cout << strtk::join("\t", int_list      ) << std::endl;
   std::cout << strtk::join("\t", double_list   ) << std::endl;
   std::cout << strtk::join("\t", string_list   ) << std::endl;
   std::cout << strtk::join("\t", float_set     ) << std::endl;
   std::cout << strtk::join("\t", float_multiset) << std::endl;
}

void parse_example03()
{
   std::cout << "parse_example03" << std::endl;
   std::string int_string    = "0,1,2,3,4,5,6,7,8,9";
   std::string uint_string   = "0,100,200,300,400,500,600,700,800,900";
   std::string double_string = "0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9";

   std::vector<int> int_list;
   std::deque<double> double_list;

   std::queue<unsigned int> uint_queue;
   std::stack<unsigned int> uint_stack;
   std::priority_queue<unsigned int> uint_prique;

   static const std::size_t n = 4;

   strtk::parse_n(int_string,",",n,int_list);
   strtk::parse_n(double_string,",",n,double_list);

   strtk::parse_n(uint_string,",",n,uint_queue);
   strtk::parse_n(uint_string,",",n,uint_stack);
   strtk::parse_n(uint_string,",",n,uint_prique);

   std::cout << strtk::join("\t",int_list) << std::endl;
   std::cout << strtk::join("\t",double_list) << std::endl;
}

struct type
{
   unsigned int id;
   std::string s;
   double d;
   int i;
   char c;
   bool b;
};

strtk_parse_begin(type)
   strtk_parse_type(s)
   strtk_parse_type(d)
   strtk_parse_type(i)
   strtk_parse_type(c)
   strtk_parse_type(b)
strtk_parse_end()

void parse_example04()
{
   std::cout << "parse_example04" << std::endl;
   type t;
   std::string s = "abcdefghijklmnop|123.456|987654321|A|1";
   if (strtk::parse(s,"|",t))
   {
      std::string o;
      strtk::construct(o,"|",t.s,t.d,t.i,t.c,t.b);
      std::cout << "type: " << o << std::endl;
   }
}

struct datetime
{
   unsigned int year;
   unsigned int month;
   unsigned int day;
   unsigned int hour;
   unsigned int minute;
   unsigned int second;
   unsigned int msecond;
};

strtk_string_to_type_begin(datetime)
   static const std::string delimiters ("-:. ");
   return strtk::parse(begin, end, delimiters,
                       t.year, t.month, t.day,
                       t.hour, t.minute, t.second, t.msecond);
strtk_string_to_type_end()

void parse_example05()
{
   std::cout << "parse_example05" << std::endl;

   static const std::string data = "2000-01-10 03:01:16.123|2001-02-22 05:12:24.234|"
                                   "2002-03-13 07:23:32.345|2003-04-24 09:34:47.456|"
                                   "2004-05-15 11:46:51.767|2005-06-26 15:57:03.678|"
                                   "2006-07-17 17:45:31.561|2007-08-26 19:06:02.809|"
                                   "2008-09-18 21:16:23.267|2009-10-26 23:12:03.798|"
                                   "2010-11-23 13:47:11.963|2011-12-26 15:35:08.168";

   std::deque<datetime> datetime_list;
   strtk::parse(data,"|",datetime_list);

   for (std::size_t i = 0; i < datetime_list.size(); ++i)
   {
      datetime& dt = datetime_list[i];
      std::cout << dt.year << "-"
                << strtk::text::right_align(2,'0',  dt.month) << "-"
                << strtk::text::right_align(2,'0',    dt.day) << " "
                << strtk::text::right_align(2,'0',   dt.hour) << ":"
                << strtk::text::right_align(2,'0', dt.minute) << ":"
                << strtk::text::right_align(2,'0', dt.second) << "."
                << strtk::text::right_align(3,'0',dt.msecond) << std::endl;
   }
}

struct event_information
{
   std::size_t id;
   std::string name;
   std::string location;
   std::deque<datetime> observation_datetimes;
};

strtk_register_userdef_type_sink(datetime)

void parse_example06()
{
   std::cout << "parse_example06" << std::endl;

   static const std::string event_data = "172493|Lunar Impact|Mare Tranquillitatis|"
                                         "2010-01-19 00:28:45.357,2010-02-18 00:57:07.109,"
                                         "2010-03-20 01:15:11.261,2010-04-21 01:07:27.972";

   strtk::deque_sink<datetime>::type deq_sink(",");

   event_information evt_info;
   strtk::parse(event_data,"|",evt_info.id,
                               evt_info.name,
                               evt_info.location,
                               deq_sink(evt_info.observation_datetimes));

}

void parse_example07()
{
   std::cout << "parse_example07" << std::endl;

   {
      static const std::string data = "1,+2,-3|abc,ijk,xyz|123.456,+234.567,-345.678";
      std::vector<int> int_vector;
      std::deque<std::string> string_deque;
      std::list<double> double_list;

      strtk::vector_sink<int>::type vec_sink(",");
      strtk::deque_sink<std::string>::type deq_sink(",");
      strtk::list_sink<double>::type lst_sink(",");

      strtk::parse(data,"|",vec_sink(int_vector),
                            deq_sink(string_deque),
                            lst_sink(double_list));

      std::cout << "int_vec: "     << strtk::join(" ",int_vector)   << std::endl;
      std::cout << "string_deq: "  << strtk::join(" ",string_deque) << std::endl;
      std::cout << "double_list: " << strtk::join(" ",double_list)  << std::endl;
   }

   {
      static const std::string data = "1,+2,-3|abc,ijk,xyz|123.456,+234.567,-345.678|-7-6,-5-4,-3-2,-1";

      std::set<int> int_set;
      std::multiset<int> int_multiset;
      std::queue<std::string> string_queue;
      std::stack<double> double_stack;
      std::priority_queue<int> int_priority_queue;

      strtk::set_sink<int>::type            set_sink(",");
      strtk::multiset_sink<int>::type       multiset_sink(",");
      strtk::queue_sink<std::string>::type  que_sink(",");
      strtk::stack_sink<double>::type       stk_sink(",");
      strtk::priority_queue_sink<int>::type prq_sink(",");

      strtk::parse(data,"|",set_sink(int_set),
                            que_sink(string_queue),
                            stk_sink(double_stack),
                            prq_sink(int_priority_queue));

      strtk::util::clear(int_set);
      strtk::util::clear(int_multiset);
      strtk::util::clear(string_queue);
      strtk::util::clear(double_stack);
      strtk::util::clear(int_priority_queue);

      strtk::parse(data,"|",multiset_sink(int_multiset),
                            que_sink(string_queue),
                            stk_sink(double_stack),
                            prq_sink(int_priority_queue));

   }

   {
      static const std::string data = "1,+2,-3,4|abc,ijk,rst,xyz|123.456,+234.567,-345.678,456.789,567.890";
      std::vector<int> int_vector;
      std::deque<std::string> string_deque;
      std::list<double> double_list;

      strtk::vector_sink<int>::type        vec_sink(",");
      strtk::deque_sink<std::string>::type deq_sink(",");
      strtk::list_sink<double>::type       lst_sink(",");

      strtk::parse(data,"|",vec_sink(  int_vector).count(2),  // consume first 2 values
                            deq_sink(string_deque).count(3),  // consume first 3 values
                            lst_sink( double_list).count(4)); // consume first 4 values

      std::cout << "int_vec(2): "     << strtk::join(" ",int_vector)   << std::endl;
      std::cout << "string_deq(3): "  << strtk::join(" ",string_deque) << std::endl;
      std::cout << "double_list(4): " << strtk::join(" ",double_list)  << std::endl;
   }
}

void parse_example08()
{
   std::cout << "parse_example08 " << std::endl;

   static const std::string data = "+123,ignore0,123.456,ignore1,abcdef,ignore2";
   int i = 0;
   double d = 0.0;
   std::string s;

   strtk::ignore_token ignore;
   strtk::parse(data,",",i,ignore,d,ignore,s);
   std::cout << "i=" << i << " d=" << d << " s=" << s << std::endl;
}

void parse_example09()
{
   {
      std::string data = "A String Value,111.111,222.222,333.333,444.444,555.555";
      std::string token;
      std::deque<double> double_list;
      strtk::parse(data,",",token,double_list);
      std::cout << "parse_example09(): " << token << strtk::join(" ",double_list) << "\n";
   }
   {
      std::string data = "A String Value,01-01-2000,111.111,222.222,333.333,444.444,555.555";
      std::string token;
      std::string date;
      std::vector<double> double_list;
      strtk::parse(data,",",token,date,double_list);
      std::cout << "parse_example09(): " << token << " " << date << " " << strtk::join(" ",double_list) << "\n";
   }
   {
      std::string data = "A String Value,01-01-2000,123456789,111.111,222.222,333.333,444.444,555.555";
      std::string token;
      std::string date;
      int i;
      std::set<double> double_list;
      strtk::parse(data,",",token,date,i,double_list);
      std::cout << "parse_example09(): " << token << " " << date << " " << i << " " << strtk::join(" ",double_list) << "\n";
   }
   {
      std::string data = "A String Value,01-01-2000,123456789,111.111,222.222,333.333,444.444,555.555";
      std::string token;
      std::string date;
      int i;
      double d;
      std::stack<double> double_list;
      strtk::parse(data,",",token,date,i,d,double_list);
   }
   {
      std::string data = "A String Value,01-01-2000,123456789,111.111,222.222,333.333,444.444,555.555";
      std::string token;
      std::string date;
      int i;
      double d1;
      double d2;
      std::queue<double> double_list;
      strtk::parse(data,",",token,date,i,d1,d2,double_list);
   }
   {
      std::string data = "A String Value,01-01-2000,123456789,111.111,222.222,333.333,444.444,555.555";
      std::string token;
      std::string date;
      int i;
      double d1;
      double d2;
      std::multiset<double> double_list;
      strtk::parse(data,",",token,date,i,d1,d2,double_list);
      std::cout << "parse_example09(): " << token << " " << date << " " << i << " " << d1 << " " << d2 << " " << strtk::join(" ",double_list) << "\n";
   }
}

void parse_example10()
{
   const std::string data = "1,12,123,1234,12345,123456,1234567,12345678,123456789,1234567890,"
                            "1,12,123,1234,12345,123456,1234567,12345678,123456789,1234567890";

   std::vector<int> x(5,0);

   {
      strtk::parse_columns(data,",",strtk::column_list(0,2,4,6,8),x[0],x[1],x[2],x[3],x[4]);

      std::cout << "parse_example10() - even columns: " << strtk::join("\t",x) << std::endl;
   }

   {
      strtk::parse_columns(data,",",strtk::column_list(1,3,5,7,9),x[0],x[1],x[2],x[3],x[4]);

      std::cout << "parse_example10() - odd columns: " << strtk::join("\t",x) << std::endl;
   }

   {
      const std::string tuple_data = "123|xxx,456.789;xyxy A string";

      int i;
      double d;
      std::string s;

      strtk::parse_columns(tuple_data,",| ;",strtk::column_list(0,2,4),i,d,s);

      std::cout << "parse_example10() - i = " << i << std::endl;
      std::cout << "parse_example10() - d = " << d << std::endl;
      std::cout << "parse_example10() - s = " << s << std::endl;
   }
}

void parse_example11()
{
   {
      static const std::string data = "var foo : InTeGeR = 3;";

      std::string variable_name;
      int initial_value;

      bool result = strtk::parse(data,
                                 " ;",
                                 strtk::expect("var").ref(),
                                 variable_name,
                                 strtk::expect(":").ref(),
                                 strtk::iexpect("Integer").ref(),
                                 strtk::expect("=").ref(),
                                 initial_value);

      if (!result)
         std::cout << "parse_example11() - Example 0 failed" << std::endl;
      else
         std::cout << "parse_example11() - " << variable_name << " = " << initial_value << std::endl;
   }

   {
      static const std::string data = "token0=+123;token1=abc;token2=-456.678;";

      int i;
      std::string s;
      double d;

      bool result = strtk::parse(data,
                                 "=;",
                                 strtk::like("to*n?").ref(),
                                 i,
                                 strtk::like("token?").ref(),
                                 s,
                                 strtk::iexpect("tOkEn2").ref(),
                                 d);

      if (!result)
         std::cout << "parse_example11() - Example 1 failed" << std::endl;
      else
      {
         std::cout << "parse_example11() - i = " << i << std::endl;
         std::cout << "parse_example11() - s = " << s << std::endl;
         std::cout << "parse_example11() - d = " << d << std::endl;
      }
   }

   {
      static const std::string data = "temperature=+123.456;name=rumpelstilzchen";

      double temperature = 0.0;
      std::string name;

      bool result = strtk::parse(data,
                                 "=;",
                                 //Process temperature section
                                 strtk::expect("temperature").ref(),
                                 strtk::inrange(temperature,-432.1,+432.1).ref(),
                                 //Process name section
                                 strtk::expect("name").ref(),
                                 strtk::inrange(name,"aaa","zzz").ref());

      if (!result)
         std::cout << "parse_example11() - Example 2 failed" << std::endl;
      else
      {
         std::cout << "parse_example11() - temperature = " << temperature << std::endl;
         std::cout << "parse_example11() - name = " << name << std::endl;
      }
   }

   {
      std::string data = "****abc123****,****abc123****,****abc123****";

      std::string s0;
      std::string s1;
      std::string s2;

      bool result = strtk::parse(data,",",
                                 strtk::trim("*",s0).ref(),
                                 strtk::trim_leading ("*",s1).ref(),
                                 strtk::trim_trailing("*",s2).ref());

      if (!result)
         std::cout << "parse_example11() - Example 3 failed" << std::endl;
      else
      {
         std::cout << "s0 = [" << s0 << "]" << std::endl;
         std::cout << "s1 = [" << s1 << "]" << std::endl;
         std::cout << "s2 = [" << s2 << "]" << std::endl;
      }
   }

   {
      std::string data = "*?*?a string*?*?,*?*123456,123.456?*?*?";

      std::string s;
      int i;
      double d;

      bool result = strtk::parse(data,",",
                                 strtk::trim("*?",s).ref(),
                                 strtk::trim_leading ("?*",i).ref(),
                                 strtk::trim_trailing("*?",d).ref());

      if (!result)
         std::cout << "parse_example11() - Example 4 failed" << std::endl;
      else
      {
         std::cout << "s = [" << s << "]" << std::endl;
         std::cout << "i = [" << i << "]" << std::endl;
         std::cout << "d = [" << d << "]" << std::endl;
      }
   }

   {
      std::string data = "AbCd,EfGhI";

      std::string s0;
      std::string s1;

      bool result = strtk::parse(data,",",
                   strtk::as_lcase(s0).ref(),
                   strtk::as_ucase(s1).ref());

      if (!result)
         std::cout << "parse_example11() - Example 5 failed" << std::endl;
      else
      {
         std::cout << "s0 = [" << s0 << "]" << std::endl;
         std::cout << "s1 = [" << s1 << "]" << std::endl;
      }
   }

}

void remove_inplace_example01()
{
   std::cout << "remove_inplace_example01" << std::endl;
   std::string s = "aa abb cdd  ee fg";
   std::cout << s << " -> ";
   strtk::remove_inplace(' ',s);
   std::cout << s << std::endl;
}

void remove_consecutives_example01()
{
   std::cout << "remove_consecutives_example01" << std::endl;
   std::string s = "aaabbcddeefg";
   std::cout << s << " -> ";
   strtk::remove_consecutives_inplace(s);
   std::cout << s << std::endl;
}

void remove_consecutives_example02()
{
   std::cout << "remove_consecutives_example02" << std::endl;
   std::string s = "aaabbcaaaddeeafg";
   std::cout << s << " -> ";
   strtk::remove_consecutives_inplace('a',s);
   std::cout << s << std::endl;
}

void remove_consecutives_example03()
{
   std::cout << "remove_consecutives_example03" << std::endl;
   std::string s = "aaabbcaaaddeeafg";
   std::cout << s << " -> ";
   strtk::remove_consecutives_inplace(strtk::multiple_char_delimiter_predicate("abcdefg"),s);
   std::cout << s << std::endl;
}

void remove_trailing_example()
{
   std::cout << "remove_trailing_example" << std::endl;
   std::string s = "The quick brown fox jumps over the lazy dog  , _";
   std::cout << "Before: ["<< s << "]" << std::endl;
   strtk::remove_trailing(" _,",s);
   std::cout << "After:  ["<< s << "]" << std::endl;
}

void remove_leading_example()
{
   std::cout << "remove_leading_example" << std::endl;
   std::string s = "_ ,  The quick brown fox jumps over the lazy dog";
   std::cout << "Before: ["<< s << "]" << std::endl;
   strtk::remove_leading(" _,",s);
   std::cout << "After:  ["<< s << "]" << std::endl;
}

void uri_extractor_example01()
{
   #ifdef strtk_enable_regex
   std::cout << "uri_extractor_example01" << std::endl;
   std::string text = "someone@somewhere.com http://www.test.net some.place.com any.one@any.where.com ftp://123.abcxyz.org";
   std::list<std::string> email_list;
   std::list<std::string> url_list;
   strtk::split_regex(strtk::email_expression,text,strtk::range_to_type_back_inserter(email_list));
   strtk::split_regex(strtk::uri_expression,text,strtk::range_to_type_back_inserter(url_list));
   std::cout << "emails: " << strtk::join(" ",email_list) << std::endl;
   std::cout << "urls: " << strtk::join(" ",url_list) << std::endl;
   #endif
}

void generate_random_example01()
{
   #ifdef strtk_enable_random
   std::cout << "generate_random_example01" << std::endl;
   const std::size_t data_size = 10;
   unsigned char* data = new unsigned char[data_size];
   strtk::generate_random_data(data,data_size,1000000);
   unsigned char* hex_data = new unsigned char[2 * data_size];
   strtk::convert_bin_to_hex(data,data + data_size, hex_data);
   std::copy(hex_data,hex_data + 2 * data_size,std::ostream_iterator<unsigned char>(std::cout));
   std::cout << std::endl;
   delete [] data;
   delete [] hex_data;
   #endif
}

void generate_random_example02()
{
   #ifdef strtk_enable_random
   std::cout << "generate_random_example02" << std::endl;
   std::deque<int> rnd_int_list;
   strtk::generate_random_values<int>(5,-5,+5,rnd_int_list);

   std::vector<unsigned int> rnd_uint_list;
   strtk::generate_random_values<unsigned int>(5,0,10,rnd_uint_list);

   std::list<double> rnd_double_list;
   strtk::generate_random_values<double>(5,-1.0,+1.0,rnd_double_list);

   std::vector<float> rnd_float_list;
   strtk::generate_random_values<float>(5,-0.5f,+0.5f,rnd_float_list);

   std::cout << strtk::join(" ",rnd_int_list) << std::endl;
   std::cout << strtk::join(" ",rnd_uint_list) << std::endl;
   std::cout << strtk::join(" ",rnd_double_list) << std::endl;
   std::cout << strtk::join(" ",rnd_float_list) << std::endl;
   #endif
}

void random_permutation_example()
{
   #ifdef strtk_enable_random
   std::cout << "random_permutation_example" << std::endl;
   std::vector<std::size_t> lst;
   for (std::size_t i = 0; i < 10; lst.push_back(i++)) ;

   const std::size_t seed[] = {
                               0x390E348F, 0x2884D0F5, 0x18AEB587, 0x31F9038C, 0x2AB63848,
                               0x2F48E5FA, 0x1CE7C0D3, 0x15B67855, 0x158BF6B8, 0x02FCCD04,
                               0x070FC0FC, 0x201061F4, 0x35A1DA56, 0x262B86ED, 0x06B60E56,
                               0x2CFFBCB1, 0x3B6F0EDB, 0x04AA3850, 0x0FB38915, 0x05FB18F7,
                               0x1A300ACF, 0x0E99E22D, 0x222E195F, 0x1E54BACF, 0x35A9284C
                              };

   for (std::size_t i = 0; i < sizeof(seed)/sizeof(std::size_t); ++i)
   {
      strtk::random_permutation(lst.begin(),lst.end(),
                                std::ostream_iterator<std::size_t>(std::cout,"\t"),seed[i]);
      std::cout << std::endl;
   }
   #endif
}

void random_combination_example()
{
   #ifdef strtk_enable_random
   std::cout << "random_combination_example" << std::endl;

   std::size_t seed[] = {
                           0x390E348F, 0x2884D0F5, 0x18AEB587, 0x31F9038C, 0x2AB63848,
                           0x2F48E5FA, 0x1CE7C0D3, 0x15B67855, 0x158BF6B8, 0x02FCCD04
                        };

   std::vector<std::size_t> lst;
   for (std::size_t i = 0; i < sizeof(seed)/sizeof(std::size_t); lst.push_back(i++)) ;

   for (std::size_t i = 0; i < sizeof(seed)/sizeof(std::size_t); ++i)
   {
      strtk::random_combination(lst.begin(),lst.end(),
                                i + 1,
                                std::ostream_iterator<std::size_t>(std::cout,"\t"),seed[i]);
      std::cout << std::endl;
   }
   #endif
}

void lexicographically_canonicalize_example()
{
   std::cout << "lexicographically_canonicalize_example" << std::endl;
   std::string str_list[] = {
                              "xyzabcabc",
                              "ijkxyzabc",
                              "abcdefghijklmnopqrstuvwxyz",
                              "zyxwvutsrqponmlkjihgfedcba",
                              "The Quick Brown Fox Jumps Over The Lazy Dog",
                              "5678901234"
                            };
   const std::size_t str_list_size = sizeof(str_list) / sizeof(std::string);

   for (std::size_t i = 0; i < str_list_size; ++i)
   {
      std::cout << str_list[i] << " --> ";
      strtk::lexicographically_canonicalize(str_list[i]);
      std::cout << str_list[i] << std::endl;
   };

   unsigned int uint_list[] = { 6,7,8,9,0,1,2,3,4,5 };
   const std::size_t uint_list_size = sizeof(uint_list) / sizeof(unsigned int);

   std::copy(uint_list,uint_list + uint_list_size,std::ostream_iterator<unsigned int>(std::cout," "));
   std::cout << " --> ";
   strtk::lexicographically_canonicalize(uint_list,uint_list + uint_list_size);
   std::copy(uint_list,uint_list + uint_list_size,std::ostream_iterator<unsigned int>(std::cout," "));
   std::cout << std::endl;

   std::list<int> int_list;
   int_list.push_back(6); int_list.push_back(7);
   int_list.push_back(8); int_list.push_back(9);
   int_list.push_back(0); int_list.push_back(1);
   int_list.push_back(2); int_list.push_back(3);
   int_list.push_back(4); int_list.push_back(5);

   std::copy(int_list.begin(),int_list.end(),std::ostream_iterator<int>(std::cout," "));
   std::cout << " --> ";
   strtk::lexicographically_canonicalize(int_list.begin(),int_list.end());
   std::copy(int_list.begin(),int_list.end(),std::ostream_iterator<int>(std::cout," "));
   std::cout << std::endl;
}

void hash_example()
{
   std::cout << "hash_example" << std::endl;
            char  chr_list[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
   unsigned char uchr_list[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
            int   int_list[] = {   -4,   -3,   -2,   -1,    0,    1,    2, 3, 4 };
   unsigned int  uint_list[] = {    0,    1,    2,    3,    4,    5,    6, 7, 8 };
         double   dbl_list[] = {  1.1,  2.2,  3.3,  4.4,  5.5,  6.6,  7.7 };

   std::cout << "hash(char):   " << strtk::hash(chr_list, sizeof( chr_list) / sizeof(char))          << std::endl;
   std::cout << "hash(uchar):  " << strtk::hash(uchr_list,sizeof(uchr_list) / sizeof(unsigned char)) << std::endl;
   std::cout << "hash(int):    " << strtk::hash(int_list, sizeof( int_list) / sizeof(int))           << std::endl;
   std::cout << "hash(uint):   " << strtk::hash(uint_list,sizeof(uint_list) / sizeof(unsigned int))  << std::endl;
   std::cout << "hash(double): " << strtk::hash(dbl_list, sizeof( dbl_list) / sizeof(double))        << std::endl;

   std::vector<int> int_vec;
   std::vector<double> dbl_deq;
   std::string str(chr_list,7);

   strtk::iota(int_vec,10,1);
   strtk::iota(dbl_deq,10,135.791);

   std::cout << "hash(int_vec): " << strtk::hash(int_vec) << std::endl;
   std::cout << "hash(dbl_deq): " << strtk::hash(dbl_deq) << std::endl;
   std::cout << "hash(std::string): " << strtk::hash(str) << std::endl;
}

void join_example()
{
   std::cout << "join_example" << std::endl;
   const std::size_t str_list_size = 5;
   std::string str_list [] = {
                               "1",
                               "22",
                               "333",
                               "4444",
                               "55555"
                             };

   std::cout << strtk::join(",",str_list,str_list + str_list_size) << std::endl;

   std::cout << "Size equals 1: " << strtk::join_if(",", strtk::size_equal_to<1>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size equals 2: " << strtk::join_if(",", strtk::size_equal_to<2>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size equals 3: " << strtk::join_if(",", strtk::size_equal_to<3>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size equals 4: " << strtk::join_if(",", strtk::size_equal_to<4>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size equals 5: " << strtk::join_if(",", strtk::size_equal_to<5>(), str_list,str_list + str_list_size) << std::endl;

   std::cout << "Size less than 1: " << strtk::join_if(",", strtk::size_less_than<1>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size less than 2: " << strtk::join_if(",", strtk::size_less_than<2>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size less than 3: " << strtk::join_if(",", strtk::size_less_than<3>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size less than 4: " << strtk::join_if(",", strtk::size_less_than<4>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size less than 5: " << strtk::join_if(",", strtk::size_less_than<5>(), str_list,str_list + str_list_size) << std::endl;

   std::cout << "Size greater than 1: " << strtk::join_if(",", strtk::size_greater_than<1>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size greater than 2: " << strtk::join_if(",", strtk::size_greater_than<2>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size greater than 3: " << strtk::join_if(",", strtk::size_greater_than<3>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size greater than 4: " << strtk::join_if(",", strtk::size_greater_than<4>(), str_list,str_list + str_list_size) << std::endl;
   std::cout << "Size greater than 5: " << strtk::join_if(",", strtk::size_greater_than<5>(), str_list,str_list + str_list_size) << std::endl;
}

void inserter_example()
{
   std::cout << "inserter_example" << std::endl;
   const std::string base = "The Quick Brown Fox Jumps Over The Lazy Dog";
   std::string str;
   std::string str_output;
   strtk::replicate(5,base,str);
   strtk::inserter(strtk::interval_inserter<std::string::value_type>(base.size(),'\n'),
                   str.begin(),str.end(),
                   std::back_inserter(str_output));
   std::cout << str_output << std::endl;

   const std::size_t i[] = {1,2,3,4,5,6,7,8,9,10,11,12};
   const std::size_t size_i = sizeof(i) / sizeof(std::size_t);
   std::deque<std::size_t> int_output;
   strtk::inserter(strtk::interval_inserter<std::size_t>(3,666),
                   i,i + size_i,
                   std::back_inserter(int_output));
   std::cout << strtk::join(" ",int_output) << std::endl;
}

void combination_example01()
{
   std::cout << "combination_example01" << std::endl;
   std::string s = "abcdefg";
   std::size_t set_size = s.size() - 1;
   std::size_t combination_index = 0;
   do
   {
      std::cout << combination_index++ << "\t" << s.substr(0,set_size) << std::endl;
   }
   while (strtk::next_combination(s.begin(),s.begin() + set_size,s.end()));
}

void combination_example02()
{
   std::cout << "combination_example02" << std::endl;
   {
      typedef std::vector<int> vec_t;
      typedef strtk::combination_iterator<vec_t::iterator> itr_type;
      vec_t val_lst;

      strtk::iota(val_lst,5,0);

      itr_type itr(4,val_lst);
      itr_type end(val_lst);

      while (end != itr)
      {
         itr_type::range_type range = *itr;
         std::cout << strtk::join(" ",range) << std::endl;
         ++itr;
      }
   }

   {
      std::string s = "abcdefg";

      typedef strtk::combination_iterator<std::string::iterator> itr_type;
      itr_type itr(6,s.begin(),s.end());
      itr_type end(s.end());

      while (end != itr)
      {
         std::cout << *itr << std::endl;
         ++itr;
      }
   }
}

void typename_example()
{
   std::cout << "typename_example" << std::endl;
   char           t00;
   unsigned char  t01;
   short          t02;
   int            t03;
   long           t04;
   unsigned short t05;
   unsigned int   t06;
   unsigned long  t07;
   std::string    t08;

   char           t09[1] = { 1 };
   unsigned char  t10[2] = { 1, 2 };
   short          t11[3] = { 1, 2, 3 };
   int            t12[4] = { 1, 2, 3, 4, };
   long           t13[5] = { 1, 2, 3, 4, 5 };
   unsigned short t14[6] = { 1, 2, 3, 4, 5, 6 };
   unsigned int   t15[7] = { 1, 2, 3, 4, 5, 6, 7 };
   unsigned long  t16[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

   std::cout << strtk::type_name(t00) << std::endl;
   std::cout << strtk::type_name(t01) << std::endl;
   std::cout << strtk::type_name(t02) << std::endl;
   std::cout << strtk::type_name(t03) << std::endl;
   std::cout << strtk::type_name(t04) << std::endl;
   std::cout << strtk::type_name(t05) << std::endl;
   std::cout << strtk::type_name(t06) << std::endl;
   std::cout << strtk::type_name(t07) << std::endl;
   std::cout << strtk::type_name(t08) << std::endl;
   std::cout << strtk::type_name(t09) << std::endl;
   std::cout << strtk::type_name(t10) << std::endl;
   std::cout << strtk::type_name(t11) << std::endl;
   std::cout << strtk::type_name(t12) << std::endl;
   std::cout << strtk::type_name(t13) << std::endl;
   std::cout << strtk::type_name(t14) << std::endl;
   std::cout << strtk::type_name(t15) << std::endl;
   std::cout << strtk::type_name(t16) << std::endl;

   std::vector<unsigned int> vui;
   std::vector<std::string> vs;

   std::deque<double> dd;
   std::deque<std::string> ds;

   std::list<double> lf;
   std::list<std::string> ls;

   std::set<double> sf;
   std::set<std::string> ss;

   std::cout << strtk::type_name(vui) << std::endl;
   std::cout << strtk::type_name( vs) << std::endl;
   std::cout << strtk::type_name( dd) << std::endl;
   std::cout << strtk::type_name( ds) << std::endl;
   std::cout << strtk::type_name( lf) << std::endl;
   std::cout << strtk::type_name( ls) << std::endl;
   std::cout << strtk::type_name( sf) << std::endl;
   std::cout << strtk::type_name( ss) << std::endl;
}

void iota_example()
{
   std::cout << "iota_example" << std::endl;
   std::deque<int> ilst;

   strtk::iota(ilst,10,1);
   std::cout << strtk::join(" ",ilst) << std::endl;

   strtk::iota(ilst,100);
   std::cout << strtk::join(" ",ilst) << std::endl;
}

void bracketize_example()
{
   std::cout << "bracketize_example" << std::endl;
   std::string int_data = "1,2,3,4,5,6,7,8,9,10";
   std::string string_data = "the quick brown fox jumps over the lazy dog";

   std::set<int> int_list;
   std::deque<std::string> string_list;

   strtk::parse(int_data,",",int_list);
   strtk::parse(string_data," ",string_list);

   std::cout << strtk::bracketize("{","}",int_list) << std::endl;
   std::cout << strtk::bracketize("<",">",string_list) << std::endl;
}

void cut_example()
{
   std::cout << "cut_example" << std::endl;
   std::string s = "0123456789";
   std::deque<std::string> str_list;
   for (std::size_t i = 0; i < s.size(); ++i)
   {
      std::rotate(s.begin(),s.begin() + i, s.end());
      str_list.push_back(s);
   }
   std::deque<std::string> cut_str_list;
   strtk::cut(1,5,str_list,std::back_inserter(cut_str_list));
   std::cout << strtk::join("\n",cut_str_list) << std::endl;
}

void exract_unique_example()
{
   std::cout << "exract_unique_example" << std::endl;
   {
      std::string s = "abcabcabcabcabcabcabc";
      std::string unique;
      strtk::extract_unique(s.begin(),s.end(),std::back_inserter(unique));
      std::cout << s << "\tunique: " << unique << std::endl;
   }

   {
      std::vector<int> int_vec;
      std::deque<int> int_deq;

      int_vec.push_back(1); int_vec.push_back(2); int_vec.push_back(3);
      int_vec.push_back(1); int_vec.push_back(2); int_vec.push_back(3);
      int_vec.push_back(1); int_vec.push_back(2); int_vec.push_back(3);

      strtk::extract_unique(int_vec.begin(),int_vec.end(),std::back_inserter(int_deq));
      std::cout << strtk::join(" ",int_vec) << "\tunique: " << strtk::join(" ",int_deq) << std::endl;
   }
}

void func(const char* s)
{
   std::cout << s << std::endl;
}

void build_string_example()
{
   std::cout << "build_string_example" << std::endl;
   func(strtk::build_string() << "ABC " << 123 << " " << 456.789);
}

void make_key_lists()
{
   std::cout << "make_key_lists" << std::endl;
   std::map<std::string,int> map;
   map["zero"]  = 0;
   map["one"]   = 1;
   map["two"]   = 2;
   map["three"] = 3;
   map["four"]  = 4;
   map["five"]  = 5;

   std::set<std::string> key_set;
   std::vector<std::string> key_vec;
   std::deque<std::string> key_deq;
   std::list<std::string> key_lst;

   strtk::util::make_key_list(map,key_set);
   strtk::util::make_key_list(map,key_vec);
   strtk::util::make_key_list(map,key_deq);
   strtk::util::make_key_list(map,key_lst);

   std::cout << "Keys(set): " << strtk::join(", ",key_set) << std::endl;
   std::cout << "Keys(vec): " << strtk::join(", ",key_vec) << std::endl;
   std::cout << "Keys(deq): " << strtk::join(", ",key_deq) << std::endl;
   std::cout << "Keys(lst): " << strtk::join(", ",key_lst) << std::endl;
}

void make_value_lists()
{
   std::cout << "make_value_lists" << std::endl;
   std::multimap<std::string,int> map;

   map.insert(std::pair<std::string,int>("one",1));

   map.insert(std::pair<std::string,int>("two",1));
   map.insert(std::pair<std::string,int>("two",2));

   map.insert(std::pair<std::string,int>("three",1));
   map.insert(std::pair<std::string,int>("three",2));
   map.insert(std::pair<std::string,int>("three",3));

   std::vector<int> val_vec;
   std::deque<int> val_deq;
   std::list<int> val_lst;

   std::string key1 = "one";
   std::string key2 = "two";
   std::string key3 = "three";

   strtk::util::make_value_list(map,key1,val_vec);
   strtk::util::make_value_list(map,key2,val_deq);
   strtk::util::make_value_list(map,key3,val_lst);

   std::cout << "Values(vec): " << strtk::join(", ",val_vec) << std::endl;
   std::cout << "Values(deq): " << strtk::join(", ",val_deq) << std::endl;
   std::cout << "Values(lst): " << strtk::join(", ",val_lst) << std::endl;
}

void globbing_example()
{
   std::cout << "globbing_example" << std::endl;
   {
      std::string pattern = "a?c";
      std::string data    = "abc";

      if (strtk::match(pattern,data))
      {
         std::cout << data << " matches pattern: " << pattern << std::endl;
      }
   }

   {
      std::string pattern = "a*c";
      std::string data    = "abbbbbbc";

      if (strtk::match(pattern,data))
      {
         std::cout << data << " matches pattern: " << pattern << std::endl;
      }
   }

   {
      std::string pattern = "a*c?e";
      std::string data    = "abbbbbbcde";

      if (strtk::match(pattern,data))
      {
         std::cout << data << " matches pattern: " << pattern << std::endl;
      }
   }

   {
      std::string pattern = "*a*c?e?";
      std::string data    = "0000abbbbbbcdef";

      if (strtk::match(pattern,data))
      {
         std::cout << data << " matches pattern: " << pattern << std::endl;
      }
   }
}

void example_replace()
{
   std::cout << "example_replace" << std::endl;
   std::string data = "abcdefabcdefabcdefabcdefabcdefabcdef";
   typedef std::pair<std::string,std::string> pair_type;
   const pair_type pattern_replace[] = {
                                          pair_type(     "a",     "w"),
                                          pair_type(    "ab",    "wx"),
                                          pair_type(   "abc",   "wxy"),
                                          pair_type(  "abcd",  "wxyz"),
                                          pair_type( "abcde", "wxyz0"),
                                          pair_type("abcdef","wxyz01")
                                       };
   const std::size_t size = sizeof(pattern_replace) / sizeof(pair_type);

   std::cout << "Data: " << data << std::endl;
   std::string modified_string;
   for (std::size_t i = 0; i < size; ++i)
   {
      strtk::replace_pattern(data,
                             pattern_replace[i].first,pattern_replace[i].second,
                             modified_string);
      std::cout << "Replace (" << pattern_replace[i].first << ") "
                << "With (" << pattern_replace[i].second << "): " << modified_string << std::endl;
   }
}

void find_example()
{
   std::cout << "find_example" << std::endl;
   std::string data = "abc 123 ABC 456 abc 789 AbC 012 abc 345 aBC 678 ABc 901";
   std::string pattern = "abc";

   {
      typedef strtk::std_string::token_deque_type match_list_type;
      match_list_type match_list;
      strtk::find_all(pattern,data,std::back_inserter(match_list));
      match_list_type::iterator itr = match_list.begin();
      while (match_list.end() != itr)
      {
         std::cout << "(" << std::string(itr->first,itr->second) << ")\t";
         ++itr;
      }
      std::cout << std::endl;
   }

   {
      typedef strtk::std_string::token_deque_type match_list_type;
      match_list_type match_list;
      strtk::ifind_all(pattern,data,std::back_inserter(match_list));
      match_list_type::iterator itr = match_list.begin();
      while (match_list.end() != itr)
      {
         std::cout << "(" << std::string(itr->first,itr->second) << ")\t";
         ++itr;
      }
      std::cout << std::endl;
   }

   {
      typedef strtk::std_string::token_deque_type match_list_type;
      match_list_type match_list;
      strtk::find_all(pattern,data,match_list);
      match_list_type::iterator itr = match_list.begin();
      while (match_list.end() != itr)
      {
         std::cout << "(" << std::string(itr->first,itr->second) << ")\t";
         ++itr;
      }
      std::cout << std::endl;
   }

   {
      typedef strtk::std_string::token_deque_type match_list_type;
      match_list_type match_list;
      strtk::ifind_all(pattern,data,match_list);
      match_list_type::iterator itr = match_list.begin();
      while (match_list.end() != itr)
      {
         std::cout << "(" << std::string(itr->first,itr->second) << ")\t";
         ++itr;
      }
      std::cout << std::endl;
   }
}

void ext_string_example()
{
   std::cout << "ext_string_example" << std::endl;
   {
      strtk::ext_string es("ext_string");
      std::cout << "es + 123 = " << es + 123 << std::endl;
      std::cout << "456 + es = " << 456 + es << std::endl;
      std::cout << "es * 3   = " << es * 3   << std::endl;
      std::cout << "3  * es  = " << 3 * es   << std::endl;
   }

   {
      strtk::ext_string es("abc1234?abc1234?abc1234");
      std::cout << "original:  " << es << std::endl;
      std::cout << "es - abc = " << es - "abc"  << std::endl;
      std::cout << "es - 123 = " << es - "1234" << std::endl;
      std::cout << "es -   ? = " << es - "?"    << std::endl;
   }

   {
      strtk::ext_string es1("ext_string");
      strtk::ext_string es2("eXt_StRiNg");
      if (es1.imatch(es2))
         std::cout << es1 << " == " << es2 << std::endl;
   }

   {
      strtk::ext_string es("eXt_StRiNg");
      std::cout << "original:  " << es << std::endl;
      std::cout << "lowercase: " << es.to_lowercase() << std::endl;
      std::cout << "uppercase: " << es.to_uppercase() << std::endl;
   }

   {
      strtk::ext_string es("   ext_string   ");
      std::cout << "original:              [" << es << "]" << std::endl;
      std::cout << "remove leading space:  [" << es.remove_leading(" ")  << "]" << std::endl;
      std::cout << "remove trailing space: [" << es.remove_trailing(" ") << "]" << std::endl;
   }

   {
      strtk::ext_string es("abc, ijk, pqr, xyz");
      std::deque<std::string> str_list;
      es.split(" ,",str_list,strtk::split_options::compress_delimiters);
      std::cout << "es: " << es << " ---> split: " << strtk::bracketize("(",") ",str_list) << std::endl;
   }

   {
      strtk::ext_string es("abc, ijk, pqr, xyz");
      std::deque<std::string> str_list;
      es.split_n(" ,",3,str_list,strtk::split_options::compress_delimiters);
      std::cout << "es: " << es << " ---> split_n(3): " << strtk::bracketize("(",") ",str_list) << std::endl;
   }

   {
      strtk::ext_string es("1, -23, 456, -7890");
      std::deque<int> int_list;
      es.parse(" ,",int_list);
      std::cout << "es: " << es << " ---> parse: " << strtk::bracketize("(",") ",int_list) << std::endl;
   }

   {
      strtk::ext_string es("1.1, -23.32, 456.654, -7890.0987");
      std::deque<double> int_list;
      es.parse(" ,",int_list);
      std::cout << "es: " << es << " ---> parsed: " << strtk::bracketize("(",") ",int_list) << std::endl;
   }
}

void non_repeated_char_example()
{
   std::cout << "non_repeated_char_example" << std::endl;
   const std::string str_list[] =
                     {
                        "",
                        "abcdefghijklmnopqrstuvwxyz",
                        "abcdefghijklmnopqrstuvwxyza",
                        "abcabcabcabc",
                        "abaabbaaabbbcabaabbaaabbb",
                        "1a2b3c4d2b3c4d"
                     };

   const std::size_t str_list_size = sizeof(str_list) / sizeof(std::string);

   for (std::size_t i = 0; i < str_list_size; ++i)
   {
      const std::size_t index = strtk::first_non_repeated_char(str_list[i]);
      if (std::string::npos == index)
      {
         std::cout << "No non-repeated chars in string." << std::endl;
         continue;
      }
      std::cout << "First non repeated char: " << str_list[i][index] << std::endl;
   }
}

void translation_table_example()
{
   std::cout << "translation_table_example" << std::endl;

   const std::string intab  = "aeiou";
   const std::string outtab = "12345";

   {
      std::string s = "Such is this simple string sample....Wowzers!";
      strtk::translation_table trans_table(intab,outtab);
      std::cout << "Before: " << s << std::endl;
      std::transform(s.begin(),s.end(),s.begin(),trans_table);
      std::cout << "After: " << s << std::endl;
   }

   {
      std::string s = "Such is this simple string sample....Wowzers!";
      std::cout << "Before: " << s << std::endl;
      std::transform(s.begin(),s.end(),s.begin(),strtk::translation_table(intab,outtab));
      std::cout << "After: " << s << std::endl;
   }

   {
      std::string s = "Such is this simple string sample....Wowzers!";
      std::cout << "Before: " << s << std::endl;
      std::transform(s.begin(),s.end(),s.begin(),strtk::translation_table("aeiou","12345"));
      std::cout << "After: " << s << std::endl;
   }

   {
      std::string s = "Such is this simple string sample....Wowzers!";
      strtk::translation_table trans_table(intab,outtab);
      std::cout << "Before: " << s << std::endl;
      strtk::translate_inplace(trans_table,s);
      std::cout << "After: " << s << std::endl;
   }

   {
      std::string s = "Such is this simple string sample....Wowzers!";
      strtk::translation_table trans_table(intab,outtab);
      std::cout << "Before: " << s << std::endl;
      std::cout << "After: " << strtk::translate(trans_table,s) << std::endl;
   }
}

void find_n_consecutive_example()
{
   std::cout << "find_n_consecutive_example" << std::endl;

   {
      std::string s = "1 22 333 4444 55555 666666 7777777 88888888 999999999";

      char* begin = const_cast<char*>(s.data());
      char* end   = const_cast<char*>(s.data() + s.size());

      typedef char* iterator_type;
      typedef strtk::details::range_type<iterator_type>::type range_type;
      range_type range(begin,end);

      for (std::size_t n = 1; n <= 9; ++n)
      {
         range_type found_itr = strtk::find_n_consecutive<iterator_type>(n,
                                                                         strtk::find_type::digits,
                                                                         strtk::find_mode::exactly_n,
                                                                         range);
         if ((end == found_itr.first) && (found_itr.first == found_itr.second))
         {
            std::cout << "No strings found for " << n << " consecutive values!" << std::endl;
            break;
         }
         else
         {
            std::cout << "Result-"       << strtk::text::right_align(2,'0',n)
                      << ": ["           << std::string(found_itr.first,found_itr.second)
                      << "] Location: [" << std::distance(begin,found_itr.first) << "]"
                      <<  "] Length: ["  << std::distance(found_itr.first,found_itr.second) << "]" << std::endl;
            range.first = found_itr.second;
         }
      }
   }

   {
      std::string s = "a bB cCc dDdD EeEeE fFfFfF gGgGgGg HhHhHhHh IiIiIiIiI";

      char* begin = const_cast<char*>(s.data());
      char* end   = const_cast<char*>(s.data() + s.size());

      typedef char* iterator_type;
      typedef strtk::details::range_type<iterator_type>::type range_type;
      range_type range(begin,end);

      for (std::size_t n = 1; n <= 9; ++n)
      {
         range_type found_itr = strtk::find_n_consecutive<iterator_type>(n,
                                                                         strtk::find_type::letters,
                                                                         strtk::find_mode::exactly_n,
                                                                         range);
         if ((end == found_itr.first) && (found_itr.first == found_itr.second))
         {
            std::cout << "No strings found for " << n << " consecutive values!" << std::endl;
            break;
         }
         else
         {
            std::cout << "Result-"       << strtk::text::right_align(2,'0',n)
                      << ": ["           << std::string(found_itr.first,found_itr.second)
                      << "] Location: [" << std::distance(begin,found_itr.first) << "]"
                      <<  "] Length: ["  << std::distance(found_itr.first,found_itr.second) << "]" << std::endl;
            range.first = found_itr.second;
         }
      }
   }
}

void split_on_consecutive_example()
{
   std::cout << "split_on_consecutive_example" << std::endl;

   std::string data = "1 A 22 BB 333 CCC 4444 DDDD 55555 EEEEE 666666 FFFFFF 7777777 GGGGGGG "
                      "8 H 99 II 000 JJJ 1111 KKKK 22222 LLLLL 333333 MMMMMM 4444444 NNNNNNN ";

   std::deque<std::string> token_list;

   {
      for (std::size_t i = 0; i < 7; ++i)
      {
         token_list.clear();
         strtk::split_on_consecutive(i,
                                     strtk::find_type::digits,
                                     strtk::find_mode::exactly_n,
                                     data,
                                     strtk::range_to_type_back_inserter(token_list));
         if (token_list.empty())
            continue;
         std::cout << i << " Consecutive digits: " << strtk::join(" ",token_list) << std::endl;
      }
   }

   {
      for (std::size_t i = 0; i < 7; ++i)
      {
         token_list.clear();
         strtk::split_on_consecutive(i,
                                     strtk::find_type::letters,
                                     strtk::find_mode::exactly_n,
                                     data,
                                     strtk::range_to_type_back_inserter(token_list));
         if (token_list.empty())
            continue;
         std::cout << i << " Consecutive letters: " << strtk::join(" ",token_list) << std::endl;
      }
   }
}

void index_of_example()
{
   std::cout << "index_of_example" << std::endl;

   std::string data = "0123456789ABC";
   std::string pattern[] = {
                              "0123456789ABC",
                              "123456789ABC",
                              "23456789ABC",
                              "3456789ABC",
                              "456789ABC",
                              "56789ABC",
                              "6789ABC",
                              "789ABC",
                              "89ABC",
                              "9ABC",
                           };
   const std::size_t pattern_size = sizeof(pattern) / sizeof(std::string);
   for (std::size_t i = 0; i < pattern_size; ++i)
   {
      std::cout << "Index of pattern[" << pattern[i] <<"]: " << strtk::index_of(pattern[i],data) << std::endl;
   }
   std::cout << "Index of pattern[xyz]: " << strtk::index_of("xyz",data) << std::endl;
}

void truncatedint_example()
{
   std::cout << "truncatedint_example" << std::endl;

   {
      int i = 0;
      std::string data = "-1234.0000";
      strtk::truncated_int<int> ti;
      if (!strtk::string_to_type_converter(data,ti(i)))
      {
         std::cout << "Failed truncated int conversion!\n";
      }
      else
         std::cout << "i = " << i << std::endl;
   }

   {
      int i = 0;
      unsigned int u = 0;
      std::string data = "-1234.0000|+1234.0000";
      strtk::truncated_int<int> ti;
      strtk::truncated_int<unsigned int> tu;

      if (!strtk::parse(data,"|",ti(i),tu(u)))
      {
         std::cout << "Failed parse!\n";
      }
      else
      {
         std::cout << "i = " << i << std::endl;
         std::cout << "u = " << u << std::endl;
      }
   }

   {
      int i = 0;
      unsigned int u = 0;
      std::string data = "-1234.0000|+1234.0000";
      strtk::truncated_int<int> ti;
      strtk::truncated_int<unsigned int> tu;

      if (!strtk::parse(data,"|",
                        ti(i).fractional_size(4),
                        tu(u).fractional_size(4))
         )
      {
         std::cout << "Failed parse!\n";
      }
      else
      {
         std::cout << "i = " << i << std::endl;
         std::cout << "u = " << u << std::endl;
      }
   }
}

int main()
{
   information();
   tokenizer_example01();
   tokenizer_example02();
   tokenizer_example03();
   tokenizer_example04();
   tokenizer_example05();
   tokenizer_example06();
   tokenizer_example07();
   tokenizer_example08();
   tokenizer_example09();
   tokenizer_example10();
   tokenizer_example11();
   tokenizer_example12();
   tokenizer_example13();
   split_example01();
   split_example02();
   split_example03();
   split_example04();
   split_example05();
   split_regex_example01();
   split_regex_example02();
   split_n_example01();
   split_n_example02();
   split_n_example03();
   split_regex_n_example01();
   split_regex_n_example02();
   offset_splitter_example01();
   offset_splitter_example02();
   offset_splitter_example03();
   construct_example();
   parse_example01();
   parse_example02();
   parse_example03();
   parse_example04();
   parse_example05();
   parse_example06();
   parse_example07();
   parse_example08();
   parse_example09();
   parse_example10();
   parse_example11();
   remove_inplace_example01();
   remove_consecutives_example01();
   remove_consecutives_example02();
   remove_consecutives_example03();
   remove_trailing_example();
   remove_leading_example();
   uri_extractor_example01();
   generate_random_example01();
   generate_random_example02();
   random_permutation_example();
   random_combination_example();
   lexicographically_canonicalize_example();
   hash_example();
   join_example();
   inserter_example();
   combination_example01();
   combination_example02();
   typename_example();
   iota_example();
   bracketize_example();
   cut_example();
   exract_unique_example();
   build_string_example();
   make_key_lists();
   make_value_lists();
   globbing_example();
   example_replace();
   find_example();
   ext_string_example();
   non_repeated_char_example();
   translation_table_example();
   find_n_consecutive_example();
   split_on_consecutive_example();
   index_of_example();
   truncatedint_example();
   return 0;
}
