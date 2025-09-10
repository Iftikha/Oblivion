#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include <iostream>
#include "Conversation.hpp"

class ResponseHandler{
    public:
        ResponseHandler(std::string username, Conversation& history);

        std::string generateCommandPrompt(std::string user_input);
        std::string generateResponsePrompt(std::string user_input);

        // bool isValidCommandResponse(std::string commandResponse);
        std::string trim(std::string text);

    private:
        std::string username;
        Conversation history;

};

#endif