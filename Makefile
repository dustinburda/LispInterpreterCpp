CXX = g++
CXXFLAGS = -std=c++17
WARNINGS := -Wall -Wextra


SOURCES = Reader.cpp Printer.cpp
HEADERS = Reader.h Printer.h MalType.h

step2_eval: step2_eval.cpp ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} ${WARNINGS} step2_eval.cpp ${SOURCES} -o step2_eval

step1_read_print: step1_read_print.cpp ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} ${WARNINGS} step1_read_print.cpp ${SOURCES} -o step1_read_print
	
step0_repl: step0_repl.cpp
	${CXX} ${CXXFLAGS} ${WARNING} step0_repl.cpp -o step0_repl

clean:
	rm -f step0_repl step1_read_print step2_eval