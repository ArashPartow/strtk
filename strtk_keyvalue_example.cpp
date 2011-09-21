/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Key-Value Pair Parser Example                                 *
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
   Description: This example demonstrates parsing of a sequence of key-value
                pairs. An example data object named "data_store" is used which
                contains 22 various types of members that need populating. Each
                member is mapped via a key to value which is registered with the
                parser. The examples below demonstrate parsing of keys as
                unsigned int and of std::string types.

                An array of 5 sequences of key-value pairs, in various orderings,
                are passed to the key-value parser. The parser processes each
                sequence, splitting them up into pairs and subsequently
                processing each pair, mapping parsed values to the appropriate
                registered value. The whole process is timed, and details such as
                total time and rate of pairs parsed per second is displayed. Note
                a running total is maintained so as to overcome the effects of
                overzealous optimizers.
*/


#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <deque>

#include "strtk.hpp"


struct data_store
{
   static const std::size_t member_count = 22;

   data_store()
   :   c1(0),
      uc1(0),
       s1(0),
      us1(0),
       i1(0),
      ui1(0),
      ll1(0),
     ull1(0),
     d1(0.0),
     f1(0.0),
    str1(""),
        c2(0),
       uc2(0),
        s2(0),
       us2(0),
        i2(0),
       ui2(0),
       ll2(0),
      ull2(0),
      d2(0.0),
      f2(0.0),
     str2("")
   {}

   char                 c1; // key = 121
   unsigned char       uc1; // key = 122
   short                s1; // key = 123
   unsigned short      us1; // key = 124
   int                  i1; // key = 125
   unsigned int        ui1; // key = 126
   long long           ll1; // key = 127
   unsigned long long ull1; // key = 128
   double               d1; // key = 129
   float                f1; // key = 110
   std::string        str1; // key = 111


   char                 c2; // key = 11321
   unsigned char       uc2; // key = 11322
   short                s2; // key = 11323
   unsigned short      us2; // key = 11324
   int                  i2; // key = 11325
   unsigned int        ui2; // key = 11326
   long long           ll2; // key = 11327
   unsigned long long ull2; // key = 11328
   double               d2; // key = 11329
   float                f2; // key = 11310
   std::string        str2; // key = 11311


};

