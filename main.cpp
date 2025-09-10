/*

________ __________.____    ._______   ____.___________    _______   
\_____  \\______   \    |   |   \   \ /   /|   \_____  \   \      \  
 /   |   \|    |  _/    |   |   |\   Y   / |   |/   |   \  /   |   \ 
/    |    \    |   \    |___|   | \     /  |   /    |    \/    |    \
\_______  /______  /_______ \___|  \___/   |___\_______  /\____|__  /
        \/       \/        \/                          \/         \/ 

A simple yet good gemini based project- This is version 0.1;

*/




#include <iostream>
#include <fstream>
// #include "./libraries/json.hpp"

// using json = nlohmann::json;

#include "include/Engine.hpp"
#include "include/Conversation.hpp"
#include "include/CommandParser.hpp"
#include "include/ResponseHandler.hpp"

void greet(){
    std::cout << " ________ __________.____    ._______   ____.___________    _______   " << std::endl;
    std::cout << " \\_____ \\\\______   \\    |   |   \\   \\ /   /|   \\_____  \\   \\      \\  " << std::endl;
    std::cout << " /   |   \\|    |  _/    |   |   |\\   Y   / |   |/   |   \\  /   |   \\ " << std::endl;
    std::cout << "/    |    \\    |   \\    |___|   | \\     /  |   /    |    \\/    |    \\" << std::endl;
    std::cout << "\\_______  /______  /_______ \\___|  \\___/   |___\\_______  /\\____|__  /" << std::endl;
    std::cout << "        \\/       \\/        \\/                          \\/         \\/" << std::endl;
}

std::string getenv(){
    std::ifstream envFile(".env");
    std::string line;
    while (std::getline(envFile, line)) {
        if (line.find("API_KEY=") == 0) {
            std::string apiKey = line.substr(8); // skip "API_KEY="
            return apiKey;
        }
    }
    return "No api key";
}

int main(){
    greet();
    std::string apikey = getenv();
    if(apikey == "No api key"){
        std::cout << "No api key found" << std::endl;
        return 1;
    }
    Engine engine(apikey);
    engine.init();
    std::cout << "Enter your name: ";
    std::string username;
    getline(std::cin, username);

    // Getting conversation history;

    Conversation history;
    
    CommandParser parser("commands/commands.json");
    bool isLoaded = parser.loadCommands();
    ResponseHandler responseHandler(username, history);
    
   while (true) {

    std::cout << "You> ";
    std::string prompt;
    getline(std::cin, prompt);

    if (prompt.empty()) {
        continue;
    }
    if (prompt == "/bye") {
        break;
    }

    // Step 1: Ask AI to generate system command
    std::string genResponse = responseHandler.generateCommandPrompt(prompt);
    if (genResponse.empty()) {
        continue;
    }

    std::string response = engine.sendRequest(genResponse);
    response = responseHandler.trim(response);  // <-- implement a trim() helper

    // Step 2: Clean AI response
    // std::string upperResp = responseHandler.toUpper(response);
    
    // Step 3: Check for INVALID
    if (response == "INVALID_COMMAND") {
        // Fallback → normal chatbot
        genResponse = responseHandler.generateResponsePrompt(prompt);
        response = engine.sendRequest(genResponse);
        history.saveConversation(genResponse, response, username);
        std::cout << "Oblivion> " << response << std::endl;
    } else {
        // Otherwise → treat as system command
        // std::cout << "Executing: " << response << std::endl;
        bool ok = parser.executeCommand(response);
        if (!ok) {
            std::cout << "Failed to run command!" << std::endl;
        }
    }
}



    return 0;
}