#include "../inc/Parser.hpp"
#include <algorithm>
#include <ranges>
#include <cctype>
#include <ranges>

Parser::Parser(std::string_view message) {

    // 1. Strip the hidden network line endings (\r\n)
    if (message.ends_with("\r\n")) {
        message.remove_suffix(2);
    } else if (message.ends_with('\n')) {
        message.remove_suffix(1);
    }

    // 2. Extract the Prefix (if the message starts with ':')
    if (message.starts_with(':')) {
        auto space_pos = message.find(' ');
        if (space_pos != std::string_view::npos) {
            _prefix = message.substr(1, space_pos - 1);
            message.remove_prefix(space_pos + 1);
        } else {
            _prefix = message.substr(1);
            message = "";
        }
    }

    // 3. Handle the "Trailing" parameter (everything after " :")
    auto trailing_pos = message.find(" :");
    std::string_view trailing_param;
    if (trailing_pos != std::string_view::npos) {
        trailing_param = message.substr(trailing_pos + 2);
        message.remove_suffix(message.size() - trailing_pos);
    }

    // 4. Split the remaining text by spaces using C++20 Ranges
    for (const auto& word : std::views::split(message, ' ')) {
        std::string_view token{word.begin(), word.end()};

        if (!token.empty()) {
            if (_command.empty()) {
                setCommand(token);
            } else {
                _params.push_back(token);
            }
        }
    }

    // 5. Append the trailing parameter to the end of the list
    if (trailing_pos != std::string_view::npos) {
        _params.push_back(trailing_param);
    }
}

void Parser::setCommand(std::string_view command) {

    _command.resize(command.size());

    std::transform(command.begin(), command.end(), _command.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });

    if (_command == "PASS" || _command == "PING" || _command == "PONG" || _command == "QUIT" || _command == "NICK" || _command == "USER" || _command == "PRIVMSG" || _command == "JOIN" || _command == "PART" || _command == "TOPIC" || _command == "INVITE" || _command == "KICK" || _command == "MODE") {
        return;
    }
    _command = "UNKNOWN";
}
