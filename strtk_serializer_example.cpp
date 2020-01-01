/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Serializer Example                                            *
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
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <list>

#include "strtk.hpp"


struct person
{
public:

   unsigned long long id;
   std::string      name;
   unsigned int      age;
   double         height;
   float          weight;
   bool        is_insane;

   person()
   {
      clear();
   }

   bool operator==(const person& p) const
   {
      return (p.id         ==        id) &&
             (p.name       ==      name) &&
             (p.age        ==       age) &&
             (p.weight     ==    weight) &&
             (p.height     ==    height) &&
             (p.is_insane  == is_insane);
   }

   bool operator!=(const person& p) const
   {
      return !operator==(p);
   }

   void clear()
   {
      id     = 0;
      name   = "";
      age    = 0;
      height = 0.0;
      weight = 0.0f;
      is_insane = false;
   }

   strtk_binary_reader_begin()
     strtk_binary_reader(id       )
     strtk_binary_reader(name     )
     strtk_binary_reader(age      )
     strtk_binary_reader(height   )
     strtk_binary_reader(weight   )
     strtk_binary_reader(is_insane)
   strtk_binary_reader_end()

   strtk_binary_writer_begin()
     strtk_binary_writer(id       )
     strtk_binary_writer(name     )
     strtk_binary_writer(age      )
     strtk_binary_writer(height   )
     strtk_binary_writer(weight   )
     strtk_binary_writer(is_insane)
   strtk_binary_writer_end()

};

bool example01(char* buffer, const unsigned int buffer_size)
{
   person p_out;
   person p_in;

   {
      strtk::binary::writer writer(buffer,buffer_size);
      writer.clear();

      p_out.id        = 12345678901234567890ULL;
      p_out.name      = "Mr. Rumpelstilzchen";
      p_out.age       = 637;
      p_out.height    = 123.4567;
      p_out.weight    = 765.345f;
      p_out.is_insane = true;

      writer(p_out);
   }

   {
      strtk::binary::reader reader(buffer,buffer_size);
      reader(p_in);
   }

   if (p_in != p_out)
   {
      std::cout << "example01() - Failed p-in to p-out comparison!" << std::endl;
      return false;
   }

   return true;
}

bool example02(char* buffer, const unsigned int buffer_size)
{
   {
      strtk::binary::writer writer(buffer,buffer_size);
      writer.clear();

      person p_out;
      p_out.id        = 12345678901234567890ULL;
      p_out.name      = "Mr. Rumpelstilzchen";
      p_out.age       = 0;
      p_out.height    = 0.0;
      p_out.weight    = 0.0f;
      p_out.is_insane = false;

      const std::size_t rounds = 1000;
      for (std::size_t i = 0; i < rounds; ++i)
      {
         p_out.id++;
         p_out.age++;
         p_out.height += 1.23;
         p_out.weight += 4.567f;
         p_out.is_insane = !p_out.is_insane;

         if (!writer(p_out))
         {
            std::cout << "example02() - Failed to write person:" << i << std::endl;
            return false;
         }
      }
   }

   {
      strtk::binary::reader reader(buffer,buffer_size);
      person p_in;
      person p_expected;
      p_expected.id        = 12345678901234567890ULL;
      p_expected.name      = "Mr. Rumpelstilzchen";
      p_expected.age       = 0;
      p_expected.height    = 0.0;
      p_expected.weight    = 0.0f;
      p_expected.is_insane = false;

      const std::size_t rounds = 1000;
      for (std::size_t i = 0; i < rounds; ++i)
      {
         p_expected.id++;
         p_expected.age++;
         p_expected.height += 1.23;
         p_expected.weight += 4.567f;
         p_expected.is_insane = !p_expected.is_insane;

         if (!reader(p_in))
         {
            std::cout << "example02() - Failed to read person:" << i << std::endl;
            return false;
         }

         if (p_in != p_expected)
         {
            std::cout << "example02() - Comparison between expected and read failed @ " << i << std::endl;
            return false;
         }
      }
   }

   return true;
}

