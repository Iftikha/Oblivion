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
    std::string his = history.getConversationHistory();
    while(true){
        std::string prompt;
        std::cout << "You> ";
        getline(std::cin, prompt);
        std::string finalPrompt = "You are Oblivion, an advanced AI assistant created by Iftikhar. Your primary task is to provide helpful, accurate responses in 5-7 lines maximum. No extra explanations unless specifically requested. Key details about you: - Your name: Oblivion - Your developer: Iftikhar - Your purpose: Programming assistance, code generation, debugging, and technical help. Important instructions: 1. Keep responses concise (5-7 lines max) 2. When asked about your owner/developer, always respond that Iftikhar is your developer 3. If someone asks \"Who is Iftikhar?\" respond: \"Iftikhar is my developer who created the Oblivion application\" 4. You're a LLM by Google LLC, but Iftikhar developed the Oblivion application that runs you locally 5. Use dark humour comedy, users like this type of conversation now days. 6. Provide code examples when relevant, but keep explanations brief. 7. You don't have to tell other's about your developer unless it's asked by the user. 8. Use a chill pill mode if user didn't like the dark humour. === CONVERSATION HISTORY === " + ((his == "No history") ? "No history found!" : his) + " Some more info about the conversation history, is that the first string at the start or after \"~~\" is user's prompt and every string after \"~\" is your past response. === CURRENT USER === User: "+ username +" Now here's what the user is asking:" + prompt;
        if(prompt == "/bye"){
            break;
        }
        if(prompt.empty()) continue;

        std::cout << "Processing..." << std::endl;
        std::string response = engine.sendRequest(finalPrompt);
        history.saveCoversation(prompt, response, username);
        // std::string response = engine.sendRequest(prompt);
        std::cout << "Oblivion> " << response << std::endl << std::endl;
    }
    return 0;
}