int main()
{
   static const std::size_t rounds = 1000000;

   // Format: key_0=value_1|key_2=value_2|......|key_n=value_n
   static const std::string data[] =
                      {
                         //Seq 00
                         "121=A|122=z|123=-123|124=456|125=-12345678|126=789012345|127=-43654364634645235|128=777777777777777777|129=1234.56789|110=0.0003|111=abcdefghijklmnopqrstuvwxyz|"
                         "11321=A|11322=z|11323=-123|11324=456|11325=-12345678|11326=789012345|11327=-43654364634645235|11328=777777777777777777|11329=1234.56789|11310=0.0003|11311=abcdefghijklmnopqrstuvwxyz",
                         //Seq 01
                         "121=A|123=-111|125=-17171717|127=-43654364634645235|129=1234.56789|111=abcdefghijklmnopqrstuvwxyz|122=z|124=999|126=789012345|128=777777777777777777|110=0.0003|"
                         "11321=A|11323=-111|11325=-17171717|11327=-43654364634645235|11329=1234.56789|11311=abcdefghijklmnopqrstuvwxyz|11322=z|11324=999|11326=789012345|11328=777777777777777777|11310=0.0003",
                         //Seq 02
                         "122=z|124=999|126=789012345|128=777777777777777777|110=0.0003|121=A|123=-111|125=-17171717|127=-43654364634645235|129=1234.56789|111=abcdefghijklmnopqrstuvwxyz|"
                         "11322=z|11324=999|11326=789012345|11328=777777777777777777|11310=0.0003|11321=A|11323=-111|11325=-17171717|11327=-43654364634645235|11329=1234.56789|11311=abcdefghijklmnopqrstuvwxyz",
                         //Seq 03
                         "121=A|122=z|123=-123|124=456|125=-12345678|126=789012345|127=-43654364634645235|128=777777777777777777|129=1234.56789|110=0.0003|111=abcdefghijklmnopqrstuvwxyz|"
                         "11321=A|11322=z|11323=-123|11324=456|11325=-12345678|11326=789012345|11327=-43654364634645235|11328=777777777777777777|11329=1234.56789|11310=0.0003|11311=abcdefghijklmnopqrstuvwxyz",
                         //Seq 04
                         "111=abcdefghijklmnopqrstuvwxyz|110=0.0003|129=1234.56789|128=777777777777777777|127=-43654364634645235|126=789012345|125=-12345678|124=456|123=-123|122=z|121=A|"
                         "11311=abcdefghijklmnopqrstuvwxyz|11310=0.0003|11329=1234.56789|11328=777777777777777777|11327=-43654364634645235|11326=789012345|11325=-12345678|11324=456|11323=-123|11322=z|11321=A"
                       };

   static const std::size_t data_size = sizeof(data) / sizeof(std::string);

   {
      typedef strtk::keyvalue::parser<strtk::keyvalue::uintkey_map> kvp_type;

      strtk::keyvalue::uintkey_map::options options;

      options.key_count            = 11330; //[110,11329]
      options.pair_block_delimiter = '|';
      options.pair_delimiter       = '=';

      kvp_type kvp(options);

      data_store ds;

      kvp.register_keyvalue(  121,ds.  c1);
      kvp.register_keyvalue(  122,ds. uc1);
      kvp.register_keyvalue(  123,ds.  s1);
      kvp.register_keyvalue(  124,ds. us1);
      kvp.register_keyvalue(  125,ds.  i1);
      kvp.register_keyvalue(  126,ds. ui1);
      kvp.register_keyvalue(  127,ds. ll1);
      kvp.register_keyvalue(  128,ds.ull1);
      kvp.register_keyvalue(  129,ds.  d1);
      kvp.register_keyvalue(  110,ds.  f1);
      kvp.register_keyvalue(  111,ds.str1);

      kvp.register_keyvalue(11321,ds.  c2);
      kvp.register_keyvalue(11322,ds. uc2);
      kvp.register_keyvalue(11323,ds.  s2);
      kvp.register_keyvalue(11324,ds. us2);
      kvp.register_keyvalue(11325,ds.  i2);
      kvp.register_keyvalue(11326,ds. ui2);
      kvp.register_keyvalue(11327,ds. ll2);
      kvp.register_keyvalue(11328,ds.ull2);
      kvp.register_keyvalue(11329,ds.  d2);
      kvp.register_keyvalue(11310,ds.  f2);
      kvp.register_keyvalue(11311,ds.str2);

      unsigned int total = 0;

      strtk::util::timer timer;

      timer.start();
      for (std::size_t i = 0; i < rounds; ++i)
      {
         for (std::size_t j = 0; j < data_size; ++j)
         {
            if (kvp(data[j]))
               total += ds.ui1;
            else
               return 1;
         }
      }
      timer.stop();

      printf("[uintkeymap] Total: %d Time %8.5fsec Rate: %12.3fpairs/sec\n",
             total,
             timer.time(),
             (rounds * data_store::member_count * data_size) / timer.time ());
   }

   {
      typedef unsigned char char_type;
      typedef strtk::keyvalue::parser<strtk::keyvalue::stringkey_map> kvp_type;
      typedef strtk::keyvalue::options<char_type> opts_type;

      opts_type options;

      options.pair_block_delimiter = '|';
      options.pair_delimiter       = '=';

      kvp_type kvp(options);

      data_store ds;

      kvp.register_keyvalue("121",ds.  c1);
      kvp.register_keyvalue("122",ds. uc1);
      kvp.register_keyvalue("123",ds.  s1);
      kvp.register_keyvalue("124",ds. us1);
      kvp.register_keyvalue("125",ds.  i1);
      kvp.register_keyvalue("126",ds. ui1);
      kvp.register_keyvalue("127",ds. ll1);
      kvp.register_keyvalue("128",ds.ull1);
      kvp.register_keyvalue("129",ds.  d1);
      kvp.register_keyvalue("110",ds.  f1);
      kvp.register_keyvalue("111",ds.str1);

      kvp.register_keyvalue("11321",ds.  c2);
      kvp.register_keyvalue("11322",ds. uc2);
      kvp.register_keyvalue("11323",ds.  s2);
      kvp.register_keyvalue("11324",ds. us2);
      kvp.register_keyvalue("11325",ds.  i2);
      kvp.register_keyvalue("11326",ds. ui2);
      kvp.register_keyvalue("11327",ds. ll2);
      kvp.register_keyvalue("11328",ds.ull2);
      kvp.register_keyvalue("11329",ds.  d2);
      kvp.register_keyvalue("11310",ds.  f2);
      kvp.register_keyvalue("11311",ds.str2);

      unsigned int total = 0;

      strtk::util::timer timer;

      timer.start();
      for (std::size_t i = 0; i < rounds; ++i)
      {
         for (std::size_t j = 0; j < data_size; ++j)
         {
            if (kvp(data[j]))
               total += ds.ui1;
            else
               return 1;
         }
      }
      timer.stop();

      printf("[strkeymap ] Total: %d Time %8.5fsec Rate: %12.3fpairs/sec\n",
             total,
             timer.time(),
             (rounds * data_store::member_count * data_size) / timer.time ());
   }

   return 0;
}

