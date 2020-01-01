/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Token Grid Examples                                           *
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
   Description: This example aims to demonstrate the usage patterns and
                functionality of the Token Grid class. The example test
                cases construct a string that represents a token grid -
                It should be noted the class can just as easily use a
                text file as input. The examples range from determining
                sums and averages of rows and columns of numeric values,
                iterating down columns, outputting rows/columns in
                specific orderings etc.
*/


#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <utility>
#include <string>
#include <vector>
#include <utility>

#include "strtk.hpp"


void token_grid_test01()
{
   std::cout << "token_grid_test01\n";

   std::string data;
   data += "1,2,3,4,5\n";
   data += "0,2,4,6,8\n";
   data += "1,3,5,7,9\n";

   strtk::token_grid grid(data,data.size(),",");

   {
      const std::size_t max_row = grid.row_count();
      const std::size_t max_col = grid.max_column_count();

      for (std::size_t row = 0; row < max_row; ++row)
      {
         for (std::size_t col = 0; col < max_col; ++col)
         {
            std::cout << grid.get<int>(row,col) << "\t";
         }

         std::cout << std::endl;
      }

      std::cout << std::endl;
   }

   {
      for (std::size_t i = 0; i < grid.row_count(); ++i)
      {
         strtk::token_grid::row_type r = grid.row(i);

         for (std::size_t j = 0; j < r.size(); ++j)
         {
            std::cout << r.get<int>(j) << "\t";
         }

         std::cout << std::endl;
      }

      std::cout << std::endl;
   }
}

void token_grid_test02()
{
   std::cout << "token_grid_test02\n";

   std::string data;

   data += "1\n";
   data += "1,2\n";
   data += "1,2,3\n";
   data += "1,2,3,4\n";
   data += "1,2,3,4,5\n";
   data += "1,2,3,4,5,6\n";
   data += "1,2,3,4,5,6,7\n";
   data += "1,2,3,4,5,6,7,8\n";
   data += "1,2,3,4,5,6,7,8,9\n";
   data += "1,2,3,4,5,6,7,8,9,10\n";

   strtk::token_grid grid(data,data.size(),",");

   {
      std::vector<int> lst(10);

      for (std::size_t i = 0; i < grid.row_count(); ++i)
      {
         switch (i)
         {
            case  0 : grid.row(i).parse(lst[0]);
                      break;
            case  1 : grid.row(i).parse(lst[0],lst[1]);
                      break;
            case  2 : grid.row(i).parse(lst[0],lst[1],lst[2]);
                      break;
            case  3 : grid.row(i).parse(lst[0],lst[1],lst[2],lst[3]);
                      break;
            case  4 : grid.row(i).parse(lst[0],lst[1],lst[2],lst[3],lst[4]);
                      break;
            case  5 : grid.row(i).parse(lst[0],lst[1],lst[2],lst[3],lst[4],lst[5]);
                      break;
            case  6 : grid.row(i).parse(lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6]);
                      break;
            case  7 : grid.row(i).parse(lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7]);
                      break;
            case  8 : grid.row(i).parse(lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7],lst[8]);
                      break;
            case  9 : grid.row(i).parse(lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7],lst[8],lst[9]);
                      break;
         }

         std::cout << strtk::join(",",lst.begin(),lst.begin() + (i+1)) << std::endl;
      }
   }

   {
      std::vector<int> lst(10);

      for (std::size_t i = 0; i < grid.row_count(); ++i)
      {
         switch (i)
         {
            case  0 : grid.row(i).parse_with_index(0,lst[0]);
                      break;
            case  1 : grid.row(i).parse_with_index(0,1,lst[0],lst[1]);
                      break;
            case  2 : grid.row(i).parse_with_index(0,1,2,lst[0],lst[1],lst[2]);
                      break;
            case  3 : grid.row(i).parse_with_index(0,1,2,3,lst[0],lst[1],lst[2],lst[3]);
                      break;
            case  4 : grid.row(i).parse_with_index(0,1,2,3,4,lst[0],lst[1],lst[2],lst[3],lst[4]);
                      break;
            case  5 : grid.row(i).parse_with_index(0,1,2,3,4,5,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5]);
                      break;
            case  6 : grid.row(i).parse_with_index(0,1,2,3,4,5,6,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6]);
                      break;
            case  7 : grid.row(i).parse_with_index(0,1,2,3,4,5,6,7,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7]);
                      break;
            case  8 : grid.row(i).parse_with_index(0,1,2,3,4,5,6,7,8,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7],lst[8]);
                      break;
            case  9 : grid.row(i).parse_with_index(0,1,2,3,4,5,6,7,8,9,lst[0],lst[1],lst[2],lst[3],lst[4],lst[5],lst[6],lst[7],lst[8],lst[9]);
                      break;
         }

         std::cout << strtk::join(",",lst.begin(),lst.begin() + (i+1)) << std::endl;
      }
   }

   {
      std::vector<int> lst(10);

      for (std::size_t i = 0; i < grid.row_count(); ++i)
      {
         lst.clear();
         grid.row(i).parse(lst);
         std::cout << strtk::join(",",lst) << std::endl;
      }
   }

   {
      std::vector<int> lst(10);

      for (std::size_t i = 0; i < grid.row_count(); ++i)
      {
         lst.clear();
         grid.row(i).parse_n(i + 1,lst);
         std::cout << strtk::join(",",lst) << std::endl;
      }
   }

}

