#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include "Reader.h"
#include "Printer.h"
#include "MalType.h"

using Environment = std::unordered_map<std::string, MalFunction>;
Environment repl_env = { {"*", MalFunction(std::multiplies<int>())} ,
                         {"/", MalFunction(std::divides<int>())},
                         {"+", MalFunction(std::plus<int>())},
                         {"-", MalFunction(std::minus<int>())}  };

mal_t_ptr eval_ast(mal_t_ptr ast, Environment& env) {
    if(auto symbol_ptr = dynamic_cast<MalSymbol*>(ast.get()); symbol_ptr != nullptr) {

    }
}

mal_t_ptr READ(std::string str) {
    auto ast = read_str(str);
    return ast;
}

mal_t_ptr EVAL(mal_t_ptr ast, Environment& env) {
    return ast;
}

std::string PRINT(mal_t_ptr eval) {
    auto printed_ast =  pr_str(eval);
    return printed_ast;
}

std::string REP(std::string str) {
    return PRINT(std::move(EVAL(std::move(READ(str)), repl_env)));
}


int main() {
     // read_str("  (  + 2   (*  3  4)  )      ");

    std::string line;

    while(!std::cin.eof()) {
        std::cout << "\nuser> ";

        std::getline(std::cin, line);

        std::string evaluated_line;
        try{
            evaluated_line = REP(line);
        } catch (...) {
            evaluated_line = "unbalanced";
        }

        std::cout << evaluated_line;
    }
}