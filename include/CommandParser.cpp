#include "CommandParser.hpp"
#include <windows.h>
#include <shellapi.h>
#include <cctype>

CommandParser::CommandParser(std::string filename){
    this->filename = filename;
    this->loaded = false;
}

std::string CommandParser::toLowwer(std::string str){
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

double CommandParser::calculateSimilarity(std::string input, std::string alias){
    std::string lower_input_str = toLowwer(input);
    std::string lower_alias_str = toLowwer(alias);
    
    if(lower_input_str == lower_alias_str) return 1.0;
    
    if(lower_alias_str.find(lower_input_str) != std::string::npos || lower_input_str.find(lower_alias_str) != std::string::npos){
        return 0.8;
    }

    std::vector<std::string> input_words = splitWords(lower_input_str);
    std::vector<std::string> alias_words = splitWords(lower_alias_str);

    int matches = 0;

    for(const std::string& word: input_words){
        for(const std::string& alias: alias_words){
            if(word == alias && word.length() > 2){
                matches++;
                break;
            }
        }
    }

    if(matches > 0){
        return (double)matches / std::max(input_words.size(), alias_words.size()) * 0.6;
    }

    return 0.0;

}

std::vector<std::string> CommandParser::splitWords(std::string text){
    std::vector<std::string> words;
    std::string word = "";

    for(char c: text){
        if(c == ' ' || c == '\t'){
            if(!word.empty()){
                words.push_back(word);
                word = "";
            }
        }else{
            word += c;
        }
    }

    if(!word.empty()){
        words.push_back(word);
    }

    return words;

}

bool CommandParser::loadCommands(){
    try{
        std::ifstream file(this->filename);
        if(!file.is_open()){
            std::cout << "Command file not found: " << filename << std::endl;
            return false;
        }

        file >> this->command_db;
        file.close();
        loaded = true;
        std::cout << "System commands loaded successfully!" << std::endl;
        return true;
    }catch(json::exception& e){
        std::cout << "Error Loading commands: " << e.what() << std::endl;
        return false;
    }
}

std::string CommandParser::parseCommand(std::string user_input){
    if(!loaded || !command_db.contains("system_commands")){
        return "";
    }

    double best_score = 0.0;
    std::string best_command = "";
    std::string best_description = "";

    for(const auto& cmd: command_db["system_commands"]){
        if(cmd.contains("aliases")){
            for(const auto& alias: cmd["aliases"]){
                double score = calculateSimilarity(user_input, alias);
                if(score > best_score && score > 0.5){
                    best_score = score;
                    best_command = cmd["command"];
                    best_description = cmd["description"];
                }
            }
        }
    }

    if(!best_command.empty()){
        std::cout << "Command Found: " << best_description << std::endl;
        return best_command;
    }

    return "";

}

bool CommandParser::executeCommand(std::string command){
    if(command.empty()){
        std::cout << "No command provided!" << std::endl;
        return false;
    }

    std::cout << "Executing: " << command << std::endl;

    if(isCommandSafe(command)){
        
        int result = system(command.c_str());
        if(result == 0){
            std::cout << "Command executed successfully!" << std::endl;
            return true;
        }else{
            std::cout << "Command execution failed!" << std::endl;
            return false;
        }
    }else{
        std::cout << "Command blocked due to security reasons!" << std::endl;
        return false;
    }
}

bool CommandParser::isCommandSafe(std::string command){
    std::vector<std::string> dangerous_keywords = {
         "format", "del /s", "rd /s", "rmdir /s", 
            "attrib +h", "reg delete", "bcdedit"
    };
    std::string lower_command = toLowwer(command);
    for(const std::string& keyword: dangerous_keywords){
        if(lower_command.find(keyword) != std::string::npos){
            return false;
        }
    }
    return true;
}

void CommandParser::showCommands(std::string category){
    if(!loaded || !command_db.contains("system_commands")){
        std::cout << "No commands loaded!" << std::endl;
        return;
    }

    std::cout << "\n========== Available Commands ========== " << std::endl;

    for(const auto& cmd: command_db["system_commands"]){
        if(category.empty() || toLowwer(cmd["category"]) == toLowwer(category)){
            std::cout << "Category: " << cmd["category"] << std::endl;
            std::cout << "Description: " << cmd["description"] << std::endl;
            std::cout << "Aliases: ";
            for(size_t i = 0; i < cmd["aliases"].size(); i++){
                std::cout << cmd["aliases"][i];
                if(i < cmd["aliases"].size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
            std::cout << "Command: " << cmd["command"] << std::endl;
            std::cout << "-----------------------------" << std::endl << std::endl;
        }
    }

}

bool CommandParser::isReady() const {
    return loaded;
}

float CommandParser::checkCommandValidity(std::string user_input){
    std::string keywords[] = {"open", "launch", "run", "play", "search"};
    std::string found_word = "";
    float results = 0.0;
    for(const auto& c : user_input){
        if(isalnum(c)){
            found_word += c;
        }else{
            for(std::string str : keywords){
                if(found_word == str){
                    results += 0.4;
                }
            }
            found_word = "";
        }
    }

    return results;
}