#ifndef READER_H_
#define READER_H_

#include <regex>
#include <cstdio>
#include <vector>
#include <optional>

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


static mal_t_ptr read_list(Reader& r) {
    r.next(); // '('

    mal_t_ptr s;

    while(r.peek.value()[0] != ')') {
        // make s push back this 
        read_form(r);
    }
    
    return s;
}

static mal_t_ptr read_atom(Reader& r) {
    mal_t_ptr s;
    // implement integers, symbols, strings, bools
    return s;
}

static mal_t_ptr read_form(Reader& r) {

    auto token = r.peek();

    mal_t_ptr ret;

    switch (token.value()[0]) {
        case '(' :
            ret = read_list(r);
            break;
        default:
            ret = read_atom(r);
    }

    return ret;

}

static void tokenize(std::string& src, std::vector<Token>& tokens) {
    
    auto regex_begin = std::sregex_iterator(src.begin(), src.end(), lisp_regex);
    auto regex_end = std::sregex_iterator();

    for(auto it = regex_begin; it != regex_end; ++it) {
        auto match = *it;

        if(match.size() < 1) {
            throw std::logic_error("There must be two groups per match!");
        }

        auto match_str = match[1].str();

        tokens.push_back(match_str);
    }

}


static void read_str(std::string src) {
    std::vector<Token> tokens;
    tokenize(src, tokens);

    auto r = Reader { tokens };

    read_form(r);
}

#endif // READER_H_


