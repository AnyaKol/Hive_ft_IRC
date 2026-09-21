#pragma once

#include <string>
#include <string_view>
#include <vector>

class Parser {
private:
    std::string_view                _prefix;
    std::string                     _command;
    std::vector<std::string_view>   _params;

public:
    Parser(std::string_view message);
    ~Parser() = default;

    const   std::string_view&                getPrefix() const {return _prefix;}
    const   std::string&                     getCommand() const {return _command;}
    const   std::vector<std::string_view>&   getParams() const {return _params;}

    void setCommand(std::string_view command);
};