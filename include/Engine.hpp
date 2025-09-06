#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

class Engine{
    public:
        Engine(std::string API_KEY, std::string gemini_model = "gemini-2.0-flash");

        void init();
        std::string sendRequest(std::string prompt);

    private:
        std::string GEMINI_API_KEY;
        std::string GEMINI_MODEL;
        bool is_init;

        std::string parseResponse(const std::string& response);
};

#endif