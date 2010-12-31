/*
 *****************************************************************
 *                     String Toolkit Library                    *
 *                                                               *
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


#ifndef INCLUDE_STRTK_HPP
#define INCLUDE_STRTK_HPP


#include <cstddef>
#include <cctype>
#include <cstring>
#include <cerrno>
#include <cmath>
#include <iterator>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>

#ifndef strtk_no_tr1_or_boost
   #define strtk_enable_lexical_cast
   #define strtk_enable_random
   #define strtk_enable_regex
#endif

#ifdef strtk_enable_lexical_cast
   #include <boost/lexical_cast.hpp>
#endif

#ifdef strtk_enable_random
   // Requires definition of a TR1 compatible random library header
   //#include <random>
   #include <boost/random.hpp>
#endif

#ifdef strtk_enable_regex
   // Requires definition of a TR1 compatible regex library header
   //#include <regex>
   #include <boost/regex.hpp>
#endif


namespace strtk
{

   static const std::size_t one_kilobyte = 1024;
   static const std::size_t one_megabyte = 1024 * one_kilobyte;
   static const std::size_t one_gigabyte = 1024 * one_megabyte;
   static const std::size_t magic_seed   = 0xA5A5A5A5;

   template<typename Tokenizer, typename Function>
   inline std::size_t for_each_token(const std::string& buffer,
                                     Tokenizer& tokenizer,
                                     Function function)
   {
      std::size_t token_count = 0;
      tokenizer.assign(buffer);
      typename Tokenizer::iterator itr = tokenizer.begin();
      typename Tokenizer::const_iterator end = tokenizer.end();
      while (end != itr)
      {
         function(*itr);
         ++itr;
         ++token_count;
      }
      return token_count;
   }

   template<typename Function>
   inline std::size_t for_each_line(std::istream& stream,
                                    Function function,
                                    const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         function(buffer);
         ++line_count;
      }
      return line_count;
   }

   template<typename Function>
   inline std::size_t for_each_line_n(std::istream& stream,
                                      const std::size_t& n,
                                      Function function,
                                      const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         function(buffer);
         if (n == ++line_count)
          break;
      }
      return line_count;
   }

   template<typename Function>
   inline std::size_t for_each_line(const std::string& file_name,
                                    Function function,
                                    const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line(stream,function,buffer_size);
      else
         return 0;
   }

   template<typename Function>
   inline std::size_t for_each_line_n(const std::string& file_name,
                                      const std::size_t& n,
                                      Function function,
                                      const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line_n(stream,n,function,buffer_size);
      else
         return 0;
   }

   template<typename Function>
   inline std::size_t for_each_line_conditional(std::istream& stream,
                                                Function function,
                                                const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         if (!function(buffer))
         {
            return line_count;
         }
         ++line_count;
      }
      return line_count;
   }

   template<typename Function>
   inline std::size_t for_each_line_n_conditional(std::istream& stream,
                                                  const std::size_t& n,
                                                  Function function,
                                                  const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      buffer.reserve(buffer_size);
      std::size_t line_count = 0;
      while (std::getline(stream,buffer))
      {
         if (!function(buffer))
         {
            return line_count;
         }
         if (n == ++line_count)
          break;
      }
      return line_count;
   }

   template<typename Function>
   inline std::size_t for_each_line_conditional(const std::string& file_name,
                                                Function function,
                                                const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line_conditional(stream,function,buffer_size);
      else
         return 0;
   }

   template<typename Function>
   inline std::size_t for_each_line_n_conditional(const std::string& file_name,
                                                  const std::size_t& n,
                                                  Function function,
                                                  const std::size_t& buffer_size = one_kilobyte)
   {
      std::ifstream stream(file_name.c_str());
      if (stream)
         return for_each_line_n_conditional(stream,n,function,buffer_size);
      else
         return 0;
   }

   template<typename T>
   inline bool read_line_as_value(std::istream& stream,
                                   T& t,
                                  const std::size_t& buffer_size = one_kilobyte)
   {
      std::string buffer;
      buffer.reserve(buffer_size);
      if (!std::getline(stream,buffer))
         return false;
      return string_to_type_converter(buffer,t);
   }

   namespace details
   {
      struct not_supported_type_tag {};
      struct unsigned_type_tag {};
      struct signed_type_tag {};
      struct real_type_tag {};
      struct byte_type_tag {};
      struct bool_type_tag {};
      struct hex_type_tag {};
      struct base64_type_tag {};
      struct ignore_token_type_tag {};
      struct stdstring_type_tag{};
      struct stdstring_range_type_tag{};
      struct sink_type_tag {};

      template<typename T>
      struct supported_conversion_to_type
      {
         typedef not_supported_type_tag type;
      };

      template<typename T>
      struct supported_conversion_from_type
      {
         typedef not_supported_type_tag type;
      };

      template<bool, typename T = void>
      struct enable_if {};

      template<typename T>
      struct enable_if<true, T> { typedef T type; };

      template<typename T>
      struct supported_iterator_type
      {
         enum { value = false };
      };

      template<typename T>
      struct is_valid_iterator
      {
         typedef typename details::enable_if<details::supported_iterator_type<T>::value,T>::type type;
      };

      template<typename T> struct numeric;
      template<typename T> inline std::size_t type_length(const T&);

      struct no_t {};
      struct yes_t {};
      template<typename T> struct is_pod
      {
         typedef no_t result_t;
         enum { result = false };
      };

   } // namespace details

   template<typename Iterator, typename T>
   inline bool string_to_type_converter(const Iterator begin, const Iterator end, T& t)
   {
      typedef typename details::is_valid_iterator<Iterator>::type itr_type;
      typename details::supported_conversion_to_type<T>::type type;
      Iterator itr = begin;
      return string_to_type_converter_impl(itr,end,t,type);
   }

   template<typename T, typename Iterator>
   inline T string_to_type_converter(const Iterator begin, const Iterator end)
   {
      typedef typename details::is_valid_iterator<Iterator>::type itr_type;
      typename details::supported_conversion_to_type<T>::type type;
      T t;
      Iterator itr = begin;
      if (string_to_type_converter_impl(itr,end,t,type))
         return t;
      else
         throw;
   }

   template<typename T>
   inline bool string_to_type_converter(const std::string& s, T& t)
   {
      return string_to_type_converter<const char*,T>(s.data(),s.data() + s.size(),t);
   }

   template<typename T>
   inline T string_to_type_converter(const std::string& s)
   {
      return string_to_type_converter<T>(s.data(),s.data() + s.size());
   }

   template<typename T>
   inline bool type_to_string(const T& t, std::string& s)
   {
      typename details::supported_conversion_from_type<T>::type type;
      return type_to_string_converter_impl(t,s,type);
   }

   template<typename T>
   inline std::string type_to_string(const T& t)
   {
      std::string s;
      if (type_to_string<T>(t,s))
         return s;
      else
         throw;
   }

   #define strtk_begin_register_string_to_type \
   namespace strtk { namespace details {

   #define strtk_end_register_string_to_type \
   }}

   #define strtk_string_to_type_begin(Type) \
   namespace strtk { namespace details { template<typename Iterator> \
   inline bool string_to_type_converter_impl(const Iterator& begin, const Iterator& end, \
                                             Type& t, details::not_supported_type_tag&) {

   #define strtk_string_to_type_end()\
   }}}

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          Sequence<T,Allocator>& sequence,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     if (!stream) return 0;
     std::string buffer;
     buffer.reserve(buffer_size);
     std::size_t line_count = 0;
     while (std::getline(stream,buffer))
     {
        ++line_count;
        sequence.push_back(string_to_type_converter<T>(buffer));
     }
     return line_count;
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::set<T,Comparator,Allocator>& set,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     if (!stream) return 0;
     std::string buffer;
     buffer.reserve(buffer_size);
     std::size_t line_count = 0;
     while (std::getline(stream,buffer))
     {
        ++line_count;
        set.insert(string_to_type_converter<T>(buffer));
     }
     return line_count;
   }

   template <typename T, typename Container>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::queue<T,Container>& queue,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     if (!stream) return 0;
     std::string buffer;
     buffer.reserve(buffer_size);
     std::size_t line_count = 0;
     while (std::getline(stream,buffer))
     {
        ++line_count;
        queue.push(string_to_type_converter<T>(buffer));
     }
     return line_count;
   }

   template <typename T, typename Container>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::stack<T,Container>& stack,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     if (!stream) return 0;
     std::string buffer;
     buffer.reserve(buffer_size);
     std::size_t line_count = 0;
     while (std::getline(stream,buffer))
     {
        ++line_count;
        stack.push(string_to_type_converter<T>(buffer));
     }
     return line_count;
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t load_from_text_file(std::istream& stream,
                                          std::priority_queue<T,Container,Comparator>& priority_queue,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     if (!stream) return 0;
     std::string buffer;
     buffer.reserve(buffer_size);
     std::size_t line_count = 0;
     while (std::getline(stream,buffer))
     {
        ++line_count;
        priority_queue.push(string_to_type_converter<T>(buffer));
     }
     return line_count;
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          Sequence<T,Allocator>& sequence,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     std::ifstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return load_from_text_file(stream,sequence,buffer_size);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::set<T,Comparator,Allocator>& set,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     std::ifstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return load_from_text_file(stream,set,buffer_size);
   }

   template <typename T, typename Container>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::queue<T,Container>& queue,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     std::ifstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return load_from_text_file(stream,queue,buffer_size);
   }

   template <typename T, typename Container>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::stack<T,Container>& stack,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     std::ifstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return load_from_text_file(stream,stack,buffer_size);
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t load_from_text_file(const std::string& file_name,
                                          std::priority_queue<T,Container,Comparator>& priority_queue,
                                          const std::size_t& buffer_size = one_kilobyte)
   {
     std::ifstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return load_from_text_file(stream,priority_queue,buffer_size);
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t write_to_text_file(std::ostream& stream,
                                         const Sequence<T,Allocator>& sequence,
                                         const std::string& delimiter = "")
   {
     if (!stream) return 0;

     std::size_t count = 0;
     typename Sequence<T,Allocator>::const_iterator itr = sequence.begin();
     typename Sequence<T,Allocator>::const_iterator end = sequence.end();

     if (!delimiter.empty())
     {
        while (end != itr)
        {
           stream << *itr << delimiter;
           ++itr;
           ++count;
        }
     }
     else
     {
        while (end != itr)
        {
           stream << *itr;
           ++itr;
           ++count;
        }
     }

     return count;
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t write_to_text_file(std::ostream& stream,
                                         const std::set<T,Comparator,Allocator>& set,
                                         const std::string& delimiter = "")
   {
     if (!stream) return 0;

     std::size_t count = 0;
     typename std::set<T,Comparator,Allocator>::const_iterator itr = set.begin();
     typename std::set<T,Comparator,Allocator>::const_iterator end = set.end();

     if (!delimiter.empty())
     {
        while (end != itr)
        {
           stream << *itr << delimiter;
           ++itr;
           ++count;
        }
     }
     else
     {
        while (end != itr)
        {
           stream << *itr;
           ++itr;
           ++count;
        }
     }

     return count;
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t write_to_text_file(const std::string& file_name,
                                         const Sequence<T,Allocator>& sequence,
                                         const std::string& delimiter = "")
   {
     std::ofstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return write_to_text_file(stream,sequence,delimiter);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t write_to_text_file(const std::string& file_name,
                                         const std::set<T,Comparator,Allocator>& set,
                                         const std::string& delimiter = "")
   {
     std::ofstream stream(file_name.c_str());
     if (!stream)
        return 0;
     else
        return write_to_text_file(stream,set,delimiter);
   }

   template<typename Predicate,
            typename InputIterator,
            typename OutputIterator>
   inline void copy_if(Predicate predicate,
                       const InputIterator begin, const InputIterator end,
                       OutputIterator out)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         if (predicate(*itr))
         {
            *(out++) = *itr;
         }
         ++itr;
      }
   }

   template<typename Predicate,
            typename InputIterator,
            typename OutputIterator>
   inline InputIterator copy_while(Predicate predicate,
                                   const InputIterator begin, const InputIterator end,
                                   OutputIterator out)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         if (!predicate(*itr))
            return itr;
         *(out++) = *(itr++);
      }
      return end;
   }

   template<typename Predicate,
            typename InputIterator,
            typename OutputIterator>
   inline InputIterator copy_until(Predicate predicate,
                                   const InputIterator begin, const InputIterator end,
                                   OutputIterator out)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         if (predicate(*itr))
            return itr;
         *(out++) = *(itr++);
      }

      return end;
   }

   template<typename InputIterator, typename OutputIterator>
   inline void extract_unique(const InputIterator begin, const InputIterator end,
                              OutputIterator out)
   {
      typedef typename std::iterator_traits<InputIterator>::value_type T;
      std::vector<T> buffer(begin,end);
      std::sort(buffer.begin(),buffer.end());
      std::unique_copy(buffer.begin(),buffer.end(),out);
   }

   template<typename Predicate, typename InputIterator>
   inline bool range_only_contains(Predicate predicate,
                                   const InputIterator begin,
                                   const InputIterator end)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         if (!predicate(*itr))
         {
            return false;
         }
         ++itr;
      }
      return true;
   }

   namespace range
   {
      template<typename T>
      class adapter
      {
      public:

         typedef T value_type;
         typedef T* iterator;
         typedef const iterator const_iterator;

         adapter(T* const begin, T* const end)
         : begin_(begin),
           end_(end)
         {}

         adapter(T* const begin, const std::size_t length)
         : begin_(begin),
           end_(begin_ + length)
         {}

         inline iterator begin() const
         {
            return begin_;
         }

         inline iterator end() const
         {
            return end_;
         }

         inline std::size_t size() const
         {
            return std::distance(begin_,end_);
         }

      private:

         iterator begin_;
         iterator end_;
      };

      template<typename T>
      inline adapter<T> type(const T* begin, const T* end)
      {
         return adapter<T>(begin,end);
      }

      template<typename T, std::size_t N>
      inline adapter<T> type(const T (&t)[N])
      {
         return adapter<T>(t,N);
      }

      static inline adapter<const char> type(const std::string& s)
      {
         return adapter<const char>(s.c_str(),s.size());
      }

      template<typename T,
               typename Allocator,
               template<typename,typename> class Sequence>
      inline adapter<typename Sequence<T,Allocator>::iterator> type(const Sequence<T,Allocator>& seq)
      {
         return adapter<typename Sequence<T,Allocator>::iterator>(seq.begin(),seq.end());
      }

   } // namespace range

   template<typename T>
   struct single_delimiter_predicate
   {
   public:

      typedef T value_type;

      single_delimiter_predicate(const T& d)
      : delimiter_(d)
      {}

      inline bool operator()(const T& d) const
      {
         return d == delimiter_;
      }

   private:

      single_delimiter_predicate<T> operator=(const single_delimiter_predicate<T>&);

      const T delimiter_;
   };

   template<typename T>
   struct multiple_delimiter_predicate
   {
   public:

      typedef T value_type;

      multiple_delimiter_predicate(const T* d_begin, const T* d_end)
      : length_(std::distance(d_begin,d_end)),
        delimiter_(new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         std::copy(d_begin,d_end, delimiter_);
      }

      multiple_delimiter_predicate(const T d[], const std::size_t& length)
      : length_(length),
        delimiter_(new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         std::copy(d,d + length, delimiter_);
      }

      template<typename Iterator>
      multiple_delimiter_predicate(const Iterator begin, const Iterator end)
      : length_(std::distance(begin,end)),
        delimiter_(new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         //static_assert(T == std::iterator_traits<Iterator>::value_type);
         std::copy(begin,end, delimiter_);
      }

      template<typename Type>
      multiple_delimiter_predicate(const range::adapter<Type>& r)
      : length_(std::distance(r.begin(),r.end())),
        delimiter_(new T[length_]),
        delimiter_end_(delimiter_ + length_)
      {
         //static_assert(T == std::iterator_traits<Iterator>::value_type);
         std::copy(r.begin(),r.end(), delimiter_);
      }

     ~multiple_delimiter_predicate()
      {
        delete[] delimiter_;
      }

      inline bool operator()(const T& d) const
      {
         return (std::find(delimiter_,delimiter_end_,d) != delimiter_end_);
      }

   private:

      multiple_delimiter_predicate(const multiple_delimiter_predicate<T>& mdp);
      multiple_delimiter_predicate operator=(const multiple_delimiter_predicate<T>& mdp);

   private:

      std::size_t length_;
      T* delimiter_;
      T* delimiter_end_;
   };

   struct multiple_char_delimiter_predicate
   {
   public:

      template<typename Iterator>
      multiple_char_delimiter_predicate(const Iterator begin, const Iterator end)
      {
         setup_delimiter_table(begin,end);
      }

      multiple_char_delimiter_predicate(const std::string& s)
      {
         setup_delimiter_table(s.begin(),s.end());
      }

      inline bool operator()(const unsigned char& c) const
      {
         return (delimiter_table_[c]);
      }

      inline bool operator()(const char& c) const
      {
         return operator()(static_cast<unsigned char>(c));
      }

   private:

      static const std::size_t table_size = 256;

      template<typename Iterator>
      inline void setup_delimiter_table(const Iterator begin, const Iterator end)
      {
         std::fill_n(delimiter_table_,table_size,false);
         for (Iterator itr = begin; itr != end; ++itr)
         {
            delimiter_table_[static_cast<unsigned char>(*itr)] = true;
         }
      }

      bool delimiter_table_[table_size];
   };

   template<typename Iterator, typename Predicate>
   inline std::size_t remove_inplace(Predicate predicate,
                                     Iterator begin,
                                     Iterator end)
   {
      Iterator itr1 = begin;
      Iterator itr2 = begin;
      std::size_t removal_count = 0;
      while (itr1 != end)
      {
         while ((end != itr1) && !predicate(*itr1))
         {
            if (itr1 != itr2)
            {
               *itr2 = *itr1;
            }
            ++itr1;
            ++itr2;
         }

         while ((end != itr1) && predicate(*itr1))
         {
            ++itr1;
            ++removal_count;
         }
      }
      return removal_count;
   }

   template<typename T, typename Predicate>
   inline std::size_t remove_inplace(Predicate predicate, const range::adapter<T>& r)
   {
      return remove_inplace(predicate,r.begin(),r.end());
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline void remove_inplace(Sequence<T,Allocator>& sequence)
   {
      const std::size_t removal_count = remove_inplace(sequence.begin(),sequence.end());
      sequence.resize(sequence.size() - removal_count);
   }

   inline void remove_inplace(const std::string::value_type c, std::string& s)
   {
      const std::size_t removal_count = remove_inplace(single_delimiter_predicate<std::string::value_type>(c),
                                                       s.begin(),
                                                       s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Predicate>
   inline void remove_inplace(Predicate predicate, std::string& s)
   {
      const std::size_t removal_count = remove_inplace(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator, typename Predicate>
   inline std::size_t remove_consecutives_inplace(Predicate predicate,
                                                  Iterator begin,
                                                  Iterator end)
   {
      if (0 == std::distance(begin,end)) return 0;
      Iterator itr1 = begin; ++itr1;
      Iterator itr2 = begin; ++itr2;
      typename std::iterator_traits<Iterator>::value_type prev = *begin;
      std::size_t removal_count = 0;
      while (itr1 != end)
      {
         while ((end != itr1) && (!predicate(*itr1) || !predicate(prev)))
         {
            if (itr1 != itr2)
            {
               *itr2 = *itr1;
            }

            prev = *itr1;
            ++itr1;
            ++itr2;
         }

         while ((end != itr1) && predicate(*itr1))
         {
            ++itr1;
            ++removal_count;
         }
      }

      return removal_count;
   }

   template<typename T, typename Predicate>
   inline std::size_t remove_consecutives_inplace(Predicate predicate, const range::adapter<T>& r)
   {
      return remove_consecutives_inplace(predicate,r.begin(),r.end());
   }

   inline void remove_consecutives_inplace(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_consecutives_inplace(single_delimiter_predicate<std::string::value_type>(c),
                                                                    s.begin(),
                                                                    s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_consecutives_inplace(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_consecutives_inplace(multiple_char_delimiter_predicate(rem_chars),
                                                                    s.begin(),
                                                                    s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   namespace details
   {
      inline std::size_t strnlen(const char* s, const std::size_t& n)
      {
         // Not the most optimal implementation. Waiting for
         // strn* extensions to become mandatory.
         const char* itr = s;
         for (std::size_t i = 0; i < n; ++i, ++itr)
         {
            if ('\0' == (*itr)) return i;
         }
         return n;
      }

      inline std::size_t strnlen(const unsigned char* s, std::size_t n)
      {
         return strnlen(reinterpret_cast<const char*>(s),n);
      }
   }

   inline void remove_consecutives_inplace(const char* rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_consecutives_inplace(multiple_char_delimiter_predicate(
                                                                    rem_chars,
                                                                    rem_chars + details::strnlen(rem_chars,256)),
                                                                    s.begin(),
                                                                    s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Predicate>
   inline void remove_consecutives_inplace(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_consecutives_inplace(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator>
   inline std::size_t remove_consecutives_inplace(Iterator begin, Iterator end)
   {
      if (0 == std::distance(begin,end)) return 0;

      Iterator itr1 = begin; ++itr1;
      Iterator itr2 = begin; ++itr2;
      typename std::iterator_traits<Iterator>::value_type prev = *begin;
      std::size_t removal_count = 0;

      while (itr1 != end)
      {
         while ((end != itr1) && (prev != (*itr1)))
         {
            if (itr1 != itr2)
            {
               *itr2 = *itr1;
            }
            prev = *itr1;
            ++itr1;
            ++itr2;
         }

         while ((end != itr1) && (prev == (*itr1)))
         {
            ++itr1;
            ++removal_count;
         }
      }

      return removal_count;
   }

   template<typename T>
   inline std::size_t remove_consecutives_inplace(const range::adapter<T>& r)
   {
      return remove_consecutives_inplace(r.begin(),r.end());
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline void remove_consecutives_inplace(Sequence<T,Allocator>& sequence)
   {
      const std::size_t removal_count = remove_consecutives_inplace(sequence.begin(),sequence.end());
      sequence.resize(sequence.size() - removal_count);
   }

   inline void remove_consecutives_inplace(std::string& s)
   {
      std::size_t removal_count = remove_consecutives_inplace(s.begin(),s.end());

      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator, typename Predicate>
   inline std::size_t remove_trailing(Predicate predicate,
                                      Iterator begin,
                                      Iterator end)
   {
      const std::size_t length = std::distance(begin,end);
      if (0 == length)
         return 0;
      Iterator itr = begin + (length - 1);
      std::size_t removal_count = 0;

      while ((begin != itr) && predicate(*itr))
      {
         --itr;
         ++removal_count;
      }

      return removal_count;
   }

   template<typename T, typename Predicate>
   inline std::size_t remove_trailing(Predicate predicate, const range::adapter<T>& r)
   {
      return remove_trailing(predicate,r.begin(),r.end());
   }

   inline void remove_trailing(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_trailing(single_delimiter_predicate<std::string::value_type>(c),
                                                        s.begin(),
                                                        s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_trailing(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_trailing(multiple_char_delimiter_predicate(rem_chars),
                                                        s.begin(),
                                                        s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_trailing(const char* rem_chars, std::string& s)
   {
      const std::size_t removal_count = remove_trailing(multiple_char_delimiter_predicate(
                                                        rem_chars,
                                                        rem_chars + details::strnlen(rem_chars,256)),
                                                        s.begin(),
                                                        s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Predicate>
   inline void remove_trailing(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_trailing(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator, typename Predicate>
   inline std::size_t remove_leading(Predicate predicate,
                                     Iterator begin,
                                     Iterator end)
   {
      const std::size_t length = std::distance(begin,end);
      if (0 == length)
         return 0;
      Iterator itr = begin;
      std::size_t removal_count = 0;

      while ((end != itr) && predicate(*itr))
      {
         ++itr;
         ++removal_count;
      }

      std::copy(itr,end,begin);
      return removal_count;
   }

   template<typename T, typename Predicate>
   inline std::size_t remove_leading(Predicate predicate, const range::adapter<T>& r)
   {
      return remove_leading(predicate,r.begin(),r.end());
   }

   inline void remove_leading(const std::string::value_type c, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_leading(single_delimiter_predicate<std::string::value_type>(c),
                                                       s.begin(),
                                                       s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_leading(const std::string& rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_leading(multiple_char_delimiter_predicate(rem_chars),
                                                       s.begin(),
                                                       s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   inline void remove_leading(const char* rem_chars, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_leading(multiple_char_delimiter_predicate(
                                                       rem_chars,
                                                       rem_chars + details::strnlen(rem_chars,256)),
                                                       s.begin(),
                                                       s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Predicate>
   inline void remove_leading(Predicate predicate, std::string& s)
   {
      if (s.empty()) return;
      const std::size_t removal_count = remove_leading(predicate,s.begin(),s.end());
      if (removal_count > 0)
      {
         s.resize(s.size() - removal_count);
      }
   }

   template<typename Iterator>
   inline void replace(const typename std::iterator_traits<Iterator>::value_type& c1,
                       const typename std::iterator_traits<Iterator>::value_type& c2,
                       const Iterator begin,
                       const Iterator end)
   {
      for (Iterator itr = begin; end != itr; ++itr)
      {
         if (c1 == *itr) *itr = c2;
      }
   }

   template<typename T>
   inline void replace(const T& c1, const T& c2, const range::adapter<T>& r)
   {
      replace(c1,c2,r.begin(),r.end());
   }

   inline void replace_pattern(const std::string& s, // input
                               const std::string& p, // pattern
                               const std::string& r, // replacement
                               std::string& n)
   {
      if (p.empty() || (p == r))
      {
         n.assign(s);
         return;
      }

      const std::size_t p_size = p.size();
      const std::size_t r_size = r.size();
      int inc = static_cast<int>(r_size) - static_cast<int>(p_size);
      std::size_t pos = 0;
      std::vector<std::size_t> delta_list;
      delta_list.reserve(std::min<std::size_t>(32,(s.size() / p_size) + 1));

      while (std::string::npos != (pos = s.find(p,pos)))
      {
         delta_list.push_back(pos);
         pos += p_size;
      }

      if (delta_list.empty())
      {
         n.assign(s);
         return;
      }

      n.resize(delta_list.size() * inc + s.size(), 0x00);
      std::string::const_iterator s_itr = s.begin();
      std::string::iterator n_itr = n.begin();
      const std::size_t delta_list_size = delta_list.size();
      std::size_t i = 0;
      std::size_t delta = delta_list[0];

      for ( ; ; )
      {
         std::copy(s_itr, s_itr + delta, n_itr);
         s_itr += p_size + delta;
         n_itr += delta;
         std::copy(r.begin(), r.end(), n_itr);
         n_itr += r_size;
         if (++i >= delta_list_size)
            break;
         delta =  delta_list[i] - (delta_list[i - 1] + p_size);
      }

      if (s.end() != s_itr)
      {
         std::copy(s_itr, s.end(), n_itr);
      }
   }

   template<typename InputIterator, typename OutputIterator>
   std::size_t replace_pattern(const InputIterator s_begin, const InputIterator s_end, // input
                               const InputIterator p_begin, const InputIterator p_end, // pattern
                               const InputIterator r_begin, const InputIterator r_end, // replacement
                               OutputIterator out)
   {
      typedef typename std::iterator_traits<InputIterator>::value_type T;

      InputIterator s_itr  = s_begin;
      InputIterator r_itr  = r_begin;
      InputIterator p_itr  = p_begin;

      const std::size_t p_size = std::distance(p_begin,p_end);
      const std::size_t r_size = std::distance(r_begin,r_end);

      if ((0 == p_size) || ((p_size == r_size) && std::equal(p_begin,p_end,r_begin)))
      {
         std::copy(s_begin,s_end,out);
         return std::distance(s_begin,s_end);
      }

      std::size_t pos = 0;
      std::size_t prev_pos = 0;
      std::size_t count = 0;
      std::size_t new_size = std::distance(s_begin,s_end);
      int inc = r_size - p_size;

      InputIterator temp_s_itr = s_itr;

      while (s_end != s_itr)
      {
         /*
            Need to replace the following search code with
            Knuth-Pratt-Morris or Boyer-Moore string search
            algorithms.
         */
         bool found = true;
         p_itr = p_begin;
         temp_s_itr = s_itr;

         while ((p_end != p_itr) && (s_end != temp_s_itr))
         {
            if (*(temp_s_itr++) != *(p_itr++))
            {
               found = false;
               break;
            }
         }

         if (found && (p_itr == p_end))
         {
            ++count;
            new_size += inc;
            s_itr = temp_s_itr;
         }
         else
            ++s_itr;
      }

      s_itr = s_begin;
      p_itr = p_begin;

      pos = 0;
      prev_pos = 0;

      temp_s_itr = s_itr;

      while (0 < count)
      {
         p_itr = p_begin;
         bool found = true;
         InputIterator pattern_start = temp_s_itr;

         while ((p_end != p_itr) && (s_end != temp_s_itr))
         {
            if (*(temp_s_itr++) != *(p_itr++))
            {
               found = false;
               temp_s_itr = pattern_start;
               ++temp_s_itr;
               break;
            }
         }

         if (!found || (p_itr != p_end)) continue;

         pos = std::distance(s_begin,temp_s_itr) - p_size;
         int diff = pos - prev_pos;

         std::copy(s_itr,s_itr + diff, out);
         s_itr = temp_s_itr;
         std::copy(r_itr,r_end, out);

         pos += p_size;
         prev_pos = pos;
         --count;
      }

      std::copy(s_itr,s_end,out);

      return new_size;
   }

   inline void remove_pattern(const std::string& s,
                              const std::string& p,
                              std::string& n)
   {
      static const std::string r("");
      replace_pattern(s,p,r,n);
   }

   template<typename Iterator>
   inline bool match(const Iterator pattern_begin,
                     const Iterator pattern_end,
                     const Iterator data_begin,
                     const Iterator data_end,
                     const typename std::iterator_traits<Iterator>::value_type& zero_or_more,
                     const typename std::iterator_traits<Iterator>::value_type& zero_or_one)
   {
      /*
         Credits: Adapted from code by Jack Handy (2001)
      */
      if (0 == std::distance(data_begin,data_end)) return false;

      Iterator d_itr = data_begin;
      Iterator p_itr = pattern_begin;
      Iterator c_itr = data_begin;
      Iterator m_itr = data_begin;

      while ((data_end != d_itr) && (zero_or_more != (*p_itr)))
      {
         if (((*p_itr) != (*d_itr)) && (zero_or_one != (*p_itr)))
         {
            return false;
         }
         ++p_itr;
         ++d_itr;
      }

      while (data_end != d_itr)
      {
         if (zero_or_more == (*p_itr))
         {
            if (pattern_end == (++p_itr))
            {
               return true;
            }
            m_itr = p_itr;
            c_itr = d_itr;
            ++c_itr;
         }
         else if (((*p_itr) == (*d_itr)) || (zero_or_one == (*p_itr)))
         {
            ++p_itr;
            ++d_itr;
         }
         else
         {
            p_itr = m_itr;
            d_itr = c_itr++;
         }
      }

      while ((p_itr != pattern_end) && (zero_or_more == (*p_itr))) ++p_itr;

      return (p_itr == pattern_end);
   }

   inline bool match(const std::string& wild_card,
                     const std::string& str)
   {
      /*
         * : Zero or more match
         ? : Zero or one match
      */
      return match(wild_card.data(),wild_card.data() + wild_card.size(),
                   str.data(),str.data() + str.size(),
                   '*',
                   '?');
   }

   inline bool imatch_char(const char c1, const char c2)
   {
      return std::toupper(c1) == std::toupper(c2);
   }

   template<typename InputIterator>
   inline bool imatch(const InputIterator begin1, const InputIterator end1,
                      const InputIterator begin2, const InputIterator end2)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return false;
      }

      InputIterator itr1 = begin1;
      InputIterator itr2 = begin2;

      while (end1 != itr1)
      {
         //if (std::toupper(*itr1, std::locale::classic()) != std::toupper(*it2, std::locale::classic()))
         if (std::toupper(*itr1) != std::toupper(*itr2))
         {
            return false;
         }
         ++itr1;
         ++itr2;
      }

      return true;
   }

   template<typename T>
   inline bool imatch(const range::adapter<T>& r1, const range::adapter<T>& r2)
   {
      return imatch(r1.begin(),r1.end(),r2.begin(),r2.end());
   }

   inline bool imatch(const std::string& s1, const std::string s2)
   {
      return imatch(s1.begin(),s1.end(),s2.begin(),s2.end());
   }

   template<typename Iterator>
   inline bool imatch(const std::string& s, const Iterator begin, const Iterator end)
   {
      for (const std::string* itr = begin; end != itr; ++itr)
      {
         if (imatch(s,*itr))
         {
            return true;
         }
      }
      return false;
   }

   template<typename Allocator,
            template<typename,typename> class Sequence>
   inline bool imatch(const std::string& s, const Sequence<std::string,Allocator>& sequence)
   {
      return imatch(s,sequence.begin(),sequence.end());
   }

   template<typename Comparator, typename Allocator>
   inline bool imatch(const std::string& s, const std::set<std::string,Comparator,Allocator>& set)
   {
      return imatch(s,set.begin(),set.end());
   }

   template<typename Iterator, typename OutputIterator>
   inline std::size_t find_all(const Iterator pattern_begin,
                               const Iterator pattern_end,
                               const Iterator begin,
                               const Iterator end,
                               OutputIterator out)
   {
      Iterator itr = begin;
      const std::size_t pattern_length = std::distance(pattern_begin,pattern_end);
      std::size_t match_count = 0;
      while (end != (itr = std::search(itr, end, pattern_begin, pattern_end)))
      {
         *out++ = std::make_pair(itr,itr + pattern_length);
         itr += pattern_length;
         ++match_count;
      }
      return match_count;
   }

   template<typename Iterator,
            typename Range,
            typename Allocator,
            template<typename, typename> class Sequence>
   inline std::size_t find_all(const Iterator pattern_begin,
                               const Iterator pattern_end,
                               const Iterator begin,
                               const Iterator end,
                               Sequence<Range,Allocator>& seq)
   {
      return find_all(pattern_begin,pattern_end,begin,end,std::back_inserter(seq));
   }

   template<typename Iterator, typename OutputIterator>
   inline std::size_t ifind_all(const Iterator pattern_begin,
                                const Iterator pattern_end,
                                const Iterator begin,
                                const Iterator end,
                                OutputIterator out)
   {
      Iterator itr = begin;
      const std::size_t pattern_length = std::distance(pattern_begin,pattern_end);
      std::size_t match_count = 0;

      while (end != (itr = std::search(itr, end, pattern_begin, pattern_end, imatch_char)))
      {
         *out++ = std::make_pair(itr,itr + pattern_length);
         itr += pattern_length;
         ++match_count;
      }

      return match_count;
   }

   template<typename OutputIterator>
   inline std::size_t find_all(const std::string& pattern,
                               const std::string& data,
                               OutputIterator out)
   {
      return find_all(pattern.data(), pattern.data() + pattern.size(),
                      data.data(), data.data() + data.size(),
                      out);
   }

   template<typename Range,
            typename Allocator,
            template<typename, typename> class Sequence>
   inline std::size_t find_all(const std::string& pattern,
                               const std::string& data,
                               Sequence<Range,Allocator>& seq)
   {
      return find_all(pattern,data,std::back_inserter(seq));
   }

   template<typename OutputIterator>
   inline std::size_t ifind_all(const std::string& pattern,
                                const std::string& data,
                                OutputIterator out)
   {
      return ifind_all(pattern.data(), pattern.data() + pattern.size(),
                       data.data(), data.data() + data.size(),
                       out);
   }

   template<typename Range,
            typename Allocator,
            template<typename, typename> class Sequence>
   inline std::size_t ifind_all(const std::string& pattern,
                                const std::string& data,
                                Sequence<Range,Allocator>& seq)
   {
      return ifind_all(pattern,data,std::back_inserter(seq));
   }

   namespace tokenize_options
   {
      typedef std::size_t type;
      enum
      {
         default_mode            = 0,
         compress_delimiters     = 1,
         include_1st_delimiter   = 2,
         include_all_delimiters  = 4
      };

      static inline bool perform_compress_delimiters(const type& split_opt)
      {
         return compress_delimiters == (split_opt & compress_delimiters);
      }

      static inline bool perform_include_1st_delimiter(const type& split_opt)
      {
         return include_1st_delimiter == (split_opt & include_1st_delimiter);
      }

      static inline bool perform_include_all_delimiters(const type& split_opt)
      {
         return include_all_delimiters == (split_opt & include_all_delimiters);
      }

   } // namespace tokenize_options

   template<typename Iterator, typename DelimiterPredicate>
   class tokenizer
   {
   private:

      template<typename Iterartor,
               typename Predicate,
               typename T = std::pair<Iterator,Iterator> >
      class tokenizer_iterator : public std::iterator<std::forward_iterator_tag,T>
      {
      protected:

         typedef Iterator iterator;
         typedef const iterator const_iterator;
         typedef typename std::pair<iterator,iterator> range_type;

      public:

         explicit inline tokenizer_iterator(const iterator begin,
                                            const iterator end,
                                            const Predicate& predicate,
                                            const tokenize_options::type tokenize_option = tokenize_options::default_mode)
         : predicate_(predicate),
           end_(end),
           range_(begin,begin),
           current_token_(end,end),
           compress_delimiters_(tokenize_options::perform_compress_delimiters(tokenize_option)),
           include_1st_delimiter_(tokenize_options::perform_include_1st_delimiter(tokenize_option)),
           include_all_delimiters_(tokenize_options::perform_include_all_delimiters(tokenize_option)),
           include_delimiters_(include_1st_delimiter_ || include_all_delimiters_),
           last_token_done_(false)
         {
            if (end != begin)
            {
               this->operator++();
            }
         }

         inline tokenizer_iterator& operator++()
         {
            if (last_token_done_)
            {
               range_.first = range_.second;
               return (*this);
            }
            else if (end_ != range_.second)
            {
               range_.first = range_.second;
            }

            while (end_ != range_.second)
            {
              if (predicate_(*(range_.second)))
              {
                 if (include_delimiters_)
                 {
                    if (include_1st_delimiter_)
                       ++range_.second;
                    else if (include_all_delimiters_)
                       while ((end_ != range_.second) && predicate_(*(range_.second))) ++range_.second;
                    current_token_ = range_;
                    if ((!include_all_delimiters_) && compress_delimiters_)
                       while ((end_ != range_.second) && predicate_(*(range_.second))) ++range_.second;
                 }
                 else
                 {
                    current_token_ = range_;
                    if (compress_delimiters_)
                       while ((end_ != (++range_.second)) && predicate_(*(range_.second))) ;
                    else
                       ++range_.second;
                 }
                 return (*this);
              }
              else
                 ++range_.second;
            }

            if (range_.first != range_.second)
            {
               current_token_.second = range_.second;
               if (!last_token_done_)
               {
                  if (predicate_(*(range_.second - 1)))
                     current_token_.first = range_.second;
                  else
                     current_token_.first = range_.first;
                  last_token_done_ = true;
               }
               else
                  range_.first = range_.second;
            }

            return (*this);
         }

         inline tokenizer_iterator operator++(int)
         {
            tokenizer_iterator tmp = (*this);
            this->operator++();
            return tmp;
         }

         inline tokenizer_iterator& operator+=(const int inc)
         {
            if (inc > 0)
            {
               for (int i = 0; i < inc; ++i, ++(*this)) ;
            }
            return (*this);
         }

         inline T operator*() const
         {
            return current_token_;
         }

         inline std::string as_string() const
         {
            return std::string(current_token_.first,current_token_.second);
         }

         inline bool operator==(const tokenizer_iterator& itr) const
         {
            return (range_ == itr.range_) && (end_ == itr.end_);
         }

         inline bool operator!=(const tokenizer_iterator& itr) const
         {
            return (range_ != itr.range_) || (end_ != itr.end_);
         }

         inline tokenizer_iterator& operator=(const tokenizer_iterator& itr)
         {
            if (this != &itr)
            {
               range_                  = itr.range_;
               current_token_          = itr.current_token_;
               end_                    = itr.end_;
               compress_delimiters_    = itr.compress_delimiters_;
               include_1st_delimiter_  = itr.include_1st_delimiter_;
               include_all_delimiters_ = itr.include_all_delimiters_;
               include_delimiters_     = itr.include_delimiters_;
            }
            return (*this);
         }

         inline std::string remaining() const
         {
            return std::string(current_token_.first,end_);
         }

      protected:

         const Predicate& predicate_;
         iterator end_;
         range_type range_;
         range_type current_token_;
         bool compress_delimiters_;
         bool include_1st_delimiter_;
         bool include_all_delimiters_;
         bool include_delimiters_;
         bool last_token_done_;
      };

   public:

      typedef typename std::iterator_traits<Iterator>::value_type value_type;
      typedef DelimiterPredicate predicate;
      typedef tokenizer_iterator<Iterator,DelimiterPredicate> iterator;
      typedef const iterator const_iterator;
      typedef iterator& iterator_ref;
      typedef const_iterator& const_iterator_ref;

      inline tokenizer(const Iterator begin,
                       const Iterator end,
                       const DelimiterPredicate& predicate,
                       const tokenize_options::type tokenize_options = tokenize_options::default_mode)
      : tokenize_options_(tokenize_options),
        predicate_(predicate),
        begin_(begin),
        end_(end),
        begin_itr_(begin_,end_,predicate_,tokenize_options_),
        end_itr_(end_,end_,predicate_,tokenize_options_)
      {}

      inline tokenizer(const std::string& s,
                       const DelimiterPredicate& predicate,
                       const tokenize_options::type tokenize_options = tokenize_options::default_mode)
      : tokenize_options_(tokenize_options),
        predicate_(predicate),
        begin_(s.data()),
        end_(s.data() + s.size()),
        begin_itr_(begin_,end_,predicate_,tokenize_options_),
        end_itr_(end_,end_,predicate_,tokenize_options_)
      {}

      inline tokenizer& operator=(const tokenizer& t)
      {
         if (this != &t)
         {
            begin_            = t.begin_;
            end_              = t.end_;
            end_itr_          = t.end_itr_;
            begin_itr_        = t.begin_itr_;
            tokenize_options_ = t.tokenize_options_;
         }
         return (*this);
      }

      inline void assign(const std::string& s) const
      {
         assign(s.data(),s.data() + s.size());
      }

      inline void assign(const std::string& s)
      {
         assign(s.data(),s.data() + s.size());
      }

      inline void assign(const Iterator begin, const Iterator end)
      {
        begin_ = begin;
        end_ = end;
        begin_itr_ = iterator(begin_,end_,predicate_,tokenize_options_);
        end_itr_ = iterator(end_,end_,predicate_,tokenize_options_);
      }

      inline const_iterator_ref begin() const
      {
         return begin_itr_;
      }

      inline const_iterator_ref end() const
      {
         return end_itr_;
      }

   private:

      tokenize_options::type tokenize_options_;
      const DelimiterPredicate& predicate_;
      Iterator begin_;
      Iterator end_;
      iterator begin_itr_;
      iterator end_itr_;
   };

   namespace std_string
   {
      template<typename DelimiterPredicate = single_delimiter_predicate<std::string::value_type> >
      struct tokenizer
      {
         typedef DelimiterPredicate predicate_type;
         typedef const std::string::value_type* string_iterator_type;
         typedef strtk::tokenizer<string_iterator_type,DelimiterPredicate> type;
         typedef strtk::tokenizer<string_iterator_type,multiple_char_delimiter_predicate> md_type;
         typedef std::pair<string_iterator_type,string_iterator_type> iterator_type;
      };

      typedef tokenizer<>::iterator_type iterator_type;
      typedef std::vector<iterator_type> token_vector_type;
      typedef std::deque<iterator_type> token_deque_type;
      typedef std::list<iterator_type> token_list_type;

   } // namespace std_string

   template <typename Sequence>
   class range_to_type_back_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                                     void,
                                                                     void,
                                                                     void,
                                                                     void>
   {
   public:

      typedef typename Sequence::value_type T;

      explicit inline range_to_type_back_inserter_iterator(Sequence& sequence)
      : sequence_(sequence)
      {}

      range_to_type_back_inserter_iterator(const range_to_type_back_inserter_iterator& it)
      : sequence_(it.sequence_)
      {}

      inline range_to_type_back_inserter_iterator& operator=(const range_to_type_back_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->sequence_ = it.sequence_;
         }
         return (*this);
      }

      template<typename Iterator>
      inline range_to_type_back_inserter_iterator& operator=(const std::pair<Iterator,Iterator>& r)
      {
         T t;
         if (string_to_type_converter(r.first,r.second,t))
            sequence_.push_back(t);
         return (*this);
      }

      inline range_to_type_back_inserter_iterator& operator=(const std::string& s)
      {
         T t;
         if (string_to_type_converter(s.data(),s.data() + s.size(),t))
            sequence_.push_back(t);
         return (*this);
      }

      template<typename Iterator>
      inline void operator()(const std::pair<Iterator,Iterator>& r)
      {
         T t;
         if (string_to_type_converter(r.first,r.second,t))
            sequence_.push_back(t);
      }

      template<typename Iterator>
      inline void operator()(const Iterator begin, const Iterator end)
      {
         sequence_.push_back(string_to_type_converter<T>(begin,end));
      }

      inline range_to_type_back_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline range_to_type_back_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline range_to_type_back_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Sequence& sequence_;
   };

   template <typename Sequence>
   inline range_to_type_back_inserter_iterator<Sequence> range_to_type_back_inserter(Sequence& sequence)
   {
      return (range_to_type_back_inserter_iterator<Sequence>(sequence));
   }

   template <typename Set>
   class range_to_type_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                                void,
                                                                void,
                                                                void,
                                                                void>
   {
   public:

      typedef typename Set::value_type T;

      explicit inline range_to_type_inserter_iterator(Set& set)
      : set_(set)
      {}

      range_to_type_inserter_iterator(const range_to_type_inserter_iterator& it)
      : set_(it.set_)
      {}

      inline range_to_type_inserter_iterator& operator=(const range_to_type_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->set_ = it.set_;
         }
         return (*this);
      }

      template<typename Iterator>
      inline range_to_type_inserter_iterator& operator=(const std::pair<Iterator,Iterator>& r)
      {
         T t;
         if (string_to_type_converter(r.first,r.second,t))
            set_.insert(t);
         return (*this);
      }

      template<typename Iterator>
      inline void operator()(const std::pair<Iterator,Iterator>& r)
      {
         T t;
         if (string_to_type_converter(r.first,r.second,t))
            set_.insert(t);
      }

      inline range_to_type_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline range_to_type_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline range_to_type_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Set& set_;
   };

   template <typename Set>
   inline range_to_type_inserter_iterator<Set> range_to_type_inserter(Set& set)
   {
      return (range_to_type_inserter_iterator<Set>(set));
   }

   template <typename Container>
   class range_to_type_push_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                                     void,
                                                                     void,
                                                                     void,
                                                                     void>
   {
   public:

      typedef typename Container::value_type T;

      explicit inline range_to_type_push_inserter_iterator(Container& container)
      : container_(container)
      {}

      range_to_type_push_inserter_iterator(const range_to_type_push_inserter_iterator& it)
      : container_(it.container_)
      {}

      inline range_to_type_push_inserter_iterator& operator=(const range_to_type_push_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->container_ = it.container_;
         }
         return (*this);
      }

      template<typename Iterator>
      inline range_to_type_push_inserter_iterator& operator=(const std::pair<Iterator,Iterator>& r)
      {
         T t;
         if (string_to_type_converter(r.first,r.second,t))
            container_.push(t);
         return (*this);
      }

      template<typename Iterator>
      inline void operator()(const std::pair<Iterator,Iterator>& r)
      {
         T t;
         if (string_to_type_converter(r.first,r.second,t))
            container_.push(t);
      }

      inline range_to_type_push_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline range_to_type_push_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline range_to_type_push_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Container& container_;
   };

   template <typename Container>
   inline range_to_type_push_inserter_iterator<Container> range_to_type_push_inserter(Container& container)
   {
      return (range_to_type_push_inserter_iterator<Container>(container));
   }

   template<typename Sequence>
   class back_inserter_with_valuetype_iterator : public std::iterator<std::output_iterator_tag,
                                                                      typename Sequence::value_type,
                                                                      void,
                                                                      void,
                                                                      void>
   {
   public:

      explicit inline back_inserter_with_valuetype_iterator(Sequence& sequence)
      : sequence_(sequence)
      {}

      back_inserter_with_valuetype_iterator(const back_inserter_with_valuetype_iterator& it)
      : sequence_(it.sequence_)
      {}

      inline back_inserter_with_valuetype_iterator& operator=(const back_inserter_with_valuetype_iterator& it)
      {
         if (this != &it)
         {
            this->sequence_ = it.sequence_;
         }
         return (*this);
      }

      inline back_inserter_with_valuetype_iterator& operator=(const typename Sequence::value_type& v)
      {
         sequence_.push_back(v);
         return (*this);
      }

      inline void operator()(const typename Sequence::value_type& v)
      {
         sequence_.push_back(v);
      }

      inline back_inserter_with_valuetype_iterator& operator*()
      {
         return (*this);
      }

      inline back_inserter_with_valuetype_iterator& operator++()
      {
         return (*this);
      }

      inline back_inserter_with_valuetype_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Sequence& sequence_;
   };

   template<typename Sequence>
   inline back_inserter_with_valuetype_iterator<Sequence> back_inserter_with_valuetype(Sequence& sequence_)
   {
      return (back_inserter_with_valuetype_iterator<Sequence>(sequence_));
   }

   template<typename Set>
   class inserter_with_valuetype_iterator : public std::iterator<std::output_iterator_tag,
                                                                 typename Set::value_type,
                                                                 void,
                                                                 void,
                                                                 void>
   {
   public:

      explicit inline inserter_with_valuetype_iterator(Set& set)
      : set_(set)
      {}

      inserter_with_valuetype_iterator(const inserter_with_valuetype_iterator& itr)
      : set_(itr.set_)
      {}

      inline inserter_with_valuetype_iterator& operator=(const inserter_with_valuetype_iterator& itr)
      {
         if (this != &itr)
         {
            this->set_ = itr.set_;
         }
         return (*this);
      }

      inline inserter_with_valuetype_iterator& operator=(const typename Set::value_type& v)
      {
         set_.insert(v);
         return (*this);
      }

      inline void operator()(const typename Set::value_type& v)
      {
         set_.insert(v);
      }

      inline inserter_with_valuetype_iterator& operator*()
      {
         return (*this);
      }

      inline inserter_with_valuetype_iterator& operator++()
      {
         return (*this);
      }

      inline inserter_with_valuetype_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Set& set_;
   };

   template<typename Set>
   inline inserter_with_valuetype_iterator<Set> inserter_with_valuetype(Set& set_)
   {
      return (inserter_with_valuetype_iterator<Set>(set_));
   }

   template <typename Container>
   class push_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                       void,
                                                       void,
                                                       void,
                                                       void>
   {
   public:

      explicit inline push_inserter_iterator(Container& container)
      : container_(container)
      {}

      inline push_inserter_iterator& operator=(const push_inserter_iterator& itr)
      {
         if (this != &itr)
         {
            this->container_ = itr.container_;
         }
         return (*this);
      }

      inline push_inserter_iterator<Container>& operator=(typename Container::const_reference v)
      {
         container_.push(v);
         return (*this);
      }

      inline push_inserter_iterator<Container>& operator*()
      {
         return (*this);
      }

      inline push_inserter_iterator<Container>& operator++()
      {
         return (*this);
      }

      inline push_inserter_iterator<Container> operator++(int)
      {
         return (*this);
      }

   private:

      Container& container_;
   };

   template <typename Container>
   inline push_inserter_iterator<Container> push_inserter(Container& c)
   {
      return push_inserter_iterator<Container>(c);
   }

   template<typename T>
   class counting_back_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                                T,
                                                                void,
                                                                void,
                                                                void>
   {
   public:

      explicit inline counting_back_inserter_iterator(std::size_t& counter)
      : counter_(counter)
      {}

      counting_back_inserter_iterator(const counting_back_inserter_iterator& it)
      : counter_(it.counter_)
      {}

      inline counting_back_inserter_iterator& operator=(const counting_back_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->counter_ = it.counter_;
         }
         return (*this);
      }

      inline counting_back_inserter_iterator& operator=(const T&)
      {
         ++counter_;
         return (*this);
      }

      inline void operator()(const T&)
      {
         ++counter_;
      }

      inline counting_back_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline counting_back_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline counting_back_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      std::size_t& counter_;
   };

   template<typename T>
   inline counting_back_inserter_iterator<T> counting_back_inserter(std::size_t& counter_)
   {
      return (counting_back_inserter_iterator<T>(counter_));
   }

   template<typename Function>
   class functional_inserter_iterator : public std::iterator<std::output_iterator_tag,
                                                             void,
                                                             void,
                                                             void,
                                                             void>
   {
   public:

      explicit inline functional_inserter_iterator(Function& function)
      : function_(function)
      {}

      functional_inserter_iterator(const functional_inserter_iterator& it)
      : function_(it.function_)
      {}

      inline functional_inserter_iterator& operator=(const functional_inserter_iterator& it)
      {
         if (this != &it)
         {
            this->function_ = it.function_;
         }
         return (*this);
      }

      template<typename T>
      inline functional_inserter_iterator& operator=(const T& t)
      {
         function_(t);
         return (*this);
      }

      template<typename T>
      inline void operator()(const T& t)
      {
         function_(t);
      }

      inline functional_inserter_iterator& operator*()
      {
         return (*this);
      }

      inline functional_inserter_iterator& operator++()
      {
         return (*this);
      }

      inline functional_inserter_iterator operator++(int)
      {
         return (*this);
      }

   private:

      Function& function_;
   };

   template<typename Function>
   inline functional_inserter_iterator<Function> functional_inserter(Function function)
   {
      return (functional_inserter_iterator<Function>(function));
   }

   namespace split_options
   {
      typedef std::size_t type;
      enum
      {
         default_mode            = 0,
         compress_delimiters     = 1,
         include_1st_delimiter   = 2,
         include_all_delimiters  = 4
      };

      static inline bool perform_compress_delimiters(const type& split_opt)
      {
         return compress_delimiters == (split_opt & compress_delimiters);
      }

      static inline bool perform_include_1st_delimiter(const type& split_opt)
      {
         return include_1st_delimiter == (split_opt & include_1st_delimiter);
      }

      static inline bool perform_include_all_delimiters(const type& split_opt)
      {
         return include_all_delimiters == (split_opt & include_all_delimiters);
      }

   } // namespace split_options

   template<typename DelimiterPredicate,
            typename Iterator,
            typename OutputIterator>
   inline std::size_t split(const DelimiterPredicate& delimiter,
                            const Iterator begin,
                            const Iterator end,
                            OutputIterator out,
                            const split_options::type split_option = split_options::default_mode)
   {
      if (begin == end) return 0;
      std::size_t token_count = 0;
      std::pair<Iterator,Iterator> range(begin,begin);
      const bool compress_delimiters = split_options::perform_compress_delimiters(split_option);
      const bool include_1st_delimiter = split_options::perform_include_1st_delimiter(split_option);
      const bool include_all_delimiters = (!include_1st_delimiter) && split_options::perform_include_all_delimiters(split_option);
      const bool include_delimiters = include_1st_delimiter || include_all_delimiters;

      while (end != range.second)
      {
         if (delimiter(*range.second))
         {
            if (include_delimiters)
            {
               if (include_1st_delimiter)
                  ++range.second;
               else if (include_all_delimiters)
                  while ((end != range.second) && delimiter(*range.second)) ++range.second;
               *(out++) = range;
               if ((!include_all_delimiters) && compress_delimiters)
                  while ((end != range.second) && delimiter(*range.second)) ++range.second;
            }
            else
            {
               *(out++) = range;
               if (compress_delimiters)
                  while ((end != (++range.second)) && delimiter(*range.second)) ;
               else
                  ++range.second;
            }
            ++token_count;
            range.first = range.second;
         }
         else
            ++range.second;
      }

      if ((range.first != range.second) || delimiter(*(range.second - 1)))
      {
         *(out++) = range;
         ++token_count;
      }

      return token_count;
   }

   template<typename OutputIterator>
   inline std::size_t split(const char* delimiters,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      return split(multiple_char_delimiter_predicate(delimiters),
                   str.data(),str.data() + str.size(),
                   out,
                   split_option);
   }

   template<typename OutputIterator>
   inline std::size_t split(const std::string& delimiters,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      str.data(), str.data() + str.size(),
                      out,
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      str.data(), str.data() + str.size(),
                      out,
                      split_option);
   }

   template<typename OutputIterator>
   inline std::size_t split(const std::string::value_type delimiter,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      return split(single_delimiter_predicate<std::string::value_type>(delimiter),
                   str.data(), str.data() + str.size(),
                   out,
                   split_option);
   }

   template<typename DelimiterPredicate,
            typename OutputIterator>
   inline std::size_t split(const DelimiterPredicate& delimiter,
                            const std::string& str,
                            OutputIterator out,
                            const split_options::type& split_option = split_options::default_mode)
   {
      return split(delimiter,
                   str.data(), str.data() + str.size(),
                   out,
                   split_option);
   }

   template<typename DelimiterPredicate,
            typename Iterator,
            typename OutputIterator>
   inline std::size_t split_n(const DelimiterPredicate& delimiter,
                              const Iterator begin,
                              const Iterator end,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      if (0 == token_count) return 0;
      if (begin == end) return 0;
      std::size_t match_count = 0;
      std::pair<Iterator,Iterator> range(begin,begin);
      const bool compress_delimiters = split_options::perform_compress_delimiters(split_option);
      const bool include_1st_delimiter = split_options::perform_include_1st_delimiter(split_option);
      const bool include_all_delimiters = (!include_1st_delimiter) && split_options::perform_include_all_delimiters(split_option);
      const bool include_delimiters = include_1st_delimiter || include_all_delimiters;

      while (end != range.second)
      {
         if (delimiter(*range.second))
         {
            if (include_delimiters)
            {
               ++range.second;
               *(out++) = range;
               if (++match_count >= token_count)
                  return match_count;
               if (compress_delimiters)
                  while ((end != range.second) && delimiter(*range.second)) ++range.second;
            }
            else
            {
               *(out++) = range;
               if (++match_count >= token_count)
                  return match_count;
               if (compress_delimiters)
                  while ((end != (++range.second)) && delimiter(*range.second)) ;
               else
                  ++range.second;
            }
            range.first = range.second;
         }
         else
            ++range.second;
      }

      if ((range.first != range.second) || delimiter(*(range.second - 1)))
      {
         *(out++) = range;
         ++match_count;
      }

      return match_count;
   }

   template<typename OutputIterator>
   inline std::size_t split_n(const char* delimiters,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(multiple_char_delimiter_predicate(delimiters),
                     str.data(), str.data() + str.size(),
                     token_count,
                     out,
                     split_option);
   }

   template<typename OutputIterator>
   inline std::size_t split_n(const std::string& delimiters,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        str.data(), str.data() + str.size(),
                        token_count,
                        out,
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        str.data(), str.data() + str.size(),
                        token_count,
                        out,
                        split_option);
   }

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_n(const std::string& delimiters,
                              const InputIterator begin,
                              const InputIterator end,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        begin,end,
                        token_count,
                        out,
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        begin,end,
                        token_count,
                        out,
                        split_option);
   }

   template<typename OutputIterator>
   inline std::size_t split_n(const std::string::value_type delimiter,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(single_delimiter_predicate<std::string::value_type>(delimiter),
                     str.data(),str.data() + str.size(),
                     token_count,
                     out,
                     split_option);
   }

   template<typename DelimiterPredicate,
            typename OutputIterator>
   inline std::size_t split_n(const DelimiterPredicate& delimiter,
                              const std::string& str,
                              const std::size_t& token_count,
                              OutputIterator out,
                              const split_options::type& split_option = split_options::default_mode)
   {
      return split_n(delimiter,
                     str.data(),str.data() + str.size(),
                     token_count,
                     out,
                     split_option);
   }

   #ifdef strtk_enable_regex

   static const std::string uri_expression     ("((https?|ftp)\\://((\\[?(\\d{1,3}\\.){3}\\d{1,3}\\]?)|(([-a-zA-Z0-9]+\\.)+[a-zA-Z]{2,4}))(\\:\\d+)?(/[-a-zA-Z0-9._?,+&amp;%$#=~\\\\]+)*/?)");
   static const std::string email_expression   ("([\\w\\-\\.]+)@((\\[([0-9]{1,3}\\.){3}[0-9]{1,3}\\])|(([\\w\\-]+\\.)+)([a-zA-Z]{2,4}))");
   static const std::string ip_expression      ("(([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+)\\.([0-2]*[0-9]+[0-9]+))");
   static const std::string ieee754_expression ("([-+]?((\\.[0-9]+|[0-9]+\\.[0-9]+)([eE][-+][0-9]+)?|[0-9]+))");

   namespace regex_match_mode
   {
      enum type
      {
         match_all = 0,
         match_1   = 1,
         match_2   = 2,
         match_3   = 3,
         match_4   = 4,
         match_5   = 5,
         match_6   = 6,
         match_7   = 7,
         match_8   = 8,
         match_9   = 9
      };
   }

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex(const boost::regex& delimiter_expression,
                                  const InputIterator begin,
                                  const InputIterator end,
                                  OutputIterator out,
                                  const regex_match_mode::type mode = regex_match_mode::match_all,
                                  const std::size_t& buffer_size = one_kilobyte)
   {
      boost::sregex_iterator itr(begin,end,delimiter_expression);
      boost::sregex_iterator itr_end;
      std::string token;
      token.reserve(buffer_size);
      std::size_t match_count = 0;
      while (itr_end != itr)
      {
         token.assign((*itr)[mode].first,(*itr)[mode].second);
         *(out++) = token;
         ++itr;
         ++match_count;
      }
      return match_count;
   }

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex(const std::string& delimiter_expression,
                                  const InputIterator begin,
                                  const InputIterator end,
                                  OutputIterator out,
                                  const regex_match_mode::type mode = regex_match_mode::match_all,
                                  const std::size_t& buffer_size = one_kilobyte)
   {
      const boost::regex regex_expression(delimiter_expression);
      return split_regex(regex_expression,
                         begin,end,
                         out,
                         mode,
                         buffer_size);
   }

   template<typename OutputIterator>
   inline std::size_t split_regex(const std::string& delimiter_expression,
                                  const std::string& text,
                                  OutputIterator out,
                                  const regex_match_mode::type mode = regex_match_mode::match_all,
                                  const std::size_t& buffer_size = one_kilobyte)
   {
      return split_regex(delimiter_expression,
                         text.begin(),text.end(),
                         out,
                         mode,
                         buffer_size);
   }

   template<typename OutputIterator>
   inline std::size_t split_regex(const boost::regex& delimiter_expression,
                                  const std::string& text,
                                  OutputIterator out,
                                  const regex_match_mode::type mode = regex_match_mode::match_all,
                                  const std::size_t& buffer_size = one_kilobyte)
   {
      return split_regex(delimiter_expression,
                         text.begin(),text.end(),
                         out,
                         mode,
                         buffer_size);
   }

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex_n(const boost::regex& delimiter_expression,
                                    const InputIterator begin,
                                    const InputIterator end,
                                    const std::size_t& token_count,
                                    OutputIterator out,
                                    const regex_match_mode::type mode = regex_match_mode::match_all,
                                    const std::size_t& buffer_size = one_kilobyte)
   {
      boost::sregex_iterator itr(begin,end,delimiter_expression);
      const boost::sregex_iterator itr_end;
      std::string token;
      token.reserve(buffer_size);
      std::size_t match_count = 0;
      while (itr_end != itr)
      {
         token.assign((*itr)[mode].first,(*itr)[mode].second);
         *(out++) = token;
         ++itr;
         if (++match_count >= token_count)
            return match_count;
      }
      return match_count;
   }

   template<typename InputIterator, typename OutputIterator>
   inline std::size_t split_regex_n(const std::string& delimiter_expression,
                                    const InputIterator begin,
                                    const InputIterator end,
                                    const std::size_t& token_count,
                                    OutputIterator out,
                                    const regex_match_mode::type mode = regex_match_mode::match_all,
                                    const std::size_t& buffer_size = one_kilobyte)
   {
      const boost::regex regex_expression(delimiter_expression);
      return split_regex_n(regex_expression,
                           begin,end,
                           token_count,
                           out,
                           mode,
                           buffer_size);
   }

   template<typename OutputIterator>
   inline std::size_t split_regex_n(const std::string& delimiter_expression,
                                    const std::string& text,
                                    const std::size_t& token_count,
                                    OutputIterator out,
                                    const regex_match_mode::type mode = regex_match_mode::match_all,
                                    const std::size_t& buffer_size = one_kilobyte)
   {
      return split_regex_n(delimiter_expression,
                           text.begin(),text.end(),
                           token_count,
                           out,
                           mode,
                           buffer_size);
   }

   template<typename OutputIterator>
   inline std::size_t split_regex_n(const boost::regex& delimiter_expression,
                                    const std::string& text,
                                    const std::size_t& token_count,
                                    OutputIterator out,
                                    const regex_match_mode::type mode = regex_match_mode::match_all,
                                    const std::size_t& buffer_size = one_kilobyte)
   {
      return split_regex_n(delimiter_expression,
                           text.begin(),text.end(),
                           token_count,
                           out,
                           mode,
                           buffer_size);
   }

   #endif // strtk_enable_regex

   template<const std::size_t offset_list_size>
   class offset_predicate
   {
   public:

      offset_predicate(const int offset_list[], const bool rotate = false)
      : rotate_(rotate),
        current_index_(0)
      {
         std::copy(offset_list, offset_list + offset_list_size, offset_list_);
         offset_list_[offset_list_size] = 0;
      }

      inline bool operator!() const
      {
         return (0 == offset_list_size);
      }

      inline void reset() const
      {
         current_index_ = 0;
      }

      inline std::size_t size() const
      {
         return offset_list_size;
      }

      inline int next() const
      {
         int result = offset_list_[current_index_++];
         if (rotate_ && (current_index_ >= offset_list_size))
         {
            current_index_ = 0;
         }
         return result;
      }

   private:

      bool rotate_;
      mutable std::size_t current_index_;
      int offset_list_[offset_list_size + 1];
   };

   inline offset_predicate<12> offsets(const int&  v1, const int&  v2, const int&  v3,
                                       const int&  v4, const int&  v5, const int&  v6,
                                       const int&  v7, const int&  v8, const int&  v9,
                                       const int& v10, const int& v11, const int& v12,
                                       const bool& rotate = false)
   {
      const int offset_list[12] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12 };
      return offset_predicate<12>(offset_list,rotate);
   }

   inline offset_predicate<11> offsets(const int&  v1, const int&  v2, const int& v3,
                                       const int&  v4, const int&  v5, const int& v6,
                                       const int&  v7, const int&  v8, const int& v9,
                                       const int& v10, const int& v11,
                                       const bool& rotate = false)
   {
      const int offset_list[11] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 };
      return offset_predicate<11>(offset_list,rotate);
   }

   inline offset_predicate<10> offsets(const int& v1, const int& v2, const int& v3,
                                       const int& v4, const int& v5, const int& v6,
                                       const int& v7, const int& v8, const int& v9,
                                       const int& v10, const bool& rotate = false)
   {
      const int offset_list[10] = { v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 };
      return offset_predicate<10>(offset_list,rotate);
   }

   inline offset_predicate<9> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const int& v8, const int& v9,
                                      const bool& rotate = false)
   {
      const int offset_list[9] = { v1, v2, v3, v4, v5, v6, v7, v8, v9 };
      return offset_predicate<9>(offset_list,rotate);
   }

   inline offset_predicate<8> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const int& v8, const bool& rotate = false)
   {
      const int offset_list[8] = { v1, v2, v3, v4, v5, v6, v7, v8 };
      return offset_predicate<8>(offset_list,rotate);
   }

   inline offset_predicate<7> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const int& v7, const bool& rotate = false)
   {
      const int offset_list[7] = { v1, v2, v3, v4, v5, v6, v7 };
      return offset_predicate<7>(offset_list,rotate);
   }

   inline offset_predicate<6> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const int& v6,
                                      const bool& rotate = false)
   {
      const int offset_list[6] = { v1, v2, v3, v4, v5, v6 };
      return offset_predicate<6>(offset_list,rotate);
   }

   inline offset_predicate<5> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const int& v5, const bool& rotate = false)
   {
      const int offset_list[5] = { v1, v2, v3, v4, v5 };
      return offset_predicate<5>(offset_list,rotate);
   }

   inline offset_predicate<4> offsets(const int& v1, const int& v2, const int& v3,
                                      const int& v4, const bool& rotate = false)
   {
      const int offset_list[4] = { v1, v2, v3, v4 };
      return offset_predicate<4>(offset_list,rotate);
   }

   inline offset_predicate<3> offsets(const int& v1, const int& v2, const int& v3,
                                      const bool& rotate = false)
   {
      const int offset_list[3] = { v1, v2, v3 };
      return offset_predicate<3>(offset_list,rotate);
   }

   inline offset_predicate<2> offsets(const int& v1, const int& v2, const bool& rotate = false)
   {
      const int offset_list[2] = { v1, v2 };
      return offset_predicate<2>(offset_list,rotate);
   }

   inline offset_predicate<1> offsets(const int& v1,
                                      const bool& rotate = false)
   {
      const int offset_list[1] = { v1 };
      return offset_predicate<1>(offset_list,rotate);
   }

   template<typename OffsetPredicate,
            typename InputIterator,
            typename OutputIterator>
   inline std::size_t offset_splitter(const InputIterator begin,
                                      const InputIterator end,
                                      const OffsetPredicate& offset,
                                      OutputIterator out)
   {
      std::size_t length = 0;
      if (0 == (length = std::distance(begin,end))) return 0;
      std::pair<InputIterator,InputIterator> range(begin,begin);
      std::size_t match_count = 0;
      int offset_length = 0;
      std::size_t increment_amount = 0;
      while ((end != range.second) && (0 < (offset_length = offset.next())))
      {
         increment_amount = std::min<std::size_t>(length,offset_length);
         range.first = range.second;
         range.second += increment_amount;
         length -= increment_amount;
         *(out++) = range;
         ++match_count;
      }
      return match_count;
   }

   template<typename OffsetPredicate,
            typename OutputIterator>
   inline std::size_t offset_splitter(const std::string& str,
                                      const OffsetPredicate& offset,
                                      OutputIterator out)
   {
      return offset_splitter(str.data(),str.data() + str.size(),offset,out);
   }

   template<typename InputIterator>
   inline std::size_t count_consecutive_duplicates(const InputIterator begin, const InputIterator end)
   {
      if (std::distance(begin,end) < 2) return 0;
      InputIterator prev = begin;
      InputIterator itr = begin;
      std::size_t count = 0;
      while (end != ++itr)
      {
         if (*prev == *itr)
            ++count;
         else
            prev = itr;
      }
      return count;
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline T min_of_cont(const Sequence<T,Allocator>& sequence)
   {
      return (*std::min_element(sequence.begin(),sequence.end()));
   }

   template<typename T,
            typename Comparator,
            typename Allocator>
   inline T min_of_cont(const std::set<T,Comparator,Allocator>& set)
   {
      return (*set.begin());
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline T max_of_cont(const Sequence<T,Allocator>& sequence)
   {
      return (*std::max_element(sequence.begin(),sequence.end()));
   }

   template<typename T,
            typename Comparator,
            typename Allocator>
   inline T max_of_cont(const std::set<T,Comparator,Allocator>& set)
   {
      return (*set.rbegin());
   }

   template<typename InputIterator>
   inline void min_max_of_range(const InputIterator begin, const InputIterator end,
                                typename std::iterator_traits<InputIterator>::value_type& min_value,
                                typename std::iterator_traits<InputIterator>::value_type& max_value)
   {
      min_value = *begin;
      max_value = *begin;
      InputIterator itr = begin;
      while (end != ++itr)
      {
         if (*itr < min_value)
            min_value = *itr;
         else if (*itr > max_value)
            max_value = *itr;
      }
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline void min_max_of_cont(const Sequence<T,Allocator>& sequence,
                               T& min_value,
                               T& max_value)
   {
      min_max_of_range(sequence.begin(),sequence.end(),
                       min_value,
                       max_value);
   }

   template<typename T,
            typename Comparator,
            typename Allocator>
   inline void min_max_of_cont(const std::set<T,Comparator,Allocator>& set,
                               T& min_value,
                               T& max_value)
   {
      min_value = (*set.begin());
      max_value = (*set.rbegin());
   }

   template<typename Iterator>
   inline void lexicographically_canonicalize(Iterator begin, Iterator end)
   {
      typedef typename std::iterator_traits<Iterator>::value_type type;
      typedef typename std::pair<Iterator,Iterator> iter_type;
      typedef typename std::list<iter_type> itr_list_type;
      itr_list_type itr_list;

      type smallest = (*std::min_element(begin,end));

      for (Iterator itr = begin; itr != end; ++itr)
      {
         if (*itr == smallest) itr_list.push_back(std::make_pair(itr,itr));
      }

      while (itr_list.size() > 1)
      {
         typename itr_list_type::iterator itr = itr_list.begin();
         while (itr_list.end() != itr)
         {
            ++(*itr).first;
            if (end == (*itr).first)
               itr = itr_list.erase(itr);
            else
               ++itr;
         }

         smallest = *(*itr_list.begin()).first;

         for (itr = (++itr_list.begin()); itr != itr_list.end(); ++itr)
         {
            if (*(*itr).first < smallest)
            {
               smallest = *(*itr).first;
            }
         }

         itr = itr_list.begin();
         while (itr_list.end() != itr)
         {
           if (*(*itr).first != smallest)
              itr = itr_list.erase(itr);
           else
              ++itr;
         }

         itr = itr_list.begin();
         while (itr_list.end() != itr)
         {
            if (end == (*itr).first)
               itr = itr_list.erase(itr);
            else
               ++itr;
         }

      }

      std::rotate(begin,(*itr_list.begin()).second,end);
   }

   inline void lexicographically_canonicalize(std::string& str)
   {
      lexicographically_canonicalize(const_cast<char*>(str.c_str()),
                                     const_cast<char*>(str.c_str() + str.size()));
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline void lexicographically_canonicalize(Sequence<T,Allocator>& sequence)
   {
      lexicographically_canonicalize(sequence.begin(),sequence.end());
   }

   inline void convert_bin_to_hex(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      static const unsigned char hex_symbol[] = { "0123456789ABCDEF" };

      for (const unsigned char* itr = begin; end != itr; ++itr)
      {
         *(out++) = hex_symbol[((*itr) >> 0x04) & 0x0F];
         *(out++) = hex_symbol[ (*itr)          & 0x0F];
      }
   }

   inline void convert_bin_to_hex(const char* begin, const char* end, char* out)
   {
      convert_bin_to_hex(reinterpret_cast<const unsigned char*>(begin),
                         reinterpret_cast<const unsigned char*>(end),
                         reinterpret_cast<unsigned char*>(out));
   }

   inline void convert_bin_to_hex(const std::string& binary_data, std::string& output)
   {
      output.resize(binary_data.size() * 2);
      convert_bin_to_hex(binary_data.data(),
                         binary_data.data() + binary_data.size(),
                         const_cast<char*>(output.c_str()));
   }

   inline bool convert_hex_to_bin(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      if (1 == (std::distance(begin,end) % 2))
         return false;
      static const unsigned char hex_to_bin[] =
                                 {
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00 - 0x07
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x08 - 0x0F
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10 - 0x17
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x18 - 0x1F
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x20 - 0x27
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x28 - 0x2F
                                    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 0x30 - 0x37
                                    0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x38 - 0x3F
                                    0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, // 0x40 - 0x47
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x48 - 0x4F
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x50 - 0x57
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x58 - 0x5F
                                    0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00, // 0x60 - 0x67
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x68 - 0x6F
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x70 - 0x77
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x78 - 0x7F
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x80 - 0x87
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x88 - 0x8F
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x90 - 0x97
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x98 - 0x9F
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xA0 - 0xA7
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xA8 - 0xAF
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xB0 - 0xB7
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xB8 - 0xBF
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xC0 - 0xC7
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xC8 - 0xCF
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xD0 - 0xD7
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xD8 - 0xDF
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xE0 - 0xE7
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xE8 - 0xEF
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xF0 - 0xF7
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 0xF8 - 0xFF
                                 };

      const unsigned char* itr = begin;
      while (end != itr)
      {
         (*out)  = static_cast<unsigned char>(hex_to_bin[*(itr++)] << 4);
         (*out) |= static_cast<unsigned char>(hex_to_bin[*(itr++)]     );
         ++out;
      }
      return true;
   }

   inline bool convert_hex_to_bin(const char* begin, const char* end, char* out)
   {
      return convert_hex_to_bin(reinterpret_cast<const unsigned char*>(begin),
                                reinterpret_cast<const unsigned char*>(end),
                                reinterpret_cast<unsigned char*>(out));
   }

   inline bool convert_hex_to_bin(const std::string& hex_data, std::string& output)
   {
      if (hex_data.empty() || (1 == (hex_data.size() % 2)))
         return false;
      output.resize(hex_data.size() >> 1);
      return convert_hex_to_bin(hex_data.data(),
                                hex_data.data() + hex_data.size(),
                                const_cast<char*>(output.c_str()));
   }

   inline std::size_t convert_bin_to_base64(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      static const unsigned char bin_to_base64 [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

      const std::size_t length = std::distance(begin,end);
      std::size_t rounds = length / 3;
      const unsigned char* itr = begin;
      for (std::size_t i = 0; i < rounds; ++i)
      {
         unsigned int block  = *(itr++) << 16;
                      block |= *(itr++) <<  8;
                      block |= *(itr++)      ;
         *(out++) = bin_to_base64[( block >> 18 ) & 0x3F];
         *(out++) = bin_to_base64[( block >> 12 ) & 0x3F];
         *(out++) = bin_to_base64[( block >>  6 ) & 0x3F];
         *(out++) = bin_to_base64[( block       ) & 0x3F];
      }

      if ((rounds = (length % 3)) > 0)
      {
         switch(rounds)
         {
            case 1 : {
                       unsigned int block  = (unsigned char) (*itr) << 16;
                       *(out++) = bin_to_base64[( block >> 18 ) & 0x3F];
                       *(out++) = bin_to_base64[( block >> 12 ) & 0x3F];
                       *(out++) = '=';
                       *(out++) = '=';
                     }
                     break;

            case 2 : {
                        unsigned int block  = *(itr++) << 16;
                                     block |= *(itr++) <<  8;
                        *(out++) = bin_to_base64[( block >> 18 ) & 0x3F];
                        *(out++) = bin_to_base64[( block >> 12 ) & 0x3F];
                        *(out++) = bin_to_base64[( block >>  6 ) & 0x3F];
                        *(out++) = '=';
                     }
                     break;
         }
      }
      return static_cast<std::size_t>((length / 3) * 4) + ((length % 3) > 0 ? 4 : 0);
   }

   inline std::size_t convert_bin_to_base64(const char* begin, const char* end, char* out)
   {
      return convert_bin_to_base64(reinterpret_cast<const unsigned char*>(begin),
                                   reinterpret_cast<const unsigned char*>(end),
                                   reinterpret_cast<unsigned char*>(out));
   }

   inline void convert_bin_to_base64(const std::string& binary_data, std::string& output)
   {
      output.resize(std::max<std::size_t>(4,binary_data.size() << 1));
      std::size_t resize = convert_bin_to_base64(binary_data.data(),
                                                 binary_data.data() + binary_data.size(),
                                                 const_cast<char*>(output.c_str()));
      output.resize(resize);
   }

   inline std::size_t convert_base64_to_bin(const unsigned char* begin, const unsigned char* end, unsigned char* out)
   {
      static const unsigned char base64_to_bin[] =
                                 {
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x00 - 0x07
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x08 - 0x0F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x10 - 0x17
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x18 - 0x1F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x20 - 0x27
                                    0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F, // 0x28 - 0x2F
                                    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, // 0x30 - 0x37
                                    0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x38 - 0x3F
                                    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // 0x40 - 0x47
                                    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, // 0x48 - 0x4F
                                    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, // 0x50 - 0x57
                                    0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x58 - 0x5F
                                    0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, // 0x60 - 0x67
                                    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, // 0x68 - 0x6F
                                    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, // 0x70 - 0x77
                                    0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x78 - 0x7F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x80 - 0x87
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x88 - 0x8F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x90 - 0x97
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x98 - 0x9F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xA0 - 0xA7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xA8 - 0xAF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xB0 - 0xB7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xB8 - 0xBF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xC0 - 0xC7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xC8 - 0xCF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xD0 - 0xD7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xD8 - 0xDF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xE0 - 0xE7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xE8 - 0xEF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xF0 - 0xF7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  // 0xF8 - 0xFF
                                 };

      const unsigned char* end_itr = end;

      if ('=' == *(end - 2))
         end_itr = end - 2;
      else if ('=' == *(end - 1))
         end_itr = end - 1;

      const std::size_t length = std::distance(begin,end_itr);
      const std::size_t rounds = length / 4;
      const unsigned char* itr = begin;

      for (std::size_t i = 0; i < rounds; ++i)
      {
         unsigned int block  = base64_to_bin[*(itr++)] << 18;
                      block |= base64_to_bin[*(itr++)] << 12;
                      block |= base64_to_bin[*(itr++)] <<  6;
                      block |= base64_to_bin[*(itr++)];

         *(out++) = static_cast<unsigned char>(( block >> 16 ) & 0xFF);
         *(out++) = static_cast<unsigned char>(( block >>  8 ) & 0xFF);
         *(out++) = static_cast<unsigned char>(( block       ) & 0xFF);
      }

      const std::size_t remainder = (length % 4);
      if (remainder > 0)
      {
         switch(remainder)
         {
            case 2 : {
                        unsigned int block  = base64_to_bin[*(itr++)] << 18;
                                     block |= base64_to_bin[*(itr++)] << 12;
                        *out = static_cast<unsigned char>(( block >> 16 ) & 0xFF);
                     }
                     break;

            case 3 : {
                        unsigned int block  = base64_to_bin[*(itr++)] << 18;
                                     block |= base64_to_bin[*(itr++)] << 12;
                                     block |= base64_to_bin[*(itr++)] <<  6;
                        *(out++) = static_cast<unsigned char>(( block >> 16 ) & 0xFF);
                        *(out  ) = static_cast<unsigned char>(( block >>  8 ) & 0xFF);
                     }
                     break;
         }
      }

      return static_cast<std::size_t>((3 * length) / 4);
   }

   inline std::size_t convert_base64_to_bin(const char* begin, const char* end, char* out)
   {
      return convert_base64_to_bin(reinterpret_cast<const unsigned char*>(begin),
                                   reinterpret_cast<const unsigned char*>(end),
                                   reinterpret_cast<unsigned char*>(out));
   }

   inline void convert_base64_to_bin(const std::string& binary_data, std::string& output)
   {
      output.resize(binary_data.size());
      std::size_t resize = convert_base64_to_bin(binary_data.data(),
                                                 binary_data.data() + binary_data.size(),
                                                 const_cast<char*>(output.c_str()));
      output.resize(resize);
   }

   inline void convert_to_printable_chars(unsigned char* begin, unsigned char* end)
   {
      static const unsigned char printable_char_table[] =
                                 {
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x00 - 0x07
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x08 - 0x0F
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x10 - 0x17
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x18 - 0x1F
                                    0x2E, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, // 0x20 - 0x27
                                    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, // 0x28 - 0x2F
                                    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, // 0x30 - 0x37
                                    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, // 0x38 - 0x3F
                                    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, // 0x40 - 0x47
                                    0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, // 0x48 - 0x4F
                                    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, // 0x50 - 0x57
                                    0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, // 0x58 - 0x5F
                                    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, // 0x60 - 0x67
                                    0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, // 0x68 - 0x6F
                                    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, // 0x70 - 0x77
                                    0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x2E, // 0x78 - 0x7F
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x80 - 0x87
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x88 - 0x8F
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x90 - 0x97
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0x98 - 0x9F
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xA0 - 0xA7
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xA8 - 0xAF
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xB0 - 0xB7
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xB8 - 0xBF
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xC0 - 0xC7
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xC8 - 0xCF
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xD0 - 0xD7
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xD8 - 0xDF
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xE0 - 0xE7
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xE8 - 0xEF
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, // 0xF0 - 0xF7
                                    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E  // 0xF8 - 0xFF
                                 };
      unsigned char* itr = begin;
      while (end != itr)
      {
         (*itr) = printable_char_table[static_cast<unsigned int>((*itr))];
         ++itr;
      }
   }

   inline void convert_to_printable_chars(std::string& str)
   {
      convert_to_printable_chars(reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str())),
                                 reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str() + str.size())));
   }

   inline void convert_to_uppercase(unsigned char* begin, unsigned char* end)
   {
      std::transform(begin,end,begin,::toupper);
      /*
      unsigned char* itr = begin;
      while (end != itr)
      {
         //(*itr) = std::toupper((*itr), std::locale::classic());
         (*itr) = static_cast<unsigned char>(::toupper(static_cast<int>(*itr)));
         ++itr;
      }
      */
   }

   inline void convert_to_uppercase(char* begin, char* end)
   {
      convert_to_uppercase(reinterpret_cast<unsigned char*>(begin),
                           reinterpret_cast<unsigned char*>(end));
   }

   inline void convert_to_uppercase(std::string& str)
   {
      convert_to_uppercase(reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str())),
                           reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str() + str.size())));
   }

   inline void convert_to_lowercase(unsigned char* begin, unsigned char* end)
   {
      std::transform(begin,end,begin,::tolower);
      /*
      unsigned char* itr = begin;
      while (end != itr)
      {
         //(*itr) = std::tolower((*itr), std::locale::classic());
         (*itr) = static_cast<unsigned char>(::tolower(static_cast<int>(*itr)));
         ++itr;
      }
      */
   }

   inline void convert_to_lowercase(char* begin, char* end)
   {
      convert_to_lowercase(reinterpret_cast<unsigned char*>(begin),
                           reinterpret_cast<unsigned char*>(end));
   }

   inline void convert_to_lowercase(std::string& str)
   {
      convert_to_lowercase(reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str())),
                           reinterpret_cast<unsigned char*>(const_cast<char*>(str.c_str() + str.size())));
   }

   inline bool twoway_bitwise_interleave(const unsigned char* begin1, const unsigned char* end1,
                                         const unsigned char* begin2, const unsigned char* end2,
                                         unsigned char* out)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return false;
      }

      static const std::size_t interleave_table_size = 256;
      static const unsigned short interleave_table[interleave_table_size] =
                                  {
                                     0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015, // 0x00 - 0x07
                                     0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055, // 0x08 - 0x0F
                                     0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115, // 0x10 - 0x17
                                     0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155, // 0x18 - 0x1F
                                     0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415, // 0x20 - 0x27
                                     0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455, // 0x28 - 0x2F
                                     0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515, // 0x30 - 0x37
                                     0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555, // 0x38 - 0x3F
                                     0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015, // 0x40 - 0x47
                                     0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055, // 0x48 - 0x4F
                                     0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115, // 0x50 - 0x57
                                     0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155, // 0x58 - 0x5F
                                     0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415, // 0x60 - 0x67
                                     0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455, // 0x68 - 0x6F
                                     0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515, // 0x70 - 0x77
                                     0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555, // 0x78 - 0x7F
                                     0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015, // 0x80 - 0x87
                                     0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055, // 0x88 - 0x8F
                                     0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115, // 0x90 - 0x97
                                     0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155, // 0x98 - 0x9F
                                     0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415, // 0xA0 - 0xA7
                                     0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455, // 0xA8 - 0xAF
                                     0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515, // 0xB0 - 0xB7
                                     0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555, // 0xB8 - 0xBF
                                     0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015, // 0xC0 - 0xC7
                                     0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055, // 0xC8 - 0xCF
                                     0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115, // 0xD0 - 0xD7
                                     0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155, // 0xD8 - 0xDF
                                     0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415, // 0xE0 - 0xE7
                                     0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455, // 0xE8 - 0xEF
                                     0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515, // 0xF0 - 0xF7
                                     0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555  // 0xF8 - 0xFF
                                  };

      const unsigned char* itr1 = begin1;
      const unsigned char* itr2 = begin2;
      while (end1 != itr1)
      {
         *(reinterpret_cast<unsigned short*>(out))  = (interleave_table[*(itr2++)] << 1);
         *(reinterpret_cast<unsigned short*>(out)) |=  interleave_table[*(itr1++)];
         out += 2;
      }
      return true;
   }

   inline bool twoway_bitwise_interleave(const char* begin1, const char* end1,
                                         const char* begin2, const char* end2,
                                         char* out)
   {
      return twoway_bitwise_interleave(reinterpret_cast<const unsigned char*>(begin1),
                                       reinterpret_cast<const unsigned char*>(end1),
                                       reinterpret_cast<const unsigned char*>(begin2),
                                       reinterpret_cast<const unsigned char*>(end2),
                                       reinterpret_cast<unsigned char*>(out));
   }

   inline bool twoway_bitwise_interleave(const std::string& str1,
                                         const std::string& str2,
                                         std::string& out)
   {
      if (str1.size() != str2.size())
      {
         return false;
      }
      out.resize(str1.size());
      return twoway_bitwise_interleave(str1.data(),str1.data() + str1.size(),
                                       str2.data(),str2.data() + str2.size(),
                                       const_cast<char*>(out.c_str()));
   }

   template<std::size_t n>
   struct interleave_ary;

   template<> struct interleave_ary<sizeof(unsigned short)>     { typedef unsigned short     type; };
   template<> struct interleave_ary<sizeof(unsigned int  )>     { typedef unsigned int       type; };
   template<> struct interleave_ary<sizeof(unsigned long long)> { typedef unsigned long long type; };

   template<std::size_t n>
   inline void create_nway_interleave_table(typename interleave_ary<n>::type table[256])
   {
      typedef typename interleave_ary<n>::type type;
      const type diff = static_cast<type>(n - 1);
      for (type i = static_cast<type>(0); i < static_cast<type>(256); ++i)
      {
         table[i] = 0x00;
         for (type j = static_cast<type>(0); j < static_cast<type>(8); ++j)
         {
            table[i] |= (i & (1 << j)) << (j * diff);
         }
      }
   }

   namespace bitwise_operation { enum type { eAND, eOR, eXOR }; }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const unsigned char* begin1, const unsigned char* end1,
                                 const unsigned char* begin2,
                                 unsigned char* out)
   {
      const unsigned char* itr1 = begin1;
      const unsigned char* itr2 = begin2;

      switch(operation)
      {
         case bitwise_operation::eAND : while (itr1 != end1) { *(out++) = *(itr1++) & *(itr2++); } return;
         case bitwise_operation::eOR  : while (itr1 != end1) { *(out++) = *(itr1++) | *(itr2++); } return;
         case bitwise_operation::eXOR : while (itr1 != end1) { *(out++) = *(itr1++) ^ *(itr2++); } return;
      }
   }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const char* begin1, const char* end1,
                                 const char* begin2,
                                 char* out)
   {
      bitwise_transform(operation,
                        reinterpret_cast<const unsigned char*>(begin1),
                        reinterpret_cast<const unsigned char*>(end1),
                        reinterpret_cast<const unsigned char*>(begin2),
                        reinterpret_cast<unsigned char*>(out));
   }

   inline void bitwise_transform(const bitwise_operation::type& operation,
                                 const std::string& str1,
                                 const std::string& str2,
                                 std::string& out)
   {
      if (str1.size() != str2.size()) return;
      out.resize(str1.size());
      bitwise_transform(operation,
                        str1.data(),str1.data() + str1.size(),
                        str2.data(),
                        const_cast<char*>(out.c_str()));
   }

   inline std::size_t high_bit_count(const unsigned char c)
   {
      static const std::size_t high_bits_in_char[256] =
                               {
                                  0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
                                  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
                                  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                  3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
                                  4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8
                               };
      return high_bits_in_char[c];
   }

   inline std::size_t high_bit_count(const unsigned short& s)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&s);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1));
   }

   inline std::size_t high_bit_count(const unsigned int& i)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&i);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1)) +
             high_bit_count(*(ptr + 2)) + high_bit_count(*(ptr + 3));
   }

   inline std::size_t high_bit_count(const long long& ll)
   {
      const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&ll);
      return high_bit_count(*(ptr + 0)) + high_bit_count(*(ptr + 1)) +
             high_bit_count(*(ptr + 2)) + high_bit_count(*(ptr + 3)) +
             high_bit_count(*(ptr + 4)) + high_bit_count(*(ptr + 5)) +
             high_bit_count(*(ptr + 6)) + high_bit_count(*(ptr + 7));
   }

   inline std::size_t high_bit_count(const unsigned char* begin, const unsigned char* end)
   {
      std::size_t count = 0;
      const unsigned char* itr = begin;
      while (end != itr)
      {
         count += high_bit_count(*itr++);
      }
      return count;
   }

   inline std::size_t high_bit_count(const char* begin, const char* end)
   {
      return high_bit_count(reinterpret_cast<const unsigned char*>(begin),
                            reinterpret_cast<const unsigned char*>(end));

   }

   inline std::size_t high_bit_count(const std::string& str)
   {
      return high_bit_count(str.data(),str.data() + str.size());
   }

   inline bool bit_state(const std::size_t& index, const unsigned char* ptr)
   {
      static const unsigned char bit_mask[] =
                                 {
                                    0x01, //00000001
                                    0x02, //00000010
                                    0x04, //00000100
                                    0x08, //00001000
                                    0x10, //00010000
                                    0x20, //00100000
                                    0x40, //01000000
                                    0x80  //10000000
                                 };
      return (0 != (ptr[(index >> 3)] & bit_mask[index & 7]));
   }

   inline void set_bit_high(const std::size_t& index, unsigned char* const ptr)
   {
      static const unsigned char bit_mask[] =
                                 {
                                    0x01, //00000001
                                    0x02, //00000010
                                    0x04, //00000100
                                    0x08, //00001000
                                    0x10, //00010000
                                    0x20, //00100000
                                    0x40, //01000000
                                    0x80  //10000000
                                 };
      ptr[(index >> 3)] |= bit_mask[index & 7];
   }

   inline void set_bit_low(const std::size_t& index, unsigned char* const ptr)
   {
      static const unsigned char bit_mask[] =
                                 {
                                    0xFE, //11111110
                                    0xFD, //11111101
                                    0xFB, //11111011
                                    0xF7, //11110111
                                    0xEF, //11101111
                                    0xDF, //11011111
                                    0xBF, //10111111
                                    0x7F  //01111111
                                 };
      ptr[(index >> 3)] &= bit_mask[index & 7];
   }

   inline std::size_t hamming_distance(const unsigned char* begin1, const unsigned char* end1,
                                       const unsigned char* begin2, const unsigned char* end2)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return std::numeric_limits<std::size_t>::max();
      }

      std::size_t distance = 0;
      const unsigned char* itr1 = begin1;
      const unsigned char* itr2 = begin2;

      while (end1 != itr1)
      {
         distance += high_bit_count(static_cast<unsigned char>(((*itr1++) ^ (*itr2++)) & 0xFF));
      }

      return distance;
   }

   inline std::size_t hamming_distance(const char* begin1, const char* end1,
                                       const char* begin2, const char* end2)
   {
      return hamming_distance(reinterpret_cast<const unsigned char*>(begin1),
                              reinterpret_cast<const unsigned char*>(end1),
                              reinterpret_cast<const unsigned char*>(begin2),
                              reinterpret_cast<const unsigned char*>(end2));
   }

   inline std::size_t hamming_distance(const std::string& str1, const std::string& str2)
   {
      return hamming_distance(str1.data(), str1.data() + str1.size(),
                              str2.data(), str2.data() + str2.size());
   }

   template<typename Iterator>
   inline std::size_t hamming_distance_elementwise(const Iterator begin1, const Iterator end1,
                                                   const Iterator begin2, const Iterator end2)
   {
      if (std::distance(begin1,end1) != std::distance(begin2,end2))
      {
         return std::numeric_limits<std::size_t>::max();
      }

      std::size_t distance = 0;
      Iterator itr1 = begin1;
      Iterator itr2 = begin2;

      while (end1 != itr1)
      {
         if (*itr1 != *itr2)
            ++distance;
      }

      return distance;
   }

   inline std::size_t hamming_distance_elementwise(const std::string& str1, const std::string& str2)
   {
      return hamming_distance_elementwise(str1.data(), str1.data() + str1.size(),
                                          str2.data(), str2.data() + str2.size());
   }

   class token_grid
   {
   public:

      typedef std::pair<unsigned char*, unsigned char*> range_type;
      typedef std::deque<range_type> itr_list_type;
      typedef std::deque<itr_list_type> itr_list_list_type;
      typedef std::pair<std::size_t,std::size_t> row_range_type;
      typedef std::pair<std::size_t,std::size_t> col_range_type;

      struct options
      {
         options()
         : row_split_option(split_options::compress_delimiters),
           column_split_option(split_options::compress_delimiters),
           row_delimiters("\n\r"),
           column_delimiters(",|;\t "),
           support_dquotes(false)
         {}

         options(split_options::type sro,
                 split_options::type sco,
                 const std::string& rd,
                 const std::string& cd,
                 const bool support_dq = false)
         : row_split_option(sro),
           column_split_option(sco),
           row_delimiters(rd),
           column_delimiters(cd),
           support_dquotes(support_dq)
         {}

         inline options& set_column_split_option(const split_options::type& option)
         {
            column_split_option = option;
            return *this;
         }

         inline options& set_row_split_option(const split_options::type& option)
         {
            row_split_option = option;
            return *this;
         }

         inline options& set_column_delimiters(const std::string& delimiters)
         {
            column_delimiters = delimiters;
            return *this;
         }

         inline options& set_row_delimiters(const std::string& delimiters)
         {
            row_delimiters = delimiters;
            return *this;
         }

         split_options::type row_split_option;
         split_options::type column_split_option;
         std::string row_delimiters;
         std::string column_delimiters;
         bool support_dquotes;
      };

      class row_type
      {
      private:

         typedef std::pair<bool,row_type*> row_pair_type;

      public:

         row_type(const itr_list_type& token_list)
         : index_(std::numeric_limits<std::size_t>::max()),
           row_list_(0),
           token_list_(&token_list),
           next_row_(false,static_cast<row_type*>(0)),
           prev_row_(false,static_cast<row_type*>(0))
         {}

         row_type(const std::size_t& index,
                  const itr_list_list_type& row_list)
         : index_(index),
           row_list_(&row_list),
           token_list_(&(*row_list_)[index_]),
           next_row_(index < (row_list.size() - 1),static_cast<row_type*>(0)),
           prev_row_(index > 0,static_cast<row_type*>(0))
         {}

        ~row_type()
         {
            if (next_row_.second) { delete next_row_.second; next_row_.second = 0; }
            if (prev_row_.second) { delete prev_row_.second; prev_row_.second = 0; }
         }

         template<typename T>
         inline T operator[](const std::size_t& index) const
         {
            itr_list_type::value_type curr_range = (*token_list_)[index];
            return string_to_type_converter<T>(curr_range.first,curr_range.second);
         }

         inline const row_type& next_row() const
         {
            if (next_row_.first && (0 == next_row_.second))
               next_row_.second = new row_type(index_ + 1,*row_list_);
            return *next_row_.second;
         }

         inline const row_type& prev_row() const
         {
            if (prev_row_.first && (0 == prev_row_.second))
               prev_row_.second = new row_type(index_ - 1,*row_list_);
            return *prev_row_.second;
         }

         template<typename T>
         inline T get(const std::size_t& index) const
         {
            return row_type::operator[]<T>(index);
         }

         inline range_type token(const std::size_t& index) const
         {
            return (*token_list_)[index];
         }

         inline std::size_t index() const
         {
            return index_;
         }

         inline std::size_t size() const
         {
            return token_list_->size();
         }

         inline std::size_t raw_length() const
         {
            return std::distance(token_list_->begin()->first,token_list_->back().second);
         }

         inline std::size_t raw_length(const std::size_t& column_index) const
         {
            const itr_list_type::value_type& range = (*token_list_)[column_index];
            return std::distance(range.first,range.second);
         }

         inline std::string as_string() const
         {
            return std::string(token_list_->begin()->first,token_list_->back().second);
         }

         inline void as_string(std::string& out) const
         {
            out.assign(token_list_->begin()->first,token_list_->back().second);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9, typename T10 >
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      const std::size_t& col7, const std::size_t& col8,
                                      const std::size_t& col9, const std::size_t& col10,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                                      T7& t7, T8& t8, T9& t9, T10& t10) const
         {
            if (!process((*token_list_)[ col1], t1)) return false;
            if (!process((*token_list_)[ col2], t2)) return false;
            if (!process((*token_list_)[ col3], t3)) return false;
            if (!process((*token_list_)[ col4], t4)) return false;
            if (!process((*token_list_)[ col5], t5)) return false;
            if (!process((*token_list_)[ col6], t6)) return false;
            if (!process((*token_list_)[ col7], t7)) return false;
            if (!process((*token_list_)[ col8], t8)) return false;
            if (!process((*token_list_)[ col9], t9)) return false;
            if (!process((*token_list_)[col10],t10)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9 >
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      const std::size_t& col7, const std::size_t& col8,
                                      const std::size_t& col9,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                                      T7& t7, T8& t8, T9& t9) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            if (!process((*token_list_)[col6],t6)) return false;
            if (!process((*token_list_)[col7],t7)) return false;
            if (!process((*token_list_)[col8],t8)) return false;
            if (!process((*token_list_)[col9],t9)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      const std::size_t& col7, const std::size_t& col8,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                                      T7& t7, T8& t8) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            if (!process((*token_list_)[col6],t6)) return false;
            if (!process((*token_list_)[col7],t7)) return false;
            if (!process((*token_list_)[col8],t8)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6, typename T7>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      const std::size_t& col7,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6,
                                      T7& t7) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            if (!process((*token_list_)[col6],t6)) return false;
            if (!process((*token_list_)[col7],t7)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5, const std::size_t& col6,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5, T6& t6) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            if (!process((*token_list_)[col6],t6)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,typename T5>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      const std::size_t& col5,
                                      T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            if (!process((*token_list_)[col5],t5)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3, const std::size_t& col4,
                                      T1& t1, T2& t2, T3& t3, T4& t4) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            if (!process((*token_list_)[col4],t4)) return false;
            return true;
         }

         template<typename T1, typename T2, typename T3>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      const std::size_t& col3,
                                      T1& t1, T2& t2, T3& t3) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            if (!process((*token_list_)[col3],t3)) return false;
            return true;
         }

         template<typename T1, typename T2>
         inline bool parse_with_index(const std::size_t& col1, const std::size_t& col2,
                                      T1& t1, T2& t2) const
         {
            if (!process((*token_list_)[col1],t1)) return false;
            if (!process((*token_list_)[col2],t2)) return false;
            return true;
         }

         template<typename T1>
         inline bool parse_with_index(const std::size_t& col, T1& t) const
         {
            return process((*token_list_)[col],t);
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9, typename T10 >
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8,
                           T9& t9, T10& t10) const
         {
            if (!process((*token_list_)[0], t1)) return false;
            if (!process((*token_list_)[1], t2)) return false;
            if (!process((*token_list_)[2], t3)) return false;
            if (!process((*token_list_)[3], t4)) return false;
            if (!process((*token_list_)[4], t5)) return false;
            if (!process((*token_list_)[5], t6)) return false;
            if (!process((*token_list_)[6], t7)) return false;
            if (!process((*token_list_)[7], t8)) return false;
            if (!process((*token_list_)[8], t9)) return false;
            if (!process((*token_list_)[9],t10)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8,
                  typename T9 >
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8,
                           T9& t9) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            if (!process((*token_list_)[5],t6)) return false;
            if (!process((*token_list_)[6],t7)) return false;
            if (!process((*token_list_)[7],t8)) return false;
            if (!process((*token_list_)[8],t9)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6,
                  typename T7, typename T8>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            if (!process((*token_list_)[5],t6)) return false;
            if (!process((*token_list_)[6],t7)) return false;
            if (!process((*token_list_)[7],t8)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6, typename T7>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            if (!process((*token_list_)[5],t6)) return false;
            if (!process((*token_list_)[6],t7)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,
                  typename T5, typename T6>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            if (!process((*token_list_)[5],t6)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4,typename T5>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4, T5& t5) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            if (!process((*token_list_)[4],t5)) return false;
            return true;
         }

         template<typename T1, typename T2,
                  typename T3, typename T4>
         inline bool parse(T1& t1, T2& t2, T3& t3, T4& t4) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            if (!process((*token_list_)[3],t4)) return false;
            return true;
         }

         template<typename T1, typename T2, typename T3>
         inline bool parse(T1& t1, T2& t2, T3& t3) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            if (!process((*token_list_)[2],t3)) return false;
            return true;
         }

         template<typename T1, typename T2>
         inline bool parse(T1& t1, T2& t2) const
         {
            if (!process((*token_list_)[0],t1)) return false;
            if (!process((*token_list_)[1],t2)) return false;
            return true;
         }

         template<typename T1>
         inline bool parse(T1& t) const
         {
            return process((*token_list_)[0],t);
         }

         template<typename T, typename OutputIterator>
         inline void parse(OutputIterator out) const
         {
            itr_list_type::const_iterator itr = token_list_->begin();
            itr_list_type::const_iterator end = token_list_->end();
            while (end != itr)
            {
               const itr_list_type::value_type& range = (*itr);
               *(out++) = string_to_type_converter<T>(range.first,range.second);
               ++itr;
            }
         }

         bool validate_column_range(const col_range_type& range) const
         {
            if ((range.first > token_list_->size()) || (range.second > token_list_->size()))
               return false;
            else if (range.first > range.second)
               return false;
            else
               return true;
         }

         col_range_type range(const std::size_t& lower_bound,
                              const std::size_t& upper_bound = std::numeric_limits<std::size_t>::max()) const
         {
            if (std::numeric_limits<std::size_t>::max() != upper_bound)
               return col_range_type(lower_bound,upper_bound);
            else
              return col_range_type(lower_bound,token_list_->size());
         }

         template<typename T,
                  typename Allocator,
                  template<typename,typename> class Sequence>
         inline bool parse(const col_range_type& range,
                           Sequence<T,Allocator>& sequence) const
         {
            if (!validate_column_range(range))
               return false;

            itr_list_type::const_iterator itr = token_list_->begin() + range.first;
            itr_list_type::const_iterator end = token_list_->begin() + range.second;
            T t;

            while (end != itr)
            {
               const itr_list_type::value_type& range = (*itr);
               if (!string_to_type_converter(range.first,range.second,t))
                  return false;
               else
                  sequence.push_back(t);
               ++itr;
            }
            return true;
         }

         template<typename T,
                  typename Comparator,
                  typename Allocator>
         inline bool parse(const col_range_type& range,
                           std::set<T,Comparator,Allocator>& set) const
         {
            if (!validate_column_range(range))
               return false;

            itr_list_type::const_iterator itr = token_list_->begin() + range.first;
            itr_list_type::const_iterator end = token_list_->begin() + range.second;
            T t;

            while (end != itr)
            {
               const itr_list_type::value_type& range = (*itr);
               if (!string_to_type_converter(range.first,range.second,t))
                  return false;
               else
                  set.insert(t);
               ++itr;
            }
            return true;
         }

         template<typename T, typename Container>
         inline bool parse(const col_range_type& range,
                           std::queue<T,Container>& queue) const
         {
            if (!validate_column_range(range))
               return false;

            itr_list_type::const_iterator itr = token_list_->begin() + range.first;
            itr_list_type::const_iterator end = token_list_->begin() + range.second;
            T t;

            while (end != itr)
            {
               const itr_list_type::value_type& range = (*itr);
               if (!string_to_type_converter(range.first,range.second,t))
                  return false;
               else
                  queue.push(t);
               ++itr;
            }
            return true;
         }

         template<typename T, typename Container>
         inline bool parse(const col_range_type& range,
                           std::stack<T,Container>& stack) const
         {
            if (!validate_column_range(range))
               return false;

            itr_list_type::const_iterator itr = token_list_->begin() + range.first;
            itr_list_type::const_iterator end = token_list_->begin() + range.second;
            T t;

            while (end != itr)
            {
               const itr_list_type::value_type& range = (*itr);
               if (!string_to_type_converter(range.first,range.second,t))
                  return false;
               else
                  stack.push(t);
               ++itr;
            }
            return true;
         }

         template<typename T,
                  typename Container,
                  typename Comparator>
         inline bool parse(const col_range_type& range,
                           std::priority_queue<T,Container,Comparator>& priority_queue) const
         {
            if (!validate_column_range(range))
               return false;

            itr_list_type::const_iterator itr = token_list_->begin() + range.first;
            itr_list_type::const_iterator end = token_list_->begin() + range.second;
            T t;

            while (end != itr)
            {
               const itr_list_type::value_type& range = (*itr);
               if (!string_to_type_converter(range.first,range.second,t))
                  return false;
               else
                  priority_queue.push(t);
               ++itr;
            }
            return true;
         }

         template<typename T,
                  typename Allocator,
                  template<typename,typename> class Sequence>
         inline bool parse(Sequence<T,Allocator>& sequence) const
         {
            return parse(range(0),sequence);
         }

         template<typename T,
                  typename Comparator,
                  typename Allocator>
         inline bool parse(std::set<T,Comparator,Allocator>& set) const
         {
            return parse(range(0),set);
         }

         template<typename T, typename Container>
         inline bool parse(std::queue<T,Container>& queue) const
         {
            return parse(range(0),queue);
         }

         template<typename T, typename Container>
         inline bool parse(std::stack<T,Container>& stack) const
         {
            return parse(range(0),stack);
         }

         template<typename T,
                  typename Container,
                  typename Comparator>
         inline bool parse(std::priority_queue<T,Container,Comparator>& priority_queue) const
         {
            return parse(range(0),priority_queue);
         }

         template<typename T,
                  typename Allocator,
                  template<typename,typename> class Sequence>
         inline std::size_t parse_n(const std::size_t& n, Sequence<T,Allocator>& sequence) const
         {
            if (0 == n) return 0;
            T t;
            std::size_t count = 0;
            itr_list_type::const_iterator itr = token_list_->begin();
            itr_list_type::const_iterator end = token_list_->end();
            while (end != itr)
            {
               const itr_list_type::value_type& range = (*itr);
               if (!string_to_type_converter(range.first,range.second,t))
                  return false;
               else
                  sequence.push_back(t);
               if (n == (++count))
                  break;
               ++itr;
            }
            return count;
         }

         template<typename T, typename OutputIterator>
         inline void parse_checked(OutputIterator out) const
         {
            T value;
            itr_list_type::const_iterator itr = token_list_->begin();
            itr_list_type::const_iterator end = token_list_->end();
            while (end != itr)
            {
               const itr_list_type::value_type& range = (*itr);
               if (string_to_type_converter(range.first,range.second,value))
               {
                  *(out++) = value;
               }
               ++itr;
            }
         }

         template<typename T,
                  typename Allocator,
                  template<typename,typename> class Sequence>
         inline void parse_checked(Sequence<T,Allocator>& sequence) const
         {
            parse_checked<T>(std::back_inserter(sequence));
         }

         template<typename T,
                  typename Comparator,
                  typename Allocator>
         inline void parse_checked(std::set<T,Comparator,Allocator>& set) const
         {
            parse_checked<T>(std::inserter(set,set.end()));
         }

         template<typename T, typename Container>
         inline void parse_checked(std::queue<T,Container>& queue) const
         {
            parse_checked<T>(push_inserter(queue));
         }

         template<typename T, typename Container>
         inline void parse_checked(std::stack<T,Container>& stack) const
         {
            parse_checked<T>(push_inserter(stack));
         }

         template<typename T,
                  typename Container,
                  typename Comparator>
         inline void parse_checked(std::priority_queue<T,Container,Comparator>& priority_queue) const
         {
            parse_checked<T>(push_inserter(priority_queue));
         }

      private:

         template<typename T>
         inline bool process(const itr_list_type::value_type& range, T& t) const
         {
            return string_to_type_converter(range.first,range.second,t);
         }

      private:

         row_type& operator=(const row_type&);

         const std::size_t index_;
         const itr_list_list_type* row_list_;
         const itr_list_type* token_list_;
         mutable row_pair_type next_row_;
         mutable row_pair_type prev_row_;
      };

      token_grid(const std::string& file_name,
                 const token_grid::options& options)
      : file_name_(file_name),
        buffer_(0),
        buffer_size_(0),
        min_column_count_(0),
        max_column_count_(0),
        options_(options),
        load_from_file_(true),
        state_(load())
      {}

      token_grid(const unsigned char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const token_grid::options& options)
      : file_name_(""),
        buffer_(const_cast<unsigned char*>(input_buffer)),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(options),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const token_grid::options& options)
      : file_name_(""),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(options),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const std::string& input_buffer,
                 const std::size_t& input_buffer_size,
                 const token_grid::options& options)
      : file_name_(""),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer.data()))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(options),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const std::string& file_name,
                 const std::string& column_delimiters = ",|;\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(file_name),
        buffer_(0),
        buffer_size_(0),
        min_column_count_(0),
        max_column_count_(0),
        options_(split_options::compress_delimiters,
                 split_options::compress_delimiters,
                 row_delimiters,
                 column_delimiters),
        load_from_file_(true),
        state_(load())
      {}

      token_grid(const unsigned char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",|;\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        buffer_(const_cast<unsigned char*>(input_buffer)),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(split_options::compress_delimiters,
                 split_options::compress_delimiters,
                 row_delimiters,
                 column_delimiters),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const char* input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",|;\t",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(split_options::compress_delimiters,
                 split_options::compress_delimiters,
                 row_delimiters,
                 column_delimiters),
        load_from_file_(false),
        state_(load())
      {}

      token_grid(const std::string& input_buffer,
                 const std::size_t& input_buffer_size,
                 const std::string& column_delimiters = ",;|\t ",
                 const std::string& row_delimiters = "\n\r")
      : file_name_(""),
        buffer_(reinterpret_cast<unsigned char*>(const_cast<char*>(input_buffer.data()))),
        buffer_size_(input_buffer_size),
        min_column_count_(0),
        max_column_count_(0),
        options_(split_options::compress_delimiters,
                 split_options::compress_delimiters,
                 row_delimiters,
                 column_delimiters),
        load_from_file_(false),
        state_(load())
      {}

     ~token_grid()
      {
         if ((load_from_file_) && (0 != buffer_))
         {
            delete [] buffer_;
            buffer_ = 0;
         }
      }

      inline bool operator!()               const { return !state_;            }
      inline std::string source_file()      const { return file_name_;         }
      inline std::size_t row_count()        const { return token_list_.size(); }
      inline std::size_t min_column_count() const { return min_column_count_;  }
      inline std::size_t max_column_count() const { return max_column_count_;  }

      inline range_type token(const unsigned int& row, const std::size_t& col) const
      {
         return token_list_[row][col];
      }

      template<typename T>
      inline T get(const unsigned int& row, const std::size_t& col)
      {
         range_type r = token(row,col);
         return string_to_type_converter<T>(r.first,r.second);
      }

      inline row_type row(const unsigned int& row_index) const
      {
         return row_type(row_index,token_list_);
      }

      inline row_range_type all_rows() const
      {
         return row_range_type(0,token_list_.size());
      }

      template<typename OutputIterator>
      inline bool extract_column_checked(const row_range_type& range,
                                         const std::size_t& index,
                                         OutputIterator out) const
      {
         if (index > max_column_count_)
            return false;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first > range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;

         while (end != itr)
         {
            if (index < (*itr).size())
            {
               process_column_checked((*itr++)[index],out);
            }
            else
               ++itr;
         }
         return true;
      }

      template<typename OutputIterator>
      inline bool extract_column_checked(const std::size_t& index,
                                         OutputIterator out) const
      {
         return extract_column_checked(all_rows(),index,out);
      }

      template<typename T,
               typename Allocator,
               template<typename,typename> class Sequence>
      inline void extract_column_checked(const std::size_t& index,
                                          Sequence<T,Allocator>& sequence) const
      {
         extract_column_checked(index,back_inserter_with_valuetype(sequence));
      }

      template<typename T,
               typename Comparator,
               typename Allocator>
      inline void extract_column_checked(const std::size_t& index,
                                         std::set<T,Comparator,Allocator>& set) const
      {
         extract_column_checked(index,inserter_with_valuetype(set));
      }

      template<typename OutputIterator>
      inline bool extract_column(const row_range_type& range,
                                 const std::size_t& index,
                                 OutputIterator out) const
      {
         if (index > max_column_count_)
            return false;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first > range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;

         while (end != itr)
         {
            process_column((*itr++)[index],out);
         }
         return true;
      }

      template<typename OutputIterator>
      inline bool extract_column(const std::size_t& index,
                                 OutputIterator out) const
      {
         return extract_column(all_rows(),index,out);
      }

      template<typename OutputIterator1, typename OutputIterator2>
      inline bool extract_column(const row_range_type& range,
                                 const std::size_t& index1,
                                 const std::size_t& index2,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2) const
      {
         if ((index1 > max_column_count_) ||
             (index2 > max_column_count_))
            return false;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first > range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;

         while (end != itr)
         {
            process_column((*itr)[index1],out1);
            process_column((*itr)[index2],out2);
            ++itr;
         }
         return true;
      }

      template<typename OutputIterator1, typename OutputIterator2, typename OutputIterator3>
      inline bool extract_column(const row_range_type& range,
                                 const std::size_t& index1,
                                 const std::size_t& index2,
                                 const std::size_t& index3,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2,
                                 OutputIterator3 out3) const
      {
         if ((index1 > max_column_count_) ||
             (index2 > max_column_count_) ||
             (index3 > max_column_count_))
            return false;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first > range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;

         while (end != itr)
         {
            process_column((*itr)[index1],out1);
            process_column((*itr)[index2],out2);
            process_column((*itr)[index3],out3);
            ++itr;
         }
         return true;
      }

      template<typename OutputIterator1, typename OutputIterator2,
               typename OutputIterator3, typename OutputIterator4>
      inline bool extract_column(const row_range_type& range,
                                 const std::size_t& index1,
                                 const std::size_t& index2,
                                 const std::size_t& index3,
                                 const std::size_t& index4,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2,
                                 OutputIterator3 out3,
                                 OutputIterator4 out4) const
      {
         if ((index1 > max_column_count_) ||
             (index2 > max_column_count_) ||
             (index3 > max_column_count_) ||
             (index4 > max_column_count_))
            return false;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first > range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;

         while (end != itr)
         {
            process_column((*itr)[index1],out1);
            process_column((*itr)[index2],out2);
            process_column((*itr)[index3],out3);
            process_column((*itr)[index4],out4);
            ++itr;
         }
         return true;
      }

      template<typename OutputIterator1, typename OutputIterator2,
               typename OutputIterator3, typename OutputIterator4,
               typename OutputIterator5>
      inline bool extract_column(const row_range_type& range,
                                 const std::size_t& index1,
                                 const std::size_t& index2,
                                 const std::size_t& index3,
                                 const std::size_t& index4,
                                 const std::size_t& index5,
                                 OutputIterator1 out1,
                                 OutputIterator2 out2,
                                 OutputIterator3 out3,
                                 OutputIterator4 out4,
                                 OutputIterator5 out5) const
      {
         if ((index1 > max_column_count_) ||
             (index2 > max_column_count_) ||
             (index3 > max_column_count_) ||
             (index4 > max_column_count_) ||
             (index5 > max_column_count_))
            return false;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first > range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;

         while (end != itr)
         {
            process_column((*itr)[index1],out1);
            process_column((*itr)[index2],out2);
            process_column((*itr)[index3],out3);
            process_column((*itr)[index4],out4);
            process_column((*itr)[index5],out5);
            ++itr;
         }
         return true;
      }

      inline void remove_row(const std::size_t& index)
      {
         if (index < token_list_.size()) token_list_.erase(token_list_.begin() + index);
      }

      template<typename Predicate>
      inline bool remove_row_if(const row_range_type& range, Predicate predicate)
      {
         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first > range.second)
            return false;

         itr_list_list_type::iterator begin = token_list_.begin() + range.first;
         itr_list_list_type::iterator end   = token_list_.begin() + range.second;

         itr_list_list_type::iterator new_end = std::remove_if(begin,end,
                                                               remove_row_if_predicate<Predicate>(predicate));

         token_list_.erase(new_end,token_list_.begin() + range.second);

         return true;
      }

      template<typename Predicate>
      inline bool remove_row_if(Predicate predicate)
      {
         return remove_row_if(all_rows(),predicate);
      }

      inline void remove_empty_tokens(const row_range_type& range)
      {
         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return;

         if (range.first > range.second)
            return;

         itr_list_list_type::iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;

         while (end != itr)
         {
            itr_list_type& row_deq = (*itr);
            itr_list_type::iterator row_itr = row_deq.begin();

            while (row_deq.end() != row_itr)
            {
               if (0 == std::distance(row_itr->first,row_itr->second))
               {
                  row_itr = (*itr).erase(row_itr);
               }
               else
                  ++row_itr;
            }
            ++itr;
         }
      }

      template<typename Predicate>
      inline void remove_token_if(const row_range_type& range, Predicate p)
      {
         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return;

         if (range.first > range.second)
            return;

         itr_list_list_type::iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;

         while (end != itr)
         {
            itr_list_type& row_deq = (*itr);
            itr_list_type::iterator row_itr = row_deq.begin();

            while (row_deq.end() != row_itr)
            {
               if (p(row_itr->first,row_itr->second))
               {
                  row_itr = (*itr).erase(row_itr);
               }
               else
                  ++row_itr;
            }
            ++itr;
         }
      }

      inline void remove_empty_tokens()
      {
         remove_empty_tokens(all_rows());
      }

      inline void enforce_column_count(const std::size_t& column_count)
      {
         itr_list_list_type::iterator itr = token_list_.begin();
         itr_list_list_type::iterator end = token_list_.end();
         itr_list_list_type new_token_list;
         while (end != itr)
         {
            if (itr->size() == column_count)
            {
               new_token_list.push_back(*itr);
            }
            ++itr;
         }
         token_list_.swap(new_token_list);
         min_column_count_ = column_count;
         max_column_count_ = column_count;
      }

      inline void enforce_min_max_column_count(const std::size_t& min_column_count,
                                               const std::size_t& max_column_count)
      {
         itr_list_list_type::iterator itr = token_list_.begin();
         itr_list_list_type::iterator end = token_list_.end();
         itr_list_list_type new_token_list;
         while (end != itr)
         {
            std::size_t column_count = itr->size();
            if ((min_column_count <= column_count) && (column_count <= max_column_count))
            {
               new_token_list.push_back(*itr);
            }
            ++itr;
         }
         token_list_.swap(new_token_list);
         min_column_count_ = min_column_count;
         max_column_count_ = max_column_count;
      }

      inline void clear(const bool force_delete_buffer = false)
      {
         if (load_from_file_ || force_delete_buffer)
            delete[] buffer_;
         buffer_ = 0;
         buffer_size_ = 0;
         token_list_.clear();
         min_column_count_ = 0;
         max_column_count_ = 0;
         state_ = false;
         file_name_ = "";
      }

      template<typename T>
      inline bool accumulate_row(const std::size_t& row, T& result) const
      {
         if (row >= token_list_.size())
            return false;
         itr_list_type::const_iterator itr = token_list_[row].begin();
         itr_list_type::const_iterator end = token_list_[row].end();
         T current_value = T();
         while (end != itr)
         {
            if (string_to_type_converter((*itr).first,(*itr).second,current_value))
              result += current_value;
            else
               return false;
            ++itr;
         }
         return true;
      }

      template<typename T>
      inline std::size_t accumulate_column(const std::size_t& col,
                                           const row_range_type& range,
                                           T& result) const
      {
         if (col > max_column_count_)
            return 0;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return 0;

         if (range.first > range.second)
            return 0;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;
         T current_value = T();

         std::size_t process_count = 0;

         while (end != itr)
         {
            if (!(*itr).empty())
            {
               if (string_to_type_converter((*itr)[col].first, (*itr)[col].second, current_value))
               {
                  result += current_value;
                  ++process_count;
               }
               else
                  return 0;
            }
            ++itr;
         }
         return process_count;
      }

      template<typename T>
      inline std::size_t accumulate_column(const std::size_t& col, T& result) const
      {
         return accumulate_column(col,all_rows(),result);
      }

      template<typename T, typename Predicate>
      inline std::size_t accumulate_column(const std::size_t& col,
                                           const row_range_type& range,
                                           Predicate p,
                                           T& result) const
      {
         if (col > max_column_count_)
            return 0;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return 0;

         if (range.first > range.second)
            return 0;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;
         T current_value = T();

         std::size_t process_count = 0;

         while (end != itr)
         {
            if (!(*itr).empty() && p(row_type(*itr)))
            {
               if (string_to_type_converter((*itr)[col].first, (*itr)[col].second, current_value))
               {
                  result += current_value;
                  ++process_count;
               }
               else
                  return 0;
            }
            ++itr;
         }

         return process_count;
      }

      template<typename T, typename Predicate>
      inline std::size_t accumulate_column(const std::size_t& col,
                                    Predicate p,
                                    T& result) const
      {
         return accumulate_column(col,all_rows(),p,result);
      }

      inline bool join_row(const std::size_t& row, const std::string& delimiter, std::string& result)
      {
         if (row >= token_list_.size())
            return false;
         itr_list_type::const_iterator itr = token_list_[row].begin();
         itr_list_type::const_iterator end = token_list_[row].end();
         result.reserve(std::distance(token_list_[row].front().first,
                                      token_list_[row].back().second) +
                        (delimiter.size() * token_list_[row].size()) +
                        result.size());
         bool appended = false;
         while (end != itr)
         {
            if (!delimiter.empty() && appended)
               result.append(delimiter);
            appended = false;
            if ((*itr).first != (*itr).second)
            {
               result.append((*itr).first,(*itr).second);
               appended = true;
            }
            ++itr;
         }
         return true;
      }

      inline bool join_column(const std::size_t& col,
                              const row_range_type& row_range,
                              const std::string& delimiter,
                              std::string& result) const
      {
         if (col > max_column_count_)
            return false;

         if ((row_range.first > token_list_.size()) || (row_range.second > token_list_.size()))
            return false;

         if (row_range.first > row_range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + row_range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + row_range.second;
         range_type range;
         bool appended = false;

         while (end != itr)
         {
            if (!delimiter.empty() && appended)
               result.append(delimiter);
            appended = false;
            if (!(*itr).empty())
            {
               range = (*itr)[col];
               if (range.first != range.second)
               {
                  result.append(range.first,range.second);
                  appended = true;
               }
            }
            ++itr;
         }

         return true;
      }

      inline bool join_column(const std::size_t& col,
                              const std::string& delimiter,
                              std::string& result) const
      {
         return join_column(col,all_rows(),delimiter,result);
      }

      template<typename Predicate>
      inline bool join_column(const std::size_t& col,
                              const row_range_type& range,
                              Predicate p,
                              const std::string& delimiter,
                              std::string& result) const
      {
         if (col > max_column_count_)
            return false;

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first > range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         itr_list_list_type::const_iterator end = token_list_.begin() + range.second;
         bool appended = false;

         while (end != itr)
         {
            if (!delimiter.empty() && appended)
               result.append(delimiter);
            appended = false;
            if (!(*itr).empty() && p(row_type(*itr)))
            {
               range_type r = (*itr)[col];
               if (r.first != r.second)
               {
                  result.append(r.first,r.second);
                  appended = true;
               }
            }
            ++itr;
         }

         return true;
      }

      template<typename Predicate>
      inline bool join_column(const std::size_t& col,
                              Predicate p,
                              const std::string& delimiter,
                              std::string& result) const
      {
         return join_column(col,all_rows(),p,delimiter,result);
      }

      template<typename TransitionPredicate, typename Function>
      inline bool sequential_partition(const row_range_type& range,
                                       TransitionPredicate p,
                                       Function f)
      {

         if ((range.first > token_list_.size()) || (range.second > token_list_.size()))
            return false;

         if (range.first >= range.second)
            return false;

         itr_list_list_type::const_iterator itr = token_list_.begin() + range.first;
         row_range_type r(range.first,range.first);

         for (std::size_t i = range.first; i < range.second; ++i, ++itr)
         {
            if (p(row_type(*itr)))
            {
               if (r.first != r.second)
               {
                  r.second = i;
                  if (!f(*this,r))
                     return false;
               }
               r.first = r.second;
            }
            else
               r.second = i;
         }

         if (r.first != range.second)
         {
            r.second = range.second;
            if (!f(*this,r))
               return false;
         }

         return true;
      }

      template<typename TransitionPredicate, typename Function>
      inline bool sequential_partition(TransitionPredicate p, Function f)
      {
         return sequential_partition(all_rows(),p,f);
      }

      static inline token_grid::options default_options()
      {
         return token_grid::options();
      }

   private:

      token_grid(const token_grid& tg);
      token_grid operator=(const token_grid& tg);

      class double_quotes_predicate
      {
      public:

         double_quotes_predicate(const std::string& delimiters)
         : in_bracket_range_(false),
           mdp_(delimiters)
         {}

         inline bool operator()(const unsigned char c) const
         {
            if ('"' == c)
            {
               in_bracket_range_ = !in_bracket_range_;
               return true;
            }
            else if (in_bracket_range_)
               return false;
            else
               return mdp_(c);
         }

         inline void reset()
         {
            in_bracket_range_ = false;
         }

      private:

         mutable bool in_bracket_range_;
         mutable strtk::multiple_char_delimiter_predicate mdp_;
      };

      bool load()
      {
         if (load_from_file_ && !load_buffer_from_file())
            return false;

         itr_list_type row_list;
         multiple_char_delimiter_predicate text_newline_predicate(options_.row_delimiters);
         split(text_newline_predicate,
               buffer_, buffer_ + buffer_size_,
               std::back_inserter(row_list),
               options_.row_split_option);

         multiple_char_delimiter_predicate token_predicate(options_.column_delimiters);
         double_quotes_predicate token_predicate_dblq(options_.column_delimiters);

         min_column_count_ = std::numeric_limits<std::size_t>::max();
         max_column_count_ = std::numeric_limits<std::size_t>::min();

         itr_list_type::iterator itr = row_list.begin();
         itr_list_type::iterator end = row_list.end();

         while (end != itr)
         {
            if (0 != std::distance(itr->first,itr->second))
            {
               itr_list_type current_token_list;
               if (!options_.support_dquotes)
                  split(token_predicate,
                        itr->first,
                        itr->second,
                        std::back_inserter(current_token_list),
                        options_.column_split_option);
               else
               {
                  split(token_predicate_dblq,
                        itr->first,
                        itr->second,
                        std::back_inserter(current_token_list),
                        options_.column_split_option);
                  token_predicate_dblq.reset();
               }

               if (!current_token_list.empty())
               {
                  token_list_.push_back(current_token_list);
                  min_column_count_ = std::min(min_column_count_,current_token_list.size());
                  max_column_count_ = std::max(max_column_count_,current_token_list.size());
               }
            }
            ++itr;
         }

         return true;
      }

      bool load_buffer_from_file()
      {
         std::ifstream stream(file_name_.c_str(),std::ios::binary);
         if (!stream)
            return false;

         stream.seekg (0,std::ios::end);
         buffer_size_ = static_cast<std::size_t>(stream.tellg());
         if (0 == buffer_size_)
            return false;

         stream.seekg (0,std::ios::beg);

         buffer_ = new unsigned char[buffer_size_];
         stream.read(reinterpret_cast<char*>(buffer_),static_cast<std::streamsize>(buffer_size_));
         stream.close(); // scope should handle this.

         return true;
      }

      template<typename OutputIterator>
      inline void process_column(const itr_list_type::value_type& range, OutputIterator out) const
      {
         typedef typename std::iterator_traits<OutputIterator>::value_type output_type;
         *(out++) = string_to_type_converter<output_type>(range.first,range.second);
      }

      template<typename OutputIterator>
      inline void process_column_checked(const itr_list_type::value_type& range, OutputIterator out) const
      {
         typedef typename std::iterator_traits<OutputIterator>::value_type output_type;
         output_type value;
         if (string_to_type_converter(range.first,range.second,value))
         {
            *(out++) = value;
         }
      }

      template<typename Predicate>
      struct remove_row_if_predicate
      {
         remove_row_if_predicate(Predicate p)
         : p_(p)
         {}

         inline bool operator()(const itr_list_type& itr_list)
         {
            return (!itr_list.empty() &&
                    p_(itr_list.front().first,itr_list.back().second));
         }

         Predicate p_;
      };

   private:

      itr_list_list_type token_list_;
      std::string file_name_;
      unsigned char* buffer_;
      std::size_t buffer_size_;
      std::size_t min_column_count_;
      std::size_t max_column_count_;
      options options_;
      bool load_from_file_;
      bool state_;
   };

   template<typename T>
   inline bool convert_string_range(const std::pair<std::string::const_iterator,std::string::const_iterator> range, T& t)
   {
      if (range.first == range.second) return false;
      t = string_to_type_converter<T>(std::string(range.first,range.second));
      return true;
   }

   struct empty_range
   {
   public:

      template<typename InputIterator>
      inline bool operator()(const InputIterator begin, const InputIterator end)
      {
         return (0 == std::distance(begin,end));
      }
   };

   struct nonempty_range
   {
   public:

      template<typename InputIterator>
      inline bool operator()(const InputIterator begin, const InputIterator end)
      {
         return (0 != std::distance(begin,end));
      }
   };

   template<typename OutputIterator>
   struct filter_non_empty_range
   {
   public:
      filter_non_empty_range(OutputIterator out)
      : out_(out)
      {}

      template<typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range)
      {
         if (range.first != range.second)
         {
            *out_++ = range;
         }
      }

   private:

      OutputIterator out_;
   };

   template<typename OutputPredicate>
   struct filter_on_wildcard_match
   {
   public:

      filter_on_wildcard_match(const std::string& match_pattern,
                               OutputPredicate& predicate,
                               bool allow_through_on_match = true)
      : allow_through_on_match_(allow_through_on_match),
        match_pattern_(match_pattern),
        predicate_(predicate)
      {}

      template<typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range) const
      {
         if (match(match_pattern_.begin(),match_pattern_.end(),range.first,range.second,'*','?') ^ allow_through_on_match_)
         {
            predicate_(range);
         }
      }

      inline void operator() (const std::string& s) const
      {
         if (match(match_pattern_,s) ^ allow_through_on_match_)
         {
            predicate_(s);
         }
      }

   private:

      filter_on_wildcard_match(const filter_on_wildcard_match& fom);
      filter_on_wildcard_match operator=(const filter_on_wildcard_match& fom);

      bool allow_through_on_match_;
      std::string match_pattern_;
      OutputPredicate& predicate_;
   };

  template<typename OutputPredicate>
   struct filter_on_match
   {
   public:

     filter_on_match(const std::string* begin, const std::string* end,
                     OutputPredicate predicate,
                     bool case_insensitive,
                     bool allow_through_on_match = true)
     : case_insensitive_(case_insensitive),
       allow_through_on_match_(allow_through_on_match),
       begin_(begin),
       end_(end),
       predicate_(predicate)
     {}

      template<typename Iterator>
      inline void operator() (const std::pair<Iterator,Iterator>& range) const
      {
         for (const std::string* itr = begin_; end_ != itr; ++itr)
         {
            if ((case_insensitive_ &&
               (imatch((*itr).data(),(*itr).data() + (*itr).size(),range.first,range.second))) ||
               (!case_insensitive_ && std::equal((*itr).begin(),(*itr).end(),range.first)))
            {
               if (allow_through_on_match_)
               {
                  predicate_(range);
               }
               return;
            }
         }

         if (!allow_through_on_match_)
         {
            predicate_(range);
            return;
         }
      }

      inline void operator() (const std::string& s) const
      {
         for (const std::string* itr = begin_; end_ != itr; ++itr)
         {
            if ((case_insensitive_ &&
               (imatch((*itr).begin(),(*itr).end(),s.begin(),s.end()))) ||
               (!case_insensitive_ && std::equal((*itr).begin(),(*itr).end(),s.begin())))
            {
               if (allow_through_on_match_)
               {
                  predicate_(s);
                  return;
               }
            }
         }

         if (!allow_through_on_match_)
         {
            predicate_(s);
            return;
         }
      }

   private:

      filter_on_match& operator=(const filter_on_match& fom);

   private:

      bool case_insensitive_;
      bool allow_through_on_match_;
      const std::string* begin_;
      const std::string* end_;
      OutputPredicate& predicate_;
   };

   template<typename Iterator, typename MatchPredicate>
   inline void skip_while_matching(Iterator& itr,
                                   const Iterator& end,
                                   const MatchPredicate& predicate)
   {
      while (end != itr)
      {
         if (predicate(*itr))
            ++itr;
         else
            break;
      }
   }

   template<std::size_t length>
   struct size_equal_to
   {
      template<typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end) const
      {
         return length == std::distance(begin,end);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence) const
      {
         return length == sequence.size();
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::set<T,Comparator,Allocator>& set) const
      {
         return length == set.size();
      }

      inline bool operator()(const std::string& str) const
      {
         return length == str.size();
      }
   };

   template<std::size_t length>
   struct size_less_than
   {
      template<typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end) const
      {
         return std::distance(begin,end) < static_cast<typename std::iterator_traits<Iterator>::difference_type>(length);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence) const
      {
         return sequence.size() < length;
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::set<T,Comparator,Allocator>& set) const
      {
         return set.size() < length;
      }

      inline bool operator()(const std::string& str) const
      {
         return str.size() < length;
      }
   };

   template<std::size_t length>
   struct size_greater_than
   {
      template<typename Iterator>
      inline bool operator()(const Iterator begin, const Iterator end) const
      {
         return std::distance(begin,end) > static_cast<typename std::iterator_traits<Iterator>::difference_type>(length);
      }

      template <typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline bool operator()(const Sequence<T,Allocator>& sequence) const
      {
         return sequence.size() > length;
      }

      template <typename T,
                typename Comparator,
                typename Allocator>
      inline bool operator()(const std::set<T,Comparator,Allocator>& set) const
      {
         return set.size() > length;
      }

      inline bool operator()(const std::string& str) const
      {
         return str.size() > length;
      }
   };

   template<typename InputIterator,
            typename T1, typename  T2, typename  T3, typename T4,
            typename T5, typename  T6, typename  T7, typename T8,
            typename T9, typename T10, typename T11, typename T12>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9& t9, T10& t10, T11& t11, T12& t12)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 12;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second, t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t4)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t5)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t6)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t7)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t8)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t9)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t10)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t11)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t12)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename  T2, typename  T3, typename T4,
            typename T5, typename  T6, typename  T7, typename T8,
            typename T9, typename T10, typename T11>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9, T10& t10, T11& t11)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 11;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second, t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t4)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t5)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t6)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t7)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t8)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t9)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t10)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t11)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9, T10& t10)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 10;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second, t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t4)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t5)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t6)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t7)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t8)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second, t9)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t10)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 9;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second,t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t4)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t5)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t6)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t7)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t8)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t9)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 8;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second,t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t4)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t5)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t6)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t7)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t8)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 7;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second,t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t4)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t5)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t6)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t7)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 6;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second,t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t4)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t5)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t6)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename T2, typename T3, typename T4,
            typename T5>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 5;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second,t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t4)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t5)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename T2, typename T3, typename T4>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 4;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second,t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t3)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t4)) return false;
      return true;
   }

   template<typename InputIterator,
            typename T1, typename T2, typename T3>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 3;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second,t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t2)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t3)) return false;
      return true;
   }

   template<typename InputIterator, typename T1, typename T2>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T1& t1, T2& t2)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 2;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      if (!string_to_type_converter((*itr).first,(*itr).second,t1)) return false; ++itr;
      if (!string_to_type_converter((*itr).first,(*itr).second,t2)) return false;
      return true;
   }

   template<typename InputIterator, typename T>
   inline bool parse(const InputIterator begin,
                     const InputIterator end,
                     const std::string& delimiters,
                     T& t)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      static const std::size_t token_count = 1;
      typedef std::pair<InputIterator,InputIterator> iterator_type;
      typedef iterator_type* iterator_type_ptr;
      iterator_type token_list[token_count];
      const std::size_t parsed_token_count = split_n(delimiters,
                                                     begin,end,
                                                     token_count,
                                                     token_list,
                                                     split_options::compress_delimiters);
      if (token_count != parsed_token_count)
         return false;
      iterator_type_ptr itr = token_list;
      return string_to_type_converter((*itr).first,(*itr).second,t);
   }

   template <typename InputIterator,
             typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            Sequence<T,Allocator>& sequence,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin,end,
                      range_to_type_back_inserter(sequence),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin, end,
                      range_to_type_back_inserter(sequence),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::set<T,Comparator,Allocator>& set,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin,end,
                      range_to_type_inserter(set),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin,end,
                      range_to_type_inserter(set),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::queue<T,Container>& queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin,end,
                      range_to_type_push_inserter(queue),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin,end,
                      range_to_type_push_inserter(queue),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::stack<T,Container>& stack,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin,end,
                      range_to_type_push_inserter(stack),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin, end,
                      range_to_type_push_inserter(stack),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse(const InputIterator begin,
                            const InputIterator end,
                            const std::string& delimiters,
                            std::priority_queue<T,Container,Comparator>& priority_queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                      begin,end,
                      range_to_type_push_inserter(priority_queue),
                      split_option);
      else
         return split(multiple_char_delimiter_predicate(delimiters),
                      begin,end,
                      range_to_type_push_inserter(priority_queue),
                      split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              Sequence<T,Allocator>& sequence,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        begin,end,
                        n,
                        range_to_type_back_inserter(sequence),
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        begin,end,
                        n,
                        range_to_type_back_inserter(sequence),
                        split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::set<T,Comparator,Allocator>& set,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        begin,end,
                        n,
                        range_to_type_inserter(set),
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        begin,end,
                        n,
                        range_to_type_inserter(set),
                        split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::queue<T,Container>& queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        begin,end,
                        n,
                        range_to_type_push_inserter(queue),
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        begin,end,
                        n,
                        range_to_type_push_inserter(queue),
                        split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::stack<T,Container>& stack,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        begin,end,
                        n,
                        range_to_type_push_inserter(stack),
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        begin,end,
                        n,
                        range_to_type_push_inserter(stack),
                        split_option);
   }

   template <typename InputIterator,
             typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse_n(const InputIterator begin,
                              const InputIterator end,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::priority_queue<T,Container,Comparator>& priority_queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      typedef typename details::is_valid_iterator<InputIterator>::type itr_type;
      if (1 == delimiters.size())
         return split_n(single_delimiter_predicate<std::string::value_type>(delimiters[0]),
                        begin,end,
                        n,
                        range_to_type_push_inserter(priority_queue),
                        split_option);
      else
         return split_n(multiple_char_delimiter_predicate(delimiters),
                        begin,end,
                        n,
                        range_to_type_push_inserter(priority_queue),
                        split_option);
   }

   template<typename T1, typename T2, typename T3, typename  T4,
            typename T5, typename T6, typename T7, typename  T8,
            typename T9, typename T10, typename T11, typename T12>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1,  T2&  t2,  T3&  t3,  T4&  t4,
                     T5& t5,  T6&  t6,  T7&  t7,  T8&  t8,
                     T9& t9, T10& t10, T11& t11, T12& t12)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10, typename T11>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2&   t2,  T3&  t3, T4& t4,
                     T5& t5, T6&   t6,  T7&  t7, T8& t8,
                     T9& t9, T10& t10, T11& t11)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9, T10& t10)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9,t10);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8,
                     T9& t9)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8,t9);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7, T8& t8)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7,t8);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6, T7& t7)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6,t7);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5, T6& t6)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5,t6);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4,
                     T5& t5)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4,t5);
   }

   template<typename T1, typename T2, typename T3, typename T4>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3, T4& t4)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3,t4);
   }

   template<typename T1, typename T2, typename T3>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2, T3& t3)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2,t3);
   }

   template<typename T1, typename T2>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T1& t1, T2& t2)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t1,t2);
   }

   template<typename T>
   inline bool parse(const std::string& data,
                     const std::string& delimiters,
                     T& t)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   t);
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            Sequence<T,Allocator>& sequence,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   sequence,
                   split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::set<T,Comparator,Allocator>& set,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   set,
                   split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::queue<T,Container>& queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   queue,
                   split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::stack<T,Container>& stack,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   stack,
                   split_option);
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse(const std::string& data,
                            const std::string& delimiters,
                            std::priority_queue<T,Container,Comparator>& priority_queue,
                            const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse(data.data(),
                   data.data() + data.size(),
                   delimiters,
                   priority_queue,
                   split_option);
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse(int argc,
                            char* argv[],
                            Sequence<T,Allocator>& sequence,
                            const bool break_on_fail = true)
   {
      T tmp;
      for (int i = 0; i < argc; ++i)
      {
         if (!string_to_type_converter(std::string(argv[i]),tmp))
         {
            if (break_on_fail)
               return i;
            else
               continue;
         }
         sequence.push_back(tmp);
      }
      return argc;
   }

   #define strtk_parse_begin(Type)\
   namespace strtk {\
   bool parse(const std::string& data, const std::string& delimiters, Type& t)\
   { return parse(data.data(),data.data() + data.size(),delimiters

   #define strtk_parse_type(T)\
   ,t.T

   #define strtk_parse_hex_type(T)\
   ,t.T

   #define strtk_parse_ignore_token()\
   ,ignore_token()

   #define strtk_parse_end()\
   );}}

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              Sequence<T,Allocator>& sequence,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(data.data(),
                     data.data() + data.size(),
                     delimiters,
                     n,
                     sequence,
                     split_option);
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::set<T,Comparator,Allocator>& set,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(data.data(),
                     data.data() + data.size(),
                     delimiters,
                     n,
                     set,
                     split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::queue<T,Container>& queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(data.data(),
                     data.data() + data.size(),
                     delimiters,
                     n,
                     queue,
                     split_option);
   }

   template <typename T,
             typename Container>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::stack<T,Container>& stack,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(data.data(),
                     data.data() + data.size(),
                     delimiters,
                     n,
                     stack,
                     split_option);
   }

   template <typename T,
             typename Container,
             typename Comparator>
   inline std::size_t parse_n(const std::string& data,
                              const std::string& delimiters,
                              const std::size_t& n,
                              std::priority_queue<T,Container,Comparator>& priority_queue,
                              const split_options::type& split_option = split_options::compress_delimiters)
   {
      return parse_n(data.data(),
                     data.data() + data.size(),
                     delimiters,
                     n,
                     priority_queue,
                     split_option);
   }

   template<typename T1, typename T2, typename  T3, typename  T4,
            typename T5, typename T6, typename  T7, typename  T8,
            typename T9, typename T10, typename T11, typename T12>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const  T2&  t2, const  T3&  t3, const  T4&  t4,
                         const T5& t5, const  T6&  t6, const  T7&  t7, const  T8&  t8,
                         const T9& t9, const T10& t10, const T11& t11, const T12& t12)
   {
      output += type_to_string( t1); output += delimiter;
      output += type_to_string( t2); output += delimiter;
      output += type_to_string( t3); output += delimiter;
      output += type_to_string( t4); output += delimiter;
      output += type_to_string( t5); output += delimiter;
      output += type_to_string( t6); output += delimiter;
      output += type_to_string( t7); output += delimiter;
      output += type_to_string( t8); output += delimiter;
      output += type_to_string( t9); output += delimiter;
      output += type_to_string(t10); output += delimiter;
      output += type_to_string(t11); output += delimiter;
      output += type_to_string(t12);
   }

   template<typename T1, typename T2, typename  T3, typename T4,
            typename T5, typename T6, typename  T7, typename T8,
            typename T9, typename T10, typename T11>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const  T2&  t2, const T3& t3, const T4& t4,
                         const T5& t5, const  T6&  t6, const T7& t7, const T8& t8,
                         const T9& t9, const T10& t10, const T11& t11)
   {
      output += type_to_string( t1); output += delimiter;
      output += type_to_string( t2); output += delimiter;
      output += type_to_string( t3); output += delimiter;
      output += type_to_string( t4); output += delimiter;
      output += type_to_string( t5); output += delimiter;
      output += type_to_string( t6); output += delimiter;
      output += type_to_string( t7); output += delimiter;
      output += type_to_string( t8); output += delimiter;
      output += type_to_string( t9); output += delimiter;
      output += type_to_string(t10); output += delimiter;
      output += type_to_string(t11);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9, const T10& t10)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8); output += delimiter;
      output += type_to_string(t9); output += delimiter;
      output += type_to_string(t10);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                         const T9& t9)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8); output += delimiter;
      output += type_to_string(t9);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7, const T8& t8)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7); output += delimiter;
      output += type_to_string(t8);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6, const T7& t7)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6); output += delimiter;
      output += type_to_string(t7);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5,typename T6>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5, const T6& t6)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5); output += delimiter;
      output += type_to_string(t6);
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                         const T5& t5)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4); output += delimiter;
      output += type_to_string(t5);
   }

   template<typename T1, typename T2, typename T3, typename T4>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3, const T4& t4)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3); output += delimiter;
      output += type_to_string(t4);
   }

   template<typename T1, typename T2, typename T3>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2, const T3& t3)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2); output += delimiter;
      output += type_to_string(t3);
   }

   template<typename T1, typename T2>
   inline void construct(std::string& output,
                         const std::string& delimiter,
                         const T1& t1, const T2& t2)
   {
      output += type_to_string(t1); output += delimiter;
      output += type_to_string(t2);
   }

   template<typename InputIterator>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const InputIterator begin,
                    const InputIterator end)
   {
      InputIterator itr = begin;
      while (end != itr)
      {
         output += type_to_string(*itr);
         if (end == (++itr))
            break;
         else
            output += delimiter;
      }
   }

   template<typename InputIterator>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const std::pair<InputIterator,InputIterator>& range)
   {
      InputIterator itr = range.first;
      while (range.second != itr)
      {
         output += type_to_string(*itr);
         if (range.second == (++itr))
            break;
         else
            output += delimiter;
      }
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const Sequence<T,Allocator>& sequence)
   {
      join(output,delimiter,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline void join(std::string& output,
                    const std::string& delimiter,
                    const std::set<T,Comparator,Allocator>& set)
   {
      join(output,delimiter,set.begin(),set.end());
   }

   inline void join(std::string& output,
                    const std::string& delimiter,
                    int argc, char* argv[])
   {
      for (int i = 0; i < argc; ++i)
      {
         output += argv[i];
         if (i < (argc - 1))
            output += delimiter;
      }
   }

   template<typename InputIterator>
   inline std::string join(const std::string& delimiter,
                           const InputIterator begin,
                           const InputIterator end)
   {
      std::string output;
      output.reserve(one_kilobyte);
      join(output,delimiter,begin,end);
      return output;
   }

   template<typename InputIterator>
   inline std::string join(const std::string& delimiter,
                           const std::pair<InputIterator,InputIterator>& range)
   {
      std::string output;
      output.reserve(one_kilobyte);
      join(output,delimiter,range.first,range.second);
      return output;
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::string join(const std::string& delimiter,
                           const Sequence<T,Allocator>& sequence)
   {
      return join(delimiter,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::string join(const std::string& delimiter,
                           const std::set<T,Comparator,Allocator>& set)
   {
      return join(delimiter,set.begin(),set.end());
   }

   inline std::string join(const std::string& delimiter, int argc, char* argv[])
   {
      std::string result;
      result.reserve(one_kilobyte);
      join(result,delimiter,argc,argv);
      return result;
   }

   template<typename InputIterator, typename Predicate>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const InputIterator begin,
                       const InputIterator end)
   {
      InputIterator itr = begin;
      bool first_time = true;
      while (end != itr)
      {
         if (predicate(*itr))
         {
            if (!first_time)
               output += delimiter;
            else
               first_time = false;
            output += type_to_string(*itr);
         }
         if (end == (++itr))
            break;
      }
   }

   template<typename InputIterator, typename Predicate>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const std::pair<InputIterator,InputIterator>& range)
   {
      InputIterator itr = range.first;
      bool first_time = true;
      while (range.second != itr)
      {
         if (predicate(*itr))
         {
            if (!first_time)
               output += delimiter;
            else
               first_time = false;
            output += type_to_string(*itr);
         }
         if (range.second == (++itr))
            break;
      }
   }

   template <typename T,
             typename Predicate,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const Sequence<T,Allocator>& sequence)
   {
      join(output,delimiter,predicate,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Predicate,
             typename Comparator,
             typename Allocator>
   inline void join_if(std::string& output,
                       const std::string& delimiter,
                       Predicate predicate,
                       const std::set<T,Comparator,Allocator>& set)
   {
      join(output,delimiter,predicate,set.begin(),set.end());
   }

   template<typename InputIterator, typename Predicate>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const InputIterator begin,
                              const InputIterator end)
   {
      std::string output;
      output.reserve(one_kilobyte);
      join_if(output,delimiter,predicate,begin,end);
      return output;
   }

   template<typename InputIterator, typename Predicate>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const std::pair<InputIterator,InputIterator>& range)
   {
      std::string output;
      output.reserve(one_kilobyte);
      join_if(output,delimiter,predicate,range.first,range.second);
      return output;
   }

   template <typename T,
             typename Predicate,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const Sequence<T,Allocator>& sequence)
   {
      return join(delimiter,predicate,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Predicate,
             typename Comparator,
             typename Allocator>
   inline std::string join_if(const std::string& delimiter,
                              Predicate predicate,
                              const std::set<T,Comparator,Allocator>& set)
   {
      return join(delimiter,predicate,set.begin(),set.end());
   }

   class build_string
   {
   public:

      build_string(const std::size_t& initial_size = 64)
      {
         data_.reserve(initial_size);
      }

      template <typename T>
      inline build_string& operator << (const T& t)
      {
         data_ += type_to_string(t);
         return (*this);
      }

      inline operator std::string () const
      {
         return data_;
      }

      inline operator const char* () const
      {
         return data_.c_str();
      }

   private:

      std::string data_;
   };

   inline void replicate(const std::size_t& n,
                         const std::string& str,
                         std::string& output)
   {
      if (0 == n) return;
      output.reserve(output.size() + (str.size() * n));
      for (std::size_t i = 0; i < n; ++i)
      {
         output.append(str);
      }
   }

   inline std::string replicate(const std::size_t& n,
                                const std::string& str)
   {
      std::string output;
      replicate(n,str,output);
      return output;
   }

   inline void replicate_inplace(const std::size_t& n,
                                 std::string& str)
   {
      std::string temp_str = str;
      str.reserve(str.size() + (str.size() * n));

      for (std::size_t i = 0; i < n; ++i)
      {
         str.append(temp_str);
      }
   }

   template<typename InputIterator>
   inline void bracketize(std::string& output,
                          const std::string& pre,
                          const std::string& post,
                          const InputIterator begin,
                          const InputIterator end)
   {
      InputIterator itr = begin;
      std::string s;
      s.reserve(one_kilobyte);
      while (end != itr)
      {
         s.clear();
         s.append(pre);
         s.append(type_to_string(*itr));
         s.append(post);
         output.append(s);
         ++itr;
      }
   }

   template <typename T,
             typename Allocator,
             template <typename,typename> class Sequence>
   inline void bracketize(std::string& output,
                          const std::string& pre,
                          const std::string& post,
                          Sequence<T,Allocator>& sequence)
   {
      bracketize(output,pre,post,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline void bracketize(std::string& output,
                          const std::string& pre,
                          const std::string& post,
                          std::set<T,Comparator,Allocator>& set)
   {
      bracketize(output,pre,post,set.begin(),set.end());
   }

   template<typename InputIterator>
   inline std::string bracketize(const std::string& pre,
                                 const std::string& post,
                                 const InputIterator begin,
                                 const InputIterator end)
   {
      std::string output;
      output.reserve(one_kilobyte);
      bracketize(output,pre,post,begin,end);
      return output;
   }

   template <typename T,
            typename Allocator,
            template <typename,typename> class Sequence>
   inline std::string bracketize(const std::string& pre,
                                 const std::string& post,
                                 Sequence<T,Allocator>& sequence)
   {
      return bracketize(pre,post,sequence.begin(),sequence.end());
   }

   template <typename T,
             typename Comparator,
             typename Allocator>
   inline std::string bracketize(const std::string& pre,
                                 const std::string& post,
                                 std::set<T,Comparator,Allocator>& set)
   {
      return bracketize(pre,post,set.begin(),set.end());
   }

   template<typename T>
   struct interval_inserter
   {
      typedef T type;

      interval_inserter(const std::size_t& interval, const T& t)
      : count_(0),
        interval_(interval),
        t_(t)
      {}

      inline bool operator()(const type&)
      {
         if (++count_ == interval_)
         {
            count_ = 0;
            return true;
         }
         else
            return false;
      }

      inline T operator()()
      {
         return t_;
      }

   private:

      std::size_t count_;
      std::size_t interval_;
      T t_;
   };

   template<typename Inserter,
            typename InputIterator,
            typename OutputIterator>
   inline std::size_t inserter(Inserter ins,
                               const InputIterator begin, const InputIterator end,
                               OutputIterator out)
   {
      std::size_t size = 0;
      InputIterator itr = begin;
      while (end != itr)
      {
         *out++ = *itr;
         if (ins(*itr++))
         {
            *out++ = ins();
            size += 2;
         }
         else
            ++size;
      }
      return size;
   }

   template <typename Iterator, typename T>
   inline void iota(Iterator begin, Iterator end, T value)
   {
      Iterator itr = begin;
      while (end != itr)
      {
         *(itr++) = value++;
      }
   }

   template<typename T>
   inline void iota(typename range::adapter<T>& r, T value)
   {
      iota(r.begin(),r.end(),value);
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline void iota(Sequence<T,Allocator>& sequence, std::size_t count, T value)
   {
      while (count)
      {
         sequence.push_back(value++);
         --count;
      }
   }

   template<typename T,
            typename Comparator,
            typename Allocator>
   inline void iota(std::set<T,Comparator,Allocator>& set, std::size_t count, T value)
   {
      while (count)
      {
         set.insert(value++);
         --count;
      }
   }

   template<typename OutputIterator, typename T>
   inline void iota(std::size_t count, T value, OutputIterator out)
   {
      while (count)
      {
         *out++ = value++;
         --count;
      }
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline void iota(Sequence<T,Allocator>& sequence, const T& value)
   {
      iota(sequence.begin(),sequence.end(),value);
   }

   template<typename T,
            typename Comparator,
            typename Allocator>
   inline void iota(std::set<T,Comparator,Allocator>& set, const T& value)
   {
      iota(set.begin(),set.end(),value);
   }

   template<typename InputIterator, typename OutputIterator>
   inline void cut(const std::size_t& r0, const std::size_t& r1,
                   const InputIterator begin, InputIterator end,
                   OutputIterator out)
   {
      // static assert: InputIterator must be of type std::string
      InputIterator itr = begin;
      while (end != itr)
      {
         const std::string& s = *itr++;
         if (s.size() < r0)
            continue;
         (*out++) = s.substr(r0,std::min(r1,s.size()) - r0);
      }
   }

   template<typename Allocator,
            template<typename,typename> class Sequence,
            typename OutputIterator>
   inline void cut(const std::size_t& r0, const std::size_t& r1,
                   const Sequence<std::string, Allocator>& sequence,
                   OutputIterator out)
   {
      cut(r0,r1,sequence.begin(),sequence.end(),out);
   }

   template<typename Iterator>
   inline void cut_inplace(const std::size_t& r0, const std::size_t& r1,
                           const Iterator begin, const Iterator end)
   {
      // static assert: InputIterator must be of type std::string
      Iterator itr = begin;
      while (end != itr)
      {
         if ((*itr).size() >= r0)
         {
            (*itr) = (*itr).substr(r0,std::min(r1,(*itr).size()) - r0);
         }
         ++itr;
      }
   }

   template<typename Allocator,
            template<typename,typename> class Sequence>
   inline void cut(const std::size_t& r0, const std::size_t& r1,
                   const Sequence<std::string, Allocator>& sequence)
   {
      cut(r0,r1,sequence.begin(),sequence.end());
   }

   template<typename Comparator,
            typename Allocator>
   inline void cut(const std::size_t& r0, const std::size_t& r1,
                   const std::set<std::string, Comparator, Allocator>& set)
   {
      cut(r0,r1,set.begin(),set.end());
   }

   #ifdef strtk_enable_random
   inline void generate_random_data(unsigned char* data,
                                    std::size_t length,
                                    unsigned int pre_gen_cnt = 0,
                                    unsigned int seed = magic_seed)
   {
      boost::mt19937 rng(static_cast<boost::mt19937::result_type>(seed));
      boost::uniform_int<unsigned int> dist(std::numeric_limits<unsigned int>::min(),std::numeric_limits<unsigned int>::max());
      boost::variate_generator<boost::mt19937&, boost::uniform_int<unsigned int> > rnd(rng,dist);

      if (pre_gen_cnt > 0)
      {
         unsigned int r = 0x00;
         for (unsigned int i = 0; i < pre_gen_cnt; r = rnd(), ++i) ;
      }

      unsigned char* itr = data;
      unsigned int* x = 0;
      while (length >= sizeof(unsigned int))
      {
         x = reinterpret_cast<unsigned int*>(itr);
         (*x) = rnd();
         itr += sizeof(unsigned int);
         length -= sizeof(unsigned int);
      }

      if (length > 0)
      {
         itr -= (sizeof(unsigned int) - length);
         x = reinterpret_cast<unsigned int*>(itr);
         (*x) = rnd();
      }
   }

   namespace details
   {
      struct rand_int_type_tag {};
      struct rand_real_type_tag {};

      template<typename T> struct supported_random_type {};

      #define strtk_register_rand_int_type_tag(T)\
      template<> struct supported_random_type<T> { typedef rand_int_type_tag type;  enum { value = true }; };

      #define strtk_register_rand_real_type_tag(T)\
      template<> struct supported_random_type<T> { typedef rand_real_type_tag type;  enum { value = true }; };

      strtk_register_rand_int_type_tag(short)
      strtk_register_rand_int_type_tag(int)
      strtk_register_rand_int_type_tag(long)
      strtk_register_rand_int_type_tag(unsigned short)
      strtk_register_rand_int_type_tag(unsigned int)
      strtk_register_rand_int_type_tag(unsigned long)

      strtk_register_rand_real_type_tag(float)
      strtk_register_rand_real_type_tag(double)
      strtk_register_rand_real_type_tag(long double)

      template<typename T, typename OutputIterator, typename RandomNumberGenerator>
      inline void generate_random_values_impl(const std::size_t& count,
                                              const T& min,
                                              const T& max,
                                              OutputIterator out,
                                              RandomNumberGenerator& rng,
                                              rand_int_type_tag)
      {
         // Note: The implied range will be: [min,max]
         using namespace boost;
         variate_generator<RandomNumberGenerator&,uniform_int<T> > rnd(rng,uniform_int<T>(min,max));
         for (std::size_t i = 0; i < count; ++i, *out++ = rnd()) ;
      }

      template<typename T, typename OutputIterator, typename RandomNumberGenerator>
      inline void generate_random_values_impl(const std::size_t& count,
                                              const T& min,
                                              const T& max,
                                              OutputIterator out,
                                              RandomNumberGenerator& rng,
                                              rand_real_type_tag)
      {
         // Note: The implied range will be: [min,max)
         using namespace boost;
         variate_generator<RandomNumberGenerator&, uniform_real<T> > rnd(rng,uniform_real<T>(min,max));
         for (std::size_t i = 0; i < count; ++i, *out++ = rnd()) ;
      }

   } // namespace details

   class uniform_real_rng
   {
      typedef boost::mt19937 rng_type;
      typedef boost::variate_generator<rng_type, boost::uniform_real<double> > variate_type;

   public:

      uniform_real_rng(const std::size_t& seed = magic_seed,
                       std::size_t pregen = 0)
      : rng_(static_cast<rng_type::result_type>(seed)),
        rnd_(rng_,boost::uniform_real<double>(0.0,1.0))
      {
         while (pregen--) rng_();
      }

      inline double operator()()
      {
         return rnd_();
      }

   private:

      rng_type rng_;
      variate_type rnd_;
   };

   template<typename T,
            typename OutputIterator>
   inline void generate_random_values(const std::size_t& count,
                                      const T& min,
                                      const T& max,
                                      OutputIterator out,
                                      const std::size_t& seed = magic_seed,
                                      const std::size_t& pregen = 0)
   {
      typename details::supported_random_type<T>::type type;
      boost::mt19937 rng(static_cast<boost::mt19937::result_type>(seed));
      for (std::size_t i = 0; i++ < pregen; rng()) ;
      generate_random_values_impl(count,min,max,out,rng,type);
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline void generate_random_values(const std::size_t& count,
                                      const T& min,
                                      const T& max,
                                      Sequence<T,Allocator>& sequence,
                                      const std::size_t& seed = magic_seed,
                                      const std::size_t& pregen = 0)
   {
      typename details::supported_random_type<T>::type type;
      boost::mt19937 rng(static_cast<boost::mt19937::result_type>(seed));
      for (std::size_t i = 0; i++ < pregen; rng()) ;
      generate_random_values_impl(count,min,max,std::back_inserter(sequence),rng,type);
   }

   template <typename Iterator,
             typename RandomNumberGenerator,
             typename OutputIterator>
   inline void random_permutation(const Iterator begin, const Iterator end,
                                  RandomNumberGenerator& rng,
                                  OutputIterator out)
   {
      const std::size_t size = std::distance(begin,end);
      if ((rng. min() < 0.0) || (rng.max() > 1.0)) return;
      std::deque<std::size_t> index;
      for (std::size_t i = 0; i < size; index.push_back(i++)) ;
      while (!index.empty())
      {
         std::size_t idx = static_cast<std::size_t>(index.size() * rng());
         *(out++) = *(begin + index[idx]);
         index.erase(index.begin() + idx);
      }
   }

   template <typename Iterator,
             typename OutputIterator>
   inline void random_permutation(const Iterator begin, const Iterator end,
                                  OutputIterator out,
                                  const std::size_t& seed = magic_seed,
                                  const std::size_t& pregen = 0)
   {
      boost::mt19937 rng(static_cast<boost::mt19937::result_type>(seed));
      for (std::size_t i = 0; i++ < pregen; rng()) ;
      boost::uniform_real<double> dist(0.0,1.0);
      boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > rnd(rng,dist);
      random_permutation(begin,end,rnd,out);
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence,
            typename OutputIterator>
   inline void random_permutation(const Sequence<T,Allocator>& sequence,
                                  OutputIterator out,
                                  const std::size_t& seed = magic_seed,
                                  const std::size_t& pregen = 0)
   {
      random_permutation(sequence.begin(),sequence.end(),out,seed,pregen);
   }

   template <typename Iterator,
             typename RandomNumberGenerator,
             typename OutputIterator>
   inline bool random_combination(const Iterator begin, const Iterator end,
                                  std::size_t set_size,
                                  RandomNumberGenerator& rng,
                                  OutputIterator out)
   {
      const std::size_t size = std::distance(begin,end);
      if ((size < set_size) || (rng. min() < 0.0) || (rng.max() > 1.0)) return false;
      std::deque<std::size_t> index;
      for (std::size_t i = 0; i < size; index.push_back(i++)) ;
      while (set_size)
      {
         std::size_t idx = static_cast<std::size_t>(index.size() * rng());
         *(out++) = *(begin + index[idx]);
         index.erase(index.begin() + idx);
         --set_size;
      }
      return true;
   }

   template <typename Iterator,
             typename OutputIterator>
   inline void random_combination(const Iterator begin, const Iterator end,
                                  const std::size_t& set_size,
                                  OutputIterator out,
                                  const std::size_t& seed = magic_seed,
                                  const std::size_t& pregen = 0)
   {
      boost::mt19937 rng(static_cast<boost::mt19937::result_type>(seed));
      for (std::size_t i = 0; i++ < pregen; rng()) ;
      boost::uniform_real<double> dist(0.0,1.0);
      boost::variate_generator<boost::mt19937&, boost::uniform_real<double> > rnd(rng,dist);
      random_combination(begin,end,set_size,rnd,out);
   }

   template <typename T,
             typename Allocator,
             template<typename,typename> class Sequence,
             typename OutputIterator>
   inline void random_combination(const Sequence<T,Allocator>& sequence,
                                  const std::size_t& set_size,
                                  OutputIterator out,
                                  const std::size_t& seed = magic_seed,
                                  const std::size_t& pregen = 0)
   {
      random_combination(sequence.begin(),sequence.end(),set_size,out,seed,pregen);
   }
   #endif // strtk_enable_random

   template <typename Iterator>
   inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
   {
      /* Credits: Thomas Draper */
      if ((first == last) || (first == k) || (last == k))
         return false;
      Iterator itr1 = first;
      Iterator itr2 = last;
      ++itr1;
      if (last == itr1)
         return false;
      itr1 = last;
      --itr1;
      itr1 = k;
      --itr2;
      while (first != itr1)
      {
         if (*--itr1 < *itr2)
         {
            Iterator j = k;
            while (!(*itr1 < *j)) ++j;
            std::iter_swap(itr1,j);
            ++itr1;
            ++j;
            itr2 = k;
            std::rotate(itr1,j,last);
            while (last != j)
            {
               ++j;
               ++itr2;
            }
            std::rotate(k,itr2,last);
            return true;
         }
      }
      std::rotate(first,k,last);
      return false;
   }

   template <typename T,
             typename Allocator,
             template<typename,typename> class Sequence>
   inline bool next_combination(Sequence<T,Allocator>& sequence, const std::size_t& size)
   {
      return next_combination(sequence.begin(), sequence.begin() + size, sequence.end());
   }

   template<typename Iterator, typename Function>
   inline void for_each_permutation(Iterator begin, Iterator end, Function function)
   {
      do
      {
         function(begin,end);
      }
      while (std::next_permutation(begin,end));
   }

   template<typename Iterator, typename Function>
   inline bool for_each_permutation_conditional(Iterator begin, Iterator end, Function function)
   {
      do
      {
         if (!function(begin,end))
            return false;
      }
      while (std::next_permutation(begin,end));
      return true;
   }

   template<typename Iterator, typename Function>
   inline void for_each_combination(Iterator begin, Iterator end, const std::size_t& size, Function function)
   {
      if (static_cast<typename std::iterator_traits<Iterator>::difference_type>(size) > std::distance(begin,end))
         return;
      do
      {
         function(begin,begin + size);
      }
      while (next_combination(begin,begin + size,end));
   }

   template<typename Iterator, typename Function>
   inline bool for_each_combination_conditional(Iterator begin, Iterator end, const std::size_t& size, Function function)
   {
      if (static_cast<typename std::iterator_traits<Iterator>::difference_type>(size) > std::distance(begin,end))
         return false;
      do
      {
         if (!function(begin,begin + size))
            return false;
      }
      while (next_combination(begin,begin + size,end));
      return true;
   }

   template<typename Iterator, typename Function>
   inline void for_each_combutation(Iterator begin, Iterator end, const std::size_t& size, Function function)
   {
      if (static_cast<typename std::iterator_traits<Iterator>::difference_type>(size) > std::distance(begin,end))
         return;
      // for each permutation of each combination
      do
      {
         do
         {
            function(begin,begin + size);
         }
         while (std::next_permutation(begin,begin + size));
      }
      while (next_combination(begin,begin + size,end));
   }

   template<typename Iterator, typename Function>
   inline bool for_each_combutation_conditional(Iterator begin, Iterator end, const std::size_t& size, Function function)
   {
      if (static_cast<typename std::iterator_traits<Iterator>::difference_type>(size) > std::distance(begin,end))
         return false;
      do
      {
         do
         {
            if (!function(begin,begin + size))
               return false;
         }
         while (std::next_permutation(begin,begin + size));
      }
      while (next_combination(begin,begin + size,end));
      return true;
   }

   template<typename Iterator>
   class combination_iterator : public std::iterator<std::forward_iterator_tag,
                                                     std::pair<Iterator,Iterator>,
                                                     void,
                                                     void>
   {
   public:

      typedef Iterator iterator;
      typedef const iterator const_iterator;
      typedef std::pair<Iterator,Iterator> range_type;

      explicit inline combination_iterator(const std::size_t& k,
                                           iterator begin, iterator end,
                                           const bool sorted = true)
      : begin_(begin),
        end_(end),
        middle_(begin + k),
        current_combination_(begin_,middle_)
      {
         if (!sorted)
         {
            std::sort(begin,end);
         }
      }

      template<typename T,
               typename Allocator,
               template<typename, typename> class Sequence>
      explicit inline combination_iterator(const std::size_t& k,
                                           Sequence<T,Allocator>& seq,
                                           const bool sorted = true)
      : begin_(seq.begin()),
        end_(seq.end()),
        middle_(begin_ + k),
        current_combination_(begin_,middle_)
      {
         if (!sorted)
         {
            std::sort(begin_,end_);
         }
      }

      inline combination_iterator(iterator end)
      : begin_(end),
        end_(end),
        middle_(end),
        current_combination_(end,end)
      {}

      template<typename T,
               typename Allocator,
               template<typename, typename> class Sequence>
      explicit inline combination_iterator(Sequence<T,Allocator>& seq)
      : begin_(seq.end()),
        end_(seq.end()),
        middle_(end_),
        current_combination_(end_,end_)
      {}

      inline combination_iterator& operator++()
      {
         if (begin_ != end_)
         {
            if (!next_combination(begin_,middle_,end_))
            {
               begin_ = middle_ = end_;
            }
         }
         return (*this);
      }

      inline combination_iterator operator++(int)
      {
         combination_iterator tmp = *this;
         this->operator++();
         return tmp;
      }

      inline combination_iterator& operator+=(const int inc)
      {
         if (inc > 0)
         {
            for (int i = 0; i < inc; ++i, ++(*this)) ;
         }
         return (*this);
      }

      inline range_type operator*() const
      {
         return current_combination_;
      }

      inline bool operator==(const combination_iterator& itr) const
      {
         return (begin_  == itr.begin_) &&
                (end_    == itr.end_) &&
                (middle_ == itr.middle_);
      }

      inline bool operator!=(const combination_iterator& itr) const
      {
         return !operator==(itr);
      }

   protected:

      iterator begin_;
      iterator end_;
      iterator middle_;
      range_type current_combination_;
   };

   namespace binary
   {
      class ushort_string_adptr
      {
      public:

         ushort_string_adptr(std::string& str)
         : s(str)
         {}
         std::string& s;
     private:
        ushort_string_adptr operator=(const ushort_string_adptr&);
      };

      class reader
      {
      private:

         // should be sourced from cstdint
         typedef unsigned int uint32_t;
         typedef unsigned short uint16_t;

      public:

         template<typename T>
         reader(T* buffer,
                const std::size_t& buffer_length)
         : original_buffer_(reinterpret_cast<char*>(buffer)),
           buffer_(reinterpret_cast<char*>(buffer)),
           buffer_length_(buffer_length * sizeof(T)),
           read_buffer_size_(0)
         {}

         inline bool operator!() const
         {
            return (0 == buffer_length_)   ||
                   (0 == original_buffer_) ||
                   (0 == buffer_);
         }

         inline void reset()
         {
            read_buffer_size_ = 0;
            buffer_ = original_buffer_;
         }

         inline void clear()
         {
            reset();
            std::memset(buffer_,0x00,buffer_length_);
         }

         inline std::size_t read_size() const
         {
            return read_buffer_size_;
         }

         template<typename T>
         inline bool operator()(T*& data, uint32_t& length)
         {
            if (!operator()(length))
               return false;

            const std::size_t raw_size = length * sizeof(T);
            if (!buffer_capacity_ok(raw_size))
               return false;

            (*data) = new T[length];
            std::copy(buffer_, buffer_ + raw_size, reinterpret_cast<char*>(*data));
            buffer_ += raw_size;
            read_buffer_size_ += raw_size;
            return true;
         }

         inline bool operator()(std::string& output)
         {
            uint32_t length = 0;
            if (!operator()(length))
               return false;

            if (!buffer_capacity_ok(length))
               return false;

            output.resize(length);
            std::copy(buffer_,
                      buffer_ + length,
                      const_cast<char*>(output.c_str()));
            buffer_ += length;
            read_buffer_size_ += length;
            return true;
         }

         inline bool operator()(ushort_string_adptr& output)
         {
            uint16_t size = 0;
            if (!operator()(size))
               return false;
            if (!buffer_capacity_ok(size))
               return false;
            output.s.resize(size);
            std::copy(buffer_,
                      buffer_ + size,
                      const_cast<char*>(output.s.c_str()));
            buffer_ += size;
            read_buffer_size_ += size;
            return true;
         }

         template<typename T>
         inline bool operator()(T* data, const std::size_t& length)
         {
            const std::size_t raw_size = length * sizeof(T);
            if (!buffer_capacity_ok(raw_size))
               return false;

            std::copy(buffer_,
                      buffer_ + raw_size,
                      reinterpret_cast<char*>(data));
            buffer_ += length;
            read_buffer_size_ += length;
            return true;
         }

         template<typename T1, typename T2>
         inline bool operator()(std::pair<T1,T2>& p)
         {
            if (!operator()(p.first))
               return false;
            if (!operator()(p.second))
               return false;
            return true;
         }

         template<typename T,
                  typename Allocator,
                  template<typename, typename> class Sequence>
         inline bool operator()(Sequence<T,Allocator>& seq)
         {
            uint32_t size = 0;
            if (!read_pod(size))
               return false;

            const std::size_t raw_size = size * sizeof(T);
            if (!buffer_capacity_ok(raw_size))
               return false;

            T t = T();

            for (std::size_t i = 0; i < size; ++i)
            {
               if (!operator()(t))
                  return false;
               seq.push_back(t);
            }
            return true;
         }

         template<typename T, typename Allocator>
         inline bool operator()(std::vector<T,Allocator>& vec)
         {
            uint32_t size = 0;
            if (!read_pod(size))
               return false;
            vec.reserve(size);

            const std::size_t raw_size = size * sizeof(T);
            if (!buffer_capacity_ok(raw_size))
               return false;

            T t;
            for (std::size_t i = 0; i < size; ++i)
            {
               if (!operator()(t))
                  return false;
               vec.push_back(t);
            }

            return true;
         }

         template<typename T,
                  typename Allocator,
                  typename Comparator>
         inline bool operator()(std::set<T,Allocator,Comparator>& set)
         {
            uint32_t size = 0;
            if (!read_pod(size))
               return false;

            const std::size_t raw_size = size * sizeof(T);
            if (!buffer_capacity_ok(raw_size))
               return false;

            T t;
            for (std::size_t i = 0; i < size; ++i)
            {
               if (!operator()(t))
                  return false;
               set.insert(t);
            }

            return true;
         }

         inline bool operator()(std::ifstream& stream, const std::size_t& length)
         {
            if (length > buffer_length_) return false;
            stream.read(original_buffer_,static_cast<std::streamsize>(length));
            return true;
         }

         inline bool operator()(std::ifstream& stream)
         {
            if (0 == read_buffer_size_) return false;
            stream.read(original_buffer_,static_cast<std::streamsize>(read_buffer_size_));
            return true;
         }

         template<typename T>
         inline bool operator()(T& output)
         {
            return selector<T,typename details::is_pod<T>::result_t>::run(*this,output);
         }

      private:

         reader();
         reader(const reader& s);
         reader& operator=(const reader& s);

         inline bool buffer_capacity_ok(const std::size_t& required_read_qty)
         {
            return ((required_read_qty + read_buffer_size_) < buffer_length_);
         }

         template<typename T, typename IsPOD>
         struct selector
         {
            template<typename Reader>
            static inline bool run(Reader& r, T& t)
            {
               return t(r);
            }
         };

         template<typename T>
         struct selector<T,details::yes_t>
         {
            template<typename Reader>
            static inline bool run(Reader& r, T& t)
            {
               return r.read_pod(t);
            }
         };

         template<typename T>
         inline bool read_pod(T& data)
         {
            static const std::size_t data_length = sizeof(T);
            if (!buffer_capacity_ok(data_length))
               return false;
            data = (*reinterpret_cast<T*>(buffer_));
            buffer_ += data_length;
            read_buffer_size_ += data_length;
            return true;
         }

         char* const original_buffer_;
         char* buffer_;
         std::size_t buffer_length_;
         std::size_t read_buffer_size_;
      };

      class writer
      {
      private:

         // should be sourced from cstdint
         typedef unsigned int uint32_t;
         typedef unsigned short uint16_t;

      public:

         template<typename T>
         writer(T* buffer, const std::size_t& buffer_length)
         : original_buffer_(reinterpret_cast<char*>(buffer)),
           buffer_(reinterpret_cast<char*>(buffer)),
           buffer_length_(buffer_length * sizeof(T)),
           written_buffer_size_(0)
         {}

         inline bool operator!() const
         {
            return (0 == buffer_length_)   ||
                   (0 == original_buffer_) ||
                   (0 == buffer_);
         }

         inline void reset()
         {
            written_buffer_size_ = 0;
            buffer_ = original_buffer_;
         }

         inline void clear()
         {
            reset();
            std::memset(buffer_,0x00,buffer_length_);
         }

         inline std::size_t write_size() const
         {
            return written_buffer_size_;
         }

         template<typename T>
         inline bool operator()(const T* data, const uint32_t& length)
         {
            if (!operator()(length))
               return false;

            const std::size_t raw_size = length * sizeof(T);
            if (!buffer_capacity_ok(raw_size))
               return false;

            const char* ptr = reinterpret_cast<const char*>(data);
            std::copy(ptr, ptr + raw_size, buffer_);
            buffer_ += raw_size;
            written_buffer_size_ += raw_size;

            return true;
         }

         template<typename T1, typename T2>
         inline bool operator()(const std::pair<T1,T2>& p)
         {
            if (!operator()(p.first))
               return false;
            if (!operator()(p.second))
               return false;
            return true;
         }

         inline bool operator()(const std::string& input)
         {
            return operator()(input.data(),input.size());
         }

         inline bool operator()(const ushort_string_adptr& input)
         {
            if (input.s.size() > std::numeric_limits<uint16_t>::max())
               return false;

            const uint16_t size = static_cast<unsigned short>(input.s.size());
            if (!operator()(size))
               return false;

            const std::size_t raw_size = size * sizeof(std::string::value_type);
            if (!buffer_capacity_ok(raw_size))
               return false;

            const char* ptr = input.s.data();
            std::copy(ptr, ptr + raw_size, buffer_);
            buffer_ += raw_size;
            written_buffer_size_ += raw_size;
            return true;
         }

         template<typename T,
                  typename Allocator,
                  template<typename, typename> class Sequence>
         inline bool operator()(const Sequence<T,Allocator>& seq)
         {
            const uint32_t size = seq.size();
            if (!operator()(size))
               return false;

            const std::size_t raw_size = size * sizeof(T);
            if (!buffer_capacity_ok(raw_size))
               return false;

            typename Sequence<T,Allocator>::const_iterator itr = seq.begin();
            typename Sequence<T,Allocator>::const_iterator end = seq.end();
            while (end != itr)
            {
               if (!operator()(*itr))
                  return false;
               ++itr;
            }
            return true;
         }

         template<typename T,
                  typename Allocator,
                  typename Comparator>
         inline bool operator()(const std::set<T,Allocator,Comparator>& set)
         {
            const uint32_t size = set.size();
            if (!operator()(size))
               return false;

            const std::size_t raw_size = size * sizeof(T);
            if (!buffer_capacity_ok(raw_size))
               return false;

            typename std::set<T,Allocator,Comparator>::const_iterator itr = set.begin();
            typename std::set<T,Allocator,Comparator>::const_iterator end = set.end();
            while (end != itr)
            {
               if (!operator()(*itr))
                  return false;
               ++itr;
            }
            return true;
         }

         inline std::size_t operator()(std::ofstream& stream)
         {
            stream.write(original_buffer_,static_cast<std::streamsize>(written_buffer_size_));
            return written_buffer_size_;
         }

         template<typename T>
         inline bool operator()(const T& input)
         {
            return selector<T,typename details::is_pod<T>::result_t>::run(*this,input);
         }

      private:

         writer();
         writer(const writer& s);
         writer& operator=(const writer& s);

         inline bool buffer_capacity_ok(const std::size_t& required_read_qty)
         {
            return ((required_read_qty + written_buffer_size_) < buffer_length_);
         }

         template<typename T, typename IsPOD>
         struct selector
         {
            template<typename Writer>
            static inline bool run(Writer& w, const T& t)
            {
               return t(w);
            }
         };

         template<typename T>
         struct selector<T,details::yes_t>
         {
            template<typename Writer>
            static inline bool run(Writer& w, const T& t)
            {
               return w.write_pod(t);
            }
         };

         template<typename T>
         inline bool write_pod(const T& data)
         {
            static const std::size_t data_length = sizeof(T);
            if ((data_length + written_buffer_size_) > buffer_length_)
            {
               return false;
            }
            *(reinterpret_cast<T*>(buffer_)) = data;
            buffer_ += data_length;
            written_buffer_size_ += data_length;
            return true;
         }

         char* const original_buffer_;
         char* buffer_;
         std::size_t buffer_length_;
         std::size_t written_buffer_size_;
      };

      #define strtk_binary_reader_begin()\
      bool operator()(strtk::binary::reader& reader)\
      { return true\

      #define strtk_binary_reader(T)\
      && reader(T)\

      #define strtk_binary_reader_end()\
      ;}\

      #define strtk_binary_writer_begin()\
      bool operator()(strtk::binary::writer& writer) const\
      { return true\

      #define strtk_binary_writer(T)\
      && writer(T)\

      #define strtk_binary_writer_end()\
      ;}\

   } // namespace binary

   class ignore_token
   {
   public:

      template<typename InputIterator>
      inline ignore_token& operator=(const std::pair<InputIterator,InputIterator>&)
      {
         return (*this);
      }

      inline ignore_token& operator=(const std::string&)
      {
         return (*this);
      }
   };

   template<typename T>
   class hex_to_number_sink
   {
      // static_assert for T either int or unsigned int and alike (could use a concept)
      class hex_value_check
      {
      public:

         inline bool operator()(const unsigned char c) const
         {
            return (('0' <= c) && (c <= '9')) ||
                   (('A' <= c) && (c <= 'F')) ||
                   (('a' <= c) && (c <= 'f'));
         }

         inline bool operator()(const char c) const
         {
            return (*this)(static_cast<unsigned char>(c));
         }
      };

   public:

      hex_to_number_sink(T& t)
      : valid_(false),
        t_(&t)
      {}

      hex_to_number_sink(const hex_to_number_sink& hns)
      : valid_(hns.valid),
        t_(hns.t_)
      {}

      inline hex_to_number_sink& operator=(const hex_to_number_sink& hns)
      {
         valid_ = hns.valid_;
         t_ = hns.t_;
         return (*this);
      }

      template<typename InputIterator>
      inline hex_to_number_sink& operator=(const std::pair<InputIterator,InputIterator>& s)
      {
         std::size_t offset = 0;
         const std::size_t size = std::distance(s.first,s.second);
         if ((size > 2) && ((*s.first) == '0') && (((*s.first + 1) == 'x') || ((*s.first + 1) == 'X')))
            offset = 2;
         if ((size - offset) > (2 * sizeof(T)))
               return (*this);

         const std::size_t buffer_size = 2 * sizeof(T);
         const std::size_t buffer_offset = ((size - offset) % 2);
         char buffer[buffer_size] = { '0' };
         if (!range_only_contains(hex_value_check(),s.first + offset,s.first + size))
         {
            valid_ = false;
            return (*this);
         }

         std::copy(s.first + offset, s.first + size, buffer + buffer_offset);
         *t_ = 0;
         valid_= convert_hex_to_bin(buffer,
                                    buffer + (size - offset) + buffer_offset,
                                    reinterpret_cast<char*>(t_));
         reverse_bytes();
         return (*this);
      }

      inline hex_to_number_sink& operator=(const std::string& s)
      {
         return this->operator =(std::make_pair(s.data(),s.data() + s.size()));
      }

      inline bool valid() const
      {
         return valid_;
      }

   private:

      inline void reverse_bytes()
      {
         unsigned char* itr1 = reinterpret_cast<unsigned char*>(t_);
         unsigned char* itr2 = itr1 + (sizeof(T) - 1);
         while (itr1 < itr2)
         {
            std::swap(*itr1,*itr2);
            ++itr1;
            --itr2;
         }
      }

   private:

      bool valid_;
      T* t_;
   };

   template<typename T>
   class base64_to_number_sink
   {
      // static_assert for T either int or unsigned int and alike (could use a concept)
   private:

      class base64_value_check
      {
      public:

         inline bool operator()(const unsigned char c) const
         {
            return (('0' <= c) && (c <= '9')) ||
                   (('A' <= c) && (c <= 'Z')) ||
                   (('a' <= c) && (c <= 'z')) ||
                    ('+' == c) ||
                    ('/' == c) ||
                    ('=' == c);
         }

         inline bool operator()(const char c) const
         {
            return (*this)(static_cast<unsigned char>(c));
         }
      };

   public:

      base64_to_number_sink(T& t)
      : valid_(false),
        t_(&t)
      {}

      base64_to_number_sink(const base64_to_number_sink& bns)
      : valid_(bns.valid),
        t_(bns.t_)
      {}

      inline base64_to_number_sink& operator=(const base64_to_number_sink& bns)
      {
         valid_ = bns.valid_;
         t_ = bns.t_;
         return (*this);
      }

      inline base64_to_number_sink& operator=(const std::string& s)
      {
         if (!range_only_contains(base64_value_check(),s.data(),s.data() + s.size()))
         {
            valid_ = false;
            return (*this);
         }

         *t_ = T(0);
         convert_base64_to_bin(s.data(),
                               s.data() + s.size(),
                               reinterpret_cast<char*>(t_));
         reverse_bytes();
         return (*this);
      }

      template<typename InputIterator>
      inline base64_to_number_sink& operator=(const std::pair<InputIterator,InputIterator>& s)
      {
         if (!range_only_contains(base64_value_check(),s.first,s.second))
         {
            valid_ = false;
            return (*this);
         }

         *t_ = T(0);
         convert_base64_to_bin(s.first, s.second,reinterpret_cast<char*>(t_));
         reverse_bytes();
         return (*this);
      }

      inline bool valid() const
      {
         return valid_;
      }

   private:

      inline void reverse_bytes()
      {
         unsigned char* itr1 = reinterpret_cast<unsigned char*>(t_);
         unsigned char* itr2 = itr1 + (sizeof(T) - 1);
         while (itr1 < itr2)
         {
            std::swap(*itr1,*itr2);
            ++itr1;
            --itr2;
         }
      }

   private:

      bool valid_;
      T* t_;
   };

   namespace details
   {

      template <typename InputIterator,
                typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline std::size_t parse_stl_container_proxy(const InputIterator begin,
                                                   const InputIterator end,
                                                   const std::string& delimiters,
                                                   Sequence<T,Allocator>& sequence,
                                                   const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse(begin,end,delimiters,sequence,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Comparator,
                typename Allocator>
      inline std::size_t parse_stl_container_proxy(const InputIterator begin,
                                                   const InputIterator end,
                                                   const std::string& delimiters,
                                                   std::set<T,Comparator,Allocator>& set,
                                                   const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse(begin,end,delimiters,set,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Container>
      inline std::size_t parse_stl_container_proxy(const InputIterator begin,
                                                   const InputIterator end,
                                                   const std::string& delimiters,
                                                   std::queue<T,Container>& queue,
                                                   const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse(begin,end,delimiters,queue,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Container>
      inline std::size_t parse_stl_container_proxy(const InputIterator begin,
                                                   const InputIterator end,
                                                   const std::string& delimiters,
                                                   std::stack<T,Container>& stack,
                                                   const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse(begin,end,delimiters,stack,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Container,
                typename Comparator>
      inline std::size_t parse_stl_container_proxy(const InputIterator begin,
                                                   const InputIterator end,
                                                   const std::string& delimiters,
                                                   std::priority_queue<T,Container,Comparator>& priority_queue,
                                                   const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse(begin,end,delimiters,priority_queue,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Allocator,
                template <typename,typename> class Sequence>
      inline std::size_t parse_n_stl_container_proxy(const InputIterator begin,
                                                     const InputIterator end,
                                                     const std::string& delimiters,
                                                     const std::size_t& n,
                                                     Sequence<T,Allocator>& sequence,
                                                     const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse_n(begin,end,delimiters,n,sequence,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Comparator,
                typename Allocator>
      inline std::size_t parse_n_stl_container_proxy(const InputIterator begin,
                                                     const InputIterator end,
                                                     const std::string& delimiters,
                                                     const std::size_t& n,
                                                     std::set<T,Comparator,Allocator>& set,
                                                     const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse_n(begin,end,delimiters,n,set,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Container>
      inline std::size_t parse_n_stl_container_proxy(const InputIterator begin,
                                                     const InputIterator end,
                                                     const std::string& delimiters,
                                                     const std::size_t& n,
                                                     std::queue<T,Container>& queue,
                                                     const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse_n(begin,end,delimiters,n,queue,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Container>
      inline std::size_t parse_n_stl_container_proxy(const InputIterator begin,
                                                     const InputIterator end,
                                                     const std::string& delimiters,
                                                     const std::size_t& n,
                                                     std::stack<T,Container>& stack,
                                                     const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse_n(begin,end,delimiters,n,stack,split_option);
      }

      template <typename InputIterator,
                typename T,
                typename Container,
                typename Comparator>
      inline std::size_t parse_n_stl_container_proxy(const InputIterator begin,
                                                     const InputIterator end,
                                                     const std::string& delimiters,
                                                     const std::size_t& n,
                                                     std::priority_queue<T,Container,Comparator>& priority_queue,
                                                     const split_options::type& split_option = split_options::compress_delimiters)
      {
         return parse_n(begin,end,delimiters,n,priority_queue,split_option);
      }

   } // namespace details

   template<typename Container>
   class sink_type
   {
   public:
      typedef typename Container::value_type value_type;

      inline sink_type(const std::string& delimiters,
                       const split_options::type& split_option = split_options::compress_delimiters)
      : delimiters_(delimiters),
        split_option_(split_option),
        container_(0),
        element_count_(std::numeric_limits<std::size_t>::max())
      {}

      inline sink_type(Container& container,
                       const std::string& delimiters,
                       const split_options::type& split_option = split_options::compress_delimiters)
      : delimiters_(delimiters),
        split_option_(split_option),
        container_(&container)
      {}

      inline sink_type& count(const std::size_t& element_count = std::numeric_limits<std::size_t>::max())
      {
         element_count_ = element_count;
         return (*this);
      }

      inline sink_type& operator()(Container& container,
                                   const std::string& delimiters = "",
                                   const split_options::type& split_option = split_options::compress_delimiters)
      {
         container_ = (&container);
         if (!delimiters.empty())
            delimiters_ = delimiters;
         split_option_ = split_option;
         return (*this);
      }

      template<typename InputIterator>
      inline bool parse(InputIterator begin, InputIterator end)
      {
         if (container_)
         {
            if (std::numeric_limits<std::size_t>::max() == element_count_)
               return (details::parse_stl_container_proxy
                         (begin,end,delimiters_,(*container_),split_option_) > 0);
            else
               return (details::parse_n_stl_container_proxy
                         (begin,end,delimiters_,element_count_,(*container_),split_option_) == element_count_);
         }
         else
            return false;
      }

   private:

      std::string delimiters_;
      split_options::type split_option_;
      Container* container_;
      std::size_t element_count_;
   };

   template<typename T> struct vector_sink { typedef sink_type<std::vector<T> > type; };
   template<typename T> struct deque_sink  { typedef sink_type<std::deque<T> >  type; };
   template<typename T> struct list_sink   { typedef sink_type<std::list<T> >   type; };
   template<typename T> struct set_sink    { typedef sink_type<std::set<T> >    type; };
   template<typename T> struct queue_sink  { typedef sink_type<std::queue<T> >  type; };
   template<typename T> struct stack_sink  { typedef sink_type<std::stack<T> >  type; };
   template<typename T> struct priority_queue_sink { typedef sink_type<std::priority_queue<T> > type; };

   namespace text
   {
      inline std::string center(const std::size_t& width,
                                const std::string::value_type& pad,
                                const std::string& str)
      {
         if (str.size() >= width) return str;
         const std::size_t pad_count = width - str.size();
         const std::size_t pad_count_2 = (pad_count >> 1) + (pad_count & 1);
         return std::string(pad_count >> 1,pad) + str + std::string(pad_count_2,pad);
      }

      inline std::string right_align(const std::size_t& width,
                                     const std::string::value_type& pad,
                                     const std::string& str)
      {
         if (str.size() >= width) return str;
         return std::string(width - str.size(),pad) + str;
      }

      inline std::string left_align(const std::size_t& width,
                                    const std::string::value_type& pad,
                                    const std::string& str)
      {
         if (str.size() >= width) return str;
         return str + std::string(width - str.size(),pad);
      }

      template<typename T>
      inline std::string center(const std::size_t& width,
                                const std::string::value_type& pad,
                                const T& t)
      {
         return center(width,pad,type_to_string(t));
      }

      template<typename T>
      inline std::string right_align(const std::size_t& width,
                                     const std::string::value_type& pad,
                                     const T& t)
      {
         return right_align(width,pad,type_to_string(t));
      }


      template<typename T>
      inline std::string left_align(const std::size_t& width,
                                    const std::string::value_type& pad,
                                    const T& t)
      {
         return left_align(width,pad,type_to_string(t));
      }

      template<typename T>
      inline std::string center(const std::size_t& width, const T& t)
      {
         return center(width,' ',type_to_string(t));
      }

      template<typename T>
      inline std::string right_align(const std::size_t& width, const T& t)
      {
         return right_align(width,' ',type_to_string(t));
      }

      template<typename T>
      inline std::string left_align(const std::size_t& width, const T& t)
      {
         return left_align(width,' ',type_to_string(t));
      }

      inline std::string remaining_string(const std::size_t& index,
                                          const std::string& str)
      {
         return (index < str.size()) ? str.substr(index,str.size() - index) : str;
      }

      inline void remaining_string(const std::size_t& index,
                                   const std::string& str,
                                   std::string& result)
      {
         result = (index < str.size()) ? str.substr(index,str.size() - index) : str;
      }

      inline bool is_letter(const char c)
      {
         return (('A' <= c) && ( c <= 'Z')) || (('a' <= c) && ( c <= 'z'));
      }

      inline bool is_lowercase_letter(const char c)
      {
         return (('a' <= c) && ( c <= 'z'));
      }

      inline bool is_uppercase_letter(const char c)
      {
         return (('a' <= c) && ( c <= 'z'));
      }

      inline bool is_digit(const char c)
      {
         return (('0' <= c) && ( c <= '9'));
      }

      inline bool is_hex_digit(const char c)
      {
         return (('0' <= c) && (c <= '9')) ||
                (('A' <= c) && (c <= 'F')) ||
                (('a' <= c) && (c <= 'f'));
      }

      inline bool is_letter_or_digit(const char c)
      {
         return (is_letter(c) || is_digit(c));
      }

      inline bool is_all_letters(const std::string& s)
      {
         for (std::size_t i = 0; i < s.size(); ++i)
         {
            if (!is_letter(s[i]))
               return false;
         }
         return true;
      }

      inline bool is_all_digits(const std::string& s)
      {
         for (std::size_t i = 0; i < s.size(); ++i)
         {
            if (!is_digit(s[i]))
               return false;
         }
         return true;
      }

   } // namespace text

   namespace details
   {
      static const unsigned char digit_table[] =
                                 {
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xFF - 0x07
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x08 - 0x0F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x10 - 0x17
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x18 - 0x1F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x20 - 0x27
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x28 - 0x2F
                                    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 0x30 - 0x37
                                    0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x38 - 0x3F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x40 - 0x47
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x48 - 0x4F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x50 - 0x57
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x58 - 0x5F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x60 - 0x67
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x68 - 0x6F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x70 - 0x77
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x78 - 0x7F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x80 - 0x87
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x88 - 0x8F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x90 - 0x97
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0x98 - 0x9F
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xA0 - 0xA7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xA8 - 0xAF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xB0 - 0xB7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xB8 - 0xBF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xC0 - 0xC7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xC8 - 0xCF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xD0 - 0xD7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xD8 - 0xDF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xE0 - 0xE7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xE8 - 0xEF
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xF0 - 0xF7
                                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  // 0xF8 - 0xFF
                                 };

      static const std::size_t digit_table_size = sizeof(digit_table) / sizeof(unsigned char);

      template<typename T>
      static inline bool is_invalid_digit(const T& t)
      {
         static const unsigned int invalid_digit = 0xFF;
         return (static_cast<T>(invalid_digit) == t);
      }

      template<typename T>
      static inline bool is_valid_digit(const T& t)
      {
         static const unsigned int invalid_digit = 0xFF;
         return (static_cast<T>(invalid_digit) != t);
      }

      static const unsigned char digitr[] =
                                 {
                                    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 };

      static const unsigned char rev_digit_table3[] =
                                 {
                                    "000100200300400500600700800900010110210310410510610710810910020120220320420"
                                    "520620720820920030130230330430530630730830930040140240340440540640740840940"
                                    "050150250350450550650750850950060160260360460560660760860960070170270370470"
                                    "570670770870970080180280380480580680780880980090190290390490590690790890990"
                                    "001101201301401501601701801901011111211311411511611711811911021121221321421"
                                    "521621721821921031131231331431531631731831931041141241341441541641741841941"
                                    "051151251351451551651751851951061161261361461561661761861961071171271371471"
                                    "571671771871971081181281381481581681781881981091191291391491591691791891991"
                                    "002102202302402502602702802902012112212312412512612712812912022122222322422"
                                    "522622722822922032132232332432532632732832932042142242342442542642742842942"
                                    "052152252352452552652752852952062162262362462562662762862962072172272372472"
                                    "572672772872972082182282382482582682782882982092192292392492592692792892992"
                                    "003103203303403503603703803903013113213313413513613713813913023123223323423"
                                    "523623723823923033133233333433533633733833933043143243343443543643743843943"
                                    "053153253353453553653753853953063163263363463563663763863963073173273373473"
                                    "573673773873973083183283383483583683783883983093193293393493593693793893993"
                                    "004104204304404504604704804904014114214314414514614714814914024124224324424"
                                    "524624724824924034134234334434534634734834934044144244344444544644744844944"
                                    "054154254354454554654754854954064164264364464564664764864964074174274374474"
                                    "574674774874974084184284384484584684784884984094194294394494594694794894994"
                                    "005105205305405505605705805905015115215315415515615715815915025125225325425"
                                    "525625725825925035135235335435535635735835935045145245345445545645745845945"
                                    "055155255355455555655755855955065165265365465565665765865965075175275375475"
                                    "575675775875975085185285385485585685785885985095195295395495595695795895995"
                                    "006106206306406506606706806906016116216316416516616716816916026126226326426"
                                    "526626726826926036136236336436536636736836936046146246346446546646746846946"
                                    "056156256356456556656756856956066166266366466566666766866966076176276376476"
                                    "576676776876976086186286386486586686786886986096196296396496596696796896996"
                                    "007107207307407507607707807907017117217317417517617717817917027127227327427"
                                    "527627727827927037137237337437537637737837937047147247347447547647747847947"
                                    "057157257357457557657757857957067167267367467567667767867967077177277377477"
                                    "577677777877977087187287387487587687787887987097197297397497597697797897997"
                                    "008108208308408508608708808908018118218318418518618718818918028128228328428"
                                    "528628728828928038138238338438538638738838938048148248348448548648748848948"
                                    "058158258358458558658758858958068168268368468568668768868968078178278378478"
                                    "578678778878978088188288388488588688788888988098198298398498598698798898998"
                                    "009109209309409509609709809909019119219319419519619719819919029129229329429"
                                    "529629729829929039139239339439539639739839939049149249349449549649749849949"
                                    "059159259359459559659759859959069169269369469569669769869969079179279379479"
                                    "579679779879979089189289389489589689789889989099199299399499599699799899999"
                                 };

      static const unsigned char rev_digit_table2[] =
                                 {
                                    "0010203040506070809001112131415161718191"
                                    "0212223242526272829203132333435363738393"
                                    "0414243444546474849405152535455565758595"
                                    "0616263646566676869607172737475767778797"
                                    "0818283848586878889809192939495969798999"
                                 };

      #define strtk_register_pod_type(T)\
      template<> struct is_pod<T>{ typedef yes_t result_t; enum {result = true }; };\
      template<> struct is_pod<const T>{ typedef yes_t result_t; enum {result = true }; };\
      template<> struct is_pod<volatile T>{ typedef yes_t result_t; enum {result = true }; };\
      template<> struct is_pod<const volatile T>{ typedef yes_t result_t; enum {result = true }; };\

      strtk_register_pod_type(bool)
      strtk_register_pod_type(signed char)
      strtk_register_pod_type(char)
      strtk_register_pod_type(short)
      strtk_register_pod_type(int)
      strtk_register_pod_type(long int)
      strtk_register_pod_type(long long int)
      strtk_register_pod_type(unsigned char)
      strtk_register_pod_type(unsigned short)
      strtk_register_pod_type(unsigned int)
      strtk_register_pod_type(unsigned long int)
      strtk_register_pod_type(unsigned long long int)
      strtk_register_pod_type(float)
      strtk_register_pod_type(double)
      strtk_register_pod_type(long double)

      template<typename>
      struct numeric { enum { length = 0, size = 32, bound_length = 0, min_exp = 0, max_exp = 0 }; };

      template<> struct numeric<short>              { enum { length =  5, size = 16, bound_length =  4}; };
      template<> struct numeric<unsigned short>     { enum { length =  5, size = 16, bound_length =  4}; };

      template<> struct numeric<int>                { enum { length = 10, size = 16, bound_length =  8}; };
      template<> struct numeric<unsigned int>       { enum { length = 10, size = 16, bound_length =  8}; };

      template<> struct numeric<long>               { enum { length = 10, size = 16, bound_length =  8}; };
      template<> struct numeric<unsigned long>      { enum { length = 10, size = 16, bound_length =  8}; };

      template<> struct numeric<long long>          { enum { length = 19, size = 24, bound_length = 18}; };
      template<> struct numeric<unsigned long long> { enum { length = 19, size = 24, bound_length = 18}; };

      template<> struct numeric<float>              { enum { min_exp =  -38, max_exp =  +38, precision = 10}; };
      template<> struct numeric<double>             { enum { min_exp = -308, max_exp = +308, precision = 15}; };
      template<> struct numeric<long double>        { enum { min_exp = -308, max_exp = +308, precision = 15}; };

      #define strtk_register_unsigned_type_tag(T)\
      template<> struct supported_conversion_to_type<T> { typedef unsigned_type_tag type; };\
      template<> struct supported_conversion_from_type<T> { typedef unsigned_type_tag type; };

      #define strtk_register_signed_type_tag(T)\
      template<> struct supported_conversion_to_type<T>{ typedef signed_type_tag type; };\
      template<> struct supported_conversion_from_type<T> { typedef signed_type_tag type; };

      #define strtk_register_real_type_tag(T)\
      template<> struct supported_conversion_to_type<T>{ typedef real_type_tag type; };

      #define strtk_register_byte_type_tag(T)\
      template<> struct supported_conversion_to_type<T>{ typedef byte_type_tag type; };\
      template<> struct supported_conversion_from_type<T> { typedef byte_type_tag type; };

      #define strtk_register_hex_type_tag(T)\
      template<> struct supported_conversion_to_type<T>{ typedef hex_type_tag type; };

      #define strtk_register_base64_type_tag(T)\
      template<> struct supported_conversion_to_type<T>{ typedef base64_type_tag type; };

      #define strtk_register_supported_iterator_type(T)\
      template<> struct supported_iterator_type<T> { enum { value = true }; };

      template<> struct supported_conversion_to_type<bool> { typedef bool_type_tag type; };
      template<> struct supported_iterator_type<bool> { enum { value = true }; };

      template<> struct supported_conversion_to_type<std::string> { typedef stdstring_type_tag type; };
      template<> struct supported_iterator_type<std::string> { enum { value = true }; };

      #define strtk_register_stdstring_range_type_tag(T)\
      template<> struct supported_conversion_to_type< std::pair<T,T> >{ typedef stdstring_range_type_tag type; };

      #define strtk_register_sink_type_tag(T)\
      template<> struct supported_conversion_to_type<sink_type<std::vector<T> > > { typedef sink_type_tag type; };\
      template<> struct supported_conversion_to_type<sink_type<std::deque<T> > > { typedef sink_type_tag type; };\
      template<> struct supported_conversion_to_type<sink_type<std::list<T> > > { typedef sink_type_tag type; };\
      template<> struct supported_conversion_to_type<sink_type<std::set<T> > > { typedef sink_type_tag type; };\
      template<> struct supported_conversion_to_type<sink_type<std::queue<T> > > { typedef sink_type_tag type; };\
      template<> struct supported_conversion_to_type<sink_type<std::stack<T> > > { typedef sink_type_tag type; };\
      template<> struct supported_conversion_to_type<sink_type<std::priority_queue<T> > > { typedef sink_type_tag type; };\

      template<> struct supported_conversion_to_type<ignore_token>{ typedef ignore_token_type_tag type; };


      #define strtk_register_sequence_iterator_type(sequence)\
      strtk_register_supported_iterator_type(sequence<char>::iterator)\
      strtk_register_supported_iterator_type(sequence<char>::const_iterator)\
      strtk_register_supported_iterator_type(sequence<unsigned char>::iterator)\
      strtk_register_supported_iterator_type(sequence<unsigned char>::const_iterator)

      strtk_register_unsigned_type_tag(unsigned short)
      strtk_register_unsigned_type_tag(unsigned int)
      strtk_register_unsigned_type_tag(unsigned long)
      strtk_register_unsigned_type_tag(unsigned long long)

      strtk_register_signed_type_tag(short)
      strtk_register_signed_type_tag(int)
      strtk_register_signed_type_tag(long)
      strtk_register_signed_type_tag(long long)

      strtk_register_real_type_tag(float)
      strtk_register_real_type_tag(double)
      strtk_register_real_type_tag(long double)

      strtk_register_byte_type_tag(unsigned char)
      strtk_register_byte_type_tag(signed char)
      strtk_register_byte_type_tag(char)

      strtk_register_hex_type_tag(hex_to_number_sink<short>)
      strtk_register_hex_type_tag(hex_to_number_sink<int>)
      strtk_register_hex_type_tag(hex_to_number_sink<long>)
      strtk_register_hex_type_tag(hex_to_number_sink<unsigned short>)
      strtk_register_hex_type_tag(hex_to_number_sink<unsigned int>)
      strtk_register_hex_type_tag(hex_to_number_sink<unsigned long>)
      strtk_register_hex_type_tag(hex_to_number_sink<unsigned long long>)

      strtk_register_base64_type_tag(base64_to_number_sink<short>)
      strtk_register_base64_type_tag(base64_to_number_sink<int>)
      strtk_register_base64_type_tag(base64_to_number_sink<long>)
      strtk_register_base64_type_tag(base64_to_number_sink<unsigned short>)
      strtk_register_base64_type_tag(base64_to_number_sink<unsigned int>)
      strtk_register_base64_type_tag(base64_to_number_sink<unsigned long>)
      strtk_register_base64_type_tag(base64_to_number_sink<unsigned long long>)

      strtk_register_stdstring_range_type_tag(std::string::iterator)
      strtk_register_stdstring_range_type_tag(std::string::const_iterator)
      strtk_register_stdstring_range_type_tag(char*)
      strtk_register_stdstring_range_type_tag(signed char*)
      strtk_register_stdstring_range_type_tag(unsigned char*)
      strtk_register_stdstring_range_type_tag(const char*)
      strtk_register_stdstring_range_type_tag(const unsigned char*)

      strtk_register_supported_iterator_type(char*)
      strtk_register_supported_iterator_type(signed char*)
      strtk_register_supported_iterator_type(unsigned char*)
      strtk_register_supported_iterator_type(const char*)
      strtk_register_supported_iterator_type(const signed char*)
      strtk_register_supported_iterator_type(const unsigned char*)
      strtk_register_supported_iterator_type(std::string::iterator)
      strtk_register_supported_iterator_type(std::string::const_iterator)

      strtk_register_sequence_iterator_type(std::vector)
      strtk_register_sequence_iterator_type(std::deque)

      strtk_register_sink_type_tag(float)
      strtk_register_sink_type_tag(double)
      strtk_register_sink_type_tag(long double)
      strtk_register_sink_type_tag(signed char)
      strtk_register_sink_type_tag(char)
      strtk_register_sink_type_tag(short)
      strtk_register_sink_type_tag(int)
      strtk_register_sink_type_tag(long)
      strtk_register_sink_type_tag(unsigned char)
      strtk_register_sink_type_tag(unsigned short)
      strtk_register_sink_type_tag(unsigned int)
      strtk_register_sink_type_tag(unsigned long)
      strtk_register_sink_type_tag(unsigned long long)
      strtk_register_sink_type_tag(std::string)

      #define strtk_register_userdef_type_sink(T)\
      namespace strtk { namespace details { strtk_register_sink_type_tag(T) }}

      template<typename Iterator, typename T, typename Tag>
      inline bool string_to_type_converter_impl(Iterator& begin, const Iterator end, T& t, not_supported_type_tag)
      {
         #ifdef strtk_enable_lexical_cast
            try
            {
               t = boost::lexical_cast<T>(std::string(begin,end));
            }
            catch (const boost::bad_lexical_cast&)
            {
               return false;
            }
            begin = end;
            return true;
         #else
            try
            {
               std::stringstream ss(std::string(begin,end));
               ss >> t;
            }
            catch (const std::exception&)
            {
               return false;
            }
            begin = end;
            return true;
         #endif
      }

      template<typename Iterator>
      inline bool string_to_type_converter_impl(Iterator& begin, const Iterator end, std::string& t, stdstring_type_tag)
      {
         t.assign(begin,end);
         begin = end;
         return true;
      }

      template<typename Iterator, typename T>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, T& result, unsigned_type_tag)
      {
         if (end == itr) return false;

         if ('+' == *itr)
            ++itr;

         if (end == itr)
            return false;

         while ((end != itr) && ('0' == *itr)) ++itr;
         const std::size_t length = std::distance(itr,end);

         if (length > numeric<T>::length)
            return false;

         T t = 0;

         if (0 != length)
         {
            static const std::size_t radix = 10;
            const Iterator interim_end = itr + length - std::min<std::size_t>(numeric<T>::bound_length,length);
            T digit = 0;

            while (interim_end != itr)
            {
               digit = static_cast<T>(digit_table[static_cast<unsigned int>(*itr)]);
               if (is_valid_digit(digit))
               {
                  t = (t * radix) + digit;
                  ++itr;
               }
               else
                  return false;
            }

            if (interim_end != end)
            {
               const T tmp = t;
               while (end != itr)
               {
                  digit = static_cast<T>(digit_table[static_cast<unsigned int>(*itr)]);
                  if (is_valid_digit(digit))
                  {
                     t = (t * radix) + digit;
                     ++itr;
                  }
                  else
                     return false;
               }

               if (tmp > t) return false;
            }
         }

         result = static_cast<T>(t);
         return true;
      }

      template<typename Iterator, typename T>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, T& result, signed_type_tag)
      {
         if (end == itr) return false;

         bool negative = false;

         if ('+' == *itr)
            ++itr;
         else if ('-' == *itr)
         {
            ++itr;
            negative = true;
         }

         if (end == itr) return false;

         while ((end != itr) && ('0' == *itr)) ++itr;
         const std::size_t length = std::distance(itr,end);

         if (length > numeric<T>::length)
            return false;

         T t = 0;

         if (0 != length)
         {
            static const std::size_t radix = 10;
            const Iterator interim_end = itr + length - std::min<std::size_t>(numeric<T>::bound_length,length);
            T digit = 0;

            while (interim_end != itr)
            {
               digit = static_cast<T>(digit_table[static_cast<unsigned int>(*itr)]);
               if (is_valid_digit(digit))
               {
                  t = (t * radix) + digit;
                  ++itr;
               }
               else
                  return false;
            }

            if (interim_end != end)
            {
               const T tmp = t;
               while (end != itr)
               {
                  digit = static_cast<T>(digit_table[static_cast<unsigned int>(*itr)]);
                  if (is_valid_digit(digit))
                  {
                     t = (t * radix) + digit;
                     ++itr;
                  }
                  else
                     return false;
               }

               if (tmp > t) return false;
            }
         }

         result = static_cast<T>((negative) ? -t : t);
         return true;
      }

      template<typename Iterator, typename T>
      inline bool string_to_type_converter_impl_ref(Iterator& itr, const Iterator end, T& result, signed_type_tag)
      {
         if (end == itr) return false;

         T t = 0;
         bool negative = false;

         if ('+' == *itr)
            ++itr;
         else if ('-' == *itr)
         {
            ++itr;
            negative = true;
         }

         if (end == itr)
            return false;

         unsigned int digit_count = 0;
         while ((end != itr) && ('0' == *itr)) ++itr;

         bool return_result = true;
         while (end != itr)
         {
            const T digit = static_cast<T>(digit_table[static_cast<unsigned int>(*itr)]);
            if (is_invalid_digit(digit))
            {
               return_result = false;
               break;
            }

            if ((++digit_count) <= numeric<T>::bound_length)
            {
               t *= 10;
               t += digit;
            }
            else
            {
               typedef unsigned long long base_type;
               static const base_type max_limit = +std::numeric_limits<T>::max();
               static const base_type min_limit = -std::numeric_limits<T>::min();
               base_type tmp = static_cast<base_type>(t) * 10 + digit;
               if (negative && static_cast<base_type>(tmp) > min_limit)
                  return_result = false;
               else if (static_cast<base_type>(tmp) > max_limit)
                  return_result = false;
               t = static_cast<T>(tmp);
            }
            ++itr;
         }

         result = static_cast<T>((negative) ? -t : t);
         return return_result;
      }

      template<typename Iterator, typename T>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, T& t, real_type_tag)
      {
         if (end == itr) return false;
         double d = 0.0;
         bool negative = false;
         if ('+' == *itr)
            ++itr;
         else if ('-' == *itr)
         {
            ++itr;
            negative = true;
         }
         if (end == itr)
            return false;

         bool instate = false;

         if ('.' != *itr)
         {
            const Iterator curr = itr;
            while ((end != itr) && ('0' == *itr)) ++itr;
            while (end != itr)
            {
               const unsigned int digit = details::digit_table[static_cast<unsigned int>(*itr)];
               if (is_valid_digit(digit))
                  d = (d * 10.0) + digit;
               else
                  break;
               ++itr;
            }
            if (curr != itr) instate = true;
         }

         int exponent = 0;

         if (end != itr)
         {
            if ('.' == *itr)
            {
               ++itr;
               const Iterator curr = itr;
               while (end != itr)
               {
                  const unsigned int digit = details::digit_table[static_cast<unsigned int>(*itr)];
                  if (is_valid_digit(digit))
                     d = (d * 10.0) + digit;
                  else
                     break;
                  ++itr;
                  --exponent;
               }

               if (curr != itr) instate = true;
            }

            if (end != itr)
            {
               typename std::iterator_traits<Iterator>::value_type c = *itr;

               if (('e' == c) || ('E' == c))
               {
                  ++itr;
                  int exp = 0;
                  if (!details::string_to_type_converter_impl_ref(itr,end,exp,details::signed_type_tag()))
                  {
                     if (end == itr)
                        return false;
                     else
                        c = *itr;
                  }

                  if ((exp < numeric<T>::min_exp) || (numeric<T>::max_exp < exp))
                     return false;

                  exponent += exp;
               }

               if (('f' == c) || ('F' == c) || ('l' == c) || ('L' == c))
                  ++itr;
            }
         }

         if ((end != itr) || (!instate))
            return false;

         if (0 != exponent)
         {
            if (
                 (std::numeric_limits<T>::max_exponent10 < exponent) ||
                 (std::numeric_limits<T>::min_exponent10 > exponent)
               )
            {
               return false;
            }

            const int e = std::abs(exponent);
            static const double fract10[] =
                         {
                            0.0,
                            1.0E+001, 1.0E+002, 1.0E+003, 1.0E+004, 1.0E+005, 1.0E+006, 1.0E+007, 1.0E+008, 1.0E+009, 1.0E+010,
                            1.0E+011, 1.0E+012, 1.0E+013, 1.0E+014, 1.0E+015, 1.0E+016, 1.0E+017, 1.0E+018, 1.0E+019, 1.0E+020,
                            1.0E+021, 1.0E+022, 1.0E+023, 1.0E+024, 1.0E+025, 1.0E+026, 1.0E+027, 1.0E+028, 1.0E+029, 1.0E+030,
                            1.0E+031, 1.0E+032, 1.0E+033, 1.0E+034, 1.0E+035, 1.0E+036, 1.0E+037, 1.0E+038, 1.0E+039, 1.0E+040,
                            1.0E+041, 1.0E+042, 1.0E+043, 1.0E+044, 1.0E+045, 1.0E+046, 1.0E+047, 1.0E+048, 1.0E+049, 1.0E+050,
                            1.0E+051, 1.0E+052, 1.0E+053, 1.0E+054, 1.0E+055, 1.0E+056, 1.0E+057, 1.0E+058, 1.0E+059, 1.0E+060,
                            1.0E+061, 1.0E+062, 1.0E+063, 1.0E+064, 1.0E+065, 1.0E+066, 1.0E+067, 1.0E+068, 1.0E+069, 1.0E+070,
                            1.0E+071, 1.0E+072, 1.0E+073, 1.0E+074, 1.0E+075, 1.0E+076, 1.0E+077, 1.0E+078, 1.0E+079, 1.0E+080,
                            1.0E+081, 1.0E+082, 1.0E+083, 1.0E+084, 1.0E+085, 1.0E+086, 1.0E+087, 1.0E+088, 1.0E+089, 1.0E+090,
                            1.0E+091, 1.0E+092, 1.0E+093, 1.0E+094, 1.0E+095, 1.0E+096, 1.0E+097, 1.0E+098, 1.0E+099, 1.0E+100,
                            1.0E+101, 1.0E+102, 1.0E+103, 1.0E+104, 1.0E+105, 1.0E+106, 1.0E+107, 1.0E+108, 1.0E+109, 1.0E+110,
                            1.0E+111, 1.0E+112, 1.0E+113, 1.0E+114, 1.0E+115, 1.0E+116, 1.0E+117, 1.0E+118, 1.0E+119, 1.0E+120,
                            1.0E+121, 1.0E+122, 1.0E+123, 1.0E+124, 1.0E+125, 1.0E+126, 1.0E+127, 1.0E+128, 1.0E+129, 1.0E+130,
                            1.0E+131, 1.0E+132, 1.0E+133, 1.0E+134, 1.0E+135, 1.0E+136, 1.0E+137, 1.0E+138, 1.0E+139, 1.0E+140,
                            1.0E+141, 1.0E+142, 1.0E+143, 1.0E+144, 1.0E+145, 1.0E+146, 1.0E+147, 1.0E+148, 1.0E+149, 1.0E+150,
                            1.0E+151, 1.0E+152, 1.0E+153, 1.0E+154, 1.0E+155, 1.0E+156, 1.0E+157, 1.0E+158, 1.0E+159, 1.0E+160,
                            1.0E+161, 1.0E+162, 1.0E+163, 1.0E+164, 1.0E+165, 1.0E+166, 1.0E+167, 1.0E+168, 1.0E+169, 1.0E+170,
                            1.0E+171, 1.0E+172, 1.0E+173, 1.0E+174, 1.0E+175, 1.0E+176, 1.0E+177, 1.0E+178, 1.0E+179, 1.0E+180,
                            1.0E+181, 1.0E+182, 1.0E+183, 1.0E+184, 1.0E+185, 1.0E+186, 1.0E+187, 1.0E+188, 1.0E+189, 1.0E+190,
                            1.0E+191, 1.0E+192, 1.0E+193, 1.0E+194, 1.0E+195, 1.0E+196, 1.0E+197, 1.0E+198, 1.0E+199, 1.0E+200,
                            1.0E+221, 1.0E+222, 1.0E+223, 1.0E+224, 1.0E+225, 1.0E+226, 1.0E+227, 1.0E+228, 1.0E+229, 1.0E+230,
                            1.0E+231, 1.0E+232, 1.0E+233, 1.0E+234, 1.0E+235, 1.0E+236, 1.0E+237, 1.0E+238, 1.0E+239, 1.0E+240,
                            1.0E+241, 1.0E+242, 1.0E+243, 1.0E+244, 1.0E+245, 1.0E+246, 1.0E+247, 1.0E+248, 1.0E+249, 1.0E+250,
                            1.0E+251, 1.0E+252, 1.0E+253, 1.0E+254, 1.0E+255, 1.0E+256, 1.0E+257, 1.0E+258, 1.0E+259, 1.0E+260,
                            1.0E+261, 1.0E+262, 1.0E+263, 1.0E+264, 1.0E+265, 1.0E+266, 1.0E+267, 1.0E+268, 1.0E+269, 1.0E+270,
                            1.0E+271, 1.0E+272, 1.0E+273, 1.0E+274, 1.0E+275, 1.0E+276, 1.0E+277, 1.0E+278, 1.0E+279, 1.0E+280,
                            1.0E+281, 1.0E+282, 1.0E+283, 1.0E+284, 1.0E+285, 1.0E+286, 1.0E+287, 1.0E+288, 1.0E+289, 1.0E+290,
                            1.0E+291, 1.0E+292, 1.0E+293, 1.0E+294, 1.0E+295, 1.0E+296, 1.0E+297, 1.0E+298, 1.0E+299, 1.0E+300,
                            1.0E+301, 1.0E+302, 1.0E+303, 1.0E+304, 1.0E+305, 1.0E+306, 1.0E+307, 1.0E+308
                         };

            static const std::size_t fract10_size = sizeof(fract10) / sizeof(double);

            if (d != 0.0)
            {
               if (static_cast<std::size_t>(e) < fract10_size)
               {
                  if (exponent > 0)
                     d *= fract10[e];
                  else
                     d /= fract10[e];
               }
               else
                  d *= std::pow(10.0, 1.0 * exponent);
            }
         }

         t = static_cast<T>((negative) ? -d : d);
         return true;
      }

      template<typename Iterator, typename T>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, T& t, byte_type_tag)
      {
         if (1 != std::distance(itr,end))
            return false;
         t = static_cast<T>(*itr);
         itr = end;
         return true;
      }

      template<typename Iterator>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, bool& t, bool_type_tag)
      {
         if (1 != std::distance(itr,end))
            return false;
         t = (('0' == *itr) ? false : true);
         itr = end;
         return true;
      }

      template<typename Iterator, typename IgnoreTokenType>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, IgnoreTokenType&, ignore_token_type_tag)
      {
         itr = end;
         return true;
      }

      template<typename Iterator, typename HexSinkType>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, HexSinkType& t, hex_type_tag)
      {
         t = std::pair<Iterator,Iterator>(itr,end);
         if (!t.valid())
            return false;
         itr = end;
         return true;
      }

      template<typename Iterator, typename Base64SinkType>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, Base64SinkType& t, base64_type_tag)
      {
         t = std::pair<Iterator,Iterator>(itr,end);
         if (!t.valid())
            return false;
         itr = end;
         return true;
      }

      template<typename Iterator, typename SinkType>
      inline bool string_to_type_converter_impl(Iterator& itr, const Iterator end, SinkType& t, sink_type_tag)
      {
         if (!t.parse(itr,end))
            return false;
         itr = end;
         return true;
      }

      template<typename T>
      inline bool type_to_string_converter_impl(const T& t, std::string& s, not_supported_type_tag)
      {
         #ifdef strtk_enable_lexical_cast
            try
            {
               s = boost::lexical_cast<std::string>(t);
            }
            catch (const boost::bad_lexical_cast&)
            {
               return false;
            }
         #else
            try
            {
               std::stringstream ss;
               ss << t;
               s = ss.str();
            }
            catch (const std::exception&)
            {
               return false;
            }
         #endif
         return true;
      }

      template<typename T>
      inline bool type_to_string_converter_impl(T value, std::string& result, unsigned_type_tag)
      {
         static const std::size_t radix = 10;
         static const std::size_t radix_sqr = radix * radix;
         static const std::size_t radix_cube = radix * radix * radix;
         char buffer[numeric<T>::size];
         char* itr = buffer + (numeric<T>::size - 1);
         T remainder = 0;
         std::size_t index = 0;

         if (0 != value)
         {
            while (value >= static_cast<T>(radix_sqr))
            {
               remainder  = value % radix_cube;
               value     /= radix_cube;
               index = remainder * 3;
               *(itr--) = static_cast<char>(details::rev_digit_table3[index + 0]);
               *(itr--) = static_cast<char>(details::rev_digit_table3[index + 1]);
               *(itr--) = static_cast<char>(details::rev_digit_table3[index + 2]);
            }

            while (value >= static_cast<T>(radix))
            {
               remainder  = value % radix_sqr;
               value     /= radix_sqr;
               index = remainder << 1;
               *(itr--) = static_cast<char>(details::rev_digit_table2[index + 0]);
               *(itr--) = static_cast<char>(details::rev_digit_table2[index + 1]);
            }

            if (0 != value)
            {
               remainder = value % radix;
               value    /= radix;
               *(itr--)  = strtk::details::digitr[remainder];
            }
         }
         else
            *(itr--) = '0';

         itr++;
         result.assign(itr, (buffer + numeric<T>::size) - itr);
         return true;
      }

      template<typename T>
      inline bool type_to_string_converter_impl(T value, std::string& result, strtk::details::signed_type_tag)
      {
         static const std::size_t radix = 10;
         static const std::size_t radix_sqr = radix * radix;
         static const std::size_t radix_cube = radix * radix * radix;
         char buffer[strtk::details::numeric<T>::size];
         char* itr = buffer + (strtk::details::numeric<T>::size - 1);
         bool negative = (value < 0);
         if (negative)
            value = static_cast<T>(-1 * value);

         T remainder = 0;
         std::size_t index = 0;

         if (0 != value)
         {
            while (value >= static_cast<T>(radix_sqr))
            {
               remainder  = value % radix_cube;
               value     /= radix_cube;
               index    = static_cast<std::size_t>(remainder * 3);
               *(itr--) = static_cast<char>(details::rev_digit_table3[index + 0]);
               *(itr--) = static_cast<char>(details::rev_digit_table3[index + 1]);
               *(itr--) = static_cast<char>(details::rev_digit_table3[index + 2]);
            }

            while (value >= static_cast<T>(radix))
            {
               remainder  = value % radix_sqr;
               value     /= radix_sqr;
               index    = static_cast<std::size_t>(remainder) << 1;
               *(itr--) = static_cast<char>(details::rev_digit_table2[index + 0]);
               *(itr--) = static_cast<char>(details::rev_digit_table2[index + 1]);
            }

            if (0 != value)
            {
               remainder = value % radix;
               value    /= radix;
               *(itr--)  = strtk::details::digitr[remainder];
            }
         }
         else
            *(itr--) = '0';

         if (negative) *(itr--) = '-';
         itr++;
         result.assign(itr, (buffer + strtk::details::numeric<T>::size) - itr);
         return true;
      }

      template<typename T>
      inline bool type_to_string_converter_impl(const T& value, std::string& result, byte_type_tag)
      {
         result.resize(1);
         result[0] = static_cast<char>(value);
         return true;
      }

      inline bool type_to_string_converter_impl(const bool& value, std::string& result, bool_type_tag)
      {
         result.resize(1);
         result[0] = value ? '1' : '0';
         return true;
      }

      inline bool type_to_string_converter_impl(const std::string& value, std::string& result, stdstring_type_tag)
      {
         result = value;
         return true;
      }

      template<typename Iterator>
      inline bool type_to_string_converter_impl(const std::pair<Iterator,Iterator> range, std::string& result, stdstring_range_type_tag)
      {
         result.assign(range.first,range.second);
         return true;
      }

      template <typename T>
      inline std::string type_name() { static const std::string s("Unknown"); return s; }

      #define strtk_register_type_name(Type)\
      template <> inline std::string type_name<Type>() { static const std::string s(#Type); return s; }

      strtk_register_type_name(signed char)
      strtk_register_type_name(unsigned char)
      strtk_register_type_name(short)
      strtk_register_type_name(int)
      strtk_register_type_name(long)
      strtk_register_type_name(long long)
      strtk_register_type_name(unsigned short)
      strtk_register_type_name(unsigned int)
      strtk_register_type_name(unsigned long)
      strtk_register_type_name(unsigned long long)
      strtk_register_type_name(double)
      strtk_register_type_name(float)
      strtk_register_type_name(long double)
      strtk_register_type_name(std::string)

      template <typename T>
      inline std::string type_name(const T&)
      {
         static const std::string s = type_name<T>();
         return s;
      }

      template <typename T1, typename T2>
      inline std::string type_name(const std::pair<T1,T2>& p)
      {
         static const std::string s = std::string("std::pair<" +
                                                  type_name(p.first) +
                                                  "," +
                                                  type_name(p.second) +
                                                  ">");
         return s;
      }

      template<typename T>
      inline std::size_t type_length()
      {
         return numeric<T>::length;
      }

      template<typename T>
      inline std::size_t type_length(const T&)
      {
         return type_length<T>();
      }

      inline std::size_t type_length(const std::string& s)
      {
         return s.size();
      }

      template<typename T1, typename T2>
      inline std::size_t type_length(const std::pair<T1,T2>&)
      {
         return type_length<T1>() + type_length<T2>();
      }

   } // namespace details

   template <typename T>
   inline std::string type_name(const T& t)
   {
      static const std::string s = details::type_name<T>(t);
      return s;
   }

   template<typename T, std::size_t N>
   inline std::string type_name(const T(&)[N])
   {
      static const std::string s = details::type_name<T>() +
                                   std::string("[") + type_to_string(N) + std::string("]");
      return s;
   }

   template <typename T1, typename T2>
   inline std::string type_name(const std::pair<T1,T2>& p)
   {
      static const std::string s = std::string("std::pair<" +
                                               type_name(p.first) +
                                               "," +
                                               type_name(p.second) +
                                               ">");
      return s;
   }

   #define strtk_register_sequence_type_name(Type)\
   template <typename T, typename Allocator>\
   inline std::string type_name(const Type<T,Allocator>&)\
   {\
      static const std::string s = std::string(#Type) + std::string("<" + details::type_name<T>() + ">");\
      return s;\
   }

   #define strtk_register_set_type_name(Type)\
   template <typename T, typename Comparator, typename Allocator>\
   inline std::string type_name(const Type<T,Comparator,Allocator>&)\
   {\
      static const std::string s = std::string(#Type) + std::string("<" + details::type_name<T>() + ">");\
      return s;\
   }

   strtk_register_sequence_type_name(std::vector)
   strtk_register_sequence_type_name(std::deque)
   strtk_register_sequence_type_name(std::list)
   strtk_register_set_type_name(std::set)

   template<typename T>
   inline std::size_t type_length()
   {
      return details::type_length<T>();
   }

   template<typename T>
   inline std::size_t type_length(const T&)
   {
      return type_length<T>();
   }

   class ext_string
   {
   public:

      explicit ext_string()
      {}

      explicit ext_string(const std::string& s)
      : s_(s)
      {}

      explicit ext_string(const char* s)
      : s_(s)
      {}

      explicit ext_string(const range::adapter<char>& r)
      : s_(r.begin(),r.end())
      {}

      ext_string(const ext_string& es)
      : s_(es.s_)
      {}

      template <typename T>
      inline ext_string& operator << (const T& t)
      {
         s_ += type_to_string(t);
         return (*this);
      }

      inline operator std::string () const
      {
         return s_;
      }

      inline std::string clone() const
      {
         return s_;
      }

      inline const std::string& as_string() const
      {
         return s_;
      }

      inline std::string& as_string()
      {
         return s_;
      }

      template<typename T>
      inline T as_type() const
      {
         return string_to_type_converter<T>(s_);
      }

      template<typename T>
      inline bool as_type(T& t) const
      {
         return string_to_type_converter(s_,t);
      }

      inline bool imatch(const std::string& s) const
      {
         return strtk::imatch(s_,s);
      }

      inline bool imatch(const ext_string& es) const
      {
         return strtk::imatch(s_,es.s_);
      }

      inline ext_string& to_lowercase()
      {
         convert_to_lowercase(s_);
         return (*this);
      }

      inline ext_string& to_uppercase()
      {
         convert_to_uppercase(s_);
         return (*this);
      }

      template<typename Predicate>
      inline ext_string& remove_leading(const Predicate& p)
      {
         if (s_.empty()) return (*this);
         strtk::remove_leading(p,s_);
         return (*this);
      }

      inline ext_string& remove_leading(const std::string& removal_set)
      {
         if (removal_set.empty())
            return (*this);
         else if (1 == removal_set.size())
            strtk::remove_leading(single_delimiter_predicate<std::string::value_type>(removal_set[0]),s_);
         else
            strtk::remove_leading(multiple_char_delimiter_predicate(removal_set),s_);
         return (*this);
      }

      template<typename Predicate>
      inline ext_string& remove_trailing(const Predicate& p)
      {
         if (s_.empty()) return (*this);
         strtk::remove_trailing(p,s_);
         return (*this);
      }

      inline ext_string& remove_trailing(const std::string& removal_set)
      {
         if (removal_set.empty())
            return (*this);
         else if (1 == removal_set.size())
            strtk::remove_trailing(single_delimiter_predicate<std::string::value_type>(removal_set[0]),s_);
         else
            strtk::remove_trailing(multiple_char_delimiter_predicate(removal_set),s_);
         return (*this);
      }

      template<typename T>
      inline ext_string& operator += (const T& t)
      {
         s_.append(type_to_string(t));
         return (*this);
      }

      inline ext_string& operator -= (const std::string& pattern)
      {
         replace(pattern,"");
         return (*this);
      }

      inline ext_string& operator *= (const std::size_t& n)
      {
         strtk::replicate_inplace(n,s_);
         return (*this);
      }

      inline void replace(const std::string& pattern, const std::string& replace_pattern)
      {
         std::string result;
         result.reserve(s_.size());
         strtk::replace_pattern(s_,pattern,replace_pattern,result);
         s_.assign(result);
      }

      template<typename DelimiterPredicate, typename OutputIterator>
      inline std::size_t split(const DelimiterPredicate& p,
                               OutputIterator out,
                               const split_options::type split_option = split_options::default_mode) const
      {
         return strtk::split(p,s_,out,split_option);
      }

      template<typename DelimiterPredicate,
               typename Allocator,
               template <typename, typename> class Sequence>
      inline std::size_t split(const DelimiterPredicate& p,
                               Sequence<std::string,Allocator>& seq,
                               const split_options::type split_option = split_options::default_mode) const
      {
         return strtk::split(p,s_,range_to_type_back_inserter(seq),split_option);
      }

      template<typename DelimiterPredicate, typename OutputIterator>
      inline std::size_t split_n(const DelimiterPredicate& p,
                                 const std::size_t& n,
                                 OutputIterator out,
                                 const split_options::type split_option = split_options::default_mode) const
      {
         return strtk::split_n(p,s_,n,out,split_option);
      }

      template<typename DelimiterPredicate,
               typename Allocator,
               template <typename, typename> class Sequence>
      inline std::size_t split_n(const DelimiterPredicate& p,
                                 const std::size_t& n,
                                 Sequence<std::string,Allocator>& seq,
                                 const split_options::type split_option = split_options::default_mode) const
      {
         return strtk::split_n(p,s_,n,range_to_type_back_inserter(seq),split_option);
      }

      template<typename T,
               typename Allocator,
               template <typename, typename> class Sequence>
      inline std::size_t parse(const std::string& delimiters, Sequence<T,Allocator>& seq) const
      {
         return strtk::parse(s_,delimiters,seq);
      }

      template<typename T,
               typename Allocator,
               template <typename, typename> class Sequence>
      inline std::size_t parse(const char* delimiters, Sequence<T,Allocator>& seq) const
      {
         return parse(std::string(delimiters),seq);
      }

      friend inline ext_string operator * (const std::size_t& n, const ext_string& s);
      friend inline ext_string operator * (const ext_string& s, const std::size_t& n);

      template<typename T>
      friend inline ext_string operator + (const ext_string& s, const T& t);

      template<typename T>
      friend inline ext_string operator + (const T& t, const ext_string& s);

      friend inline ext_string operator - (const ext_string& s, const std::string& pattern);
      friend inline ext_string operator - (const ext_string& s, const char* pattern);
      friend inline ext_string operator - (const ext_string& s, const ext_string& pattern);

      static inline ext_string all_digits()
      {
         static const ext_string digits("0123456789");
         return digits;
      }

      static inline ext_string all_letters()
      {
         static const ext_string letters("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
         return letters;
      }

      static inline ext_string all_lowercase_letters()
      {
         static const ext_string letters("abcdefghijklmnopqrstuvwxyz");
         return letters;
      }

      static inline ext_string all_uppercase_letters()
      {
         static const ext_string letters("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
         return letters;
      }

      static inline ext_string all_chars()
      {
         ext_string s;
         s.as_string().resize(256);
         strtk::iota(s.as_string().begin(),
                     s.as_string().end(),
                     static_cast<std::string::value_type>(0x00));
         return s;
      }

   private:

      std::string s_;
   };

   inline ext_string operator * (const std::size_t& n, const ext_string& s)
   {
      return ext_string(replicate(n, s.s_));
   }

   inline ext_string operator * (const ext_string& s, const std::size_t& n)
   {
      return ext_string(replicate(n, s.s_));
   }

   template<typename T>
   inline ext_string operator + (const ext_string& s, const T& t)
   {
      return ext_string(s.s_ + type_to_string(t));
   }

   template<typename T>
   inline ext_string operator + (const T& t, const ext_string& s)
   {
      return ext_string(type_to_string(t) + s.s_);
   }

   inline ext_string operator - (const ext_string& s, const std::string& pattern)
   {
      std::string tmp;
      tmp.reserve(s.s_.size());
      remove_pattern(s,pattern,tmp);
      return ext_string(tmp);
   }

   inline ext_string operator - (const ext_string& s, const char* pattern)
   {
      return s - std::string(pattern);
   }

   inline ext_string operator - (const ext_string& s, const ext_string& pattern)
   {
      return s - std::string(pattern.as_string());
   }

   static inline std::ostream& operator<<(std::ostream& os, const ext_string& es)
   {
      return (os << es.as_string());
   }

   namespace fileio
   {

      inline bool file_exists(const std::string& file_name)
      {
         std::ifstream file(file_name.c_str(), std::ios::binary);
         return ((!file) ? false : true);
      }

      inline std::size_t file_size(const std::string& file_name)
      {
         std::ifstream file(file_name.c_str(),std::ios::binary);
         if (!file) return 0;
         file.seekg (0, std::ios::end);
         return static_cast<std::size_t>(file.tellg());
      }

      inline bool load_file(const std::string& file_name, char* buffer, std::size_t buffer_size)
      {
         std::ifstream in_stream(file_name.c_str(),std::ios::binary);
         if (!in_stream) return false;
         in_stream.read(buffer,static_cast<std::streamsize>(buffer_size));
         in_stream.close();
         return true;
      }

      inline bool load_file(const std::string& file_name, std::string& buffer)
      {
         buffer.resize(file_size(file_name));
         return load_file(file_name,const_cast<char*>(buffer.c_str()),buffer.size());
      }

      inline bool write_file(const std::string& file_name, char* buffer, const std::size_t& buffer_size)
      {
         std::ofstream out_stream(file_name.c_str(),std::ios::binary);
         if (!out_stream) return false;
         out_stream.write(buffer,static_cast<std::streamsize>(buffer_size));
         out_stream.close();
         return true;
      }

      inline bool write_file(const std::string& file_name, const std::string& buffer)
      {
         return write_file(file_name,const_cast<char*>(buffer.data()),buffer.size());
      }

      inline bool copy_file(const std::string& src_file_name, const std::string& dest_file_name)
      {
         std::ifstream src_file(src_file_name.c_str(),std::ios::binary);
         std::ofstream dest_file(dest_file_name.c_str(),std::ios::binary);
         if (!src_file) return false;
         if (!dest_file) return false;

         static const std::size_t block_size = 16 * one_kilobyte;
         char buffer[block_size];

         std::size_t remaining_bytes = file_size(src_file_name);

         while (remaining_bytes >= block_size)
         {
            src_file.read(&buffer[0],static_cast<std::streamsize>(block_size));
            dest_file.write(&buffer[0],static_cast<std::streamsize>(block_size));
            remaining_bytes -= block_size;
         }

         if (remaining_bytes > 0)
         {
            src_file.read(&buffer[0],static_cast<std::streamsize>(remaining_bytes));
            dest_file.write(&buffer[0],static_cast<std::streamsize>(remaining_bytes));
            remaining_bytes = 0;
         }
         src_file.close();
         dest_file.close();
         return true;
      }

      inline bool concatenate(const std::string& file_name1,
                              const std::string& file_name2,
                              const std::string& output_file_name)
      {
         std::ifstream file1(file_name1.c_str(),std::ios::binary);
         std::ifstream file2(file_name2.c_str(),std::ios::binary);
         std::ofstream out_file(output_file_name.c_str(),std::ios::binary);

         if (!file1 || !file2 || !out_file) return false;

         static const std::size_t block_size = 16 * one_kilobyte;
         char buffer[block_size];
         unsigned int round = 0;
         std::size_t remaining_bytes = 0;

         while (round < 2)
         {
            std::ifstream& input_stream = ((round == 0) ? file1 : file2);
            remaining_bytes = ((round == 0) ? file_size(file_name1) : file_size(file_name2));

            while (remaining_bytes >= block_size)
            {
               input_stream.read(&buffer[0],static_cast<std::streamsize>(block_size));
               out_file.write(&buffer[0],static_cast<std::streamsize>(block_size));
               remaining_bytes -= block_size;
            }

            if (remaining_bytes > 0)
            {
               input_stream.read(&buffer[0],static_cast<std::streamsize>(remaining_bytes));
               out_file.write(&buffer[0],static_cast<std::streamsize>(remaining_bytes));
               remaining_bytes = 0;
            }

            input_stream.close();
            ++round;
         }
         out_file.close();
         return true;
      }

      inline bool files_identical(const std::string& file_name1, const std::string& file_name2)
      {
         std::ifstream file1(file_name1.c_str(),std::ios::binary);
         std::ifstream file2(file_name2.c_str(),std::ios::binary);
         if (!file1) return false;
         if (!file2) return false;
         if (file_size(file_name1) != file_size(file_name2)) return false;

         static const std::size_t block_size = 16 * one_kilobyte;
         char buffer1[block_size];
         char buffer2[block_size];

         std::size_t remaining_bytes = file_size(file_name1);

         while (remaining_bytes >= block_size)
         {
            file1.read(&buffer1[0],static_cast<std::streamsize>(block_size));
            file2.read(&buffer2[0],static_cast<std::streamsize>(block_size));
            if (0 != std::memcmp(buffer1,buffer2,block_size))
               return false;
            remaining_bytes -= block_size;
         }

         if (remaining_bytes > 0)
         {
            file1.read(&buffer1[0],static_cast<std::streamsize>(remaining_bytes));
            file2.read(&buffer2[0],static_cast<std::streamsize>(remaining_bytes));
            if (0 != std::memcmp(buffer1,buffer2,remaining_bytes))
               return false;
            remaining_bytes = 0;
         }

         file1.close();
         file2.close();

         return true;
      }

      namespace details
      {
         template<typename T>
         inline bool read_pod_proxy(std::ifstream& stream, T& t)
         {
            return (false == stream.read(reinterpret_cast<char*>(&t),
                                         static_cast<std::streamsize>(sizeof(T))).fail());
         }
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8,
               typename T9, typename T10>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8,
                           T9& t9, T10& t10)
      {
         return read_pod_proxy(stream, t1) &&
                read_pod_proxy(stream, t2) &&
                read_pod_proxy(stream, t3) &&
                read_pod_proxy(stream, t4) &&
                read_pod_proxy(stream, t5) &&
                read_pod_proxy(stream, t6) &&
                read_pod_proxy(stream, t7) &&
                read_pod_proxy(stream, t8) &&
                read_pod_proxy(stream, t9) &&
                read_pod_proxy(stream,t10);
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8,
               typename T9>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8,
                           T9& t9)
      {
         return read_pod_proxy(stream,t1) &&
                read_pod_proxy(stream,t2) &&
                read_pod_proxy(stream,t3) &&
                read_pod_proxy(stream,t4) &&
                read_pod_proxy(stream,t5) &&
                read_pod_proxy(stream,t6) &&
                read_pod_proxy(stream,t7) &&
                read_pod_proxy(stream,t8) &&
                read_pod_proxy(stream,t9);
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7, T8& t8)
      {
         return read_pod_proxy(stream,t1) &&
                read_pod_proxy(stream,t2) &&
                read_pod_proxy(stream,t3) &&
                read_pod_proxy(stream,t4) &&
                read_pod_proxy(stream,t5) &&
                read_pod_proxy(stream,t6) &&
                read_pod_proxy(stream,t7) &&
                read_pod_proxy(stream,t8);
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6, T7& t7)
      {
         return read_pod_proxy(stream,t1) &&
                read_pod_proxy(stream,t2) &&
                read_pod_proxy(stream,t3) &&
                read_pod_proxy(stream,t4) &&
                read_pod_proxy(stream,t5) &&
                read_pod_proxy(stream,t6) &&
                read_pod_proxy(stream,t7);
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5, T6& t6)
      {
         return read_pod_proxy(stream,t1) &&
                read_pod_proxy(stream,t2) &&
                read_pod_proxy(stream,t3) &&
                read_pod_proxy(stream,t4) &&
                read_pod_proxy(stream,t5) &&
                read_pod_proxy(stream,t6);
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2, T3& t3, T4& t4,
                           T5& t5)
      {
         return read_pod_proxy(stream,t1) &&
                read_pod_proxy(stream,t2) &&
                read_pod_proxy(stream,t3) &&
                read_pod_proxy(stream,t4) &&
                read_pod_proxy(stream,t5);
      }

      template<typename T1, typename T2, typename T3, typename T4>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2, T3& t3, T4& t4)
      {
         return read_pod_proxy(stream,t1) &&
                read_pod_proxy(stream,t2) &&
                read_pod_proxy(stream,t3) &&
                read_pod_proxy(stream,t4);
      }

      template<typename T1, typename T2, typename T3>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2, T3& t3)
      {
         return read_pod_proxy(stream,t1) &&
                read_pod_proxy(stream,t2) &&
                read_pod_proxy(stream,t3);
      }

      template<typename T1, typename T2>
      inline bool read_pod(std::ifstream& stream,
                           T1& t1, T2& t2)
      {
         return read_pod_proxy(stream,t1) &&
                read_pod_proxy(stream,t2);
      }

      template<typename T>
      inline bool read_pod(std::ifstream& stream, T& t)
      {
         return read_pod_proxy(stream,t);
      }

      template<typename T, std::size_t N>
      inline bool read_pod(std::ifstream& stream, T (&t)[N])
      {
         return (false != stream.read(reinterpret_cast<char*>(&t[0]),sizeof(T) * N).fail());
      }

      template<typename T,
               typename Allocator,
               template<typename,typename> class Sequence>
      inline bool read_pod(std::ifstream& stream,
                           const std::size_t& count,
                           Sequence<T,Allocator>& sequence)
      {
         T t;

         for (std::size_t i = 0; i < count; ++i)
         {
            if (!stream.read(reinterpret_cast<char*>(&t),static_cast<std::streamsize>(sizeof(T))).fail())
            {
               sequence.push_back(t);
            }
            else
               return false;
         }

         return true;
      }

      template<typename T,
               typename Comparator,
               typename Allocator>
      inline bool read_pod(std::ifstream& stream,
                           const std::size_t& count,
                           std::set<T,Comparator,Allocator>& set)
      {
         T t;

         for (std::size_t i = 0; i < count; ++i)
         {
            if (!stream.read(reinterpret_cast<char*>(&t),static_cast<std::streamsize>(sizeof(T))).fail())
            {
               set.insert(t);
            }
            else
               return false;
         }

         return true;
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8,
               typename T9, typename T10>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                            const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                            const T9& t9, const T10& t10)
      {
         stream.write(reinterpret_cast<char*>(&const_cast< T1&>( t1)),static_cast<std::streamsize>(sizeof( T1)));
         stream.write(reinterpret_cast<char*>(&const_cast< T2&>( t2)),static_cast<std::streamsize>(sizeof( T2)));
         stream.write(reinterpret_cast<char*>(&const_cast< T3&>( t3)),static_cast<std::streamsize>(sizeof( T3)));
         stream.write(reinterpret_cast<char*>(&const_cast< T4&>( t4)),static_cast<std::streamsize>(sizeof( T4)));
         stream.write(reinterpret_cast<char*>(&const_cast< T5&>( t5)),static_cast<std::streamsize>(sizeof( T5)));
         stream.write(reinterpret_cast<char*>(&const_cast< T6&>( t6)),static_cast<std::streamsize>(sizeof( T6)));
         stream.write(reinterpret_cast<char*>(&const_cast< T7&>( t7)),static_cast<std::streamsize>(sizeof( T7)));
         stream.write(reinterpret_cast<char*>(&const_cast< T8&>( t8)),static_cast<std::streamsize>(sizeof( T8)));
         stream.write(reinterpret_cast<char*>(&const_cast< T9&>( t9)),static_cast<std::streamsize>(sizeof( T9)));
         stream.write(reinterpret_cast<char*>(&const_cast<T10&>(t10)),static_cast<std::streamsize>(sizeof(T10)));
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8,
               typename T9>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                            const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                            const T9& t9)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t1)),static_cast<std::streamsize>(sizeof(T1)));
         stream.write(reinterpret_cast<char*>(&const_cast<T2&>(t2)),static_cast<std::streamsize>(sizeof(T2)));
         stream.write(reinterpret_cast<char*>(&const_cast<T3&>(t3)),static_cast<std::streamsize>(sizeof(T3)));
         stream.write(reinterpret_cast<char*>(&const_cast<T4&>(t4)),static_cast<std::streamsize>(sizeof(T4)));
         stream.write(reinterpret_cast<char*>(&const_cast<T5&>(t5)),static_cast<std::streamsize>(sizeof(T5)));
         stream.write(reinterpret_cast<char*>(&const_cast<T6&>(t6)),static_cast<std::streamsize>(sizeof(T6)));
         stream.write(reinterpret_cast<char*>(&const_cast<T7&>(t7)),static_cast<std::streamsize>(sizeof(T7)));
         stream.write(reinterpret_cast<char*>(&const_cast<T8&>(t8)),static_cast<std::streamsize>(sizeof(T8)));
         stream.write(reinterpret_cast<char*>(&const_cast<T9&>(t9)),static_cast<std::streamsize>(sizeof(T9)));
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7, typename T8>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                            const T5& t5, const T6& t6, const T7& t7, const T8& t8)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t1)),static_cast<std::streamsize>(sizeof(T1)));
         stream.write(reinterpret_cast<char*>(&const_cast<T2&>(t2)),static_cast<std::streamsize>(sizeof(T2)));
         stream.write(reinterpret_cast<char*>(&const_cast<T3&>(t3)),static_cast<std::streamsize>(sizeof(T3)));
         stream.write(reinterpret_cast<char*>(&const_cast<T4&>(t4)),static_cast<std::streamsize>(sizeof(T4)));
         stream.write(reinterpret_cast<char*>(&const_cast<T5&>(t5)),static_cast<std::streamsize>(sizeof(T5)));
         stream.write(reinterpret_cast<char*>(&const_cast<T6&>(t6)),static_cast<std::streamsize>(sizeof(T6)));
         stream.write(reinterpret_cast<char*>(&const_cast<T7&>(t7)),static_cast<std::streamsize>(sizeof(T7)));
         stream.write(reinterpret_cast<char*>(&const_cast<T8&>(t8)),static_cast<std::streamsize>(sizeof(T8)));
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6, typename T7>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                            const T5& t5, const T6& t6, const T7& t7)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t1)),static_cast<std::streamsize>(sizeof(T1)));
         stream.write(reinterpret_cast<char*>(&const_cast<T2&>(t2)),static_cast<std::streamsize>(sizeof(T2)));
         stream.write(reinterpret_cast<char*>(&const_cast<T3&>(t3)),static_cast<std::streamsize>(sizeof(T3)));
         stream.write(reinterpret_cast<char*>(&const_cast<T4&>(t4)),static_cast<std::streamsize>(sizeof(T4)));
         stream.write(reinterpret_cast<char*>(&const_cast<T5&>(t5)),static_cast<std::streamsize>(sizeof(T5)));
         stream.write(reinterpret_cast<char*>(&const_cast<T6&>(t6)),static_cast<std::streamsize>(sizeof(T6)));
         stream.write(reinterpret_cast<char*>(&const_cast<T7&>(t7)),static_cast<std::streamsize>(sizeof(T7)));
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5, typename T6>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                            const T5& t5, const T6& t6)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t1)),static_cast<std::streamsize>(sizeof(T1)));
         stream.write(reinterpret_cast<char*>(&const_cast<T2&>(t2)),static_cast<std::streamsize>(sizeof(T2)));
         stream.write(reinterpret_cast<char*>(&const_cast<T3&>(t3)),static_cast<std::streamsize>(sizeof(T3)));
         stream.write(reinterpret_cast<char*>(&const_cast<T4&>(t4)),static_cast<std::streamsize>(sizeof(T4)));
         stream.write(reinterpret_cast<char*>(&const_cast<T5&>(t5)),static_cast<std::streamsize>(sizeof(T5)));
         stream.write(reinterpret_cast<char*>(&const_cast<T6&>(t6)),static_cast<std::streamsize>(sizeof(T6)));
      }

      template<typename T1, typename T2, typename T3, typename T4,
               typename T5>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                            const T5& t5)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t1)),static_cast<std::streamsize>(sizeof(T1)));
         stream.write(reinterpret_cast<char*>(&const_cast<T2&>(t2)),static_cast<std::streamsize>(sizeof(T2)));
         stream.write(reinterpret_cast<char*>(&const_cast<T3&>(t3)),static_cast<std::streamsize>(sizeof(T3)));
         stream.write(reinterpret_cast<char*>(&const_cast<T4&>(t4)),static_cast<std::streamsize>(sizeof(T4)));
         stream.write(reinterpret_cast<char*>(&const_cast<T5&>(t5)),static_cast<std::streamsize>(sizeof(T5)));
      }

      template<typename T1, typename T2, typename T3, typename T4>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2, const T3& t3, const T4& t4)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t1)),static_cast<std::streamsize>(sizeof(T1)));
         stream.write(reinterpret_cast<char*>(&const_cast<T2&>(t2)),static_cast<std::streamsize>(sizeof(T2)));
         stream.write(reinterpret_cast<char*>(&const_cast<T3&>(t3)),static_cast<std::streamsize>(sizeof(T3)));
         stream.write(reinterpret_cast<char*>(&const_cast<T4&>(t4)),static_cast<std::streamsize>(sizeof(T4)));
      }

      template<typename T1, typename T2, typename T3>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2, const T3& t3)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t1)),static_cast<std::streamsize>(sizeof(T1)));
         stream.write(reinterpret_cast<char*>(&const_cast<T2&>(t2)),static_cast<std::streamsize>(sizeof(T2)));
         stream.write(reinterpret_cast<char*>(&const_cast<T3&>(t3)),static_cast<std::streamsize>(sizeof(T3)));
      }

      template<typename T1, typename T2>
      inline void write_pod(std::ofstream& stream,
                            const T1& t1, const T2& t2)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t1)),static_cast<std::streamsize>(sizeof(T1)));
         stream.write(reinterpret_cast<char*>(&const_cast<T1&>(t2)),static_cast<std::streamsize>(sizeof(T2)));
      }

      template<typename T>
      inline void write_pod(std::ofstream& stream, const T& t)
      {
         stream.write(reinterpret_cast<char*>(&const_cast<T&>(t)),static_cast<std::streamsize>(sizeof(T)));
      }

      template<typename T, std::size_t N>
      inline void write_pod(std::ofstream& stream, T (&t)[N])
      {
         stream.write(reinterpret_cast<char*>(&t[0]),static_cast<std::streamsize>(sizeof(T) * N));
      }

      template<typename T,
               typename Allocator,
               template<typename,typename> class Sequence>
      inline void write_pod(std::ofstream& stream,
                            const Sequence<T,Allocator>& sequence)
      {
         typename Sequence<T,Allocator>::iterator itr = sequence.begin();
         typename Sequence<T,Allocator>::iterator end = sequence.end();
         while (end != itr)
         {
            stream.write(reinterpret_cast<char*>(&const_cast<T&>(*itr)),static_cast<std::streamsize>(sizeof(T)));
            ++itr;
         }
      }

      template<typename T,
               typename Comparator,
               typename Allocator>
      inline void write_pod(std::ofstream& stream,
                            const std::set<T,Comparator,Allocator>& set)
      {
         typename std::set<T,Comparator,Allocator>::iterator itr = set.begin();
         typename std::set<T,Comparator,Allocator>::iterator end = set.end();
         while (end != itr)
         {
            stream.write(reinterpret_cast<char*>(&const_cast<T&>(*itr)),static_cast<std::streamsize>(sizeof(T)));
            ++itr;
         }
      }

      inline bool read_at_offset(std::ifstream& stream,
                                 const std::size_t& offset,
                                 char* buffer,
                                 const std::size_t& buffer_size)
      {
         if (!stream) return false;
         stream.seekg(offset,std::ios_base::beg);
         if (stream.fail()) return false;
         stream.read(buffer,buffer_size);
         if (stream.fail()) return false;
         stream.close();
         return true;
      }

      inline bool read_at_offset(const std::string& file_name,
                                 const std::size_t& offset,
                                 char* buffer,
                                 const std::size_t& buffer_size)
      {
         std::ifstream stream(file_name.c_str(), std::ios::in | std::ios::binary);
         if (!stream) return false;
         return read_at_offset(stream,offset,buffer,buffer_size);
      }

      inline bool read_at_offset(const std::string& file_name,
                                 const std::size_t& offset,
                                 std::string& buffer,
                                 const std::size_t& buffer_size)
      {
         std::ifstream stream(file_name.c_str(), std::ios::in | std::ios::binary);
         if (!stream) return false;
         buffer.resize(buffer_size);
         return read_at_offset(stream,
                               offset,
                               const_cast<char*>(buffer.c_str()),
                               buffer_size);
      }

   } // namespace fileio

   template<typename T1,  typename T2, typename  T3, typename  T4,
            typename T5,  typename T6, typename  T7, typename  T8,
            typename T9, typename T10, typename T11, typename T12>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2&   t2, T3&   t3, T4&   t4,
                                  T5& t5, T6&   t6, T7&   t7, T8&   t8,
                                  T9& t9, T10& t10, T11& t11, T12& t12)
   {
      t1 = (*reinterpret_cast< T1*>(data)); data += sizeof( T1);
      t2 = (*reinterpret_cast< T2*>(data)); data += sizeof( T2);
      t3 = (*reinterpret_cast< T3*>(data)); data += sizeof( T3);
      t4 = (*reinterpret_cast< T4*>(data)); data += sizeof( T4);
      t5 = (*reinterpret_cast< T5*>(data)); data += sizeof( T5);
      t6 = (*reinterpret_cast< T6*>(data)); data += sizeof( T6);
      t7 = (*reinterpret_cast< T7*>(data)); data += sizeof( T7);
      t8 = (*reinterpret_cast< T8*>(data)); data += sizeof( T8);
      t9 = (*reinterpret_cast< T9*>(data)); data += sizeof( T9);
     t10 = (*reinterpret_cast<T10*>(data)); data += sizeof(T10);
     t11 = (*reinterpret_cast<T11*>(data)); data += sizeof(T11);
     t12 = (*reinterpret_cast<T12*>(data)); data += sizeof(T12);
     return data;
   }

   template<typename T1,  typename T2, typename  T3, typename T4,
            typename T5,  typename T6, typename  T7, typename T8,
            typename T9, typename T10, typename T11>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2&   t2, T3&   t3, T4& t4,
                                  T5& t5, T6&   t6, T7&   t7, T8& t8,
                                  T9& t9, T10& t10, T11& t11)
   {
      t1 = (*reinterpret_cast< T1*>(data)); data += sizeof( T1);
      t2 = (*reinterpret_cast< T2*>(data)); data += sizeof( T2);
      t3 = (*reinterpret_cast< T3*>(data)); data += sizeof( T3);
      t4 = (*reinterpret_cast< T4*>(data)); data += sizeof( T4);
      t5 = (*reinterpret_cast< T5*>(data)); data += sizeof( T5);
      t6 = (*reinterpret_cast< T6*>(data)); data += sizeof( T6);
      t7 = (*reinterpret_cast< T7*>(data)); data += sizeof( T7);
      t8 = (*reinterpret_cast< T8*>(data)); data += sizeof( T8);
      t9 = (*reinterpret_cast< T9*>(data)); data += sizeof( T9);
     t10 = (*reinterpret_cast<T10*>(data)); data += sizeof(T10);
     t11 = (*reinterpret_cast<T11*>(data)); data += sizeof(T11);
     return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2, T3& t3, T4& t4,
                                  T5& t5, T6& t6, T7& t7, T8& t8,
                                  T9& t9, T10& t10)
   {
       t1 = (*reinterpret_cast< T1*>(data)); data += sizeof( T1);
       t2 = (*reinterpret_cast< T2*>(data)); data += sizeof( T2);
       t3 = (*reinterpret_cast< T3*>(data)); data += sizeof( T3);
       t4 = (*reinterpret_cast< T4*>(data)); data += sizeof( T4);
       t5 = (*reinterpret_cast< T5*>(data)); data += sizeof( T5);
       t6 = (*reinterpret_cast< T6*>(data)); data += sizeof( T6);
       t7 = (*reinterpret_cast< T7*>(data)); data += sizeof( T7);
       t8 = (*reinterpret_cast< T8*>(data)); data += sizeof( T8);
       t9 = (*reinterpret_cast< T9*>(data)); data += sizeof( T9);
      t10 = (*reinterpret_cast<T10*>(data)); data += sizeof(T10);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2, T3& t3, T4& t4,
                                  T5& t5, T6& t6, T7& t7, T8& t8,
                                  T9& t9)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      t2 = (*reinterpret_cast<T2*>(data)); data += sizeof(T2);
      t3 = (*reinterpret_cast<T3*>(data)); data += sizeof(T3);
      t4 = (*reinterpret_cast<T4*>(data)); data += sizeof(T4);
      t5 = (*reinterpret_cast<T5*>(data)); data += sizeof(T5);
      t6 = (*reinterpret_cast<T6*>(data)); data += sizeof(T6);
      t7 = (*reinterpret_cast<T7*>(data)); data += sizeof(T7);
      t8 = (*reinterpret_cast<T8*>(data)); data += sizeof(T8);
      t9 = (*reinterpret_cast<T9*>(data)); data += sizeof(T9);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2, T3& t3, T4& t4,
                                  T5& t5, T6& t6, T7& t7, T8& t8)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      t2 = (*reinterpret_cast<T2*>(data)); data += sizeof(T2);
      t3 = (*reinterpret_cast<T3*>(data)); data += sizeof(T3);
      t4 = (*reinterpret_cast<T4*>(data)); data += sizeof(T4);
      t5 = (*reinterpret_cast<T5*>(data)); data += sizeof(T5);
      t6 = (*reinterpret_cast<T6*>(data)); data += sizeof(T6);
      t7 = (*reinterpret_cast<T7*>(data)); data += sizeof(T7);
      t8 = (*reinterpret_cast<T8*>(data)); data += sizeof(T8);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2, T3& t3, T4& t4,
                                  T5& t5, T6& t6, T7& t7)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      t2 = (*reinterpret_cast<T2*>(data)); data += sizeof(T2);
      t3 = (*reinterpret_cast<T3*>(data)); data += sizeof(T3);
      t4 = (*reinterpret_cast<T4*>(data)); data += sizeof(T4);
      t5 = (*reinterpret_cast<T5*>(data)); data += sizeof(T5);
      t6 = (*reinterpret_cast<T6*>(data)); data += sizeof(T6);
      t7 = (*reinterpret_cast<T7*>(data)); data += sizeof(T7);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2, T3& t3, T4& t4,
                                  T5& t5, T6& t6)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      t2 = (*reinterpret_cast<T2*>(data)); data += sizeof(T2);
      t3 = (*reinterpret_cast<T3*>(data)); data += sizeof(T3);
      t4 = (*reinterpret_cast<T4*>(data)); data += sizeof(T4);
      t5 = (*reinterpret_cast<T5*>(data)); data += sizeof(T5);
      t6 = (*reinterpret_cast<T6*>(data)); data += sizeof(T6);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2, T3& t3, T4& t4,
                                  T5& t5)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      t2 = (*reinterpret_cast<T2*>(data)); data += sizeof(T2);
      t3 = (*reinterpret_cast<T3*>(data)); data += sizeof(T3);
      t4 = (*reinterpret_cast<T4*>(data)); data += sizeof(T4);
      t5 = (*reinterpret_cast<T5*>(data)); data += sizeof(T5);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2, T3& t3, T4& t4)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      t2 = (*reinterpret_cast<T2*>(data)); data += sizeof(T2);
      t3 = (*reinterpret_cast<T3*>(data)); data += sizeof(T3);
      t4 = (*reinterpret_cast<T4*>(data)); data += sizeof(T4);
      return data;
   }

   template<typename T1, typename T2, typename T3>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2, T3& t3)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      t2 = (*reinterpret_cast<T2*>(data)); data += sizeof(T2);
      t3 = (*reinterpret_cast<T3*>(data)); data += sizeof(T3);
      return data;
   }

   template<typename T1, typename T2>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1, T2& t2)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      t2 = (*reinterpret_cast<T2*>(data)); data += sizeof(T2);
      return data;
   }

   template<typename T1>
   inline unsigned char* read_pod(unsigned char* data,
                                  T1& t1)
   {
      t1 = (*reinterpret_cast<T1*>(data)); data += sizeof(T1);
      return data;
   }

   template<typename T, std::size_t N>
   inline unsigned char* read_pod(unsigned char* data, T (&t)[N])
   {
      T* begin = reinterpret_cast<T*>(data);
      T* end = begin + N;
      std::copy(begin,end,&t[0]);
      return data + (N * sizeof(T));
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline unsigned char* read_pod(unsigned char* data,
                                  const std::size_t& n,
                                  const Sequence<T,Allocator>& sequence)
   {
      T* ptr = reinterpret_cast<T>(data);
      std::copy(ptr, ptr + n, std::back_inserter(sequence));
      return data + (sequence.size() * sizeof(T));
   }

   template<typename T,
            typename Allocator,
            typename Comparator>
   inline unsigned char* read_pod(unsigned char* data,
                                  const std::size_t& n,
                                  const std::set<T,Comparator,Allocator>& set)
   {
      T* ptr = reinterpret_cast<T>(data);
      std::copy(ptr, ptr + n, std::inserter(set,set.begin()));
      return data + (set.size() * sizeof(T));
   }

   template<typename T1,  typename T2, typename  T3, typename  T4,
            typename T5,  typename T6, typename  T7, typename  T8,
            typename T9, typename T10, typename T11, typename T12>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2&   t2, const T3&   t3, const T4& t4,
                                   const T5& t5, const T6&   t6, const T7&   t7, const T8& t8,
                                   const T9& t9, const T10& t10, const T11& t11, const T12& t12)
   {
      (*reinterpret_cast< T1*>(data)) =  t1; data += sizeof( T1);
      (*reinterpret_cast< T2*>(data)) =  t2; data += sizeof( T2);
      (*reinterpret_cast< T3*>(data)) =  t3; data += sizeof( T3);
      (*reinterpret_cast< T4*>(data)) =  t4; data += sizeof( T4);
      (*reinterpret_cast< T5*>(data)) =  t5; data += sizeof( T5);
      (*reinterpret_cast< T6*>(data)) =  t6; data += sizeof( T6);
      (*reinterpret_cast< T7*>(data)) =  t7; data += sizeof( T7);
      (*reinterpret_cast< T8*>(data)) =  t8; data += sizeof( T8);
      (*reinterpret_cast< T9*>(data)) =  t9; data += sizeof( T9);
      (*reinterpret_cast<T10*>(data)) = t10; data += sizeof(T10);
      (*reinterpret_cast<T11*>(data)) = t11; data += sizeof(T11);
      (*reinterpret_cast<T12*>(data)) = t12; data += sizeof(T12);
      return data;
   }

   template<typename T1,  typename T2, typename  T3, typename T4,
            typename T5,  typename T6, typename  T7, typename T8,
            typename T9, typename T10, typename T11>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2&   t2, const T3&   t3, const T4& t4,
                                   const T5& t5, const T6&   t6, const T7&   t7, const T8& t8,
                                   const T9& t9, const T10& t10, const T11& t11)
   {
      (*reinterpret_cast< T1*>(data)) =  t1; data += sizeof( T1);
      (*reinterpret_cast< T2*>(data)) =  t2; data += sizeof( T2);
      (*reinterpret_cast< T3*>(data)) =  t3; data += sizeof( T3);
      (*reinterpret_cast< T4*>(data)) =  t4; data += sizeof( T4);
      (*reinterpret_cast< T5*>(data)) =  t5; data += sizeof( T5);
      (*reinterpret_cast< T6*>(data)) =  t6; data += sizeof( T6);
      (*reinterpret_cast< T7*>(data)) =  t7; data += sizeof( T7);
      (*reinterpret_cast< T8*>(data)) =  t8; data += sizeof( T8);
      (*reinterpret_cast< T9*>(data)) =  t9; data += sizeof( T9);
      (*reinterpret_cast<T10*>(data)) = t10; data += sizeof(T10);
      (*reinterpret_cast<T11*>(data)) = t11; data += sizeof(T11);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9, typename T10>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                                   const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                                   const T9& t9, const T10& t10)
   {
      (*reinterpret_cast< T1*>(data)) =  t1; data += sizeof( T1);
      (*reinterpret_cast< T2*>(data)) =  t2; data += sizeof( T2);
      (*reinterpret_cast< T3*>(data)) =  t3; data += sizeof( T3);
      (*reinterpret_cast< T4*>(data)) =  t4; data += sizeof( T4);
      (*reinterpret_cast< T5*>(data)) =  t5; data += sizeof( T5);
      (*reinterpret_cast< T6*>(data)) =  t6; data += sizeof( T6);
      (*reinterpret_cast< T7*>(data)) =  t7; data += sizeof( T7);
      (*reinterpret_cast< T8*>(data)) =  t8; data += sizeof( T8);
      (*reinterpret_cast< T9*>(data)) =  t9; data += sizeof( T9);
      (*reinterpret_cast<T10*>(data)) = t10; data += sizeof(T10);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8,
            typename T9>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                                   const T5& t5, const T6& t6, const T7& t7, const T8& t8,
                                   const T9& t9)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      (*reinterpret_cast<T2*>(data)) = t2; data += sizeof(T2);
      (*reinterpret_cast<T3*>(data)) = t3; data += sizeof(T3);
      (*reinterpret_cast<T4*>(data)) = t4; data += sizeof(T4);
      (*reinterpret_cast<T5*>(data)) = t5; data += sizeof(T5);
      (*reinterpret_cast<T6*>(data)) = t6; data += sizeof(T6);
      (*reinterpret_cast<T7*>(data)) = t7; data += sizeof(T7);
      (*reinterpret_cast<T8*>(data)) = t8; data += sizeof(T8);
      (*reinterpret_cast<T9*>(data)) = t9; data += sizeof(T9);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7, typename T8>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                                   const T5& t5, const T6& t6, const T7& t7, const T8& t8)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      (*reinterpret_cast<T2*>(data)) = t2; data += sizeof(T2);
      (*reinterpret_cast<T3*>(data)) = t3; data += sizeof(T3);
      (*reinterpret_cast<T4*>(data)) = t4; data += sizeof(T4);
      (*reinterpret_cast<T5*>(data)) = t5; data += sizeof(T5);
      (*reinterpret_cast<T6*>(data)) = t6; data += sizeof(T6);
      (*reinterpret_cast<T7*>(data)) = t7; data += sizeof(T7);
      (*reinterpret_cast<T8*>(data)) = t8; data += sizeof(T8);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6, typename T7>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                                   const T5& t5, const T6& t6, const T7& t7)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      (*reinterpret_cast<T2*>(data)) = t2; data += sizeof(T2);
      (*reinterpret_cast<T3*>(data)) = t3; data += sizeof(T3);
      (*reinterpret_cast<T4*>(data)) = t4; data += sizeof(T4);
      (*reinterpret_cast<T5*>(data)) = t5; data += sizeof(T5);
      (*reinterpret_cast<T6*>(data)) = t6; data += sizeof(T6);
      (*reinterpret_cast<T7*>(data)) = t7; data += sizeof(T7);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5, typename T6>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                                   const T5& t5, const T6& t6)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      (*reinterpret_cast<T2*>(data)) = t2; data += sizeof(T2);
      (*reinterpret_cast<T3*>(data)) = t3; data += sizeof(T3);
      (*reinterpret_cast<T4*>(data)) = t4; data += sizeof(T4);
      (*reinterpret_cast<T5*>(data)) = t5; data += sizeof(T5);
      (*reinterpret_cast<T6*>(data)) = t6; data += sizeof(T6);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4,
            typename T5>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2, const T3& t3, const T4& t4,
                                   const T5& t5)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      (*reinterpret_cast<T2*>(data)) = t2; data += sizeof(T2);
      (*reinterpret_cast<T3*>(data)) = t3; data += sizeof(T3);
      (*reinterpret_cast<T4*>(data)) = t4; data += sizeof(T4);
      (*reinterpret_cast<T5*>(data)) = t5; data += sizeof(T5);
      return data;
   }

   template<typename T1, typename T2, typename T3, typename T4>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2, const T3& t3, const T4& t4)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      (*reinterpret_cast<T2*>(data)) = t2; data += sizeof(T2);
      (*reinterpret_cast<T3*>(data)) = t3; data += sizeof(T3);
      (*reinterpret_cast<T4*>(data)) = t4; data += sizeof(T4);
      return data;
   }

   template<typename T1, typename T2, typename T3>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2, const T3& t3)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      (*reinterpret_cast<T2*>(data)) = t2; data += sizeof(T2);
      (*reinterpret_cast<T3*>(data)) = t3; data += sizeof(T3);
      return data;
   }

   template<typename T1, typename T2>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1, const T2& t2)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      (*reinterpret_cast<T2*>(data)) = t2; data += sizeof(T2);
      return data;
   }

   template<typename T1>
   inline unsigned char* write_pod(unsigned char* data,
                                   const T1& t1)
   {
      (*reinterpret_cast<T1*>(data)) = t1; data += sizeof(T1);
      return data;
   }

   template<typename T, std::size_t N>
   inline unsigned char* write_pod(unsigned char* data, const T (&t)[N])
   {
      T* ptr = reinterpret_cast<T*>(data);
      std::copy(t,t + N,ptr);
      return data + (N * sizeof(T));
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline unsigned char* write_pod(unsigned char* data,
                                   const Sequence<T,Allocator>& sequence)
   {
      T* ptr = reinterpret_cast<T>(data);
      std::copy(sequence.begin(),sequence.end(),ptr);
      return data + (sequence.size() * sizeof(T));
   }

   template<typename T,
            typename Allocator,
            typename Comparator>
   inline unsigned char* write_pod(unsigned char* data,
                                   const std::set<T,Comparator,Allocator>& set)
   {
      T* ptr = reinterpret_cast<T>(data);
      std::copy(set.begin(),set.end(),ptr);
      return data + (set.size() * sizeof(T));
   }

   namespace trie
   {
      template<typename KeyIterator, typename ValueType>
      class prefix
      {

         template <typename Iterator,
                   typename Value,
                   typename KeyValue = typename std::iterator_traits<Iterator>::value_type>
         struct node
         {
         public:

            typedef KeyValue key_value_t;
            typedef Value value_t;

            typedef node<Iterator,Value,KeyValue> node_t;
            typedef node_t* node_ptr;
            typedef const node_ptr const_node_ptr;

            typedef std::vector<node_ptr> node_list_t;
            typedef typename node_list_t::const_iterator node_list_iterator;

            explicit node(const key_value_t& key_value)
            : key_value_(key_value),
              value_holder_(false)
            {}

            node(const key_value_t& key_value,
                 const value_t& v)
            : key_value_(key_value),
              value_holder_(true),
              value_(v)
            {}

           ~node()
            {
               if (!node_list_.empty())
               {
                  node_list_iterator itr = node_list_.begin();
                  node_list_iterator end = node_list_.end();
                  while (end != itr)
                  {
                     delete *itr++;
                  }
               }
            }

            inline node_ptr get_node(const key_value_t& key_value)
            {
               if (node_list_.empty())
                  return 0;
               node_list_iterator itr = node_list_.begin();
               const node_list_iterator end = node_list_.end();
               while (end != itr)
               {
                  if (key_value == (*itr)->key_value_)
                     return *itr;
                  else
                     ++itr;
               }
               return 0;
            }

            inline void assign_value(const value_t& v)
            {
               value_ = v;
               value_holder_ = true;
            }

            inline void add_node(node_ptr n)
            {
               node_list_.push_back(n);
            }

            inline bool value_holder() const
            {
               return value_holder_;
            }

            inline const value_t& value() const
            {
               return value_;
            }

            inline const key_value_t& key() const
            {
               return key_value_;
            }

         private:

            node(const node_t& n);
            node_t& operator=(const node_t& n);

            key_value_t key_value_;
            bool value_holder_;
            value_t value_;
            node_list_t node_list_;
         };

      public:

         //typedef KeyIterator key_iterator_t;
         typedef typename std::iterator_traits<KeyIterator>::value_type key_value_t;
         typedef ValueType value_t;

         typedef node<KeyIterator,value_t> node_t;
         typedef node_t* node_ptr;

         prefix()
         : head_(0)
         {}

         template<typename key_iterator_t>
         inline void insert(const key_iterator_t begin,
                            const key_iterator_t end,
                            const value_t& v)
         {
            if (0 == std::distance(begin,end))
               return;
            key_iterator_t itr = begin;
            key_value_t key = *itr;
            node_ptr parent = 0;
            node_ptr n = head_;
            node_ptr next_node = 0;
            n = head_ = ((0 == head_) ? new node_t(*itr) : head_);

            while (end != itr)
            {
               key = *itr;
               if (0 == (next_node = n->get_node(key)))
               {
                  n->add_node(next_node = new node_t(key));
               }
               parent = n;
               n = next_node;
               ++itr;
            }

            parent->assign_value(v);
         }

         template<typename key_iterator_t>
         inline bool find(const key_iterator_t begin,
                          const key_iterator_t end,
                          value_t& v) const
         {
            if ((0 == head_) || (0 == std::distance(begin,end)))
               return false;
            key_iterator_t itr = begin;
            node_ptr parent = head_;
            node_ptr n = head_;
            while (end != itr)
            {
               node_ptr next_node = n->get_node(*itr);
               if (0 == next_node)
                  return false;
               parent = n;
               n = next_node;
               ++itr;
            }
            if (!parent->value_holder())
               return false;
             v = parent->value();
             return true;
         }

         template<typename key_iterator_t>
         inline bool find_prefix(const key_iterator_t begin, const key_iterator_t end) const
         {
            if ((0 == head_) || (0 == std::distance(begin,end)))
               return false;

            key_iterator_t itr = begin;
            node_ptr n = head_;

            while (end != itr)
            {
               if (0 == (n = n->get_node(*itr)))
                  return false;
               ++itr;
            }

            return true;
         }

        ~prefix()
         {
            delete head_;
         }

      private:

         node_ptr head_;
      };

      template<typename Value>
      inline void insert(prefix<std::string::const_iterator,Value>& trie,
                         const std::string& key,
                         const Value& value = Value(0))
      {
         trie.insert(key.begin(),key.end(),value);
      }

      template<typename Value>
      inline void insert(prefix<std::string::iterator,Value>& trie,
                         const char* key,
                         const Value& value = Value(0))
      {
         insert_into_trie(trie,std::string(key),value);
      }

      template<typename Value>
      inline bool find(prefix<std::string::const_iterator,Value>& trie,
                       const std::string& key,
                       Value& v)
      {
         return trie.find(key.begin(),key.end(),v);
      }

      template<typename Value>
      inline bool find(prefix<std::string::const_iterator,Value>& trie,
                       const char* key,
                       Value& v)
      {
         return find_prefix(trie,std::string(key),v);
      }

      template<typename Value>
      inline bool find_prefix(prefix<std::string::const_iterator,Value>& trie,
                              const std::string& key)
      {
         return trie.find_prefix(key.begin(),key.end());
      }

      template<typename Value>
      inline bool find_prefix(prefix<std::string::const_iterator,Value>& trie,
                              const char* key)
      {
         return find_prefix(trie,std::string(key));
      }

   } // namespace trie

   template<typename ValueType, typename KeyIterator = std::string::const_iterator>
   struct prefix_trie
   {
      typedef trie::prefix<KeyIterator,ValueType> type;
      typedef trie::prefix<KeyIterator,ValueType> std_string;
      typedef trie::prefix<char*,ValueType> char_ptr;
      typedef trie::prefix<const char*,ValueType> const_char_ptr;
      typedef trie::prefix<unsigned char*,ValueType> uchar_ptr;
      typedef trie::prefix<const unsigned char*,ValueType> const_uchar_ptr;
   };

   namespace details
   {

      inline void compute_pod_hash(const char data[], unsigned int& hash)
      {
         hash ^=  ((hash <<  7) ^  data[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (data[1] ^ (hash >> 5)));
      }

      inline void compute_pod_hash(const unsigned char data[], unsigned int& hash)
      {
         hash ^=  ((hash <<  7) ^  data[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (data[1] ^ (hash >> 5)));
      }

      inline void compute_pod_hash(const int& data, unsigned int& hash)
      {
         const unsigned char* itr = reinterpret_cast<const unsigned char*>(&data);
         hash ^=  ((hash <<  7) ^  itr[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[1] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  itr[2] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[3] ^ (hash >> 5)));
      }

      inline void compute_pod_hash(const unsigned int& data, unsigned int& hash)
      {
         compute_pod_hash(static_cast<int>(data),hash);
      }

      inline void compute_pod_hash(const unsigned long long& data, unsigned int& hash)
      {
         const unsigned char* itr = reinterpret_cast<const unsigned char*>(&data);
         hash ^=  ((hash <<  7) ^  itr[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[1] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  itr[2] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[3] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  itr[4] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[5] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  itr[6] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[7] ^ (hash >> 5)));
      }

      inline void compute_pod_hash(const double& data, unsigned int& hash)
      {
         const unsigned char* itr = reinterpret_cast<const unsigned char*>(&data);
         hash ^=  ((hash <<  7) ^  itr[0] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[1] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  itr[2] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[3] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  itr[4] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[5] ^ (hash >> 5)));
         hash ^=  ((hash <<  7) ^  itr[6] * (hash >> 3));
         hash ^= ~((hash << 11) + (itr[7] ^ (hash >> 5)));
      }

      template<std::size_t block_size, typename Iterator>
      inline void compute_block(Iterator itr, std::size_t& length, unsigned int& hash)
      {
         while (length >= block_size)
         {
            for (std::size_t i = 0; i < block_size; ++i, ++itr)
            {
               compute_pod_hash((*itr),hash);
            }
            length -= block_size;
         }
      }

      template<std::size_t block_size>
      inline void compute_block(unsigned char* itr, std::size_t& length, unsigned int& hash)
      {
         while (length >= block_size)
         {
            for (std::size_t i = 0; i < block_size; ++i, ++itr)
            {
               compute_pod_hash((*itr),hash);
            }
            length -= block_size;
         }
      }

      template<std::size_t block_size>
      inline void compute_block(char* itr, std::size_t& length, unsigned int& hash)
      {
         compute_block<block_size>(reinterpret_cast<unsigned char*>(itr),length,hash);
      }

      static const unsigned int hash_seed = 0xAAAAAAAA;

      template<typename Iterator>
      inline void hash(const Iterator itr, std::size_t length, unsigned int& hash_value)
      {
         if (length >= 64) compute_block<64>(itr,length,hash_value);
         if (length >= 32) compute_block<32>(itr,length,hash_value);
         if (length >= 16) compute_block<16>(itr,length,hash_value);
         if (length >=  8) compute_block< 8>(itr,length,hash_value);
         if (length >=  4) compute_block< 4>(itr,length,hash_value);
         if (length >=  2) compute_block< 2>(itr,length,hash_value);
         if (length ==  0) compute_block< 1>(itr,length,hash_value);
      }

   } // namespace details

   template<typename Iterator>
   inline unsigned int hash(const Iterator itr,
                            std::size_t length,
                            unsigned int seed = details::hash_seed)
   {
      unsigned int hash_value = seed;
      details::hash(itr,length,hash_value);
      return hash_value;
   }

   inline unsigned int hash(const std::string& s, unsigned int seed = details::hash_seed)
   {
      unsigned int hash_value = seed;
      return hash(s.begin(),s.size(),hash_value);
   }

   template<typename T,
            typename Allocator,
            template<typename,typename> class Sequence>
   inline unsigned int hash(const Sequence<T,Allocator>& sequence, unsigned int seed = details::hash_seed)
   {
      unsigned int hash_value = seed;
      return hash(sequence.begin(),sequence.size(),hash_value);
   }

   namespace util
   {
      template<typename T>
      class scoped_restore
      {
      public:

         scoped_restore(T& t, const bool restore = true)
         : restore_(restore),
           reference_(t),
           copy_(t)
         {}

        ~scoped_restore()
         {
            if (restore_)
            {
               reference_ = copy_;
            }
         }

         inline bool& restore()
         {
            return restore_;
         }

      private:

         bool restore_;
         T& reference_;
         T copy_;
      };

      template<typename T>
      class attribute
      {
      public:

         attribute()
         : initialised_(false)
         {}

         attribute(const T& t)
         {
            assign(t);
            prev_t_ = t;
         }

         inline attribute& operator=(const T& t)
         {
            prev_t_ = t_;
            assign(t);
         }

         inline T& operator()()
         {
            return t_;
         }

         inline const T& operator()() const
         {
            return t_;
         }

         inline operator T() const
         {
            return t_;
         }

         inline bool initialised() const
         {
            return initialised_;
         }

         inline bool changed() const
         {
            return (t_ != prev_t_);
         }

         inline const T& value() const
         {
            return t_;
         }

         inline T& value()
         {
            return t_;
         }

         inline const T& previous() const
         {
            return prev_t_;
         }

         inline T& previous()
         {
            return prev_t_;
         }

      private:

         inline void assign(const T& t)
         {
            t_ = t;
            initialised_ = true;
         }

         T t_;
         T prev_t_;
         bool initialised_;

      };

      template<typename Key,
               typename T,
               typename Comparator,
               typename MapAllocator,
               typename OutputIterator>
      inline void make_key_list(const std::map<Key,T,Comparator,MapAllocator>& map,
                                OutputIterator out)
      {
         if (map.empty()) return;
         typedef typename std::map<Key,T,Comparator,MapAllocator> map_type;
         typename map_type::const_iterator itr = map.begin();
         typename map_type::const_iterator end = map.end();
         while (end != itr)
         {
            *out++ = (itr++)->first;
         }
      }

      template<typename Key,
               typename T,
               typename Comparator,
               typename MapAllocator,
               typename SetAllocator>
      inline void make_key_list(const std::map<Key,T,Comparator,MapAllocator>& map,
                                std::set<Key,Comparator,SetAllocator>& set)
      {
         make_key_list(map,std::inserter(set,set.begin()));
      }

      template<typename Key,
               typename T,
               typename Comparator,
               typename MapAllocator,
               typename SequenceAllocator,
               template<typename,typename> class Sequence>
      inline void make_key_list(const std::map<Key,T,Comparator,MapAllocator>& map,
                                Sequence<Key,SequenceAllocator>& sequence)
      {
         make_key_list(map,std::back_inserter(sequence));
      }

      template<typename Key,
               typename T,
               typename Comparator,
               typename MapAllocator,
               typename OutputIterator>
      inline void make_value_list(const std::multimap<Key,T,Comparator,MapAllocator>& map,
                                  const Key& key,
                                  OutputIterator out)
      {
         if (map.empty()) return;
         typedef typename std::multimap<Key,T,Comparator,MapAllocator> map_type;
         typename map_type::const_iterator itr = map.find(key);
         typename map_type::const_iterator end = map.end();
         while ((end != itr) && (key == itr->first))
         {
            *out++ = (itr++)->second;
         }
      }

      template<typename Key,
               typename T,
               typename Comparator,
               typename MapAllocator,
               typename SequenceAllocator,
               template<typename,typename> class Sequence>
      inline void make_value_list(const std::multimap<Key,T,Comparator,MapAllocator>& map,
                           const Key& key,
                           Sequence<T,SequenceAllocator>& sequence)
      {
         make_value_list(map,key,std::back_inserter(sequence));
      }

      template<typename T,
               typename Allocator,
               template<typename,typename> class Sequence>
      inline void delete_all(Sequence<T*,Allocator>& sequence)
      {
         typename Sequence<T*,Allocator>::iterator itr = sequence.begin();
         typename Sequence<T*,Allocator>::iterator end = sequence.end();
         while (end != itr)
         {
            delete (*itr);
            ++itr;
         }
         sequence.clear();
      }

      template<typename Key,
               typename T,
               typename Comparator,
               typename Allocator>
      inline void delete_all(const std::map<Key,T*,Comparator,Allocator>& cont)
      {
         typename std::map<Key,T*,Comparator,Allocator>::iterator itr = cont.begin();
         typename std::map<Key,T*,Comparator,Allocator>::iterator end = cont.end();
         while (end != itr)
         {
            delete (*itr).second;
            ++itr;
         }
         cont.clear();
      }

      template<typename T,
               typename Comparator,
               typename Allocator>
      inline void delete_all(const std::set<T*,Comparator,Allocator>& cont)
      {
         typename std::set<T*,Comparator,Allocator>::iterator itr = cont.begin();
         typename std::set<T*,Comparator,Allocator>::iterator end = cont.end();
         while (end != itr)
         {
            delete (*itr);
            ++itr;
         }
         cont.clear();
      }

      template<typename Predicate,
               typename T,
               typename Allocator,
               template<typename,typename> class Sequence>
      inline void delete_if(const Predicate& p,
                           Sequence<T*,Allocator>& sequence)
      {
         typename Sequence<T*,Allocator>::iterator itr = sequence.begin();
         while (sequence.end() != itr)
         {
            if (p(*itr))
            {
               delete (*itr);
               itr = sequence.erase(itr);
            }
            else
               ++itr;
         }
      }

      template<typename Predicate,
               typename Key,
               typename T,
               typename Comparator,
               typename Allocator>
      inline void delete_if(const Predicate& p,
                            const std::map<Key,T*,Comparator,Allocator>& cont)
      {
         typename std::map<Key,T*,Comparator,Allocator>::iterator itr = cont.begin();
         while (cont.end() != itr)
         {
            if (p(*itr))
            {
               delete (*itr).second;
               itr = cont.erase(itr);
            }
            else
               ++itr;
         }
      }

      template<typename Predicate,
               typename T,
               typename Comparator,
               typename Allocator>
      inline void delete_if(const Predicate& p,
                            const std::set<T*,Comparator,Allocator>& cont)
      {
         typename std::set<T*,Comparator,Allocator>::iterator itr = cont.begin();
         while (cont.end() != itr)
         {
            if (p(*itr))
            {
               delete (*itr).second;
               itr = cont.erase(itr);
            }
            else
               ++itr;
         }
      }

   } // namespace util

} // namespace strtk

namespace
{

   static inline std::ostream& operator<<(std::ostream& os,
                                          const strtk::std_string::tokenizer<strtk::single_delimiter_predicate<char> >::type::iterator& range)
   {
      os << std::string((*range).first,(*range).second);
      return os;
   }

   static inline std::ostream& operator<<(std::ostream& os,
                                          const strtk::std_string::tokenizer<strtk::single_delimiter_predicate<unsigned char> >::type::iterator& range)
   {
      os << std::string((*range).first,(*range).second);
      return os;
   }

   static inline std::ostream& operator<<(std::ostream& os,
                                          const strtk::std_string::tokenizer<strtk::multiple_char_delimiter_predicate>::type::iterator& range)
   {
      os << std::string((*range).first,(*range).second);
      return os;
   }

   #define strtk_register_pair_to_ostream(Iterator)\
   static inline std::ostream& operator<<(std::ostream& os, const std::pair<Iterator,Iterator>& range)\
   { os << std::string(range.first,range.second); return os; }\
   static inline std::ostream& operator<<(std::ostream& os, std::pair<Iterator,Iterator>& range)\
   { os << std::string(range.first,range.second); return os; }\

   strtk_register_pair_to_ostream(char*)
   strtk_register_pair_to_ostream(unsigned char*)
   strtk_register_pair_to_ostream(const char*)
   strtk_register_pair_to_ostream(const unsigned char*)
   strtk_register_pair_to_ostream(std::string::iterator)
   strtk_register_pair_to_ostream(std::string::const_iterator)
   strtk_register_pair_to_ostream(const std::string::iterator)
   strtk_register_pair_to_ostream(const std::string::const_iterator)

} // namespace anonymous

#ifdef WIN32
   #ifndef NOMINMAX
      #define NOMINMAX
   #endif
   #ifndef WIN32_LEAN_AND_MEAN
      #define WIN32_LEAN_AND_MEAN
   #endif
   #include <windows.h>
#else
   #include <sys/time.h>
   #include <sys/types.h>
#endif
namespace strtk
{
   namespace util
   {
      class timer
      {
      public:

         #ifdef WIN32
            timer()
            : in_use_(false)
            {
               QueryPerformanceFrequency(&clock_frequency);
            }

            inline void start()
            {
               in_use_ = true;
               QueryPerformanceCounter(&start_time);
            }

            inline void stop()
            {
               QueryPerformanceCounter(&stop_time);
               in_use_ = false;
            }

            inline double time() const
            {
               return (1.0 *(stop_time.QuadPart - start_time.QuadPart)) / (1.0 * clock_frequency.QuadPart);
            }

         #else

            timer()
            : in_use_(false)
            {}

            inline void start()
            {
               in_use_ = true;
               gettimeofday(&start_time, 0);
            }

            inline void stop()
            {
               gettimeofday(&stop_time,  0);
               in_use_ = false;
            }

            inline double time() const
            {
               double diff = (stop_time.tv_sec - start_time.tv_sec) * 1000000.0;
               if (stop_time.tv_usec > start_time.tv_usec)
                  diff += (1.0 * (stop_time.tv_usec - start_time.tv_usec));
               else if (stop_time.tv_usec < start_time.tv_usec)
                  diff -= (1.0 * (start_time.tv_usec - stop_time.tv_usec));
               return (diff / 1000000.0);
            }
         #endif

            inline bool in_use() const
            {
               return in_use_;
            }

      private:

            bool in_use_;

         #ifdef WIN32
            LARGE_INTEGER start_time;
            LARGE_INTEGER stop_time;
            LARGE_INTEGER clock_frequency;
         #else
            struct timeval start_time;
            struct timeval stop_time;
            struct timeval clock_frequency;
         #endif
      };
   } // namespace util

   namespace information
   {
      static const char* library = "String Toolkit";
      static const char* version = "2.718281828459045235360287471352";
      static const char* date    = "20110101";

      static inline std::string data()
      {
         static const std::string info_str = std::string(library) +
                                             std::string(" v") + std::string(version) +
                                             std::string(" (") + date + std::string(")");
         return info_str;
      }

   } // namespace information

} // namespace strtk


#endif