void token_grid_test03()
{
   std::cout << "token_grid_test03\n";

   std::string data;
   data += "1,2,3,4,5\n";
   data += "1,2,3,4,5\n";
   data += "1,2,3,4,5\n";
   data += "1,2,3,4,5\n";

   std::vector<int> lst0;
   std::vector<int> lst1;
   std::vector<int> lst2;
   std::vector<int> lst3;
   std::vector<int> lst4;

   strtk::token_grid grid(data,data.size(),",");

   grid.extract_column(grid.all_rows(),
                       0,
                       strtk::back_inserter_with_valuetype(lst0));
   std::cout << strtk::join("\t",lst0) << std::endl;
   lst0.clear();

   grid.extract_column(grid.all_rows(),
                       0,1,
                       strtk::back_inserter_with_valuetype(lst0),
                       strtk::back_inserter_with_valuetype(lst1));

   std::cout << strtk::join("\t",lst0) << std::endl;
   std::cout << strtk::join("\t",lst1) << std::endl;

   lst0.clear();
   lst1.clear();

   grid.extract_column(grid.all_rows(),
                       0,1,2,
                       strtk::back_inserter_with_valuetype(lst0),
                       strtk::back_inserter_with_valuetype(lst1),
                       strtk::back_inserter_with_valuetype(lst2));

   std::cout << strtk::join("\t",lst0) << std::endl;
   std::cout << strtk::join("\t",lst1) << std::endl;
   std::cout << strtk::join("\t",lst2) << std::endl;

   lst0.clear();
   lst1.clear();
   lst2.clear();

   grid.extract_column(grid.all_rows(),
                       0,1,2,3,
                       strtk::back_inserter_with_valuetype(lst0),
                       strtk::back_inserter_with_valuetype(lst1),
                       strtk::back_inserter_with_valuetype(lst2),
                       strtk::back_inserter_with_valuetype(lst3));

   std::cout << strtk::join("\t",lst0) << std::endl;
   std::cout << strtk::join("\t",lst1) << std::endl;
   std::cout << strtk::join("\t",lst2) << std::endl;
   std::cout << strtk::join("\t",lst3) << std::endl;

   lst0.clear();
   lst1.clear();
   lst2.clear();
   lst3.clear();

   grid.extract_column(grid.all_rows(),
                       0,1,2,3,4,
                       strtk::back_inserter_with_valuetype(lst0),
                       strtk::back_inserter_with_valuetype(lst1),
                       strtk::back_inserter_with_valuetype(lst2),
                       strtk::back_inserter_with_valuetype(lst3),
                       strtk::back_inserter_with_valuetype(lst4));

   std::cout << strtk::join("\t",lst0) << std::endl;
   std::cout << strtk::join("\t",lst1) << std::endl;
   std::cout << strtk::join("\t",lst2) << std::endl;
   std::cout << strtk::join("\t",lst3) << std::endl;
   std::cout << strtk::join("\t",lst4) << std::endl;

   lst0.clear();
   lst1.clear();
   lst2.clear();
   lst3.clear();
   lst4.clear();
}

