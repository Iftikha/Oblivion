#ifndef CONVERSATION_HPP
#define CONVERSATION_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

class Conversation{
    public:
        Conversation();

        void saveConversation(std::string prompt, std::string response, std::string username);
        std::string getConversationHistory();

        // void viewAllConversationHistory();   // For later use, if i added a page of view history where users can see the past interactions with the LLM;

        
    private:
        std::vector<json> historyJsonVector;
        std::string filename;
};

#endif