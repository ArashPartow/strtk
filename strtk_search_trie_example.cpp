/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
 * Search Trie Example                                           *
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
   Description: This example demonstrates the various usage patterns
                of the prefix-trie data structure. It also performs
                a speed comparison between itself, STL map and set
                in the area of multiple string searchs.
*/


#include <cstddef>
#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <set>

#include "strtk.hpp"


void trie_example00()
{
   const std::size_t s_size = 3;
   std::string s[s_size] = { "a", "ab", "abc" };
   strtk::prefix_trie<std::size_t>::std_string trie;

   strtk::trie::insert(trie,s[0],static_cast<std::size_t>(0));
   strtk::trie::insert(trie,s[1],static_cast<std::size_t>(1));
   strtk::trie::insert(trie,s[2],static_cast<std::size_t>(2));

   for (std::size_t i = 0; i < s_size; ++i)
   {
      if (strtk::trie::find_prefix(trie,s[i]))
         std::cout << "Found Prefix: " << s[i] << std::endl;
   }

   for (std::size_t i = 0; i < s_size; ++i)
   {
      std::size_t v = 0;
      if (strtk::trie::find(trie,s[i],v))
         std::cout << "Found: (" << s[i] << "," << v << ")" << std::endl;
   }
}

void trie_example01()
{
   const std::size_t l_size = 4;
   unsigned int i_list[][l_size] =
                            {
                               {  1,  2,  3,  4 },
                               {  5,  6,  7,  8 },
                               {  9, 10, 11, 12 },
                               { 13, 14, 15, 16 }
                            };

   strtk::trie::prefix<unsigned int*,std::string> trie;

   trie.insert(i_list[0],i_list[0] + l_size, " 1,  2,  3,  4");
   trie.insert(i_list[1],i_list[1] + l_size, " 5,  6,  7,  8");
   trie.insert(i_list[2],i_list[2] + l_size, " 9, 10, 11, 12");
   trie.insert(i_list[3],i_list[3] + l_size, "13, 14, 15, 16");

   for (std::size_t i = 0; i < l_size; ++i)
   {
      if (trie.find_prefix(i_list[i],i_list[i] + l_size))
         std::cout << "Found Prefix: " << strtk::join(",",i_list[i], i_list[i] + l_size) << std::endl;
   }

   for (std::size_t i = 0; i < l_size; ++i)
   {
      std::string s = "";
      if (trie.find(i_list[i],i_list[i] + l_size,s))
         std::cout << "Found Prefix: " << strtk::join(",",i_list[i], i_list[i] + l_size) << " = " << s << std::endl;
   }
}

template<typename Allocator,
         template<typename,typename> class Sequence>
void create_string_list(Sequence<std::string,Allocator>& str_list)
{
   std::string s = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

   for (std::size_t j = 0; j < s.size(); ++j)
   {
      for (std::size_t i = 1; i <= s.size(); ++i)
      {
         for (std::size_t k = 0; k < s.size() - i; ++k)
         {
            str_list.push_back(s.substr(k,i));
         }
      }

      std::rotate(s.begin(),s.begin() + 1, s.end());
   }
}

bool trie_example02()
{
   static const std::size_t rounds = 100;
   std::deque<std::string> str_list;
   create_string_list(str_list);

   {
      strtk::prefix_trie<std::size_t>::std_string trie;

      for (std::size_t i = 0; i < str_list.size(); ++i)
      {
         strtk::trie::insert(trie,str_list[i],i);
      }

      std::size_t count = 0;

      strtk::util::timer t;
      t.start();

      for (std::size_t r = 0; r < rounds; ++r)
      {
         for (std::size_t i = 0; i < str_list.size(); ++i)
         {
            if (!strtk::trie::find_prefix(trie,str_list[i]))
            {
               std::cout << "Failed to find: " << str_list[i] << std::endl;
               return false;
            }

            ++count;
         }
      }

      t.stop();

      std::cout << strtk::text::left_align(14,' ',"[strtk::trie]") << "String Count: " << count << " Total Time: " << t.time() <<std::endl;
   }

   {
      std::map<std::string,std::size_t> m;

      for (std::size_t i = 0; i < str_list.size(); ++i)
      {
         m.insert(std::make_pair(str_list[i],i));
      }

      strtk::util::timer t;
      t.start();

      std::size_t count = 0;

      for (std::size_t r = 0; r < rounds; ++r)
      {
         for (std::size_t i = 0; i < str_list.size(); ++i)
         {
            if (m.end() == m.find(str_list[i]))
            {
               std::cout << "Failed to find: " << str_list[i] << std::endl;
               return false;
            }

            ++count;
         }
      }

      t.stop();

      std::cout << strtk::text::left_align(14,' ',"[std::map]") << "String Count: " << count << " Total Time: " << t.time() <<std::endl;
   }

   {
      std::set<std::string> s;

      for (std::size_t i = 0; i < str_list.size(); ++i)
      {
         s.insert(str_list[i]);
      }

      strtk::util::timer t;
      t.start();

      std::size_t count = 0;

      for (std::size_t r = 0; r < rounds; ++r)
      {
         for (std::size_t i = 0; i < str_list.size(); ++i)
         {
            if (s.end() == s.find(str_list[i]))
            {
               std::cout << "Failed to find: " << str_list[i] << std::endl;
               return false;
            }

            ++count;
         }
      }

      t.stop();

      std::cout << strtk::text::left_align(14,' ',"[std::set]") << "String Count: " << count << " Total Time: " << t.time() <<std::endl;
   }

   return true;
}

int main()
{
   trie_example00();
   trie_example01();
   trie_example02();
   return 0;
}