bool example03(char* buffer, const unsigned int buffer_size)
{
   const std::string file_name = "data.txt";
   const std::size_t rounds = 1000;

   {
      strtk::binary::writer writer(buffer,buffer_size);
      writer.clear();

      person p;
      p.id        = 12345678901234567890ULL;
      p.name      = "Mr. Rumpelstilzchen";
      p.age       = 0;
      p.height    = 123.4567;
      p.weight    = 765.345f;
      p.is_insane = true;

      for (unsigned int i = 0; i < rounds; ++i)
      {
         if (!writer(p))
         {
            std::cout << "example03() - Failed to write person:" << i << std::endl;
            return false;
         }

         p.id++;
         p.age++;
         p.height += 1.23;
         p.weight += 4.567f;
         p.is_insane = !p.is_insane;
      }

      std::ofstream o_stream(file_name.c_str(),std::ios::binary);

      if (!o_stream)
      {
         std::cout << "example03() - ERROR Could not open file!(1)" << std::endl;
         return false;
      }

      writer(o_stream);
      o_stream.close();
   }

   {
      strtk::binary::reader reader(buffer,buffer_size);
      reader.clear();

      const std::size_t length = strtk::fileio::file_size(file_name);

      std::ifstream i_stream(file_name.c_str(),std::ios::binary);
      if (!i_stream)
      {
         std::cout << "example03() - ERROR Could not open file!(2)" << std::endl;
         return false;
      }

      reader(i_stream,length);
      reader.reset();

      person p_expected;
      p_expected.id        = 12345678901234567890ULL;
      p_expected.name      = "Mr. Rumpelstilzchen";
      p_expected.age       = 0;
      p_expected.height    = 123.4567;
      p_expected.weight    = 765.345f;
      p_expected.is_insane = true;

      person p_in;

      for (unsigned int i = 0; i < rounds; ++i)
      {
         p_in.clear();
         if (!reader(p_in))
         {
            std::cout << "example03() - Failed to read person:" << i << std::endl;
            return false;
         }

         if (p_in != p_expected)
         {
            std::cout << "example03() - Comparison between expected and read failed @ " << i << std::endl;
            return false;
         }

         p_expected.id++;
         p_expected.age++;
         p_expected.height += 1.23;
         p_expected.weight += 4.567f;
         p_expected.is_insane = !p_expected.is_insane;
      }
   }

   return true;
}

