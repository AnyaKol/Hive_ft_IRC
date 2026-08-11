#include "../inc/Parser.hpp"
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
            prefix = message.substr(1, space_pos - 1);
            message.remove_prefix(space_pos + 1);
        } else {
            prefix = message.substr(1);
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
            if (name.empty()) {
                name = token;
            } else {
                params.push_back(token);
            }
        }
    }

    // 5. Append the trailing parameter to the end of the list
    if (trailing_pos != std::string_view::npos) {
        params.push_back(trailing_param);
    }
}