void token_grid_test04()
{
   std::cout << "token_grid_test04\n";

   std::string data;
   data.reserve(160);
   data = strtk::replicate(10,"1,2,3,4,5\n1,2,3,4\n1,2,3\n1,2\n1\n");

   strtk::token_grid grid(data,data.size(),",");

   grid.enforce_column_count(5);
   for (unsigned int i = 0; i < grid.row_count(); ++i)
   {
      strtk::token_grid::row_type row = grid.row(i);

      for (unsigned j = 0; j < row.size(); ++j)
      {
         strtk::token_grid::range_t r = row.token(j);
         std::cout << r << "\t";
      }

      std::cout << std::endl;
   }
}

void token_grid_test05()
{
   std::cout << "token_grid_test05\n";

   std::string data;
   data.reserve(160);
   data = strtk::replicate(10,"1,2,3,4,5\n1,2,3,4\n1,2,3\n1,2\n1\n");

   strtk::token_grid grid(data,data.size(),",");

   for (unsigned int i = 0; i < grid.row_count(); ++i)
   {
      strtk::token_grid::row_type row = grid.row(i);

      for (unsigned j = 0; j < row.size(); ++j)
      {
         std::cout << grid.get<int>(i,j) << "\t";
      }

      std::cout << std::endl;
   }
}

void token_grid_test06()
{
   std::cout << "token_grid_test06\n";

   std::string data;
   data += "1.1,1.1,1.1,1.1,1.1,1.1\n"
           "2.2,2.2,2.2,2.2,2.2,2.2\n"
           "3.3,3.3,3.3,3.3,3.3,3.3\n"
           "4.4,4.4,4.4,4.4,4.4,4.4\n"
           "5.5,5.5,5.5,5.5,5.5,5.5\n"
           "6.6,6.6,6.6,6.6,6.6,6.6\n"
           "7.7,7.7,7.7,7.7,7.7,7.7\n";

   strtk::token_grid grid(data,data.size(),",");

   std::vector<double> avg_c(grid.row(0).size(),0.0);
   std::vector<double> avg_r(grid.row_count(),0.0);
   std::vector<double> tmp(grid.row(0).size(),0.0);
   std::fill(avg_c.begin(),avg_c.end(),0.0);

   for (unsigned int i = 0; i < grid.row_count(); ++i)
   {
      grid.row(i).parse<double>(tmp.begin());
      std::transform(avg_c.begin(),avg_c.end(),tmp.begin(),avg_c.begin(),std::plus<double>());
      avg_r[i] = std::accumulate(tmp.begin(),tmp.end(),0.0) / tmp.size();
   }

   for (unsigned int i = 0; i < avg_c.size(); avg_c[i++] /= grid.row_count()) ;

   std::cout << "Column Averages:\t";
   std::copy(avg_c.begin(),avg_c.end(),std::ostream_iterator<double>(std::cout,"\t"));
   std::cout << "\n";

   std::cout << "Row Averages:\t";
   std::copy(avg_r.begin(),avg_r.end(),std::ostream_iterator<double>(std::cout,"\t"));
   std::cout << "\n";
}