bool example04(char* buffer, const unsigned int buffer_size)
{
   {
      // Write out and then read back in an array of unsigned ints.
      std::deque<unsigned int> lst;
      const unsigned int max_count = 2000;

      {
         strtk::binary::writer writer(buffer,buffer_size);
         writer.clear();

         for (unsigned int i = 0; i < max_count; lst.push_back(i++)) ;

         if (!writer(lst))
         {
            std::cout << "example04() - Failed to write list of 'unsigned int'" << std::endl;
            return false;
         }

         lst.clear();
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);

         if (!reader(lst))
         {
            std::cout << "example04() - Failed to read list of 'unsigned int'" << std::endl;
            return false;
         }

         for (unsigned int i = 0; i < max_count; ++i)
         {
            if (lst[i] != i)
            {
               std::cout << "example04() - 'unsigned int' failure at index: " << i << std::endl;
               return false;
            }
         }
      }
   }

   {
      // Write out and then read back in an array of floats.
      std::vector<float> lst;
      const unsigned int max_count = 2000;

      const float magic[] = {
                              111.111f, 333.333f, 555.555f,
                              777.777f, 135.531f, 357.753f
                            };

      const std::size_t magic_count = sizeof(magic) / sizeof(float);

      {
         strtk::binary::writer writer(buffer,buffer_size);

         for (std::size_t i = 0; i < max_count; ++i)
         {
            const float d = magic[i % magic_count] * i;
            lst.push_back(d);
         }

         if (!writer(lst))
         {
            std::cout << "example04() - Failed to write list of " << strtk::type_name(lst) << std::endl;
            return false;
         }

         lst.clear();
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);

         if (!reader(lst))
         {
            std::cout << "example04() - Failed to read list of " << strtk::type_name(lst) << std::endl;
            return false;
         }

         for (std::size_t i = 0; i < max_count; ++i)
         {
            const float f = magic[i % magic_count] * i;
            if (lst[i] != f)
            {
               std::cout << "example04() - 'float' failure at index: " << i
                         << " expected value: "                        << f << std::endl;
               return false;
            }
         }
      }
   }

   {
      // Write out and then read back in an array of doubles.
      std::list<double> lst;
      const unsigned int max_count = 1000;

      const double magic[] = {
                                111.111, 333.333, 555.555,
                                777.777, 135.531, 357.753
                             };

      const std::size_t magic_count = sizeof(magic) / sizeof(double);

      {
         strtk::binary::writer writer(buffer,buffer_size);
         writer.clear();
         for (std::size_t i = 0; i < max_count; ++i)
         {
            const double d = magic[i % magic_count] * i;
            lst.push_back(d);
         }

         if (!writer(lst))
         {
            std::cout << "example04() - Failed to write list of " << strtk::type_name(lst) << std::endl;
            return false;
         }

         lst.clear();
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);

         if (!reader(lst))
         {
            std::cout << "example04() - Failed to read list of " << strtk::type_name(lst) << std::endl;
            return false;
         }

         std::list<double>::iterator itr = lst.begin();
         for (std::size_t i = 0; i < max_count; ++i, ++itr)
         {
            const double d = magic[i % magic_count] * i;
            if (*itr != d)
            {
               std::cout << "example04() - 'double' failure at index: " << i
                         << " expected value: "                         << d << std::endl;
               return false;
            }
         }
      }
   }

   {
      // Write out and then read back in a set of int.
      std::set<int> lst;
      const int max_count = 10000;
      {
         strtk::binary::writer writer(buffer,buffer_size);
         writer.clear();
         for (int i = -(max_count / 2); i < (max_count / 2); ++i)
         {
            lst.insert(i);
         }

         if (!writer(lst))
         {
            std::cout << "example04() - Failed to write list of " << strtk::type_name(lst) << std::endl;
            return false;
         }

         lst.clear();
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);

         if (!reader(lst))
         {
            std::cout << "example04() - Failed to read list of " << strtk::type_name(lst) << std::endl;
            return false;
         }

         int i = -(max_count / 2);
         std::set<int>::iterator itr = lst.begin();

         while (lst.end() != itr)
         {
            if (i != *itr)
            {
               std::cout << "example04() - expected value: " << i
                         << " read value: " << *itr << std::endl;
               return false;

            }
            ++i;
            ++itr;
         }
      }
   }

   {
      // Write out and then read back in a series of elaborate std::pair based types.
      typedef std::pair<std::string,unsigned int> p1_t;
      typedef std::pair<p1_t,p1_t> p2_t;
      typedef std::pair<p2_t,p2_t> p3_t;
      typedef std::pair<p3_t,p3_t> p4_t;

      p1_t p1_out("abcxyz",123456789);
      p2_t p2_out(p1_out,p1_out);
      p3_t p3_out(p2_out,p2_out);
      p4_t p4_out(p3_out,p3_out);

      strtk::binary::writer writer(buffer,buffer_size);

      if (!writer(p1_out))
      {
         std::cout << "example04() - Failed to write type: " << strtk::type_name(p1_out) << std::endl;
         return false;
      }

      if (!writer(p2_out))
      {
         std::cout << "example04() - Failed to write type: " << strtk::type_name(p2_out) << std::endl;
         return false;
      }

      if (!writer(p3_out))
      {
         std::cout << "example04() - Failed to write type: " << strtk::type_name(p3_out) << std::endl;
         return false;
      }

      if (!writer(p4_out))
      {
         std::cout << "example04() - Failed to write type: " << strtk::type_name(p4_out) << std::endl;
         return false;
      }

      strtk::binary::reader reader(buffer,buffer_size);

      p1_t p1_in;
      p2_t p2_in;
      p3_t p3_in;
      p4_t p4_in;

      if (!reader(p1_in))
      {
         std::cout << "example04() - Failed to read type: " << strtk::type_name(p1_in) << std::endl;
         return false;
      }

      if (!reader(p2_in))
      {
         std::cout << "example04() - Failed to read type: " << strtk::type_name(p2_in) << std::endl;
         return false;
      }

      if (!reader(p3_in))
      {
         std::cout << "example04() - Failed to read type: " << strtk::type_name(p3_in) << std::endl;
         return false;
      }

      if (!reader(p4_in))
      {
         std::cout << "example04() - Failed to read type: " << strtk::type_name(p4_in) << std::endl;
         return false;
      }

      if (p1_in != p1_out)
      {
         std::cout << "example04() - Comparison between p1_in and p1_out failed." << std::endl;
         return false;
      }

      if (p2_in != p2_out)
      {
         std::cout << "example04() - Comparison between p2_in and p2_out failed." << std::endl;
         return false;
      }

      if (p3_in != p3_out)
      {
         std::cout << "example04() - Comparison between p3_in and p3_out failed." << std::endl;
         return false;
      }

      if (p4_in != p4_out)
      {
         std::cout << "example04() - Comparison between p4_in and p4_out failed." << std::endl;
         return false;
      }
   }

   {
      static const std::size_t max = 10;
      std::vector< std::pair<unsigned long long,person> > p_out_list;
      std::deque< std::pair<unsigned long long,person> > p_in_list;

      {
         person p;
         p.id        = 0LL;
         p.name      = "Mr. Rumpelstilzchen";
         p.age       = 123;
         p.height    = 123.456;
         p.weight    = 333.7777f;
         p.is_insane = false;

         for (std::size_t i = 0; i < max; ++i)
         {
            p_out_list.push_back(std::make_pair(i,p));
            p.id  += 1;
            p.age += 3;
            p.height += 1.1;
            p.weight += 2.2f;
         }

         strtk::binary::writer writer (buffer,buffer_size);

         if (!writer(p_out_list))
         {
            std::cout << "example04() - Failed to write type: " << strtk::type_name(p_out_list) << std::endl;
            return false;
         }
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);

         if (!reader(p_in_list))
         {
            std::cout << "example04() - Failed to read type: " << strtk::type_name(p_in_list) << std::endl;
            return false;
         }
      }

      if (p_out_list.size() != p_in_list.size())
      {
         std::cout << "example04() - Failure between sizes of p_out_list and p_in_list!" << std::endl;
         return false;
      }

      if (!std::equal(p_out_list.begin(),p_out_list.end(),p_in_list.begin()))
      {
         std::cout << "example04() - Failure in comparison between p_out_list and p_in_list!" << std::endl;
         return false;
      }
   }

   return true;
}

