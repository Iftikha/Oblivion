/*

________ __________.____    ._______   ____.___________    _______   
\_____  \\______   \    |   |   \   \ /   /|   \_____  \   \      \  
 /   |   \|    |  _/    |   |   |\   Y   / |   |/   |   \  /   |   \ 
/    |    \    |   \    |___|   | \     /  |   /    |    \/    |    \
\_______  /______  /_______ \___|  \___/   |___\_______  /\____|__  /
        \/       \/        \/                          \/         \/ 

A simple yet good gemini based project- This is version 3.0;
Here's a small paragraph about the project:

This project is a C++ console application that acts as an AI assistant, utilizing the Gemini model via
API calls. It combines a conversational interface with system-level command execution, allowing users to
interact with a smart chatbot that can also perform tasks like opening applications, searching the web,
or shutting down the computer. The program includes features for conversation history, and can even
generate and play audio responses, providing a more dynamic and engaging user experience.

*/










#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
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
    parser.loadCommands();
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


    float result = parser.checkCommandValidity(prompt);

    std::string response = "";
    if(result > 0.3){
        response = engine.sendRequest(genResponse);
        response = responseHandler.trim(response); 
    }

    // Step 3: Check for INVALID
    if (response == "INVALID_COMMAND" || response == "") {
        // Fallback → normal chatbot
        genResponse = responseHandler.generateResponsePrompt(prompt);
        response = engine.sendRequest(genResponse);
        history.saveConversation(prompt, response, username);
        ResponseHandler audioGenerator(apikey);
        std::string filename = audioGenerator.generateAudio(response);
        std::cout << filename << std::endl;
        if(filename.find(".wav")){
            std::thread audioThread(&ResponseHandler::playAudio, &audioGenerator, filename);
            audioThread.detach();
        }else{
            std::cout << filename << std::endl;
        }
        std::cout << "Oblivion> ";
        for(char c: response){
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            std::cout << c << std::flush;
        }
    } else if(response != "") {
        bool ok = parser.executeCommand(response);
        if (!ok) {
            std::cout << "Failed to run command!" << std::endl;
        }
    }
}
    return 0;
}