void token_grid_test07()
{
   std::cout << "token_grid_test07\n";

   std::string data;
   data += "The The  The The  The   The The The The\n";
   data += "quick quick  quick quick  quick   quick quick  quick quick\n";
   data += "brown  brown brown brown brown brown brown brown brown\n";
   data += "fox fox  fox  fox fox   fox fox  fox    fox\n";
   data += "jumps jumps jumps jumps jumps jumps jumps jumps jumps\n";
   data += "over  over  over  over  over  over  over  over  over\n";
   data += "the   the the    the   the   the the  the   the\n";
   data += "lazy  lazy lazy lazy  lazy  lazy lazy  lazy  lazy\n";
   data += "dog   dog  dog   dog   dog   dog   dog   dog dog\n\n\n";

   strtk::token_grid grid(data,data.size()," ");

   if (grid.min_column_count() != grid.max_column_count())
   {
      std::cout << "token_grid_test07 - ERROR!" << std::endl;
      std::cout << "min column count: " << grid.min_column_count() << std::endl;
      std::cout << "max column count: " << grid.max_column_count() << std::endl;
   }

   std::vector<std::string> words;
   words.reserve(grid.row_count());

   for (std::size_t i = 0; i < grid.min_column_count(); ++i)
   {
      words.clear();
      grid.extract_column(i,strtk::back_inserter_with_valuetype(words));
      std::cout << strtk::join(" ",words.begin(),words.end()) << std::endl;
   }
}

void token_grid_test08()
{
   std::cout << "token_grid_test08\n";

   std::string data;
   data += "1.1,1.1,1.1,1.1,1.1,1.1\n"
           "2.2,2.2,2.2,2.2,2.2,2.2\n"
           "3.3,3.3,3.3,3.3,3.3,3.3\n"
           "4.4,4.4,4.4,4.4,4.4,4.4\n"
           "5.5,5.5,5.5,5.5,5.5,5.5\n"
           "6.6,6.6,6.6,6.6,6.6,6.6\n"
           "7.7,7.7,7.7,7.7,7.7,7.7\n";

   strtk::token_grid grid(data,data.size(),",");

   for (std::size_t r = 0; r < grid.row_count(); ++r)
   {
      double sum = 0.0;
      if (grid.accumulate_row(r,sum))
         std::cout << "sum(row["<< r <<"]) = " << sum << std::endl;
      else
         std::cout << "failed row["<< r <<"]" << std::endl;
   }

   for (std::size_t c = 0; c < grid.max_column_count(); ++c)
   {
      double sum = 0.0;
      if (grid.accumulate_column(c,sum))
         std::cout << "sum(col["<< c <<"]) = " << sum << std::endl;
      else
         std::cout << "failed col["<< c <<"]" << std::endl;
   }
}

struct is_even
{
   inline bool operator()(const strtk::token_grid::row_type& row)
   {
      return 0 == (static_cast<unsigned int>(strtk::string_to_type_converter<double>(row.token(0))) % 2);
   }

   inline bool operator()(const strtk::token_grid::range_t& range)
   {
      return 0 == (static_cast<unsigned int>(strtk::string_to_type_converter<double>(range)) % 2);
   }
};

void token_grid_test09()
{
   std::cout << "token_grid_test09\n";

   std::string data;
   data += "1.1,1.1,1.1,1.1,1.1,1.1\n"
           "2.2,2.2,2.2,2.2,2.2,2.2\n"
           "3.3,3.3,3.3,3.3,3.3,3.3\n"
           "4.4,4.4,4.4,4.4,4.4,4.4\n"
           "5.5,5.5,5.5,5.5,5.5,5.5\n"
           "6.6,6.6,6.6,6.6,6.6,6.6\n"
           "7.7,7.7,7.7,7.7,7.7,7.7\n";

   {
      strtk::token_grid grid(data,data.size(),",");

      for (std::size_t r = 0; r < grid.row_count(); ++r)
      {
         std::string row = "";
         if (grid.join_row(r,"|",row))
            std::cout << "row["<< r <<"] = " << row << std::endl;
         else
            std::cout << "failed row["<< r <<"]" << std::endl;
      }

      for (std::size_t c = 0; c < grid.max_column_count(); ++c)
      {
         std::string col = "";
         if (grid.join_column(c,"|",col))
            std::cout << "col["<< c <<"] = " << col << std::endl;
         else
            std::cout << "failed col["<< c <<"]" << std::endl;
      }
   }

   {
      strtk::token_grid grid(data,data.size(),",");

      for (std::size_t r = 0; r < grid.row_count(); ++r)
      {
         std::string row = "";
         if (grid.join_row(r,is_even(),"|",row))
            std::cout << "row["<< r <<"] = " << row << std::endl;
         else
            std::cout << "failed row["<< r <<"]" << std::endl;
      }

      for (std::size_t c = 0; c < grid.max_column_count(); ++c)
      {
         std::string col = "";
         if (grid.join_column(c,is_even(),"|",col))
            std::cout << "col["<< c <<"] = " << col << std::endl;
         else
            std::cout << "failed col["<< c <<"]" << std::endl;
      }
   }

}