bool example05(char* buffer, const unsigned int buffer_size)
{
   {
      const std::size_t rounds = 100000;
      const std::size_t person_count = 1000;
      person p;
      p.id        = 12345678901234567890ULL;
      p.name      = "Mr. Rumpelstilzchen";
      p.age       = 637;
      p.height    = 123.4567;
      p.weight    = 765.345f;
      p.is_insane = true;

      {
         strtk::binary::writer writer(buffer,buffer_size);
         unsigned long long total_written = 0;

         strtk::util::timer t;
         t.start();

         for (std::size_t r = 0; r < rounds; ++r)
         {
            writer.reset();

            for (std::size_t i = 0; i < person_count; ++i)
            {
               if (!writer(p))
               {
                  std::cout << "example05() - Failed to write index " << i << " @ round " << r << std::endl;
                  return false;
               }
            }

            total_written += writer.amount_written();
         }

         t.stop();

         printf("[strtk::binary::writer] Person Count:%10llu  Total time:%8.4f  Rate:%18.4fpersons/s %9.3fMB/s\n",
                static_cast<unsigned long long>(rounds * person_count),
                t.time(),
                (rounds * person_count) / t.time(),
                total_written / (1048576.0 * t.time()));
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);
         unsigned long long total_read = 0;

         strtk::util::timer t;
         t.start();

         for (std::size_t r = 0; r < rounds; ++r)
         {
            reader.reset();

            for (std::size_t i = 0; i < person_count; ++i)
            {
               if (!reader(p))
               {
                  std::cout << "example05() - Failed to read index " << i << " @ round " << r << std::endl;
                  return false;
               }
            }

            total_read += reader.amount_read();
         }

         t.stop();

         printf("[strtk::binary::reader] Person Count:%10llu  Total time:%8.4f  Rate:%18.4fpersons/s %9.3fMB/s\n",
                static_cast<unsigned long long>(rounds * person_count),
                t.time(),
                (rounds * person_count) / t.time(),
                total_read / (1048576.0 * t.time()));
      }
   }

   {
      const std::size_t rounds = 10000;
      const std::size_t max_count = 160000;
      const double magic[] = {
                                111.111, 333.333, 555.555,
                                777.777, 135.531, 357.753
                             };
      const std::size_t magic_count = sizeof(magic) / sizeof(double);

      std::vector<double> dbl_list;
      dbl_list.reserve(max_count);

      for (std::size_t i = 0; i < max_count; ++i)
      {
         dbl_list.push_back(magic[i % magic_count] * i);
      }

      {
         strtk::binary::writer writer(buffer,buffer_size);
         unsigned long long total_written = 0;

         strtk::util::timer t;
         t.start();

         for (std::size_t r = 0; r < rounds; ++r)
         {
            writer.reset();

            if (!writer(dbl_list))
            {
               std::cout << "example05() - Failed to write " << strtk::type_name(dbl_list) << " @ round " << r << std::endl;
               return false;
            }

            total_written += writer.amount_written();
         }

         t.stop();

         printf("[strtk::binary::writer] Double Count:%10llu  Total time:%8.4f  Rate:%18.4fdoubles/s %9.3fMB/s\n",
                static_cast<unsigned long long>(rounds * max_count),
                t.time(),
                (rounds * max_count) / t.time(),
                total_written / (1048576.0 * t.time()));
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);
         unsigned long long total_read = 0;

         dbl_list.clear();

         strtk::util::timer t;
         t.start();

         for (std::size_t r = 0; r < rounds; ++r)
         {
            reader.reset();

            if (!reader(dbl_list))
            {
               std::cout << "example05() - Failed to read " << strtk::type_name(dbl_list) << " @ round " << r << std::endl;
               return false;
            }

            dbl_list.clear();
            total_read += reader.amount_read();

         }

         t.stop();

         printf("[strtk::binary::reader] Double Count:%10llu  Total time:%8.4f  Rate:%18.4fdoubles/s %9.3fMB/s\n",
                static_cast<unsigned long long>(rounds * max_count),
                t.time(),
                (rounds * max_count) / t.time(),
                total_read / (1048576.0 * t.time()));
      }
   }

   {
      const std::size_t rounds = 1000;
      const std::size_t size   = 10;

      std::string s = "0123456789abcdefghij";
      std::vector<std::string> str_list;
      str_list.reserve(200000);

      strtk::for_each_combination(s.begin(),s.end(),
                                  size,
                                  strtk::range_to_type_back_inserter(str_list));

      {
         strtk::binary::writer writer(buffer,buffer_size);
         writer.clear();

         unsigned long long total_written = 0;

         strtk::util::timer t;
         t.start();

         for (std::size_t r = 0; r < rounds; ++r)
         {
            writer.reset();

            if (!writer(str_list))
            {
               std::cout << "example05() - Failed to write string permutation " << strtk::type_name(str_list) << " @ round " << r << std::endl;
               return false;
            }

            total_written += writer.amount_written();

         }

         t.stop();

         printf("[strtk::binary::writer] String-Permutation Count:%10llu  Total time:%8.4f  Rate:%14.4fstr/s %8.3fMB/s\n",
                static_cast<unsigned long long>(rounds * str_list.size()),
                t.time(),
                (rounds * str_list.size()) / t.time(),
                total_written / (1048576.0 * t.time()));
      }

      {
         strtk::binary::reader reader(buffer,buffer_size);

         unsigned long long total_read = 0;

         strtk::util::timer t;
         t.start();

         for (std::size_t r = 0; r < rounds; ++r)
         {
            str_list.clear();
            reader.reset();

            if (!reader(str_list))
            {
               std::cout << "example05() - Failed to read string permutation " << strtk::type_name(str_list) << " @ round " << r << std::endl;
               return false;
            }

            total_read += reader.amount_read();
         }

         t.stop();

         printf("[strtk::binary::reader] String-Permutation Count:%10llu  Total time:%8.4f  Rate:%14.4fstr/s %8.3fMB/s\n",
                static_cast<unsigned long long>(rounds * str_list.size()),
                t.time(),
                (rounds * str_list.size()) / t.time(),
                total_read / (1048576.0 * t.time()));
      }
   }

   return true;
}

