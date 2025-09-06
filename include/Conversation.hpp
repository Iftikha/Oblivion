#ifndef CONVERSATION_HPP
#define CONVERSATION_HPP

#include <iostream>
#include <vector>
#include <fstream>

class Conversation{
    public:
        Conversation();

        void saveCoversation(std::string prompt, std::string response, std::string username);
        std::string getConversationHistory();

        // void viewAllConversationHistory();   // For later use, if i added a page of view history where users can see the past interactions with the LLM;

        
    private:
        std::vector<std::string> prompt;
        std::vector<std::string> response;
        std::vector<std::string> username;
        std::string filename;
};

#endif