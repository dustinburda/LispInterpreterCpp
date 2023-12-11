//
// Created by Dustin Burda on 12/7/23.
//
#include <iostream>
#include <string>
#include <sstream>
#include "Reader.h"

std::string READ (const std::string& in) {
    auto ast = in;
    return ast;
}

std::string EVAL(const std::string& ast) {
    return ast;
}

std::string PRINT(const std::string& ast) {
    auto out = ast;
    return out;
}

std::string rep(const std::string& in) {
    return PRINT(EVAL(READ(in)));
}

int main() {
    while(true) {
        std::cout << "user> ";

        std::string line;
        getline(std::cin, line);

        if(std::cin.eof())
            break;

        std::cout << rep(line) << "\n";
    }
}