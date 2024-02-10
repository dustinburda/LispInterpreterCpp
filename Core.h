//
// Created by Dustin Burda on 2/2/24.
//

#ifndef MAL_MASTER_CORE_H
#define MAL_MASTER_CORE_H

#include <unordered_map>
#include <string>
#include "MalType.h"

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

//TODO implement this
static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> list = [](auto args) {
    mal_t_ptr s;
    return s;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> is_list = [](auto args) {
    mal_t_ptr s;
    return s;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> is_empty = [](auto args) {
    mal_t_ptr s;
    return s;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> count = [](auto args) {
    mal_t_ptr s;
    return s;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> eq = [](auto args) {
    mal_t_ptr s;
    return s;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> l = [](auto args) {
    mal_t_ptr s;
    return s;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> le = [](auto args) {
    mal_t_ptr s;
    return s;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> g = [](auto args) {
    mal_t_ptr s;
    return s;
};

static std::function<mal_t_ptr(std::vector<mal_t_ptr>)> ge = [](auto args) {
    mal_t_ptr s;
    return s;
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

    }

    const std::unordered_map<std::string, mal_t_ptr>& ns() const {
        return ns_;
    }

private:
    std::unordered_map<std::string, mal_t_ptr> ns_;
};


#endif //MAL_MASTER_CORE_H
