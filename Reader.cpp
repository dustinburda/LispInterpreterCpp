#include "Reader.h"
#include "Printer.h"

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
    } else if (curr_token == "+" || curr_token == "-" || curr_token == "*" || curr_token == "/") {
        s = std::make_shared<MalSymbol>(curr_token);
    } else if (curr_token == "nil") {
        s = std::make_shared<MalNil>(curr_token);
    } else if (curr_token == "true" || curr_token == "false") {
        s = std::make_shared<MalBool>(curr_token);
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
        case '[':
            ret = read_vec(r);
            break;
        case '{':
            ret = read_map(r);
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



static bool is_balanced(std::string& src) {
    std::stack<char> s;
    std::unordered_map<char, char> m = { {'}', '{'}, {')', '(' }, {']', '['} };

    bool in_quote = false;
    int num_quote_symbols = 0;
    for (auto char_elem : src) {
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

    return s.empty() && (num_quote_symbols % 2 == 0);
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