struct symbol_predicate
{
public:

   symbol_predicate(const std::string& symbol)
   : symbol_(symbol)
   {}

   bool operator()(const strtk::token_grid::row_type& row)
   {
      return symbol_ == row.get<std::string>(1);
   }

private:

   std::string symbol_;
};

void token_grid_test10()
{
   std::cout << "token_grid_test10\n";

                                   //Date,Symbol,Open,Close,High,Low,Volume
   const std::string market_data = "20090701,GOOG,424.2000,418.9900,426.4000,418.1500,2310768\n"
                                   "20090701,MSFT,24.0500,24.0400,24.3000,23.9600,54915127\n"
                                   "20090702,GOOG,415.4100,408.4900,415.4100,406.8100,2517630\n"
                                   "20090702,MSFT,23.7600,23.3700,24.0400,23.2100,65427699\n"
                                   "20090703,GOOG,408.4900,408.4900,408.4900,408.4900,0\n"
                                   "20090703,MSFT,23.3700,23.3700,23.3700,23.3700,0\n"
                                   "20090706,GOOG,406.5000,409.6100,410.6400,401.6600,2262557\n"
                                   "20090706,MSFT,23.2100,23.2000,23.2800,22.8700,49207638\n"
                                   "20090707,GOOG,408.2400,396.6300,409.1900,395.9801,3260307\n"
                                   "20090707,MSFT,23.0800,22.5300,23.1400,22.4600,52842412\n"
                                   "20090708,GOOG,400.0000,402.4900,406.0000,398.0600,3441854\n"
                                   "20090708,MSFT,22.3100,22.5600,22.6900,2200000,73023306\n"
                                   "20090709,GOOG,406.1200,410.3900,414.4500,405.8000,3275816\n"
                                   "20090709,MSFT,22.6500,22.4400,22.8100,22.3700,46981174\n"
                                   "20090710,GOOG,409.5700,414.4000,417.3700,408.7000,2929559\n"
                                   "20090710,MSFT,22.1900,22.3900,22.5400,22.1500,43238698\n";

   static const std::size_t volume_column = 6;

   strtk::token_grid grid(market_data,market_data.size(),",");

   struct stock_info
   {
      stock_info()
      : total_volume(0),
        day_count(0),
        average_daily_volume(0.0)
      {}

      unsigned long long total_volume;
      std::size_t day_count;
      double average_daily_volume;
   };

   stock_info goog;
   stock_info msft;

   goog.day_count = grid.accumulate_column(volume_column,symbol_predicate("GOOG"),goog.total_volume);
   msft.day_count = grid.accumulate_column(volume_column,symbol_predicate("MSFT"),msft.total_volume);

   goog.average_daily_volume = (1.0 * goog.total_volume) / goog.day_count;
   msft.average_daily_volume = (1.0 * msft.total_volume) / msft.day_count;

   std::cout << "[GOOG] Total Volume: " << goog.total_volume << std::endl;
   std::cout << "[MSFT] Total Volume: " << msft.total_volume << std::endl;

   std::cout << "[GOOG] ADV: " << goog.average_daily_volume << std::endl;
   std::cout << "[MSFT] ADV: " << msft.average_daily_volume << std::endl;
}

