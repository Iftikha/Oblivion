#include "./Engine.hpp"
#include <curl/curl.h>
#include "./json.hpp"
#include <iostream>
using json = nlohmann::json;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

Engine::Engine( std::string API_KEY, std::string gemini_model ){
    this->GEMINI_API_KEY = API_KEY;
    this->GEMINI_MODEL = gemini_model;
    this->is_init = false;
}

void Engine::init(){
    std::cout << "Initializing Engine.." << std::endl;
    std::cout << "Selected Model: " << this->GEMINI_MODEL << std::endl;
    this->is_init = true;
}

std::string Engine::sendRequest(std::string prompt){
    if(!is_init){
        return "Error: Engine not initialized! Please run init() first!";
    }
    
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    
    if(!curl){
        return "Error: Failed to initialize CURL";
    }
    
    std::string url = "https://generativelanguage.googleapis.com/v1beta/models/" + this->GEMINI_MODEL + ":generateContent";
    
    json request_body;
    request_body["contents"][0]["parts"][0]["text"] = prompt;
    std::string json_string = request_body.dump();
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string secondHeader = "X-goog-api-key: " + this->GEMINI_API_KEY;
    headers = curl_slist_append(headers, secondHeader.c_str());
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
    // Enable verbose mode for debugging (optional)
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    // For testing only (disable SSL verification)
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
   res = curl_easy_perform(curl);

    if(res != CURLE_OK) {
        std::string error = "CURL Error: " + std::string(curl_easy_strerror(res));
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return error;
    }

    // HTTP response
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if(http_code != 200) {
        return "HTTP Error: " + std::to_string(http_code) + " - " + readBuffer;
    }

    if(readBuffer.empty()) {
        return "Error: Empty response from API";
    }

    return parseResponse(readBuffer);

}

std::string Engine::parseResponse(const std::string& response){
    try{
        // Debug: Print the raw response
        // std::cout << "Raw response: " << response << std::endl;
        
        json parsed = json::parse(response);
        
        if(parsed.contains("candidates") && !parsed["candidates"].empty()){
            return parsed["candidates"][0]["content"]["parts"][0]["text"];
        }else if(parsed.contains("error")){
            return "API Error: " + parsed["error"]["message"].get<std::string>();
        }
    }catch(json::exception& e){
        return "JSON Parse Error: " + std::string(e.what()) + "\nResponse: " + response;
    }
    return "No valid response received!";
}
