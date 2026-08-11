#pragma once

#include <string_view>
#include <vector>

class Parser {
public:
    std::string_view                prefix;
    std::string_view                name;
    std::vector<std::string_view>   params;

    Parser(std::string_view message);
    ~Parser() = default;
};