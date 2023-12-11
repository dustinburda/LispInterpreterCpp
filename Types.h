//
// Created by Dustin Burda on 12/9/23.
//

#ifndef MAL_MASTER_TYPES_H
#define MAL_MASTER_TYPES_H

#include <vector>
#include <string>
#include <memory>
#include <map>

struct mal_t {

};
using mal_t_ptr = std::shared_ptr<mal_t>;

class mal_list_t : public mal_t {
public:
    mal_list_t() = default;

    void add(mal_t_ptr& mal_elem) {
        list_.push_back(mal_elem);
    }

private:
    std::vector<mal_t_ptr> list_;
};


enum class symbol_t {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE
};

class mal_symbol_t : public mal_t {
public:
    explicit mal_symbol_t(char symbol) : symbol_ {symbol} {}

private:
    [[maybe_unused]] symbol_t symbol_;
};

class mal_int_t : public mal_t {
public:
    explicit mal_int_t(int num) : integer_ {num} {}

private:
    [[maybe_unused]]  int64_t integer_;
};

class mal_vector_t : public mal_t {
public:
    mal_vector_t() = default;

    mal_t_ptr operator[](std::size_t i) const {
        if(i >= vector_.size())
            throw std::logic_error("Invalid vector access!");
        return vector_[i];
    }

    void add(mal_t_ptr& mal_elem) {
        vector_.push_back(mal_elem);
    }

private:
    std::vector<mal_t_ptr> vector_;
};

class mal_string_t : public mal_t {
public:
    mal_string_t() = default;

    explicit mal_string_t(std::string& str) : string_ {str} {}
    explicit mal_string_t(std::string str) : string_ {std::move(str)} {}

    void set_string(std::string src) {
        string_ = src;
    }
private:
    std::string string_;
};

class mal_map_t : public mal_t {
    std::unordered_map<mal_t_ptr, mal_t_ptr> map_;
};

#endif //MAL_MASTER_TYPES_H
