#ifndef MALTYPE_H_
#define MALTYPE_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <stdexcept>


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
    Function,
    Keyword,
    Quote
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

struct MalFunction : MalType {
    MalFunction(std::function<mal_t_ptr(std::vector<mal_t_ptr>)>f) : MalType {Mal_T::Function}, fn { f } {}

    mal_t_ptr apply_fn(std::vector<mal_t_ptr> args) {
        return fn(args);
    }

    std::function<mal_t_ptr(std::vector<mal_t_ptr>)> fn;
};

struct MalKeyword : MalType {
    explicit MalKeyword(std::string keyword) : MalType { Mal_T::Keyword }, keyword_ { keyword } {}
    std::string keyword_;
};

struct MalQuote : MalType {
    explicit MalQuote(std::string quote_symbol) : MalType { Mal_T::Quote}, quote_symbol_ { quote_symbol } {}
    std::string quote_symbol_;
};

#endif // MALTYPE_H_