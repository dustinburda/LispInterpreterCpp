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

static const std::regex lisp_reg {"[\\s,]*(~@|[\\[\\]{}()'`~^@]|\"(?:\\\\.|[^\\\\\"])*\"?|;.*|[^\\s\\[\\]{}('\"`,;)]*)"};
using Token = std::string;

class Reader {
public:
    Reader() = default;
    explicit Reader(std::vector<Token> tokens) : pos_{0}, tokens_{std::move(tokens)} {}

    std::optional<std::string> Next() {
        if(pos_ == tokens_.size() -1 )
            return std::nullopt;

        pos_++;

        return tokens_[pos_ - 1];
    }

    std::optional<std::string> Peek() const {
        if(pos_ >= tokens_.size())
            return std::nullopt;

        return tokens_[pos_];
    }

private:
    size_t pos_;
    std::vector<std::string> tokens_;
};


static mal_t_ptr read_list(Reader& reader) {
    mal_t_ptr list = std::make_shared<mal_list_t>();

    // Repeatedly call read_form until ')'
    while(reader.Peek().has_value() && reader.Peek() != ")") {
        list->add(read_form(reader));
    }

    if(!reader.Peek().has_value())
        throw std::logic_error("Encountered EOF before closed )");

    return list;
}

static mal_t_ptr read_atom(Reader& reader) {
    auto token = reader.Next();

    if(!token.has_value())
        return nullptr;

    std::unordered_set<std::string> symbol_table { "^", "\s", };


    if(std::isdigit( token[0]) ) {
        return std::make_shared<mal_num_t>(std::stoi(token));
    } else if(symbol_table.count(token) > 0){
        return std::make_shared< mal_symbol_t>(token);
    } else if(token[0] == '"') {
        assert(token[0] == token[token->size() - 1] && token[0] == '"');
        return std::make_shared<mal_string_t>(token);
    } else if(token[0] == ':') {
        return std::make_shared<mal_keyword>(token);
    }
}

static mal_t_ptr read_form(Reader& reader) {
    auto curr_token = reader.Peek().value();

    mal_t_ptr type;
    switch(curr_token[0]) {
        case '(':
            type = read_list(reader);
            break;
        default:
            type = read_atom(reader);

    }

}

static void tokenize(std::string& src, std::vector<Token>& tokens) {

    //TODO: Check for balanced parenthesis here

    auto begin_it = std::sregex_iterator(src.begin(), src.end(), lisp_reg);
    auto end_it = std::sregex_iterator();

    for(auto it = begin_it; it != end_it; ++it) {
        std::smatch matches = *it;

        if(matches.size() <= 1 || matches[1].str().empty() )
            continue;

        auto matched_string = matches[1].str();

        if( matched_string[0] == '"' && matched_string[matched_string.size() - 1] != '"')
            throw std::logic_error("Unmatched quote!");

        tokens.push_back( matches[1].str() );

    }
}

[[maybe_unused]] static void read_str(std::string src) {

    std::vector<Token> tokens;
    tokenize(src, tokens);

    // Reader consumes tokens
    Reader reader { tokens };

    read_form(reader);
}

#endif //MAL_MASTER_READER_H