bool example06(char* buffer, const unsigned int buffer_size)
{
   std::string s = "abc123";

   {
      std::fill_n(buffer, buffer_size, static_cast<unsigned char>(0));
      strtk::binary::pascal_string ps;
      strtk::binary::short_string  ss;

      strtk::binary::writer writer(buffer,buffer_size);

      if (!writer(ps.set(s)))
      {
         std::cout << "example06() - Failed to write Pascal String " << s << std::endl;
         return false;
      }

      if (!writer(ss.set(s)))
      {
         std::cout << "example06() - Failed to write Short String " << s << std::endl;
         return false;
      }

      std::string r1; r1.clear();
      std::string r2; r2.clear();

      strtk::binary::reader reader(buffer,buffer_size);

      if (!reader(ps.set(r1)))
      {
         std::cout << "example06() - Failed to read Pascal String " << s << std::endl;
         return false;
      }

      if (!reader(ss.set(r2)))
      {
         std::cout << "example06() - Failed to read Short String " << s << std::endl;
         return false;
      }

      std::cout << "Pascal String: [" << r1 << "]" << std::endl;
      std::cout << "Short String: [" << r2 << "]" << std::endl;
   }

   {
      std::fill_n(buffer, buffer_size, static_cast<unsigned char>(0));
      strtk::binary::pascal_string ps;
      strtk::binary::short_string  ss;

      strtk::binary::writer writer(buffer,buffer_size);

      if (!writer(strtk::binary::pascal_string(s)))
      {
         std::cout << "example06() - Failed to write Pascal String " << s << std::endl;
         return false;
      }

      if (!writer(strtk::binary::short_string(s)))
      {
         std::cout << "example06() - Failed to write Short String " << s << std::endl;
         return false;
      }

      std::string r1; r1.clear();
      std::string r2; r2.clear();

      strtk::binary::reader reader(buffer,buffer_size);

      if (!reader(strtk::binary::pascal_string(r1)))
      {
         std::cout << "example06() - Failed to read Pascal String " << s << std::endl;
         return false;
      }

      if (!reader(strtk::binary::short_string(r2)))
      {
         std::cout << "example06() - Failed to read Short String " << s << std::endl;
         return false;
      }

      std::cout << "Pascal String: [" << r1 << "]" << std::endl;
      std::cout << "Short String: [" << r2 << "]" << std::endl;
   }

   return true;
}

