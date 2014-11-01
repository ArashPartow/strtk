/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Number Statistics                                             *
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
   Description: This example parses numeric input from either an input  file
                or  the  stdin,  outputting  simple  statistical information
                regarding the  rows and  columns of  values (min,  max, sum,
                mean,  median). Tokens  that can't  be parsed  into a  valid
                double type are ignored.

                Example Input:
                 56,    62,   819,   291,    16
                240,   309,   890,   537,   487
                381,   322,   148,   603,   532
                694,   619,   604,   766,   102
                374,   696,   312,   771,    36

                Example Output:
                C[0]    56      694     1745    349     374
                C[1]    62      696     2008    401.6   322
                C[2]    148     890     2773    554.6   604
                C[3]    291     771     2968    593.6   603
                C[4]    16      532     1173    234.6   102
                R[0]    16      819     1244    248.8   62
                R[1]    240     890     2463    492.6   487
                R[2]    148     603     1986    397.2   381
                R[3]    102     766     2785    557     619
                R[4]    36      771     2189    437.8   374
*/


#include <cstddef>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <string>
#include <deque>
#include <vector>

#include "strtk.hpp"


template<typename Allocator,
         template<typename,typename> class Sequence>
void compute_stats(Sequence<double,Allocator>& value_list)
{
   //Format: min max sum mean median
   double min_value = 0.0;
   double max_value = 0.0;
   strtk::min_max_of_cont(value_list,min_value,max_value);
   std::cout << min_value << "\t" << max_value << "\t";

   double sum = std::accumulate(value_list.begin(),value_list.end(),0.0);
   std::cout << sum << "\t";
   std::cout << sum / value_list.size()  << "\t";

   std::nth_element(value_list.begin(),value_list.begin() + (value_list.size() / 2), value_list.end());
   std::cout << *(value_list.begin() + (value_list.size() / 2)) << "\t";
}

int main(int argc, char* argv[])
{
   std::deque<std::string> str_list;

   switch (argc)
   {
      case 1  : strtk::load_from_text_file(std::cin,str_list);
                break;

      case 2  : strtk::load_from_text_file(argv[1],str_list);
                break;

      default :
               {
                  std::cout << "usage: strtk_numstats <file name>" << std::endl;
                  std::cout << "usage: cat data.txt | strtk_numstats" << std::endl;
                  return 1;
               }
   }

   if (str_list.empty())
      return 0;

   std::size_t total_length = 0;
   for (std::size_t i = 0; i < str_list.size(); total_length += str_list[i++].size()) ;

   std::string buffer;
   buffer.reserve(total_length + str_list.size());
   strtk::join(buffer,"\n",str_list);

   std::vector<double> value_list;
   value_list.reserve(str_list.size());

   str_list.clear();

   strtk::token_grid::options options;
   options.set_column_delimiters(", ");

   strtk::token_grid grid(buffer.data(),
                          buffer.size(),
                          options);

   grid.remove_empty_tokens();

   for (std::size_t column = 0; column < grid.max_column_count(); ++column)
   {
      value_list.clear();
      grid.extract_column_checked(column,value_list);

      if (value_list.empty())
         continue;

      std::cout << "C["<< column << "]" << "\t";
      compute_stats(value_list);
      std::cout << std::endl;
   }

   for (std::size_t row = 0; row < grid.row_count(); ++row)
   {
      value_list.clear();
      grid.row(row).parse_checked(value_list);

      if (value_list.empty())
         continue;

      std::cout << "R["<< row << "]" << "\t";
      compute_stats(value_list);
      std::cout << std::endl;
   }

   return 0;
}
