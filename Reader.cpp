#include "Reader.h"
#include "Printer.h"

mal_t_ptr read_quote(Reader& r, const std::string& quote_str) {

    mal_t_ptr s = std::make_shared<MalList>();

    auto mal_list_ptr =  dynamic_cast<MalList*>(s.get());

    mal_t_ptr quote_ptr = std::make_shared<MalQuote>(quote_str);
    mal_list_ptr->add(quote_ptr);
    r.next(); //quote token

    while(r.peek() != std::nullopt) {
        auto mal_t_elem = read_form(r);
        if(mal_t_elem != nullptr)
            mal_list_ptr->add(mal_t_elem);
    }


    return s;
}

mal_t_ptr read_list(Reader& r) {
    r.next(); // '('

    mal_t_ptr s = std::make_shared<MalList>();

    auto mal_list_ptr =  dynamic_cast<MalList*>(s.get());
    while(r.peek().value()[0] != ')') {
        auto mal_t_elem = read_form(r);
        if(mal_t_elem != nullptr)
            mal_list_ptr->add(mal_t_elem);
    }

    r.next(); // ')'

    return s;
}

mal_t_ptr read_vec(Reader& r) {
    r.next(); // '['

    mal_t_ptr s = std::make_shared<MalVec>();

    auto mal_list_ptr =  dynamic_cast<MalVec*>(s.get());
    while(r.peek().value()[0] != ']') {
        auto mal_t_elem = read_form(r);
        if(mal_t_elem != nullptr)
            mal_list_ptr->add(mal_t_elem);
    }

    r.next(); // ']'

    return s;
}

mal_t_ptr read_map(Reader& r) {
    r.next(); // '{'

    mal_t_ptr s = std::make_shared<MalMap>();

    auto mal_map_ptr = dynamic_cast<MalMap*>(s.get());

    while(r.peek().value()[0] != '}') {
        auto mal_t_k = read_form(r);
        auto mal_t_v = read_form(r);
        if(mal_t_k != nullptr && mal_t_v != nullptr)
            mal_map_ptr->add(mal_t_k, mal_t_v);
        else
            throw std::logic_error("Every key must have a value!");
    }

    r.next(); // '}'

    return s;
}

mal_t_ptr read_atom(Reader& r) {
    if(r.peek() == std::nullopt) {
        return nullptr;
    }

    mal_t_ptr s;

    auto curr_token = r.next().value();

    if( (curr_token.size() > 1 && curr_token[0] == '-' && std::isdigit(curr_token[1])) || std::isdigit(curr_token[0])) {
        s = std::make_shared<MalNumber>(std::stoi(curr_token));
    } else if (curr_token[0] == ':') {
        s = std::make_shared<MalKeyword>(curr_token.substr(1, curr_token.size() - 1));
    } else if (curr_token[0] == '"' && curr_token[curr_token.size() - 1] == '"') {
        s = std::make_shared<MalString>(curr_token.substr(1, curr_token.size() - 2));
    } else if (curr_token == "nil") {
        s = std::make_shared<MalNil>(curr_token);
    } else if (curr_token == "true" || curr_token == "false") {
        s = std::make_shared<MalBool>(curr_token);
    } else {
        s = std::make_shared<MalSymbol>(curr_token);
    }

    return s;
}

mal_t_ptr read_form(Reader& r) {

    auto token = r.peek();

    mal_t_ptr ret;

        if (token.value()[0] == '(' ) {
            ret = read_list(r);
        } else if (token.value()[0] == '[') {
            ret = read_vec(r);
        } else if(token.value()[0] ==  '{') {
            ret = read_map(r);
        } else if(token.value().substr(0, 2) == "~@") {
            ret = read_quote(r, token.value().substr(0, 2));
        } else if (token.value()[0] == '\'' || token.value()[0] == '`' || token.value()[0] == '~' || token.value()[0] == '@') {
            ret = read_quote(r, std::string(1, token.value()[0]));
        } else {
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


static bool is_quote_balanced(std::string& src) {
    bool in_quote = false;
    int num_quote_symbols = 0;
    for(int i = 0; i < src.size(); i++) {
        char char_elem = src[i];
        if(i < src.size() - 1 && char_elem == '\\' && (src[i+1] == '\"' || src[i+1] == '\\')) {
            i += 1;
            continue;
        }

        if(char_elem == ';' && !in_quote) {
            break;
        }
        if(in_quote) {
            if(char_elem == '"'){
                in_quote = false;
                num_quote_symbols++;
            }
            continue;
        }

        if(char_elem == '"') {
            in_quote = true;
            num_quote_symbols++;
            continue;
        }
    }

    return num_quote_symbols % 2 == 0;
}

static bool is_balanced(std::string& src) {
    if(!is_quote_balanced(src)) {
        return false;
    }


    std::stack<char> s;
    std::unordered_map<char, char> m = { {'}', '{'}, {')', '(' }, {']', '['} };
    bool in_quote = false;
    for (auto char_elem : src) {

        if(char_elem == ';' && !in_quote) {
            break;
        }
        if(in_quote) {
            if(char_elem == '"'){
                in_quote = false;
            }
            continue;
        }

        if(char_elem == '"') {
            in_quote = true;
            continue;
        }


        if(char_elem == '{' || char_elem == '(' || char_elem == '[') {
            s.push(char_elem);
        } else if (char_elem == '}' || char_elem == ')' || char_elem == ']') {
            if(s.empty()) {
                return false;
            }
            if(s.top() != m[char_elem]) {
                return false;
            }
            s.pop();
        }
    }

    return s.empty();
}



mal_t_ptr read_str(std::string src) {
    if(!is_balanced((src))) {
        throw std::logic_error("Unbalanced!");
    }

    std::vector<Token> tokens;
    tokenize(src, tokens);

    auto r = Reader { tokens };

    auto ptr = read_form(r);

    return ptr;
}