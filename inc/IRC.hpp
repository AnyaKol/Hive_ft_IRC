#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace IRC
{
    // ==========================================
    // Server Constants & Configuration
    // ==========================================
    constexpr std::uint16_t PORT_MIN             = 1024;
    constexpr std::size_t   NICKLEN              = 9;
    constexpr std::size_t   USERLEN              = 9;
    constexpr std::size_t   CHANNELLEN           = 50;
    constexpr std::size_t   EVENT_QUEUE_SIZE     = 64;
    constexpr std::size_t   MAX_CHANNEL_SIZE     = 200;

    constexpr std::string_view SERVER_NAME            = "ircserv";
    constexpr std::string_view SERVER_VERSION         = "1.0";
    constexpr std::string_view AVAILABLE_USER_MODES   = "";
    constexpr std::string_view AVAILABLE_CHANNEL_MODES= "itkol";


    // ==========================================
    // Numeric RPL (Reply) Codes
    // ==========================================
    namespace RPL
    {
        constexpr std::string_view WELCOME         = "001";
        constexpr std::string_view YOURHOST        = "002";
        constexpr std::string_view CREATED         = "003";
        constexpr std::string_view MYINFO          = "004";
        constexpr std::string_view ISUPPORT        = "005";

        constexpr std::string_view CHANNELMODEIS   = "324";
        constexpr std::string_view CREATIONTIME    = "329";
        constexpr std::string_view NOTOPIC         = "331";
        constexpr std::string_view TOPIC           = "332";
        constexpr std::string_view TOPICWHOTIME    = "333";
        constexpr std::string_view INVITING        = "341";
        constexpr std::string_view NAMREPLY        = "353";
        constexpr std::string_view ENDOFNAMES      = "366";
    }

    // ==========================================
    // Numeric ERR (Error) Codes
    // ==========================================
    namespace ERR
    {
        constexpr std::string_view NOSUCHNICK       = "401";
        constexpr std::string_view NOSUCHCHANNEL    = "403";
        constexpr std::string_view CANNOTSENDTOCHAN = "404";
        constexpr std::string_view NOORIGIN         = "409";
        constexpr std::string_view NORECIPIENT      = "411";
        constexpr std::string_view NOTEXTTOSEND     = "412";
        constexpr std::string_view UNKNOWNCOMMAND   = "421";
        constexpr std::string_view NONICKNAMEGIVEN  = "431";
        constexpr std::string_view ERRONEUSNICKNAME = "432";
        constexpr std::string_view NICKNAMEINUSE    = "433";
        constexpr std::string_view USERNOTINCHANNEL = "441";
        constexpr std::string_view NOTONCHANNEL     = "442";
        constexpr std::string_view USERONCHANNEL    = "443";

        constexpr std::string_view NOTREGISTERED    = "451";
        constexpr std::string_view NEEDMOREPARAMS   = "461";
        constexpr std::string_view ALREADYREGISTERED= "462";
        constexpr std::string_view PASSWDMISMATCH   = "464";
        constexpr std::string_view CHANNELISFULL    = "471";
        constexpr std::string_view UNKNOWNMODE      = "472";
        constexpr std::string_view INVITEONLYCHAN   = "473";
        constexpr std::string_view BADCHANNELKEY    = "475";
        constexpr std::string_view BADCHANMASK      = "476";
        constexpr std::string_view CHANOPRIVSNEEDED = "482";

        constexpr std::string_view INVALIDKEY       = "525";
        constexpr std::string_view INVALIDMODEPARAM = "696";
    }

    // ==========================================
    // RFC-Compliant Reply Builders
    // Format: :<server> <code> <nick> <params...> :<message>\r\n
    // ==========================================
    namespace Reply
    {
        inline std::string target(std::string_view nick)
        {
            return nick.empty() ? "*" : std::string(nick);
        }

        // Generic reply builder
        inline std::string build(std::string_view code, std::string_view nick, std::string_view params, std::string_view trailing)
        {
            std::string msg = ":" + std::string(SERVER_NAME) + " " + std::string(code) + " " + target(nick);
            if (!params.empty())
                msg += " " + std::string(params);
            if (!trailing.empty())
                msg += " :" + std::string(trailing);
            msg += "\r\n";
            return msg;
        }

        // --- Registration & Connection Handshake ---
        inline std::string welcome(std::string_view nick, std::string_view user, std::string_view host)
        {
            return ":" + std::string(SERVER_NAME) + " 001 " + target(nick) +
                   " :Welcome to the IRC Network " + std::string(nick) + "!" + std::string(user) + "@" + std::string(host) + "\r\n";
        }

        inline std::string yourHost(std::string_view nick)
        {
            return ":" + std::string(SERVER_NAME) + " 002 " + target(nick) +
                   " :Your host is " + std::string(SERVER_NAME) + ", running version " + std::string(SERVER_VERSION) + "\r\n";
        }

        inline std::string created(std::string_view nick, std::string_view datetime)
        {
            return ":" + std::string(SERVER_NAME) + " 003 " + target(nick) +
                   " :This server was created " + std::string(datetime) + "\r\n";
        }

        inline std::string myInfo(std::string_view nick)
        {
            return ":" + std::string(SERVER_NAME) + " 004 " + target(nick) + " " +
                   std::string(SERVER_NAME) + " " + std::string(SERVER_VERSION) + " " +
                   std::string(AVAILABLE_USER_MODES) + " " + std::string(AVAILABLE_CHANNEL_MODES) + "\r\n";
        }

        inline std::string isSupport(std::string_view nick, std::string_view tokens)
        {
            return ":" + std::string(SERVER_NAME) + " 005 " + target(nick) + " " +
                   std::string(tokens) + " :are supported by this server\r\n";
        }

        // --- Channel Info & Actions ---
        inline std::string channelModeIs(std::string_view nick, std::string_view channel, std::string_view modes, std::string_view params = "")
        {
            std::string res = ":" + std::string(SERVER_NAME) + " 324 " + target(nick) + " " + std::string(channel) + " " + std::string(modes);
            if (!params.empty())
                res += " " + std::string(params);
            res += "\r\n";
            return res;
        }

        inline std::string creationTime(std::string_view nick, std::string_view channel, std::string_view timestamp)
        {
            return ":" + std::string(SERVER_NAME) + " 329 " + target(nick) + " " + std::string(channel) + " " + std::string(timestamp) + "\r\n";
        }

        inline std::string noTopic(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 331 " + target(nick) + " " + std::string(channel) + " :No topic is set\r\n";
        }

        inline std::string topic(std::string_view nick, std::string_view channel, std::string_view topicText)
        {
            return ":" + std::string(SERVER_NAME) + " 332 " + target(nick) + " " + std::string(channel) + " :" + std::string(topicText) + "\r\n";
        }

        inline std::string topicWhoTime(std::string_view nick, std::string_view channel, std::string_view setter, std::string_view timestamp)
        {
            return ":" + std::string(SERVER_NAME) + " 333 " + target(nick) + " " + std::string(channel) + " " + std::string(setter) + " " + std::string(timestamp) + "\r\n";
        }

        inline std::string inviting(std::string_view nick, std::string_view targetNick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 341 " + target(nick) + " " + std::string(targetNick) + " " + std::string(channel) + "\r\n";
        }

        inline std::string namReply(std::string_view nick, std::string_view channel, std::string_view namesList)
        {
            return ":" + std::string(SERVER_NAME) + " 353 " + target(nick) + " = " + std::string(channel) + " :" + std::string(namesList) + "\r\n";
        }

        inline std::string endOfNames(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 366 " + target(nick) + " " + std::string(channel) + " :End of /NAMES list\r\n";
        }

        // --- Error Replies ---
        inline std::string errNoSuchNick(std::string_view nick, std::string_view targetNick)
        {
            return ":" + std::string(SERVER_NAME) + " 401 " + target(nick) + " " + std::string(targetNick) + " :No such nick/channel\r\n";
        }

        inline std::string errNoSuchChannel(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 403 " + target(nick) + " " + std::string(channel) + " :No such channel\r\n";
        }

        inline std::string errCannotSendToChannel(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 404 " + target(nick) + " " + std::string(channel) + " :Cannot send to channel\r\n";
        }

        inline std::string errNoOrigin(std::string_view nick)
        {
            return ":" + std::string(SERVER_NAME) + " 409 " + target(nick) + " :No origin specified\r\n";
        }

        inline std::string errNoRecipient(std::string_view nick, std::string_view command)
        {
            return ":" + std::string(SERVER_NAME) + " 411 " + target(nick) + " :No recipient given (" + std::string(command) + ")\r\n";
        }

        inline std::string errNoTextToSend(std::string_view nick)
        {
            return ":" + std::string(SERVER_NAME) + " 412 " + target(nick) + " :No text to send\r\n";
        }

        inline std::string errUnknownCommand(std::string_view nick, std::string_view command)
        {
            return ":" + std::string(SERVER_NAME) + " 421 " + target(nick) + " " + std::string(command) + " :Unknown command\r\n";
        }

        inline std::string errNoNickGiven(std::string_view nick)
        {
            return ":" + std::string(SERVER_NAME) + " 431 " + target(nick) + " :No nickname given\r\n";
        }

        inline std::string errErroneousNick(std::string_view nick, std::string_view badNick)
        {
            return ":" + std::string(SERVER_NAME) + " 432 " + target(nick) + " " + std::string(badNick) + " :Erroneous nickname\r\n";
        }

        inline std::string errNickInUse(std::string_view nick, std::string_view badNick)
        {
            return ":" + std::string(SERVER_NAME) + " 433 " + target(nick) + " " + std::string(badNick) + " :Nickname is already in use\r\n";
        }

        inline std::string errUserNotInChannel(std::string_view nick, std::string_view targetNick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 441 " + target(nick) + " " + std::string(targetNick) + " " + std::string(channel) + " :They aren't on that channel\r\n";
        }

        inline std::string errNotOnChannel(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 442 " + target(nick) + " " + std::string(channel) + " :You're not on that channel\r\n";
        }

        inline std::string errUserAlreadyInChannel(std::string_view nick, std::string_view targetNick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 443 " + target(nick) + " " + std::string(targetNick) + " " + std::string(channel) + " :is already on channel\r\n";
        }

        inline std::string errNotRegistered(std::string_view nick)
        {
            return ":" + std::string(SERVER_NAME) + " 451 " + target(nick) + " :You have not registered\r\n";
        }

        inline std::string errNeedMoreParams(std::string_view nick, std::string_view command)
        {
            return ":" + std::string(SERVER_NAME) + " 461 " + target(nick) + " " + std::string(command) + " :Not enough parameters\r\n";
        }

        inline std::string errAlreadyRegistered(std::string_view nick)
        {
            return ":" + std::string(SERVER_NAME) + " 462 " + target(nick) + " :Unauthorized command (already registered)\r\n";
        }

        inline std::string errPasswordMismatch(std::string_view nick)
        {
            return ":" + std::string(SERVER_NAME) + " 464 " + target(nick) + " :Password incorrect\r\n";
        }

        inline std::string errChannelIsFull(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 471 " + target(nick) + " " + std::string(channel) + " :Cannot join channel (+l)\r\n";
        }

        inline std::string errUnknownMode(std::string_view nick, char modeChar)
        {
            return ":" + std::string(SERVER_NAME) + " 472 " + target(nick) + " " + std::string(1, modeChar) + " :is unknown mode char to me\r\n";
        }

        inline std::string errInviteOnlyChannel(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 473 " + target(nick) + " " + std::string(channel) + " :Cannot join channel (+i)\r\n";
        }

        inline std::string errBadChannelKey(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 475 " + target(nick) + " " + std::string(channel) + " :Cannot join channel (+k)\r\n";
        }

        inline std::string errBadChannelMask(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 476 " + target(nick) + " " + std::string(channel) + " :Bad Channel Mask\r\n";
        }

        inline std::string errChannelOpRequired(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 482 " + target(nick) + " " + std::string(channel) + " :You're not channel operator\r\n";
        }

        inline std::string errInvalidKey(std::string_view nick, std::string_view channel)
        {
            return ":" + std::string(SERVER_NAME) + " 525 " + target(nick) + " " + std::string(channel) + " :Key is not well-formed\r\n";
        }

        inline std::string errInvalidModeParam(std::string_view nick, std::string_view channel, char mode, std::string_view param, std::string_view desc)
        {
            return ":" + std::string(SERVER_NAME) + " 696 " + target(nick) + " " + std::string(channel) + " " + std::string(1, mode) + " " + std::string(param) + " :" + std::string(desc) + "\r\n";
        }
    }
}

/* usage examples:
// When a regular user tries to KICK or change channel MODE:
std::string reply = IRC::Reply::errChannelOpRequired(client.getNickname(), channel.getName());
send(client.getSocket(), reply.c_str(), reply.length(), 0);

// When a password in PASS is incorrect:
std::string reply = IRC::Reply::errPasswordMismatch(client.getNickname());
send(client.getSocket(), reply.c_str(), reply.length(), 0);

// When trying to join an invite-only room (+i):
std::string reply = IRC::Reply::errInviteOnlyChannel(client.getNickname(), channel.getName());
send(client.getSocket(), reply.c_str(), reply.length(), 0);
 */