void token_grid_test11()
{
   std::cout << "token_grid_test11\n";

   std::string data = "1.1,2.1,3.1,4.1,5.1,6.1,7.1\n"
                      "1.2,2.2,3.2,4.2,5.2,6.2,7.2\n"
                      "1.3,2.3,3.3,4.3,5.3,6.3,7.3\n"
                      "1.4,2.4,3.4,4.4,5.4,6.4,7.4\n"
                      "1.5,2.5,3.5,4.5,5.5,6.5,7.5\n"
                      "1.6,2.6,3.6,4.6,5.6,6.6,7.6\n"
                      "1.7,2.7,3.7,4.7,5.7,6.7,7.7\n";

   strtk::token_grid grid(data,data.size(),",");

   for (std::size_t r = 0; r < grid.row_count(); ++r)
   {
      if ((0 < r) && (r < grid.row_count() - 1))
      {
         strtk::token_grid::row_type row  = grid.row(r    );
         strtk::token_grid::row_type next = grid.row(r + 1);
         strtk::token_grid::row_type prev = grid.row(r - 1);

         for (std::size_t c = 0; c < row.size(); c++)
         {
            double average = (row.get<double>(c) + next.get<double>(c) + prev.get<double>(c)) / 3.0;
            std::cout << average << "\t";
         }
      }
      else
         std::cout << strtk::replicate(grid.max_column_count(),"N/A\t");

      std::cout << std::endl;
   }
}

class summarizer
{
public:

   enum column_index
   {
      tick_time_column = 0,
      tick_value_column = 1
   };

   summarizer(std::deque<double>& sum_value)
   : next_tick_time_(0),
     sum_value_(sum_value)
   {}

   bool operator()(const strtk::token_grid::row_type& row)
   {
      if (row.get<std::size_t>(tick_time_column) >= next_tick_time_)
      {
         next_tick_time_ = row.get<std::size_t>(tick_time_column) + 3;
         return true;
      }
      else
         return false;
   }

   bool operator()(const strtk::token_grid& grid,
                   const strtk::token_grid::row_range_t& range)
   {
      double bucket_sum = 0.0;

      if (!grid.accumulate_column(tick_value_column,range,bucket_sum))
      {
         std::cout << "failed to accumulate!" << std::endl;
         return false;
      }
      else
         sum_value_.push_back(bucket_sum);

      return true;
   }

private:

   summarizer& operator=(const summarizer&);

   std::size_t next_tick_time_;
   std::deque<double>& sum_value_;
};

void token_grid_test12()
{
   std::cout << "token_grid_test12\n";

                      //time index, value
   std::string data = "10000,123.456\n"
                      "10001,612.345\n"
                      "10002,561.234\n"
                      "10003,456.123\n"
                      "10004,345.612\n"
                      "10005,234.561\n"
                      "10006,123.456\n";

   strtk::token_grid grid(data,data.size(),",");

   std::deque<double> sum_value;
   summarizer s(sum_value);
   grid.sequential_partition(s,s);

   for (std::size_t i = 0; i < sum_value.size(); ++i)
   {
      std::cout << "bucket[" << i << "] = " << sum_value[i] << std::endl;
   }
}

struct match_predicate
{
   inline bool operator()(const strtk::token_grid::row_type& row) const
   {
      return (*this)(row.range());
   }

   inline bool operator()(const strtk::token_grid::range_t& range) const
   {
      return range.second != std::find(range.first,range.second,'6');
   }
};

void token_grid_test13()
{
   std::cout << "token_grid_test13\n";

   std::string data = "abc,123\n"
                      "ijk,345\n"
                      "mno,567\n"
                      "rst,789\n"
                      "xyz,901\n";

   strtk::token_grid grid(data,data.size(),",");

   std::cout << "Before Removal" << std::endl;

   for (std::size_t r = 0; r < grid.row_count(); ++r)
   {
      std::cout << r << "[" << grid.row(r).as_string() << "]" << std::endl;
   }

   strtk::token_grid::row_range_t range(1,4);

   grid.remove_row_if(range,match_predicate());

   std::cout << "After Removal" << std::endl;

   for (std::size_t r = 0; r < grid.row_count(); ++r)
   {
      std::cout << r << "[" << grid.row(r).as_string() << "]" << std::endl;
   }
}

