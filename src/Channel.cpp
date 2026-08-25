#include "Channel.hpp"
#include "Client.hpp"
#include <cmath> // for NAN

namespace mode {
    enum allModes {
        INVITE_ONLY = static_cast<int>('i'),
        PROTECTED_TOPIC = static_cast<int>('t'),
        HAS_KEY = static_cast<int>('k'),
        HAS_USER_LIMIT = static_cast<int>('l')
    };
}

Channel::Channel(std::string_view name, Client& creator) {
    this->_name = name;
    this->_topic = "";
    this->key = "";
    this->_userLimit = NAN; // or can set to 0 if _hasUserLimit = false
    this->addOperator(creator);
}

#pragma region Getters and setters
//Getters
std::string&	Channel::getName(void) const {
    return( this->_name );
}

std::string&	Channel::getTopic(void) const {
    return( this->_topic );
}

std::size_t Channel::getUserLimit(void) const {
    return( this->_userLimit );
}

//Setters
void	Channel::setName(std::string_view name) {
    if (name.empty())
        //print error
    else
        this->_name = name;
}

void	Channel::setTopic(std::string_view topic, Client& client) {
    if (this->_protectedTopic) {
        if ( !this->_operators.contains(&client) ) {
            //print error
            return;
        }
    }
    this->_topic = topic;
}

void	Channel::setUserLimit(std::size_t limit) {
    if (!this->_hasUserLimit) {
        //print error
        return;
    }
    if (this->_members.size() + this->_operators.size() > limit)
        //print error
    else
        this->_userLimit = limit;
}
#pragma endregion

void	Channel::addMember(Client& client) {
    std::pair<iterator, bool>   result;

    if (this->isFull()) {
        //print error
        return;
    }
    result = this->_members.insert(&client);
    if (!result.second)
        //print error
}

void	Channel::addOperator(Client& client) {
    std::pair<iterator, bool>   result;

    if (this->isFull()) {
        //print error
        return;
    }
    result = this->_operators.insert(&client);
    if (!result.second)
        //print error
}

void	Channel::removeMember(Client& client) {
    this->_members.erase( this->_members.find(&client) );
}

void	Channel::removeOperator(Client& client) {
    this->_operators.erase( this->_operators.find(&client) );
}

void	Channel::memberToOperator(Client& client) {
    this->removeMember(client);
    this->addOperator(client);
}

void	Channel::operatorToMember(Client& client) {
    this->removeOperator(client);
    this->addMember(client);
}

/* Channel modes:
 *  i - _inviteOnly
 *  t - _protectedTopic
 *  k - _hasKey
 *  o - memberToOperator / operatorToMember in MODE
 *  l - _hasUserLimit
 */
void	Channel::changeMode(char mode, bool value) {
    switch (static_cast<int>(mode)) {
        case mode::INVITE_ONLY:
            this->_inviteOnly = value;
            break;
        case mode::PROTECTED_TOPIC:
            this->_protectedTopic = value;
            break;
        case mode::HAS_KEY:
            this->_hasKey = value;
            break;
        case mode::HAS_USER_LIMIT:
            this->_hasUserLimit = value;
            break;
        default:
            // print error
    }
}

bool	Channel::isFull(void) const {
    if (this->_members.size() + this->_operators.size() == this->_userLimit)
        return (true);
    return (false);
}