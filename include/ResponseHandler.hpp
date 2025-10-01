#ifndef RESPONSEHANDLER_HPP
#define RESPONSEHANDLER_HPP

#include <iostream>
#include <windows.h>
#include "Conversation.hpp"

class ResponseHandler{
    public:
        ResponseHandler(std::string username, Conversation& history);
        ResponseHandler(std::string API_KEY);
        ~ResponseHandler();

        std::string generateCommandPrompt(std::string user_input);
        std::string generateResponsePrompt(std::string user_input);

        // bool isValidCommandResponse(std::string commandResponse);
        std::string trim(std::string text);

        std::string generateAudio(std::string response, const std::string& voice = "Sadachbia");

        void playAudio(std::string filename);

    private:
        std::string username;
        Conversation history;
        std::string API_KEY;
        std::string BASEURL;
        std::string audioOutputDir;

        static size_t WriteCallBack(void* contents, size_t size, size_t nmemb, std::string* userp);

        void initializeDirectory();

        std::string cleanTextForTTS(const std::string& text);

        std::string generateAudioFilename();
        std::string base64_decode(const std::string& in);

};

#endif