.PHONY = all test clean

SRCS := $(wildcard */*.cpp)
BINS := $(SRCS:$.cpp=$.o)

TEST_SRCS := $(wildcard */tests/*.cpp) 
TESTS := $(TEST_SRCS:%.cpp=%)

all: ${BINS}

%.o:%.cpp
	g++ -o $@ -c $<

test: ${TESTS} syntax_analysis.lib lexical_analysis.lib
	for TEST in ${TESTS}; do  \
		./$$TEST ;			  \
	done

lexical_analysis.lib: $(wildcard lexical_analysis/*.o)
	ar rvs $@ $^
		
syntax_analysis.lib: $(wildcard syntax_analysis/*.o)
	ar rvs $@ $^

syntax_analysis/%_test : syntax_analysis/%_test.cpp  syntax_analysis.lib
	g++ -o $@  $< -l boost_unit_test_framework syntax_analysis.lib

lexical_analysis/%_test : lexical_analysis/%_test.cpp  lexical_analysis.lib
	g++ -o $@  $< -l boost_unit_test_framework lexical_analysis.lib

clean:
	rm -f *.o *.lib 
	rm -f syntax_analysis/*.o lexical_analysis/*.o
	rm -f syntax_analysis/*_test
