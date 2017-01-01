#
# *****************************************************************
# *                     String Toolkit Library                    *
# *                                                               *
# * Author: Arash Partow (2002-2014)                              *
# * URL: http://www.partow.net/programming/strtk/index.html       *
# *                                                               *
# * Copyright notice:                                             *
# * Free use of the String Toolkit Library is permitted under the *
# * guidelines and in accordance with the most current version of *
# * the MIT License.                                              *
# * http://www.opensource.org/licenses/MIT                        *
# *                                                               *
# *****************************************************************
#


COMPILER         = -c++
#COMPILER        = -clang
OPTIMIZATION_OPT = -O1
NO_EXTRA_LIBS    = -Dstrtk_no_tr1_or_boost
BASE_OPTIONS     = -ansi -pedantic-errors -Wall -Wextra -Werror -Wno-long-long
OPTIONS          = $(BASE_OPTIONS) $(OPTIMIZATION_OPT) -o
REGEX            = -lboost_regex
PTHREAD          = -lpthread
LINKER_OPT       = -lstdc++ -lm

BUILD_LIST+=strtk_examples
BUILD_LIST+=strtk_tokenizer_test
BUILD_LIST+=strtk_bloom_filter_example
BUILD_LIST+=strtk_combinations
BUILD_LIST+=strtk_combinator_example
BUILD_LIST+=strtk_converters_example
BUILD_LIST+=strtk_glober
BUILD_LIST+=strtk_hexview
BUILD_LIST+=strtk_ipv4_parser
BUILD_LIST+=strtk_keyvalue_example
BUILD_LIST+=strtk_nth_combination_example
BUILD_LIST+=strtk_numstats
BUILD_LIST+=strtk_parse_test
BUILD_LIST+=strtk_period_parser
BUILD_LIST+=strtk_randomizer
BUILD_LIST+=strtk_random_line
BUILD_LIST+=strtk_search_trie_example
BUILD_LIST+=strtk_serializer_example
BUILD_LIST+=strtk_text_parser_example01
BUILD_LIST+=strtk_text_parser_example02
BUILD_LIST+=strtk_tokengrid_example
BUILD_LIST+=strtk_tokenizer_cmp
BUILD_LIST+=strtk_wordfreq

all: $(BUILD_LIST)

strtk_tokenizer_cmp: strtk_tokenizer_cmp.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_tokenizer_cmp strtk_tokenizer_cmp.cpp $(LINKER_OPT)

strtk_examples: strtk_examples.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_examples strtk_examples.cpp  $(LINKER_OPT) $(REGEX)

strtk_keyvalue_example: strtk_keyvalue_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_keyvalue_example strtk_keyvalue_example.cpp $(LINKER_OPT)

strtk_text_parser_example01: strtk_text_parser_example01.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_text_parser_example01 strtk_text_parser_example01.cpp $(LINKER_OPT)

strtk_text_parser_example02: strtk_text_parser_example02.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_text_parser_example02 strtk_text_parser_example02.cpp $(LINKER_OPT)

strtk_tokenizer_test: strtk_tokenizer_test.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_tokenizer_test strtk_tokenizer_test.cpp $(LINKER_OPT)

strtk_parse_test: strtk_parse_test.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_parse_test strtk_parse_test.cpp $(LINKER_OPT)

strtk_period_parser: strtk_period_parser.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_period_parser strtk_period_parser.cpp $(LINKER_OPT)

strtk_ipv4_parser: strtk_ipv4_parser.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_ipv4_parser strtk_ipv4_parser.cpp $(LINKER_OPT)

strtk_hexview: strtk_hexview.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_hexview strtk_hexview.cpp $(LINKER_OPT)

strtk_bloom_filter_example: strtk_bloom_filter_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_bloom_filter_example strtk_bloom_filter_example.cpp $(LINKER_OPT)

strtk_converters_example: strtk_converters_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_converters_example strtk_converters_example.cpp $(LINKER_OPT)

strtk_combinations: strtk_combinations.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_combinations strtk_combinations.cpp $(LINKER_OPT)

strtk_combinator_example: strtk_combinator_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_combinator_example strtk_combinator_example.cpp $(LINKER_OPT)