void token_grid_test14()
{
   std::cout << "token_grid_test14\n";

   const std::string data = ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n"
                            ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n"
                            ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n"
                            ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n"
                            ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n"
                            ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n"
                            ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n"
                            ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n"
                            ",0,,1,,,2,,3,,,4,,5,,,6,,7,,,8,,9,,,\n";

   {
      strtk::token_grid::options options;

      options
         .set_column_delimiters(",")
         .set_column_split_option(strtk::split_options::default_mode);

      strtk::token_grid grid(data,data.size(),options);

      std::cout << "Before Empty Token Removal" << std::endl;

      for (std::size_t r = 0; r < grid.row_count(); ++r)
      {
         strtk::token_grid::row_type row = grid.row(r);

         for (std::size_t c = 0; c < row.size(); ++c)
         {
            std::cout << "[" << row.get<std::string>(c) << "] ";
         }

         std::cout << std::endl;
      }

      grid.remove_empty_tokens();

      std::cout << "After Empty Token Removal" << std::endl;

      for (std::size_t r = 0; r < grid.row_count(); ++r)
      {
         strtk::token_grid::row_type row = grid.row(r);

         for (std::size_t c = 0; c < row.size(); ++c)
         {
            std::cout << "[" << row.get<std::string>(c) << "] ";
         }

         std::cout << std::endl;
      }

      std::cout << std::endl;
   }

   {
      strtk::token_grid::options options;

      options
         .set_column_delimiters(",")
         .set_column_split_option(strtk::split_options::default_mode);

      strtk::token_grid grid(data,data.size(),options);

      std::cout << "Before Empty Token Removal" << std::endl;
      for (std::size_t r = 0; r < grid.row_count(); ++r)
      {
         strtk::token_grid::row_type row = grid.row(r);

         for (std::size_t c = 0; c < row.size(); ++c)
         {
            std::cout << "[" << row.get<std::string>(c) << "] ";
         }

         std::cout << std::endl;
      }

      strtk::token_grid::row_range_t range(3,7);
      grid.remove_empty_tokens(range);

      std::cout << "After Empty Token Removal" << std::endl;

      for (std::size_t r = 0; r < grid.row_count(); ++r)
      {
         strtk::token_grid::row_type row = grid.row(r);

         for (std::size_t c = 0; c < row.size(); ++c)
         {
            std::cout << "[" << row.get<std::string>(c) << "] ";
         }

         std::cout << std::endl;
      }

      std::cout << std::endl;
   }

   {
      strtk::token_grid::options options;

      options
         .set_column_delimiters(",")
         .set_column_split_option(strtk::split_options::default_mode);

      strtk::token_grid grid(data,data.size(),options);

      std::cout << "Before Empty Token Removal" << std::endl;

      for (std::size_t r = 0; r < grid.row_count(); ++r)
      {
         strtk::token_grid::row_type row = grid.row(r);

         for (std::size_t c = 0; c < row.size(); ++c)
         {
            std::cout << "[" << row.get<std::string>(c) << "] ";
         }

         std::cout << std::endl;
      }

      strtk::token_grid::row_range_t range(4,8);
      grid.remove_token_if(range,strtk::size_equal_to<0>());

      std::cout << "After Empty Token Removal" << std::endl;

      for (std::size_t r = 0; r < grid.row_count(); ++r)
      {
         strtk::token_grid::row_type row = grid.row(r);

         for (std::size_t c = 0; c < row.size(); ++c)
         {
            std::cout << "[" << row.get<std::string>(c) << "] ";
         }

         std::cout << std::endl;
      }

      std::cout << std::endl;
   }
}

