//
// Created by Dustin Burda on 12/9/23.
//

#ifndef MAL_MASTER_READER_H
#define MAL_MASTER_READER_H

#include <string>
#include <vector>
#include <regex>
#include <optional>
#include "Types.h"

class Reader {
public:
    Reader() = default;
    explicit Reader(std::vector<Token> tokens) : pos_{0}, tokens_{std::move(tokens)} {}

    std::optional<std::string> Next() {
        if(pos == tokens_.size() -1 )
            return std::nullopt;

        pos_++;

        return tokens_[pos_ - 1];
    }

    std::optional<std::string> Peek() const {
        if(pos == tokens_.size() - 1)
            return std::nullopt;
        return tokens_[pos_];
    }

    void ReadList() {}
    void ReadAtom() {}

private:
    size_t pos_;
    std::vector<std::string> tokens_;
};


static const std::regex lisp_reg {"[\\s,]*(~@|[\\[\\]{}()'`~^@]|\"(?:\\\\.|[^\\\\\"])*\"?|;.*|[^\\s\\[\\]{}('\"`,;)]*)"};
using Token = std::string;

static MalType read_form(Reader& reader) {
    auto curr_token = reader.Peek().value();

    MalType type;
    switch(curr_token[0]) {
        case '(':
            type = read_list(reader);
            break;
        default:
            type = read_atom(reader);

    }

    return type;
}

static void tokenize(std::string& src, std::vector<Token>& tokens) {
    auto begin_it = std::sregex_iterator(src_.begin(), src_.end(), lisp_reg);
    auto end_it = std::sregex_iterator();

    for(auto it = begin_it; it != end_it; ++it) {
        std::smatch matches = *i;

        if(matches.size() <= 1 || matches[1].str().empty() )
            continue;

        tokens.push_back( matches[1].str() );

    }
}

static void read_str(std::string src) {

    std::vector<Token> tokens;
    tokenize(tokens);

    // Reader consumes tokens
    Reader reader { tokens };

    read_form(reader);
}

#endif //MAL_MASTER_READER_H
