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

(2)  If  the Boost  libraries  (random, regex,  lexical_cast)  are not
available  the  following  defines   should  be  commented  out   from
strtk.hpp:
   (*) strtk_enable_lexical_cast (strtk.hpp:44)
   (*) strtk_enable_random (strtk.hpp:45)
   (*) strtk_enable_regex (strtk.hpp:46)

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
(*) GNU Compiler Collection (4.3+)
(*) Intel® C++ Compiler (9.x+)
(*) Microsoft Visual Studio C++ Compiler (8.1+)
(*) Comeau C++ Compiler (4.3+)


[FILES]
(00) Makefile
(01) readme.txt
(02) strtk.hpp
(03) strtk_combinations.cpp
(04) strtk_combinator_example.cpp
(05) strtk_converters_example.cpp
(06) strtk_examples.cpp
(07) strtk_glober.cpp
(08) strtk_hexview.cpp
(09) strtk_ipv4_parser.cpp
(00) strtk_keyvalue_example.cpp
(11) strtk_numstats.cpp
(12) strtk_parse_test.cpp
(13) strtk_period_parser.cpp
(14) strtk_random_line.cpp
(15) strtk_randomizer.cpp
(16) strtk_search_trie_example.cpp
(17) strtk_serializer_example.cpp
(18) strtk_text_parser_example01.cpp
(19) strtk_text_parser_example02.cpp
(10) strtk_tokengrid_example.cpp
(21) strtk_tokenizer_cmp.cpp
(22) strtk_tokenizer_test.cpp
(23) strtk_wordfreq.cpp
