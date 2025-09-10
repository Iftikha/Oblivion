#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

class CommandParser{
    public:
        CommandParser(std::string filename);

        bool loadCommands();
        std::string parseCommand(const std::string user_input);
        bool executeCommand(const std::string command);
        bool isCommandSafe(const std::string command);
        void showCommands(std::string category = "");
        bool isReady() const ;

    private:
        json command_db;
        std::string filename;
        bool loaded;

        std::string toLowwer(std::string str);
        double calculateSimilarity(std::string input, std::string alias);
        std::vector<std::string> splitWords(std::string text);
        
};

#endif