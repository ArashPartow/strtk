/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Period Parser                                                 *
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
   Description: This example demonstrates how to build a simple parser
                that takes input in the form of days, hours, minutes and
                seconds, validates each of the values and then proceeds
                to combine them all into a millisecond period of time.
*/


#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "strtk.hpp"


class period_parser
{
public:

   period_parser()
   : predicate_("DHRMINSECdhrminsec :")
   {}

   bool operator()(const std::string s, long long& period)
   {
      if (s.empty())
         return false;
      return operator ()(const_cast<char*>(s.data()), const_cast<char*>(s.data() + s.size()), period);
   }

   template<typename Iterator>
   bool operator()(Iterator begin, Iterator end, long long& period)
   {
      static const strtk::split_options::type split_options = strtk::split_options::compress_delimiters +
                                                              strtk::split_options::include_1st_delimiter;
      std::size_t token_count = split_n(predicate_, begin, end, max_token_count, token_list, split_options);
      if (0 == token_count)
         return false;

      period = 0;
      double t = 0.0;
      bool encounterd [] = { false, false, false, false };

      for (std::size_t i = 0; i < token_count; ++i)
      {
         if (!strtk::string_to_type_converter(token_list[i].first, token_list[i].second - 1, t))
            return false;

         switch (std::toupper(*(token_list[i].second - 1)))
         {
            case 'D' : if (encounterd[0]) return false; else { t = 86400000.0 * t; encounterd[0] = true; } break;
            case 'H' : if (encounterd[1]) return false; else { t =  3600000.0 * t; encounterd[1] = true; } break;
            case 'M' : if (encounterd[2]) return false; else { t =    60000.0 * t; encounterd[2] = true; } break;
            case 'S' : if (encounterd[3]) return false; else { t =     1000.0 * t; encounterd[3] = true; } break;
            default  : return false;
         }

         if (('D' != std::toupper(*(token_list[i].second - 1))) && (t < 0.0))
            return false;

         period += static_cast<long long>(t);
      }

      return true;
   }

private:

   enum { max_token_count = 4 };
   typedef std::pair<char*,char*> iterator_type;

   strtk::multiple_char_delimiter_predicate predicate_;
   iterator_type token_list[max_token_count];
};

int main()
{
   period_parser parser;
   const std::string period_string[] =
               {
                  "123.456D789.012Hr345.678Min901.234Sec",
                  "789.012Hr345.678Min901.234Sec123.456D",
                  "345.678Min901.234Sec123.456D789.012Hr",
                  "901.234Sec123.456D789.012Hr345.678Min",
                  "123.456D 789.012Hr 345.678Min 901.234Sec",
                  "789.012Hr 345.678Min 901.234Sec 123.456D",
                  "345.678Min 901.234Sec 123.456D 789.012Hr",
                  "901.234Sec 123.456D 789.012Hr 345.678Min",
                  "123.456D:789.012Hr:345.678Min:901.234Sec",
                  "789.012Hr:345.678Min:901.234Sec:123.456D",
                  "345.678Min:901.234Sec:123.456D:789.012Hr",
                  "901.234Sec:123.456D:789.012Hr:345.678Min",
                  "0123.456D0789.012Hr0345.678Min0901.234Sec",
                  "0789.012Hr0345.678Min0901.234Sec0123.456D",
                  "0345.678Min0901.234Sec0123.456D0789.012Hr",
                  "0901.234Sec0123.456D0789.012Hr0345.678Min",
                  "0123.456D 0789.012Hr 0345.678Min 0901.234Sec",
                  "0789.012Hr 0345.678Min 0901.234Sec 0123.456D",
                  "0345.678Min 0901.234Sec 0123.456D 0789.012Hr",
                  "0901.234Sec 0123.456D 0789.012Hr 0345.678Min"
               };

   const std::size_t period_string_size = sizeof(period_string) / sizeof(std::string);

   long long t = 0;

   for (std::size_t i = 0; i < period_string_size; ++i)
   {
      if (parser(period_string[i],t))
         std::cout << "Period: " << t << "msec" << std::endl;
      else
         std::cout << "Failed to parse: " << period_string[i] << std::endl;
   }

   return 0;
}