strtk_nth_combination_example: strtk_nth_combination_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_nth_combination_example strtk_nth_combination_example.cpp $(LINKER_OPT)

strtk_glober: strtk_glober.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_glober strtk_glober.cpp $(LINKER_OPT)

strtk_tokengrid_example: strtk_tokengrid_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_tokengrid_example strtk_tokengrid_example.cpp $(LINKER_OPT)

strtk_search_trie_example: strtk_search_trie_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_search_trie_example strtk_search_trie_example.cpp $(LINKER_OPT)

strtk_serializer_example: strtk_serializer_example.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_serializer_example strtk_serializer_example.cpp $(LINKER_OPT)

strtk_randomizer: strtk_randomizer.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_randomizer strtk_randomizer.cpp $(LINKER_OPT)

strtk_random_line: strtk_random_line.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_random_line strtk_random_line.cpp $(LINKER_OPT)

strtk_numstats: strtk_numstats.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_numstats strtk_numstats.cpp $(LINKER_OPT)

strtk_wordfreq: strtk_wordfreq.cpp strtk.hpp
	$(COMPILER) $(OPTIONS) strtk_wordfreq strtk_wordfreq.cpp $(LINKER_OPT)

pgo: strtk_parse_test.cpp strtk_tokenizer_cmp.cpp strtk.hpp
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-generate -DUSE_SPIRIT -o strtk_tokenizer_cmp strtk_tokenizer_cmp.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-generate -o strtk_parse_test strtk_parse_test.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-generate -o strtk_serializer_example strtk_serializer_example.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-generate -o strtk_search_trie_example strtk_search_trie_example.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-generate -o strtk_converters_example strtk_converters_example.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-generate -o strtk_bloom_filter_example strtk_bloom_filter_example.cpp $(LINKER_OPT)
	./strtk_tokenizer_cmp
	./strtk_parse_test
	./strtk_serializer_example
	./strtk_search_trie_example
	./strtk_converters_example
	./strtk_bloom_filter_example
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-use -DUSE_SPIRIT -o strtk_tokenizer_cmp strtk_tokenizer_cmp.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-use -o strtk_parse_test strtk_parse_test.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-use -o strtk_serializer_example strtk_serializer_example.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-use -o strtk_search_trie_example strtk_search_trie_example.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-use -o strtk_converters_example strtk_converters_example.cpp $(LINKER_OPT)
	$(COMPILER) $(BASE_OPTIONS) -O3 -march=native -fprofile-use -o strtk_bloom_filter_example strtk_bloom_filter_example.cpp $(LINKER_OPT)

strip_bin:
	strip -s strtk_bloom_filter_example
	strip -s strtk_converters_example
	strip -s strtk_combinations
	strip -s strtk_combinator_example
	strip -s strtk_glober
	strip -s strtk_examples
	strip -s strtk_hexview
	strip -s strtk_ipv4_parser
	strip -s strtk_keyvalue_example
	strip -s strtk_nth_combination_example
	strip -s strtk_numstats
	strip -s strtk_parse_test
	strip -s strtk_period_parser
	strip -s strtk_randomizer
	strip -s strtk_random_line
	strip -s strtk_search_trie_example
	strip -s strtk_serializer_example
	strip -s strtk_text_parser_example01
	strip -s strtk_text_parser_example02
	strip -s strtk_tokengrid_example
	strip -s strtk_tokenizer_cmp
	strip -s strtk_tokenizer_test
	strip -s strtk_wordfreq

valgrind_check:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_bloom_filter_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_converters_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_combinations
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_combinator_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_glober
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_examples
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_hexview
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_ipv4_parser
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_keyvalue_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_nth_combination_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_numstats
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_parse_test
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_period_parser
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_randomizer
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_random_line
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_search_trie_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_serializer_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_text_parser_example01
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_text_parser_example02
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_tokengrid_example
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_tokenizer_cmp
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_tokenizer_test
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ./strtk_wordfreq

clean:
	rm -f core.* *~ *.o *.bak *stackdump gmon.out *.gcda *.gcno *.gcnor *.gch
