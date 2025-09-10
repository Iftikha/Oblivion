#include "Conversation.hpp"

Conversation::Conversation() {
    this->filename = "history/history.txt";
    
    // Check if file exists, if not create it
    std::ifstream checkFile(this->filename);
    if (!checkFile.is_open()) {
        std::ofstream createFile(this->filename);
        createFile.close();
    }
    checkFile.close();
    
    // Now open for reading
    std::ifstream history(this->filename);
    if (!history.is_open()) {
        std::cout << "Failed to open History file." << std::endl;
    } else {
        std::string json_str;
        while (getline(history, json_str)) {
            if (json_str.empty()) continue;
            try {
                json historyJson = json::parse(json_str);
                historyJsonVector.push_back(historyJson);
            } catch (const std::exception& e) {
                std::cerr << "JSON parse error: " << e.what() << "\nLine: " << json_str << std::endl;
            }
        }
        history.close();
        std::cout << "History loaded!" << std::endl;
    }
}

void Conversation::saveConversation(std::string prompt, std::string response, std::string username) {
    if (this->filename.empty()) {
        return;
    }
    
    std::ofstream history(this->filename, std::ios::app);
    if (!history.is_open()) {
        std::cout << "Failed to open file. History couldn't save." << std::endl;
        return;
    }

    json historyJson;
    historyJson["username"] = username;
    historyJson["prompt"]   = prompt;
    historyJson["response"] = response;

    std::string str_history = historyJson.dump();
    this->historyJsonVector.push_back(historyJson);

    history << str_history << "\n";
    history.close();
}

std::string Conversation::getConversationHistory() {
    if (historyJsonVector.empty()) {
        return "No history";
    }

    int sizeOfVector = historyJsonVector.size();
    int historyCount = (sizeOfVector < 7) ? sizeOfVector : 7;

    std::string str_history = "{\n";
    for (int i = 0; i < historyCount; i++) {
        int index = sizeOfVector - 1 - i;
        str_history += historyJsonVector[index].dump();
        if (i != historyCount - 1) {
            str_history += ",";
        }
    }
    str_history += "\n}";
    return str_history;
}
