//
// Created by Dustin Burda on 12/9/23.
//

#include "Reader.h"


mal_t_ptr read_atom(Reader& reader) {
    auto token = reader.Next();

    if(!token.has_value())
        return nullptr;


    if(std::isdigit( token[0])  || (token[0] == '-' && std::isdigit(token[1])) ) {
        return std::make_shared<mal_num_t>(std::stod(token));
    } else if(token[0] == '"') {
        assert(token[0] == token[token->size() - 1] && token[0] == '"');
        return std::make_shared<mal_string_t>(token);
    } else if(token[0] == ':') {
        return std::make_shared<mal_keyword>(token.value());
    } else {
        return std::make_shared<mal_symbol_t>(token);
    }
}

mal_t_ptr read_form(Reader& reader) {
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

mal_t_ptr read_list(Reader& reader) {
    mal_t_ptr list = std::make_shared<mal_list_t>();

    // Repeatedly call read_form until ')'
    while(reader.Peek().has_value() && reader.Peek() != ")") {
        list->add(read_form(reader));
    }

    if(!reader.Peek().has_value())
        throw std::logic_error("Encountered EOF before closed )");

    return list;
}