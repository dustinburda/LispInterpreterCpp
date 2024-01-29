#include <iostream>
#include <string>
#include "Reader.h"
#include "Printer.h"


mal_t_ptr READ(std::string str) {
    auto ast = read_str(str);
    return ast;
}

mal_t_ptr EVAL(mal_t_ptr ast, std::string env) {
    return ast;
}

std::string PRINT(mal_t_ptr eval) {
    auto printed_ast =  pr_str(eval);
    return printed_ast;
}

std::string REP(std::string str) {
    return PRINT(std::move(EVAL(std::move(READ(str)), "")));
}


int main() {
     // read_str("  (  + 2   (*  3  4)  )      ");

    std::string line;

    while(!std::cin.eof()) {
        std::cout << "user> ";

        std::getline(std::cin, line);

        std::cout << REP(line) << "\n";
    }
}