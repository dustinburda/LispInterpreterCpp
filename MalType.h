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
    MalType(Mal_T mal_t) : type { mal_t }  {};
    virtual ~MalType() {};

    Mal_T get_type() { return type; }
    Mal_T type;
};

struct MalList : MalType {
    MalList() : MalType { Mal_T::List } {}

    void add(mal_t_ptr& mal_t) {
        mal_list_.push_back(mal_t);
    }

    std::vector<mal_t_ptr> mal_list_;
};


struct MalVec : MalType {
    MalVec() : MalType { Mal_T::Vec } {}

    void add(mal_t_ptr& mal_t) {
        mal_vec_.push_back(mal_t);
    }
    
    std::vector<mal_t_ptr> mal_vec_;
};
//======================================================
struct MalMap : MalType {
    MalMap() : MalType { Mal_T::Map} {}

    void add(mal_t_ptr k, mal_t_ptr v) {
        malmap_[k] = v;
    }

    std::unordered_map<mal_t_ptr, mal_t_ptr> malmap_;
};
//=====================================================


struct MalNumber : MalType {
    explicit MalNumber(int number) : MalType {Mal_T::Number},  number_{ number } {}
    int number_;
};

struct MalSymbol : MalType {
    explicit MalSymbol(std::string symbol) : MalType {Mal_T::Symbol}, symbol_ { std::move(symbol) } {}
    std::string symbol_;
};


struct MalString : MalType {
    explicit MalString(std::string s) : MalType {Mal_T::String}, string_ { std::move(s) } {}
    std::string string_;
};

struct MalBool : MalType {
    explicit MalBool(std::string token) : MalType {Mal_T::Bool}, bool_ {token == "true"} {} 
    bool bool_;
};

struct MalNil : MalType {
    explicit MalNil(std::string nil) : MalType { Mal_T::Nil}, nil_{nil} {}
    std::string nil_;
};

// TODO: may have to apply a template later
struct MalFunction : MalType {
    MalFunction(std::function<int(int, int)> f) : MalType {Mal_T::Function}, fn { f } {}

    int apply_fn(std::vector<mal_t_ptr> args) {
        int result = 0;

        for(int index = 0; auto mal_num_ptr : args) {
            auto num_ptr = dynamic_cast<MalNumber*>(mal_num_ptr.get());
            
            if(num_ptr == nullptr)
                throw std::logic_error("Must pass in a number!");

            if(index == 0) {
                result = num_ptr->number_; 
            } else {
                result = fn(result, num_ptr->number_);
            }
            index++;
        }

        return result;
    }

    std::function<int(int, int)> fn;
};


#endif // MALTYPE_H_