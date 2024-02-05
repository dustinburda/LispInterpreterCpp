#include "Printer.h"

std::string pr_str(mal_t_ptr mal_ptr) {
    std::string result;

    auto mal_raw_ptr = mal_ptr.get();
    switch (mal_raw_ptr->get_type()) {
        case Mal_T::Number:
        {
            auto number_ptr = dynamic_cast<MalNumber*>(mal_raw_ptr);
            result = std::to_string(number_ptr->number_);
            break;
        }
        case Mal_T::Quote:
        {
            auto quote_ptr = dynamic_cast<MalQuote*>(mal_raw_ptr);
            auto quote_str = quote_ptr->quote_symbol_;

            if (quote_str == "\'") {
                result = "quote";
            } else if (quote_str == "`") {
                result = "quasiquote";
            } else if (quote_str == "~") {
                result = "unquote";
            } else if (quote_str == "~@") {
                result = "splice-unquote";
            } else if (quote_str == "@") {
                result = "deref";
            }
            break;
        }
        case Mal_T::Symbol:
        {
            auto symbol_ptr = dynamic_cast<MalSymbol*>(mal_raw_ptr);
            result = symbol_ptr->symbol_;
            break;
        } case Mal_T::Keyword:
        {
            // TODO: do we need this?
            auto kw_ptr = dynamic_cast<MalKeyword*>(mal_raw_ptr);
            result = ":" + kw_ptr->keyword_;
            break;
        }
        case Mal_T::String:
        {
            auto string_ptr = dynamic_cast<MalString*>(mal_raw_ptr);
            result = "\"" + string_ptr->string_ + "\"";
            break;
        }
        case Mal_T::Nil:
        {
            auto nil_ptr = dynamic_cast<MalNil*>(mal_raw_ptr);
            result = "nil";
            break;
            }
        case Mal_T::Bool:
        {
            auto bool_ptr = dynamic_cast<MalBool*>(mal_raw_ptr);
            result = (bool_ptr->bool_) ? "true" : "false";
            break;
            }
        case Mal_T::List:
        {
            auto list_ptr = dynamic_cast<MalList*>(mal_raw_ptr);
            result += "(";

            int elem_num { 0 };
            for (auto elem : list_ptr->mal_list_) {
                auto prefix = (elem_num == 0) ? "" : " ";
                result += prefix + pr_str(elem);

                elem_num++;
            }

            result += ")";
            break;
            }
        case Mal_T::Vec:
        {
            auto vec_ptr = dynamic_cast<MalVec*>(mal_raw_ptr);
            result += "[";

            int elem_num { 0 };
            for (auto elem : vec_ptr->mal_vec_) {
                auto prefix = (elem_num == 0) ? "" : " ";
                result += prefix + pr_str(elem);

                elem_num++;
            }

            result += "]";
            break;
            }
        case Mal_T::Map:
        {
            auto map_ptr = dynamic_cast<MalMap*>(mal_raw_ptr);
            result += "{";

            int elem_num { 0 };
            for(auto [k, v] : map_ptr->malmap_) {
                auto key = pr_str(k);
                auto value = pr_str(v);
                auto prefix = (elem_num == 0) ? "" : " ";
                result += prefix + key + " " + value;

                elem_num++;
            }

            result += "}";
            break;
        }
        default:
            throw std::logic_error("There should be no unimplemented types!");
    }
    
    return result;
}