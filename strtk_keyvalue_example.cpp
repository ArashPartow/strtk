/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Key-Value Pair Parser Example                                 *
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
   Description: This example demonstrates parsing of a sequence of key-value
                pairs.

                In the first  example, a data  object named "data_store"  is
                used which contains 22 members of varying type that  require
                populating. Each member is mapped via a key to a value which
                is registered with the key-value parser. The examples  below
                demonstrate  parsing of  keys as  both unsigned  int and  of
                std::string types.

                The process consists of an array of 5 sequences of key-value
                pairs, in which the  pairs have been permuted  randomly, are
                passed  to  the  specified  key-value  parser.  The   parser
                processes each  sequence, splitting  them up  into pairs and
                subsequently processing each pair, mapping parsed values  to
                the  appropriate  registered member.  The  whole process  is
                timed, and  details such  as total  time and  rate of  pairs
                parsed  per second  is displayed.  Note a  running total  is
                maintained  so as  to overcome  the effects  of overzealous
                optimizers.

                In the second  example a data  object named complex_data  is
                used that is comprised of  six unique types. Three of  which
                are STL value sequences. Twelve sequences of key-value pairs
                representing POD  data and  sequences of  values. Parsing of
                the  sequences  are  carried out,  upon  a  successful parse
                operation  the  members  of  the  complex_data  instance are
                printed to stdout.
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

void example01()
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
            {
               std::cout << "Failed to parse data: " << data[j] << std::endl;
               return;
            }
         }
      }
      timer.stop();

      printf("[uintkeymap] Total: %d Time %8.5fsec Rates: %12.3fpairs/sec %12.3fseq/sec\n",
             total,
             timer.time(),
             (rounds * data_store::member_count * data_size) / timer.time (),
             (rounds * data_size) / timer.time ());
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
            {
               std::cout << "Failed to parse data: " << data[j] << std::endl;
               return;
            }
         }
      }
      timer.stop();

      printf("[strkeymap ] Total: %d Time %8.5fsec Rates: %12.3fpairs/sec %12.3fseq/sec\n",
             total,
             timer.time(),
             (rounds * data_store::member_count * data_size) / timer.time (),
             (rounds * data_size) / timer.time ());
   }
}

struct complex_data
{
   int                    v0; // key = 00
   double                 v1; // key = 01
   std::string            v2; // key = 02
   std::vector<int>       v3; // key = 03
   std::deque<double>     v4; // key = 04
   std::list<std::string> v5; // key = 05

   void reset()
   {
      v3.clear();
      v4.clear();
      v5.clear();
   }
};

void example02()
{
   static const std::string data[] =
                     {
                        "00=123456|01=1234567.1234567|02=Simple text|03=-3,-2,-1,0,1,2,3|04=1.1,2.2,3.3,4.4,5.5,6.6|05=Text1,Text2,Text3,Text4",
                        "01=1234567.1234567|02=Simple text|03=-3,-2,-1,0,1,2,3|04=1.1,2.2,3.3,4.4,5.5,6.6|05=Text1,Text2,Text3,Text4|00=123456",
                        "02=Simple text|03=-3,-2,-1,0,1,2,3|04=1.1,2.2,3.3,4.4,5.5,6.6|05=Text1,Text2,Text3,Text4|00=123456|01=1234567.1234567",
                        "03=-3,-2,-1,0,1,2,3|04=1.1,2.2,3.3,4.4,5.5,6.6|05=Text1,Text2,Text3,Text4|00=123456|01=1234567.1234567|02=Simple text",
                        "04=1.1,2.2,3.3,4.4,5.5,6.6|05=Text1,Text2,Text3,Text4|00=123456|01=1234567.1234567|02=Simple text|03=-3,-2,-1,0,1,2,3",
                        "05=Text1,Text2,Text3,Text4|00=123456|01=1234567.1234567|02=Simple text|03=-3,-2,-1,0,1,2,3|04=1.1,2.2,3.3,4.4,5.5,6.6",
                        "00=123456|03=-3,-2,-1,0,1,2,3|02=Simple text|04=1.1,2.2,3.3,4.4,5.5,6.6|01=1234567.1234567|05=Text1,Text2,Text3,Text4",
                        "03=-3,-2,-1,0,1,2,3|02=Simple text|04=1.1,2.2,3.3,4.4,5.5,6.6|01=1234567.1234567|05=Text1,Text2,Text3,Text4|00=123456",
                        "02=Simple text|04=1.1,2.2,3.3,4.4,5.5,6.6|01=1234567.1234567|05=Text1,Text2,Text3,Text4|00=123456|03=-3,-2,-1,0,1,2,3",
                        "04=1.1,2.2,3.3,4.4,5.5,6.6|01=1234567.1234567|05=Text1,Text2,Text3,Text4|00=123456|03=-3,-2,-1,0,1,2,3|02=Simple text",
                        "01=1234567.1234567|05=Text1,Text2,Text3,Text4|00=123456|03=-3,-2,-1,0,1,2,3|02=Simple text|04=1.1,2.2,3.3,4.4,5.5,6.6",
                        "05=Text1,Text2,Text3,Text4|00=123456|03=-3,-2,-1,0,1,2,3|02=Simple text|04=1.1,2.2,3.3,4.4,5.5,6.6|01=1234567.1234567",
                     };

   static const std::size_t data_size = sizeof(data) / sizeof(std::string);

   typedef strtk::keyvalue::parser<strtk::keyvalue::uintkey_map> kvp_type;

   strtk::keyvalue::uintkey_map::options options;

   options.key_count            =   6; //[0,5]
   options.pair_block_delimiter = '|';
   options.pair_delimiter       = '=';

   kvp_type kvp(options);

   complex_data cd;

   strtk::vector_sink<int>::type vec_sink(",");
   strtk::deque_sink<double>::type deq_sink(",");
   strtk::list_sink<std::string>::type lst_sink(",");

   kvp.register_keyvalue(0,cd.v0);
   kvp.register_keyvalue(1,cd.v1);
   kvp.register_keyvalue(2,cd.v2);
   kvp.register_keyvalue(3,vec_sink(cd.v3));
   kvp.register_keyvalue(4,deq_sink(cd.v4));
   kvp.register_keyvalue(5,lst_sink(cd.v5));

   for (std::size_t i = 0; i < data_size; ++i)
   {
      if (kvp(data[i]))
      {
         std::cout << "Seq[" << strtk::text::right_align(2,'0',i) << "] "
                   << "(Key0: " << cd.v0 << ") "
                   << "(Key1: " << cd.v1 << ") "
                   << "(Key2: " << cd.v2 << ") "
                   << "(Key3: " << strtk::join(" ",cd.v3) << ") "
                   << "(Key4: " << strtk::join(" ",cd.v4) << ") "
                   << "(Key5: " << strtk::join(" ",cd.v5) << ")\n";
         cd.reset();
      }
      else
         std::cout << "Failed to parse data: " << data[i] << std::endl;
   }
}

int main()
{
   example01();
   example02();
   return 0;
}
