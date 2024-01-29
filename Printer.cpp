#include "Printer.h"

std::string pr_str(mal_t_ptr mal_ptr) {
    std::string result;

    auto mal_raw_ptr = mal_ptr.get();

    if(auto number_ptr = dynamic_cast<MalNumber*>(mal_raw_ptr); number_ptr != nullptr) {
        result = std::to_string(number_ptr->number_);
    } else if (auto symbol_ptr = dynamic_cast<MalSymbol*>(mal_raw_ptr); symbol_ptr != nullptr) {
        result = symbol_ptr->symbol_;
    } else if (auto string_ptr = dynamic_cast<MalString*>(mal_raw_ptr); string_ptr != nullptr) {
        result = string_ptr->string_;
    } else if (auto nil_ptr = dynamic_cast<MalNil*>(mal_raw_ptr); nil_ptr != nullptr) {
        result = "nil";
    } else if (auto bool_ptr = dynamic_cast<MalBool*>(mal_raw_ptr); bool_ptr != nullptr) {
        result = (bool_ptr->bool_) ? "true" : "false";
    } else if (auto list_ptr = dynamic_cast<MalList*>(mal_raw_ptr); list_ptr != nullptr) {
        result += "(";

        int elem_num { 0 };
        for (auto elem : list_ptr->mal_list_) {
            auto prefix = (elem_num == 0) ? "" : " ";
            result += prefix + pr_str(elem);

            elem_num++;
        }

        result += ")";
    } else if (auto vec_ptr = dynamic_cast<MalVec*>(mal_raw_ptr); vec_ptr != nullptr) {
        result += "[";

        int elem_num { 0 };
        for (auto elem : vec_ptr->mal_vec_) {
            auto prefix = (elem_num == 0) ? "" : " ";
            result += prefix + pr_str(elem);

            elem_num++;
        }

        result += "]";
    }

    return result;
}