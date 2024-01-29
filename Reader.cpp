#include "Reader.h"
#include "Printer.h"

mal_t_ptr read_list(Reader& r) {
    r.next(); // '('

    mal_t_ptr s = std::make_shared<MalList>();

    auto mal_list_ptr = dynamic_cast<MalList*>(s.get());
    while(r.peek().value()[0] != ')') {
        auto mal_t_elem = read_form(r);
        if(mal_t_elem != nullptr)
            mal_list_ptr->add(mal_t_elem);
    }

    r.next(); // ')'

    return s;
}

mal_t_ptr read_atom(Reader& r) {
    if(r.peek() == std::nullopt) {
        return nullptr;
    }

    mal_t_ptr s;

    auto curr_token = r.next().value();

    if(curr_token[0] == '-' || std::isdigit(curr_token[0])) {
        s = std::make_shared<MalNumber>(std::stoi(curr_token));
    } else if (curr_token == "+" || curr_token == "-" || curr_token == "*" || curr_token == "/") {
        s = std::make_shared<MalSymbol>(curr_token);
    } else {
        s = std::make_shared<MalString>(curr_token);
    }

    return s;
}

mal_t_ptr read_form(Reader& r) {

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

void tokenize(std::string& src, std::vector<Token>& tokens) {

    auto regex_begin = std::sregex_iterator(src.begin(), src.end(), lisp_regex);
    auto regex_end = std::sregex_iterator();

    for(auto it = regex_begin; it != regex_end; ++it) {
        auto match = *it;

        if(match.size() < 1) {
            throw std::logic_error("There must be two groups per match!");
        }

        auto match_str = match[1].str();

        if(match_str != "")
            tokens.push_back(match_str);
    }

}


mal_t_ptr read_str(std::string src) {
    std::vector<Token> tokens;
    tokenize(src, tokens);

    auto r = Reader { tokens };

    auto ptr = read_form(r);

    return ptr;
}