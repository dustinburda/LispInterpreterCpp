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


class Core {
public:
    Core() {
        ns_["*"] =  std::make_shared<MalFunction>(multiply);
        ns_["/"]=  std::make_shared<MalFunction>(divide);
        ns_["+"] = std::make_shared<MalFunction>(plus);
        ns_["-"] = std::make_shared<MalFunction>(minus);
        ns_["def!"] = std::make_shared<MalSymbol>("def!");
        ns_["let*"] = std::make_shared<MalSymbol>("let*");
    }

    const std::unordered_map<std::string, mal_t_ptr>& ns() const {
        return ns_;
    }

private:
    std::unordered_map<std::string, mal_t_ptr> ns_;
};


#endif //MAL_MASTER_CORE_H
