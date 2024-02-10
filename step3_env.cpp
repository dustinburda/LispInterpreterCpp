#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include "Reader.h"
#include "Printer.h"
#include "MalType.h"
#include "Environment.h"
#include "Core.h"

Env repl_env { nullptr, {}, {} };

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
        case Mal_T::Vec:
        {
            auto vec_ptr = dynamic_cast<MalVec*>(ast.get());

            auto new_vec = std::make_shared<MalVec>();
            for(auto& elem :vec_ptr->mal_vec_) {
                mal_t_ptr evaluated_list_elem = EVAL(elem, env);
                new_vec->add(evaluated_list_elem);
            }

            s = new_vec;
            break;
        }
        case Mal_T::Map:
        {
            auto map_ptr = dynamic_cast<MalMap*>(ast.get());

            auto new_map = std::make_shared<MalMap>();
            for(auto [k, v] : map_ptr->malmap_) {
                auto evaluated_val = EVAL(v, env);
                new_map->add(k, evaluated_val);
            }

            s = new_map;
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
        auto symbol = dynamic_cast<MalSymbol*>(list_ptr->mal_list_[0].get());
        if(symbol != nullptr) {
            // printf("First element is a symbol! \n");
            if(symbol->symbol_ == "def!") {
                // DEF SYMBOL
                auto key = dynamic_cast<MalSymbol*>(list_ptr->mal_list_[1].get())->symbol_;
                auto val = EVAL(list_ptr->mal_list_[2], env);
                return env.set(key, val);
            } else if (symbol->symbol_ == "let*") {
                // LET SYMBOL
                auto let_env = Env { &env, {}, {} };

                // LET SYMBOL - LIST SUPPORT
                auto binding_list = dynamic_cast<MalList*>(list_ptr->mal_list_[1].get());
                if(binding_list != nullptr) {
                    if(binding_list->mal_list_.size() % 2 != 0)
                        throw std::logic_error("second parameter of let* statement must be a list of even size!");

                    for(int i = 0; i < binding_list->mal_list_.size(); i+= 2) {
                        auto first = dynamic_cast<MalSymbol*>(binding_list->mal_list_[i].get());
                        mal_t_ptr second = EVAL(binding_list->mal_list_[i+1], let_env);

                        let_env.set(first->symbol_, second);
                    }

                    return EVAL(list_ptr->mal_list_[2], let_env);
                }

                
                // LET SYMBOL - VEC SUPPORT
                auto binding_vec = dynamic_cast<MalVec*>(list_ptr->mal_list_[1].get());
                if(binding_vec != nullptr) {
                    if(binding_vec->mal_vec_.size() % 2 != 0)
                        throw std::logic_error("second parameter of let* statement must be a list of even size!");

                    for(int i = 0; i < binding_vec->mal_vec_.size(); i+= 2) {
                        auto first = dynamic_cast<MalSymbol*>(binding_vec->mal_vec_[i].get());
                        mal_t_ptr second = EVAL(binding_vec->mal_vec_[i+1], let_env);

                        let_env.set(first->symbol_, second);
                    }

                    return EVAL(list_ptr->mal_list_[2], let_env);
                }



            }
        }

        mal_t_ptr evaluated_list = eval_ast(ast, env);
        auto evaluated_list_ptr = dynamic_cast<MalList*>(evaluated_list.get());
        // Get first element as function
        auto fn = dynamic_cast<MalFunction*>(evaluated_list_ptr->mal_list_[0].get());
        if(fn != nullptr) {

            auto remaining_args = std::vector<mal_t_ptr>(evaluated_list_ptr->mal_list_.begin() + 1,
                                                         evaluated_list_ptr->mal_list_.end());

            auto num_ptr = fn->apply_fn(remaining_args);
            return num_ptr;
        }

        return evaluated_list; // TODO return this by default
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
    repl_env.set("*", std::make_shared<MalFunction>(multiply));
    repl_env.set("/", std::make_shared<MalFunction>(divide));
    repl_env.set("+", std::make_shared<MalFunction>(plus));
    repl_env.set("-", std::make_shared<MalFunction>(minus));
    repl_env.set("def!", std::make_shared<MalSymbol>("def!"));
    repl_env.set("let*", std::make_shared<MalSymbol>("let*"));

    std::string line;

    while(!std::cin.eof()) {
        std::cout << "\nuser> ";

        std::getline(std::cin, line);

        std::string evaluated_line;
        try{
            evaluated_line = REP(line);
        } catch(SymbolNotFoundException& e) {
            evaluated_line = e.what();
        } catch (...) {
            evaluated_line = "abc not found"; // TODO: make custom exception
        }

        std::cout << evaluated_line;
    }
}