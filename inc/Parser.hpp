#pragma once

#include <string>
#include <string_view>
#include <vector>

class Parser {
public:
    std::string_view                _prefix;
    std::string                     _command;
    std::vector<std::string_view>   _params;

    Parser(std::string_view message);
    ~Parser() = default;

    void setCommand(std::string_view command);
};