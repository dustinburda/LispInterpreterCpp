//
// Created by Dustin Burda on 2/2/24.
//

#ifndef MAL_MASTER_CORE_H
#define MAL_MASTER_CORE_H

#include <unordered_map>
#include <string>
#include <iostream>
#include "MalType.h"
#include "Printer.h"

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> plus = [](auto args) {
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


static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> minus = [](auto args) {
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

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> multiply = [](auto args) {
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

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> divide = [](auto args) {
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

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> prn = [](auto args) {
    size_t arg_length = args.size();
    for(size_t i = 0; i < arg_length; i++) {
        std::cout << pr_str(args[i]);

        if(i != arg_length - 1)
            std::cout << " ";
    }
    std::cout << "\n";
    return std::make_shared<MalNil>("nil");
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> pr_str_func = [](auto args) {
    size_t arg_length = args.size();

    std::string result;
    for(size_t i = 0; i < arg_length; i++) {
        auto string_ptr = dynamic_cast<MalString*>(args[i].get());

        std::string str;
        if(string_ptr)
            str = "\\\"" + (dynamic_cast<MalString*>(args[i].get())->string_) + "\\\"";
        else
            str = pr_str(args[i]);

        result += str;
        if(i != arg_length - 1) {
            result += " ";
        }
    }
    return std::make_shared<MalString>(result);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> str = [](auto args) {
    size_t arg_length = args.size();

    std::string result;
    for(size_t i = 0; i < arg_length; i++) {
        result += pr_str(args[i]);

        if(i != arg_length - 1)
            result += " ";
    }
    return std::make_shared<MalString>(result);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> list = [](auto args) {
    auto list_ptr = std::make_shared<MalList>();

    for(auto elem : args) {
        list_ptr->add(elem);
    }

    return list_ptr;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> is_list = [](auto args) {
    bool is_list_ = (dynamic_cast<MalList*>(args[0].get()) != nullptr);

    return std::make_shared<MalBool>(is_list_);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> is_empty = [](auto args) {
    auto list_ptr = dynamic_cast<MalList*>(args[0].get());
    auto vec_ptr = dynamic_cast<MalVec*>(args[0].get());

    bool is_empty;
    if(list_ptr)
        is_empty = list_ptr->mal_list_.empty();
    if(vec_ptr)
        is_empty = vec_ptr->mal_vec_.empty();

    return std::make_shared<MalBool>(is_empty);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> count = [](auto args) {
    auto list_ptr = dynamic_cast<MalList*>(args[0].get());

    if(!list_ptr) {
        return std::make_shared<MalNumber>(0);
    }

    size_t count = list_ptr->mal_list_.size();

    return std::make_shared<MalNumber>(count);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> eq = [](auto args) {
    if(args[0]->get_type() != args[1]->get_type()) {
        return std::make_shared<MalBool>(false);
    }

    // TODO: EQUALITY
    bool is_equal = (*(args[0]) == *(args[1]));
    return std::make_shared<MalBool>(is_equal);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> l = [](auto args) {
    auto num1_ptr = dynamic_cast<MalNumber*>(args[0].get());
    auto num2_ptr = dynamic_cast<MalNumber*>(args[1].get());

    return std::make_shared<MalBool>(num1_ptr->number_ < num2_ptr->number_);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> le = [](auto args) {
    auto num1_ptr = dynamic_cast<MalNumber*>(args[0].get());
    auto num2_ptr = dynamic_cast<MalNumber*>(args[1].get());

    return std::make_shared<MalBool>(num1_ptr->number_ <= num2_ptr->number_);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> g = [](auto args) {
    auto num1_ptr = dynamic_cast<MalNumber*>(args[0].get());
    auto num2_ptr = dynamic_cast<MalNumber*>(args[1].get());

    return std::make_shared<MalBool>(num1_ptr->number_ > num2_ptr->number_);
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> ge = [](auto args) {
    auto num1_ptr = dynamic_cast<MalNumber*>(args[0].get());
    auto num2_ptr = dynamic_cast<MalNumber*>(args[1].get());

    return std::make_shared<MalBool>(num1_ptr->number_ >= num2_ptr->number_);
};


class Core {
public:
    Core() {
        ns_["*"] =  std::make_shared<MalFunction>(multiply);
        ns_["/"]=  std::make_shared<MalFunction>(divide);
        ns_["+"] = std::make_shared<MalFunction>(plus);
        ns_["-"] = std::make_shared<MalFunction>(minus);
        ns_["def!"] = std::make_shared<MalSymbol>("def!");
        ns_["let*"] = std::make_shared<MalSymbol>("let*");
        ns_["list"] = std::make_shared<MalFunction>(list);
        ns_["list?"] = std::make_shared<MalFunction>(is_list);
        ns_["empty?"] = std::make_shared<MalFunction>(is_empty);
        ns_["count"] = std::make_shared<MalFunction>(count);
        ns_["="] = std::make_shared<MalFunction>(eq);
        ns_["<"] = std::make_shared<MalFunction>(l);
        ns_["<="] = std::make_shared<MalFunction>(le);
        ns_[">"] = std::make_shared<MalFunction>(g);
        ns_[">="] = std::make_shared<MalFunction>(ge);
        ns_["prn"] = std::make_shared<MalFunction>(prn);
        ns_["pr-str"] = std::make_shared<MalFunction>(pr_str_func);
        ns_["str"] = std::make_shared<MalFunction>(str);
    }

    const std::unordered_map<std::string, mal_t_ptr>& ns() const {
        return ns_;
    }

private:
    std::unordered_map<std::string, mal_t_ptr> ns_;
};


#endif //MAL_MASTER_CORE_H
