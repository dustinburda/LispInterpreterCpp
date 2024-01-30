#ifndef MALTYPE_H_
#define MALTYPE_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>


struct MalType;
using mal_t_ptr = std::shared_ptr<MalType>;

enum class Mal_T {
    List,
    Vec,
    Map,
    Number,
    Symbol,
    String,
    Bool,
    Nil,
    Function
};

struct MalType {
    MalType() {};
    virtual ~MalType() {};

};

struct MalList : MalType {
    MalList() = default;

    void add(mal_t_ptr& mal_t) {
        mal_list_.push_back(mal_t);
    }

    Mal_T type = Mal_T::List;
    std::vector<mal_t_ptr> mal_list_;
};


struct MalVec : MalType {
    MalVec() = default;

    void add(mal_t_ptr& mal_t) {
        mal_vec_.push_back(mal_t);
    }

     Mal_T type = Mal_T::Vec;
    std::vector<mal_t_ptr> mal_vec_;
};
//======================================================
struct MalMap : MalType {
    MalMap() = default;

    void add(mal_t_ptr k, mal_t_ptr v) {
        malmap_[k] = v;
    }

     Mal_T type = Mal_T::Map;
    std::unordered_map<mal_t_ptr, mal_t_ptr> malmap_;
};
//=====================================================


struct MalNumber : MalType {
    explicit MalNumber(int number) : number_{ number } {}

    Mal_T type = Mal_T::Number;
    int number_;
};

struct MalSymbol : MalType {
    explicit MalSymbol(std::string symbol) : symbol_ { std::move(symbol) } {}

    Mal_T type = Mal_T::Symbol;
    std::string symbol_;
};


struct MalString : MalType {
    explicit MalString(std::string s) : string_ { std::move(s) } {}

    Mal_T type = Mal_T::String;
    std::string string_;
};

struct MalBool : MalType {
    explicit MalBool(std::string token) : bool_ {token == "true"} {} 

    Mal_T type = Mal_T::Bool;
    bool bool_;
};

struct MalNil : MalType {
    explicit MalNil(std::string nil) : nil_{nil} {}
    
    Mal_T type = Mal_T::Nil;
    std::string nil_;
};

// TODO: may have to apply a template later
struct MalFunction : MalType {
    MalFunction(std::function<int(int, int)> f) : fn { f } {}

    int apply_fn(std::vector<mal_t_ptr> args) {
        int result = 0;

        for(int index = 0; auto mal_num_ptr : args) {
            auto num_ptr = dynamic_cast<MalNumber*>(mal_num_ptr.get());
            
            if(num_ptr = nullptr)
                throw std::logic_error("Must pass in a number!");

            if(index == 0) {
                result = num_ptr->number_; 
            } else {
                result = fn(result, num_ptr->number_);
            }
            index++;
        }
    }

    Mal_T type = Mal_T::Function;
    std::function<int(int, int)> fn;
};


#endif // MALTYPE_H_