CXX = g++
CXXFLAGS = -std=c++20
WARNINGS := -Wall -Wextra


SOURCES = Reader.cpp Printer.cpp Environment.cpp Core.cpp
HEADERS = Reader.h Printer.h MalType.h Environment.h Core.h

step4_if_fn_do: step4_if_fn_do.cpp ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} ${WARNINGS} step4_if_fn_do.cpp ${SOURCES} -o step4_if_fn_do

step3_env: step3_env.cpp ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} ${WARNINGS} step3_env.cpp ${SOURCES} -o step3_env

step2_eval: step2_eval.cpp ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} ${WARNINGS} step2_eval.cpp ${SOURCES} -o step2_eval

step1_read_print: step1_read_print.cpp ${SOURCES} ${HEADERS}
	${CXX} ${CXXFLAGS} ${WARNINGS} step1_read_print.cpp ${SOURCES} -o step1_read_print
	
step0_repl: step0_repl.cpp
	${CXX} ${CXXFLAGS} ${WARNING} step0_repl.cpp -o step0_repl

clean:
	rm -f step0_repl step1_read_print step2_eval step3_env step4_if_fn_do