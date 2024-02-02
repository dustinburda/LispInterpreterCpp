#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include "Reader.h"
#include "Printer.h"
#include "MalType.h"
#include "Environment.h"

Env repl_env { nullptr };

mal_t_ptr READ(std::string str);
mal_t_ptr EVAL(mal_t_ptr ast, Env& env);
std::string PRINT(mal_t_ptr eval);
std::string REP(std::string str);


mal_t_ptr eval_ast(mal_t_ptr ast, Env& env) {
    mal_t_ptr s;
    switch (ast->get_type()) {
        case Mal_T::Symbol:
        {
            auto symbol_ptr = dynamic_cast<MalSymbol*>(ast.get());
            std::string symbol = symbol_ptr->symbol_;

            s = env.get(symbol);
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

mal_t_ptr EVAL(mal_t_ptr ast, Env& env) {
    if(ast->get_type() != Mal_T::List) {
        return eval_ast(ast, env);
    } else if (auto list_ptr = dynamic_cast<MalList*>(ast.get()); list_ptr->mal_list_.size() == 0) {
        return ast;
    } else {
        // printf("Evaluating a list.... \n");

        // TODO: create a separate list_ptr here
        if(list_ptr == nullptr) {
            throw std::logic_error("list_ptr must point to a list!");
        }

        auto symbol = dynamic_cast<MalSymbol*>(list_ptr->mal_list_[0].get());
        if(symbol != nullptr) {
            // printf("First element is a symbol! \n");
            if(symbol->symbol_ == "def!") {

                auto key = dynamic_cast<MalSymbol*>(list_ptr->mal_list_[1].get())->symbol_;
                auto val = EVAL(list_ptr->mal_list_[2], env);
                return env.set(key, val);
            } else if (symbol->symbol_ == "let*") {
                auto let_env = Env { &env };

                auto binding_list = dynamic_cast<MalList*>(list_ptr->mal_list_[1].get());
                if(binding_list == nullptr || (binding_list->mal_list_.size() % 2 != 0))
                    throw std::logic_error("second parameter of let* statement must be a list of even size!");

                for(int i = 0; i < binding_list->mal_list_.size(); i+= 2) {
                    auto first = dynamic_cast<MalSymbol*>(binding_list->mal_list_[i].get());
                    mal_t_ptr second = EVAL(binding_list->mal_list_[i+1], let_env);

                    let_env.set(first->symbol_, second);
                }

                return EVAL(list_ptr->mal_list_[2], let_env);
            }
        }

        mal_t_ptr evaluated_list = eval_ast(ast, env);
        auto evaluated_list_ptr = dynamic_cast<MalList*>(evaluated_list.get());
        // Get first element as function
        auto fn = dynamic_cast<MalFunction*>(evaluated_list_ptr->mal_list_[0].get());
        if(fn == nullptr)
            return evaluated_list; // TODO return this by default
        //Apply first element to remainder of list
        // printf("HELLO! \n");
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
    // TODO, remove std::move here to allow copy elision
    return PRINT(std::move(EVAL(std::move(READ(str)), repl_env)));
}


int main() {
    repl_env.set("*", std::make_shared<MalFunction>(std::multiplies<int>()));
    repl_env.set("/", std::make_shared<MalFunction>(std::divides<int>()));
    repl_env.set("+", std::make_shared<MalFunction>(std::plus<int>()));
    repl_env.set("-", std::make_shared<MalFunction>(std::minus<int>()));
    repl_env.set("def!", std::make_shared<MalSymbol>("def!"));
    repl_env.set("let*", std::make_shared<MalSymbol>("let*"));

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