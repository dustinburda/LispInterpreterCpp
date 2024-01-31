#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include "Reader.h"
#include "Printer.h"
#include "MalType.h"

using Environment = std::unordered_map<std::string, mal_t_ptr>;

Environment repl_env = { {"*", std::make_shared<MalFunction>(std::multiplies<int>())} ,
                         {"/", std::make_shared<MalFunction>(std::divides<int>())},
                         {"+", std::make_shared<MalFunction>(std::plus<int>())},
                         {"-", std::make_shared<MalFunction>(std::minus<int>())}  };

mal_t_ptr READ(std::string str);
mal_t_ptr EVAL(mal_t_ptr ast, Environment& env);
std::string PRINT(mal_t_ptr eval);
std::string REP(std::string str);


mal_t_ptr eval_ast(mal_t_ptr ast, Environment& env) {
    mal_t_ptr s;
    switch (ast->get_type()) {
        case Mal_T::Symbol:
        {
            auto symbol_ptr = dynamic_cast<MalSymbol*>(ast.get());
            std::string symbol = symbol_ptr->symbol_;

            if(env.count(symbol) == 0) {
                throw std::logic_error("Symbol must have an associated function!");
            }

            s = env[symbol];
            break;
        }
        case Mal_T::List:
        {
            auto list_ptr = dynamic_cast<MalList*>(ast.get());

            auto new_list = std::make_shared<MalList>();
            for(auto& elem : list_ptr->mal_list_) {
                mal_t_ptr evaluated_list_elem = EVAL(elem, env);
                new_list->add(evaluated_list_elem);
            }

            s = new_list;
            break;
        } 
        default: 
        {
            s = ast;
        }
    }
    return s;
}

mal_t_ptr READ(std::string str) {
    auto ast = read_str(str);
    return ast;
}

mal_t_ptr EVAL(mal_t_ptr ast, Environment& env) {
    if(ast->get_type() != Mal_T::List) {
        return eval_ast(ast, env);
    } else if (auto list_ptr = dynamic_cast<MalList*>(ast.get()); list_ptr->mal_list_.size() == 0) {
        return ast;
    } else {
        mal_t_ptr evaluated_list = eval_ast(ast, env);
        auto evaluated_list_ptr = dynamic_cast<MalList*>(evaluated_list.get());

        if(list_ptr == nullptr) {
            throw std::logic_error("list_ptr must point to a list!");
        }

        auto fn = dynamic_cast<MalFunction*>(evaluated_list_ptr->mal_list_[0].get());
        
        if(fn == nullptr)
            return evaluated_list;
        
        auto remaining_args = std::vector<mal_t_ptr>(evaluated_list_ptr->mal_list_.begin() + 1, evaluated_list_ptr->mal_list_.end());

        int result = fn->apply_fn(remaining_args);

        auto num_ptr = std::make_shared<MalNumber>(result);
        return num_ptr;
    }
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
            evaluated_line = "Error!";
        }

        std::cout << evaluated_line;
    }
}