void token_grid_test15()
{
   std::cout << "token_grid_test15\n";

   const std::string data = "\"ICAO Code\"|\"IATA Code\"|Airport|City|Country\n"
                            "AYGA|GKA|\"Goroka Gatue\"|Goroka|Papua New Guinea\n"
                            "BGCO|GCO|\"Nerlerit Inaat Constable Pynt\"|\"Nerlerit Inaat\"|Greenland\n"
                            "BZGD|ZGD|Godley|Auckland|New Zealand\n"
                            "CYQM|YQM|\"Greater Moncton International\"|Moncton|Canada\n"
                            "EDRK|ZNV|\"Koblenz Winningen\"|Koblenz|Germany\n"
                            "FAHU|AHU|Kwazulu-Natal|\"HMS Bastard Memorial\"|South Africa\n"
                            "FQMP|MZB|\"Mocimboa Da Praia\"|\"Mocimboa Da Praia\"|Mozambique\n"
                            "KINS|INS|\"Indian Springs AF AUX\"|Indian Springs|USA\n"
                            "UHNN|HNN|Nikolaevsk|\"Nikolaevsk Na Amure\"|Russia\n"
                            "WBKK|BKI|\"Kota Kinabalu International\"|Kota Kinabalu|Malaysia\n"
                            "ZSJD|JDZ|\"Jingdezhen Airport\"|Jingdezhen|China\n";

   strtk::token_grid::options options;

   options
      .set_column_delimiters("|")
      .support_dquotes = true;

   strtk::token_grid grid(data,data.size(),options);

   for (std::size_t r = 0; r < grid.row_count(); ++r)
   {
      strtk::token_grid::row_type row = grid.row(r);

      for (std::size_t c = 0; c < row.size(); ++c)
      {
         std::cout << "[" << row.get<std::string>(c) << "] ";
      }

      std::cout << std::endl;
   }
}

void token_grid_test16()
{
   std::cout << "token_grid_test16\n";

   const std::string market_data = "Date,Symbol,Open,Close,High,Low,Volume\n"
                                   "20090701,GOOG,424.2000,418.9900,426.4000,418.1500,2310768\n"
                                   "20090701,MSFT,24.0500,24.0400,24.3000,23.9600,54915127\n"
                                   "20090702,GOOG,415.4100,408.4900,415.4100,406.8100,2517630\n"
                                   "20090702,MSFT,23.7600,23.3700,24.0400,23.2100,65427699\n"
                                   "20090703,GOOG,408.4900,408.4900,408.4900,408.4900,0\n"
                                   "20090703,MSFT,23.3700,23.3700,23.3700,23.3700,0\n"
                                   "20090706,GOOG,406.5000,409.6100,410.6400,401.6600,2262557\n"
                                   "20090706,MSFT,23.2100,23.2000,23.2800,22.8700,49207638\n"
                                   "20090707,GOOG,408.2400,396.6300,409.1900,395.9801,3260307\n"
                                   "20090707,MSFT,23.0800,22.5300,23.1400,22.4600,52842412\n"
                                   "20090708,GOOG,400.0000,402.4900,406.0000,398.0600,3441854\n"
                                   "20090708,MSFT,22.3100,22.5600,22.6900,2200000,73023306\n"
                                   "20090709,GOOG,406.1200,410.3900,414.4500,405.8000,3275816\n"
                                   "20090709,MSFT,22.6500,22.4400,22.8100,22.3700,46981174\n"
                                   "20090710,GOOG,409.5700,414.4000,417.3700,408.7000,2929559\n"
                                   "20090710,MSFT,22.1900,22.3900,22.5400,22.1500,43238698\n";

   strtk::token_grid::options options;

   options
      .set_column_delimiters(",")
      .set_column_split_option(strtk::split_options::default_mode);

   strtk::token_grid grid(market_data,market_data.size(),options);

   std::vector<std::size_t> column_width;

   grid.get_column_widths(column_width);

   for (std::size_t r = 0; r < grid.row_count(); ++r)
   {
      const strtk::token_grid::row_type& row = grid.row(r);

      for (std::size_t c = 0; c < row.size(); ++c)
      {
         std::string cell;

         if (row.is_null(c))
            cell = std::string(column_width[c],' ');
         else if ((c <= 1) || (0 == r))
            cell = strtk::text::center(column_width[c],row.get<std::string>(c));
         else
            cell = strtk::text::right_align(column_width[c],row.get<std::string>(c));

         printf("[%s] ",cell.c_str());
      }

      printf("\n");
   }
}

int main()
{
   token_grid_test01();
   token_grid_test02();
   token_grid_test03();
   token_grid_test04();
   token_grid_test05();
   token_grid_test06();
   token_grid_test07();
   token_grid_test08();
   token_grid_test09();
   token_grid_test10();
   token_grid_test11();
   token_grid_test12();
   token_grid_test13();
   token_grid_test14();
   token_grid_test15();
   token_grid_test16();
   return 0;
}
