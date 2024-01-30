#ifndef MALTYPE_H_
#define MALTYPE_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>


struct MalType;
using mal_t_ptr = std::shared_ptr<MalType>;

struct MalType {
    MalType() {};
    virtual ~MalType() {};

};

struct MalList : MalType {
    MalList() = default;

    void add(mal_t_ptr& mal_t) {
        mal_list_.push_back(mal_t);
    }

    std::vector<mal_t_ptr> mal_list_;
};


struct MalVec : MalType {
    MalVec() = default;

    void add(mal_t_ptr& mal_t) {
        mal_vec_.push_back(mal_t);
    }

    std::vector<mal_t_ptr> mal_vec_;
};
//======================================================
struct MalMap : MalType {
    MalMap() = default;

    void add(mal_t_ptr k, mal_t_ptr v) {
        malmap_[k] = v;
    }

    std::unordered_map<mal_t_ptr, mal_t_ptr> malmap_;
};
//=====================================================


struct MalNumber : MalType {
    explicit MalNumber(int number) : number_{ number } {}

    int number_;
};

struct MalSymbol : MalType {
    explicit MalSymbol(std::string symbol) : symbol_ { std::move(symbol) } {}

    std::string symbol_;
};


struct MalString : MalType {
    explicit MalString(std::string s) : string_ { std::move(s) } {}

    std::string string_;
};

struct MalBool : MalType {
    explicit MalBool(std::string token) : bool_ {token == "true"} {} 

    bool bool_;
};

struct MalNil : MalType {
    explicit MalNil(std::string nil) : nil_{nil} {}
    std::string nil_;
};


#endif // MALTYPE_H_