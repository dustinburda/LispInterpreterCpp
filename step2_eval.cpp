#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include "Reader.h"
#include "Printer.h"
#include "MalType.h"

using Environment = std::unordered_map<std::string, mal_t_ptr>;

std::function<mal_t_ptr(std::vector<mal_t_ptr>)> plus = [](auto args) {
    int result = dynamic_cast<MalNumber*>(args[0].get())->number_;

    int index = 0;
    for(auto arg : args) {
        if(index == 0) {
            index++;
            continue;
        }
        result += dynamic_cast<MalNumber*>(arg.get())->number_;
        index++;
    }

    return std::make_shared<MalNumber>(result);
};


std::function<mal_t_ptr(std::vector<mal_t_ptr>)> minus = [](auto args) {
    int result = dynamic_cast<MalNumber*>(args[0].get())->number_;

    int index = 0;
    for(auto arg : args) {
        if(index == 0) {
            index++;
            continue;
        }
        result -= dynamic_cast<MalNumber*>(arg.get())->number_;
        index++;
    }

    return std::make_shared<MalNumber>(result);
};

std::function<mal_t_ptr(std::vector<mal_t_ptr>)> multiply = [](auto args) {
    int result = dynamic_cast<MalNumber*>(args[0].get())->number_;

    int index = 0;
    for(auto arg : args) {
        if(index == 0) {
            index++;
            continue;
        }
        result *= dynamic_cast<MalNumber*>(arg.get())->number_;
        index++;
    }

    return std::make_shared<MalNumber>(result);
};

std::function<mal_t_ptr(std::vector<mal_t_ptr>)> divide = [](auto args) {
    int result = dynamic_cast<MalNumber*>(args[0].get())->number_;

    int index = 0;
    for(auto arg : args) {
        if(index == 0) {
            index++;
            continue;
        }
        result /= dynamic_cast<MalNumber*>(arg.get())->number_;
        index++;
    }

    return std::make_shared<MalNumber>(result);
};


Environment repl_env = { {"*", std::make_shared<MalFunction>(multiply)} ,
                         {"/", std::make_shared<MalFunction>(divide)},
                         {"+", std::make_shared<MalFunction>(plus)},
                         {"-", std::make_shared<MalFunction>(minus)}  };

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

mal_t_ptr EVAL(mal_t_ptr ast, Environment& env) {
    if(ast->get_type() != Mal_T::List) {
        return eval_ast(ast, env);
    } else if (auto list_ptr = dynamic_cast<MalList*>(ast.get()); list_ptr && list_ptr->mal_list_.size() == 0) {
        return ast;
    } else {
        mal_t_ptr evaluated_list = eval_ast(ast, env);
        auto evaluated_list_ptr = dynamic_cast<MalList*>(evaluated_list.get());

        auto fn = dynamic_cast<MalFunction*>(evaluated_list_ptr->mal_list_[0].get());
        if(fn != nullptr) {
            auto remaining_args = std::vector<mal_t_ptr>(evaluated_list_ptr->mal_list_.begin() + 1,
                                                         evaluated_list_ptr->mal_list_.end());

            auto num_ptr = fn->apply_fn(remaining_args);
            return num_ptr;
        }

        return evaluated_list;
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