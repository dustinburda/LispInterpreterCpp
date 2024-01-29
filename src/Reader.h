#ifndef READER_H_
#define READER_H_

#include <regex>
#include <cstdio>
#include <vector>
#include <optional>
#include "MalType.h"

using Token = std::string;

static const std::regex lisp_regex { "[\\s,]*(~@|[\\[\\]{}()'`~^@]|\"(?:\\\\.|[^\\\\\"])*\"?|;.*|[^\\s\\[\\]{}('\"`,;)]*)" } ;

class Reader {
public:
    Reader() = delete;
    explicit Reader(std::vector<Token> tokens) : tokens_{std::move(tokens)}, curr_pos_{0} {}

    std::optional<Token> next() {
        if(curr_pos_ == tokens_.size()) {
            return std::nullopt;
        }

        auto ret = tokens_[curr_pos_];
        curr_pos_++;

        return ret;
    }

    std::optional<Token> peek() const {
        if(curr_pos_ == tokens_.size()) {
            return std::nullopt;
        }

        return tokens_[curr_pos_];
    }

    // Not copyable
    Reader(const Reader& other) = delete;
    Reader& operator=(const Reader& other) = delete;

    // Note moveable
    Reader(const Reader&& other) = delete;
    Reader& operator=(const Reader&& other) = delete;

private:
    std::vector<Token> tokens_;
    size_t curr_pos_;
};


mal_t_ptr read_list(Reader& r);
mal_t_ptr read_atom(Reader& r);
mal_t_ptr read_form(Reader& r);
void tokenize(std::string& src, std::vector<Token>& tokens);
void read_str(std::string src);

#endif // READER_H_