bool example07(char* buffer, const unsigned int buffer_size)
{
   strtk::binary::writer writer(buffer,buffer_size);
   strtk::binary::reader reader(buffer,buffer_size);

   int i = 123;
   int j = 0;

   if (!writer(i,4,strtk::binary::writer::left_padding,'0'))
   {
      std::cout << "Failed to write padded 4 char int!" << std::endl;
      return false;
   }

   if (!reader(j,4))
   {
      std::cout << "Failed to read 4 char int!" << std::endl;
      return false;
   }

   if (i != j)
      return false;

   return true;
}

bool example08(char* buffer, const unsigned int buffer_size)
{
   {
      strtk::binary::writer writer(buffer,buffer_size);
      writer.reset(true);
      std::vector<int> v;
      std::deque<double> d;
      std::list<char> c;
      strtk::util::push_back(v,1,2,3,4,5);
      strtk::util::push_back(d,1.1,2.2,3.3,4.4,5.5);
      strtk::util::push_back(c,'A','B','C','D','E','F');
      writer(v);
      writer(d);
      writer(c);
   }

   {
      strtk::binary::reader reader(buffer,buffer_size);
      std::vector<int> v;
      std::deque<double> d;
      std::list<char> c;
      reader(v);
      reader(d);
      reader(c);
   }

   return true;
}

