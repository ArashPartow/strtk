C++ String Toolkit Library

[INTRODUCTION]
The C++ String Toolkit  Library (StrTk) consists of  robust, optimized
and portable string processing algorithms for the C++ language.  StrTk
is  designed to  be easy  to use  and integrate  within existing  code
bases. Furthermore the library has  a rich set of features  that makes
light work of any kind of string processing task.



[COPYRIGHT NOTICE]
Free  use  of  the  String  Toolkit  Library  is  permitted  under the
guidelines and  in accordance  with the  most current  version of  the
Common Public License.
http://www.opensource.org/licenses/cpl1.0.php



[DOWNLOADS & UPDATES]
All updates  and the most recent version of the C++ String Toolkit can
be found at:
(1) http://www.partow.net/programming/strtk/index.html
(2) http://strtk.partow.net

Code repository:
(1) http://code.google.com/p/strtk/
(2) svn checkout http://strtk.googlecode.com/svn/trunk/ strtk



[INSTALLATION]
(1) strtk.hpp  should be  placed in  a project  or system include path
(e.g: /usr/include/).

(2) If the Boost libraries  (random, regex,  lexical_cast etc) are not
available  or  it  not  wished   they  be  used  then  the   following
preprocessor directive needs defining, either in code before strtk.hpp
is included or as a compiler switch:
   (*) strtk_no_tr1_or_boost

(3) It is advisable to have either Boost installed or a TR1  compliant
C++ standard library. Installation of Boost on:
(*) Win32  : http://www.boostpro.com/download
(*) Ubuntu : boost packages via apt-get or synaptic
(*) Fedora : yum -y install boost-devel
(*) http://www.boost.org/doc/libs/release/more/getting_started/index.html



[COMPILATION]
(1) For a complete build: make clean all
(2) For a PGO build: make clean pgo
(3) To strip executables: make strip_bin



[COMPILER COMPATIBILITY]
(*) GNU Compiler Collection (4.1+)
(*) Intel C++ Compiler (9.x+)
(*) Clang/LLVM (1.1+)
(*) PGI C++ (10.x+)
(*) Microsoft Visual Studio C++ Compiler (8.1+)
(*) Comeau C++ Compiler (4.3+)
(*) IBM XL C/C++ (10.x+)
(*) C++ Builder (XE4+)



[FILES]
(00) Makefile
(01) readme.txt
(02) strtk.hpp
(03) strtk_bloom_filter_example.cpp
(04) strtk_combinations.cpp
(05) strtk_combinator_example.cpp
(06) strtk_converters_example.cpp
(07) strtk_examples.cpp
(08) strtk_glober.cpp
(09) strtk_hexview.cpp
(10) strtk_ipv4_parser.cpp
(11) strtk_keyvalue_example.cpp
(12) strtk_nth_combination_example.cpp
(13) strtk_numstats.cpp
(14) strtk_parse_test.cpp
(15) strtk_period_parser.cpp
(16) strtk_random_line.cpp
(17) strtk_randomizer.cpp
(18) strtk_search_trie_example.cpp
(19) strtk_serializer_example.cpp
(20) strtk_text_parser_example01.cpp
(21) strtk_text_parser_example02.cpp
(22) strtk_tokengrid_example.cpp
(23) strtk_tokenizer_cmp.cpp
(24) strtk_tokenizer_test.cpp
(25) strtk_wordfreq.cpp
