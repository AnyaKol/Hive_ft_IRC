#include "../inc/IRC.hpp"
#include "../inc/Channel.hpp"

inline bool isValidChannelName(std::string_view name)
{
    if (name.size() < 2 || name.size() > IRC::CHANNELLEN)
        return false;
    if (name[0] != '#' && name[0] != '&')
        return false;
    for (char c : name)
    {
        if (c == ' ' || c == ',' || c == '\a' || static_cast<unsigned char>(c) <= 32)
            return false;
    }
    return true;
}