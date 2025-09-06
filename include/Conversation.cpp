#include "Conversation.hpp"

Conversation::Conversation(){
    this->filename = "history/history.txt";
    
    // First check if file exists, if not create it
    std::ifstream checkFile(this->filename);
    if (!checkFile.is_open()) {
        std::ofstream createFile(this->filename);
        createFile.close();
    }
    checkFile.close();
    
    // Now open for reading
    std::fstream history(this->filename, std::ios::in);
    if(!history.is_open()){
        std::cout << "Failed to open History file." << std::endl;
    }else{
        std::string prompt, response, username;
        while(!history.eof()){
            getline(history, username, '~');
            getline(history, prompt, '~');
            getline(history, response);
            if(!prompt.empty() && !response.empty()) {  // Only add non-empty entries
                this->prompt.push_back(prompt);
                this->username.push_back(username);
                this->response.push_back(response);
            }
        }
        history.close();
        std::cout << "History loaded!" << std::endl;
    }
}

void Conversation::saveCoversation(std::string prompt, std::string response, std::string username){
    if(this->filename.empty()){
        return;
    }
    
    std::ofstream history(this->filename, std::ios::app);
    if(!history.is_open()){
        std::cout << "Failed to open file. History couldn't save." << std::endl;
        return;
    }
    std::string str_history = "username: " + username + '~' + prompt + "~" + response + "\n";
    this->prompt.push_back(prompt);
    this->response.push_back(response);
    this->username.push_back(username);
    history << str_history;
    history.close();
}

std::string Conversation::getConversationHistory(){
    std::string str_history = "";
    int sizeofPromptVec = this->prompt.size();
    int sizeofResponseVec = this->response.size();
    int isizeofUsernameVec = this->username.size();
    
    if(sizeofPromptVec == 0 && sizeofResponseVec == 0){
        return "No history";
    }
    
    // Get last 7 conversations (or available count)
    int historyCount = (sizeofPromptVec < 7) ? sizeofPromptVec: 7;
    
    for(int i = 0; i < historyCount; i++){
        int index = sizeofPromptVec - 1 - i;  // Correct backward indexing
        str_history += "~~" + this->username[index]+ '~' + this->prompt[index] + "~" + this->response[index] + "\n";
    }

    return str_history;
}