bool example09(char* buffer)
{
   char in_char = -17;
   unsigned char in_uchar = 200;
   short in_short = -20000;
   unsigned short in_ushort = 55555;
   int in_int = -1111111;
   unsigned int in_uint = 79797979;
   long in_long = -43294761;
   unsigned long in_ulong = 78292365;
   float in_float = 1234.5678f;
   double in_double = 9876.54321;
   long double in_ldouble = 456789.123456;

   char out_char = 0;
   unsigned char out_uchar = 0;
   short out_short = 0;
   unsigned short out_ushort = 0;
   int out_int = 0;
   unsigned int out_uint = 0;
   long out_long = 0;
   unsigned long out_ulong = 0;
   float out_float = 0.0f;
   double out_double = 0.0;
   long double out_ldouble = 0.0;

   unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);

   strtk::write_pod(ptr,
                    in_char,
                    in_uchar,
                    in_short,
                    in_ushort,
                    in_int,
                    in_uint,
                    in_long,
                    in_ulong,
                    in_float,
                    in_double,
                    in_ldouble);

   strtk::read_pod(ptr,
                   out_char,
                   out_uchar,
                   out_short,
                   out_ushort,
                   out_int,
                   out_uint,
                   out_long,
                   out_ulong,
                   out_float,
                   out_double,
                   out_ldouble);

   if (in_char    != out_char)    { std::cout << "example06() - Failed char"    << std::endl; return false; }
   if (in_uchar   != out_uchar)   { std::cout << "example06() - Failed uchar"   << std::endl; return false; }
   if (in_short   != out_short)   { std::cout << "example06() - Failed short"   << std::endl; return false; }
   if (in_ushort  != out_ushort)  { std::cout << "example06() - Failed ushort"  << std::endl; return false; }
   if (in_int     != out_int)     { std::cout << "example06() - Failed int"     << std::endl; return false; }
   if (in_uint    != out_uint)    { std::cout << "example06() - Failed uint"    << std::endl; return false; }
   if (in_long    != out_long)    { std::cout << "example06() - Failed long"    << std::endl; return false; }
   if (in_ulong   != out_ulong)   { std::cout << "example06() - Failed ulong"   << std::endl; return false; }
   if (in_float   != out_float)   { std::cout << "example06() - Failed float"   << std::endl; return false; }
   if (in_double  != out_double)  { std::cout << "example06() - Failed double"  << std::endl; return false; }
   if (in_ldouble != out_ldouble) { std::cout << "example06() - Failed ldouble" << std::endl; return false; }

   return true;
}

bool example10(char* buffer)
{
   const size_t size = 10;
   const int intlst[size] = { -1, 2, -3, 4, -5, 6, -7, 8, -9, 10 };
   const unsigned int uintlst[size] = {
                                        734594, 1375762, 5432543,     3454, 32132,
                                         65463,  976765, 2355754, 74239542, 32523
                                      };
   const float fltlst[size] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.10f };
   const double dbllst[size] = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10 };

   int r_intlst[size] = { 0 };
   unsigned int r_uintlst[size] = { 0 };
   float r_fltlst[size] = { 0.0f };
   double r_dbllst[size] = { 0.0 };

   unsigned char* ptr = reinterpret_cast<unsigned char*>(buffer);
   ptr = strtk::write_pod(ptr,intlst);
   ptr = strtk::write_pod(ptr,uintlst);
   ptr = strtk::write_pod(ptr,fltlst);
   ptr = strtk::write_pod(ptr,dbllst);

   ptr = reinterpret_cast<unsigned char*>(buffer);
   ptr = strtk::read_pod(ptr,r_intlst);
   ptr = strtk::read_pod(ptr,r_uintlst);
   ptr = strtk::read_pod(ptr,r_fltlst);
   ptr = strtk::read_pod(ptr,r_dbllst);

   if (!std::equal(intlst, intlst + size, r_intlst))
   {
      std::cout << "example07() - failed int list compare." << std::endl;
      return false;
   }

   if (!std::equal(uintlst, uintlst + size, r_uintlst))
   {
      std::cout << "example07() - failed unsigned int list compare." << std::endl;
      return false;
   }

   if (!std::equal(fltlst, fltlst + size, r_fltlst))
   {
      std::cout << "example07() - failed float list compare." << std::endl;
      return false;
   }

   if (!std::equal(dbllst, dbllst + size, r_dbllst))
   {
      std::cout << "example07() - failed double list compare." << std::endl;
      return false;
   }

   return true;
}

int main()
{
   static const std::size_t max_buffer_size = 10 * strtk::one_megabyte; // 10MB
   char* buffer = new char[max_buffer_size];

   example01(buffer,max_buffer_size);
   example02(buffer,max_buffer_size);
   example03(buffer,max_buffer_size);
   example04(buffer,max_buffer_size);
   example05(buffer,max_buffer_size);
   example06(buffer,max_buffer_size);
   example07(buffer,max_buffer_size);
   example08(buffer,max_buffer_size);
   example09(buffer);
   example10(buffer);

   delete[] buffer;

   return 0;
}
