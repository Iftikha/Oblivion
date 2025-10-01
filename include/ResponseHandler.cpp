#include <chrono>
#include <thread>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <curl/curl.h>
#include "ResponseHandler.hpp"
#include "json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

struct WavHeader {
    char chunkID[4] = {'R', 'I', 'F', 'F'};
    uint32_t chunkSize;
    char format[4] = {'W', 'A', 'V', 'E'};
    char subchunk1ID[4] = {'f', 'm', 't', ' '};
    uint32_t subchunk1Size = 16;
    uint16_t audioFormat = 1; // PCM
    uint16_t numChannels = 1; // Mono
    uint32_t sampleRate = 24000; // API might return this, or you need to know it
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample = 16; // A common value
    char subchunk2ID[4] = {'d', 'a', 't', 'a'};
    uint32_t subchunk2Size;
};


ResponseHandler::ResponseHandler(std::string username, Conversation& history){
    this->username = username;
    this->history = history;
    this->API_KEY = "";
    this->BASEURL = "";
    this->audioOutputDir = "";
}

ResponseHandler::ResponseHandler(std::string API_KEY){
    this->username = "";
    this->API_KEY = API_KEY;
    this->BASEURL = "https://generativelanguage.googleapis.com/v1beta";
    this->audioOutputDir = "audio_output";
    curl_global_init(CURL_GLOBAL_DEFAULT);
    initializeDirectory();
}


std::string ResponseHandler::generateCommandPrompt(std::string user_input){
     std::string prompt = 
        "You are a command generator. Convert the user's request into a valid Windows command. "
        "If the request is about browsing/searching, convert it to a browser launch with proper URL. "
        "Examples:\n"
        "User: search tilawat on youtube → Output: start chrome \"https://www.youtube.com/results?search_query=tilawat\"\n"
        "User: open facebook → Output: start chrome \"https://facebook.com\"\n"
        "User: shutdown → Output: shutdown /s /t 0\n"
        "User: restart → Output: shutdown /r /t 0\n"
        "If you cannot generate any command, return 'INVALID_COMMAND'.\n"
        "User input: " + user_input;

    return trim(prompt);
}

std::string ResponseHandler::generateResponsePrompt(std::string user_input) {
    std::string his = this->history.getConversationHistory();
    // NOTE: getCondensedJSON should return last 10 records in compact JSON format

    std::string prompt =
        "You are Oblivion, an AI assistant by Iftikhar.\n"
        "Rules:\n"
        "- Keep replies concise (5-7 lines).\n"
        "- No greetings unless user greets first.\n"
        "- Add code only if useful.\n"
        "- Dark humour allowed, but stop if disliked.\n"
        "- If asked, always say your developer is Iftikhar.\n"
        "Context:\n"
        "- Use only recent history for consistency.\n"
        "- History is JSON, last message is most important.\n"
        "=== HISTORY(JSON) ===\n" +
        ((his == "No history") ? "[]" : his) +
        "\n=== USER(" + username + ") === " + user_input;

    return trim(prompt);
}


std::string ResponseHandler::trim(std::string text) {
    // Trim leading/trailing whitespace & quotes
    size_t start = text.find_first_not_of(" \n\r\t\"");
    size_t end   = text.find_last_not_of(" \n\r\t\"");
    std::string cleaned = (start == std::string::npos) ? "" : text.substr(start, end - start + 1);

    // Remove prefix "Output:" if present
    std::string prefix = "Output:";
    if (cleaned.rfind(prefix, 0) == 0) {
        cleaned = cleaned.substr(prefix.length());
        size_t s = cleaned.find_first_not_of(" \n\r\t\"");
        size_t e = cleaned.find_last_not_of(" \n\r\t\"");
        cleaned = (s == std::string::npos) ? "" : cleaned.substr(s, e - s + 1);
    }

    // ✅ Fix missing closing quote if unbalanced
    size_t firstQuote = cleaned.find('"');
    size_t lastQuote  = cleaned.rfind('"');
    if (firstQuote != std::string::npos && lastQuote == firstQuote) {
        // only one quote found → add closing quote
        cleaned += '"';
    }

    return cleaned;
}


size_t ResponseHandler::WriteCallBack(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

void ResponseHandler::initializeDirectory(){
    if(!fs::exists(audioOutputDir)){
        fs::create_directory(audioOutputDir);
        std::cout << "Audio Output Directory Created Successfully!" << std::endl;
    }
}

std::string ResponseHandler::cleanTextForTTS(const std::string& text){
    std::string cleanText = text;
    size_t pos = 0;
    while((pos = cleanText.find("**", pos)) != std::string::npos){
        cleanText.replace(pos, 1, ". ");
        pos += 2;
    }

    pos = 0;
    while((pos = cleanText.find("  ", pos)) != std::string::npos){
        cleanText.replace(pos, 2, " ");
    }

    if(cleanText.length() > 1000){
        cleanText = cleanText.substr(0, 997) + "...";
    }

    return cleanText;
}

std::string ResponseHandler::generateAudioFilename(){
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);

    std::ostringstream oss;
    oss << audioOutputDir << "/audio_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".wav";

    return oss.str();
}

ResponseHandler::~ResponseHandler(){
    curl_global_cleanup();
}

std::string ResponseHandler::generateAudio(std::string response, const std::string& voice){
    if(response.empty()){ 
        return "Empty response passed!"; 
    }
    response = "Read aloud and fast, with calm but humourus voice: " + response; 
    CURL* curl; 
    CURLcode res; 
    std::string readBuffer; 
    curl = curl_easy_init(); 
    if(!curl){ 
        return "Error: Failed to initialize CURL!";
    } 
    std::string url = BASEURL + "/models/gemini-2.5-flash-preview-tts:generateContent"; 
    json request_body; 
    request_body["contents"][0]["parts"][0]["text"] = response; 
    request_body["generationConfig"]["responseModalities"] = {"AUDIO"}; 
    request_body["generationConfig"]["speechConfig"]["voiceConfig"]["prebuiltVoiceConfig"]["voiceName"] = voice; 
    std::string jsonString = request_body.dump(); 
    struct curl_slist *headers = NULL; 
    headers = curl_slist_append(headers, "Content-Type: application/json"); 
    std::string apiKeyHeader = "x-goog-api-key: " + this->API_KEY; 
    headers = curl_slist_append(headers, apiKeyHeader.c_str()); 
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str()); 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack); 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); 
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); 
    res = curl_easy_perform(curl); 
    if(res != CURLE_OK){ 
        curl_slist_free_all(headers); 
        curl_easy_cleanup(curl); 
        return "CURL Error: " + std::string(curl_easy_strerror(res)); 
    } 
    long http_code = 0; 
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code); 
    curl_slist_free_all(headers); 
    curl_easy_cleanup(curl); 
    if(http_code != 200){ 
        return "HTTP Error: " + std::to_string(http_code) + " - " + readBuffer; 
    } 
    if(readBuffer.empty()){ 
        return "Error: Empty response from API!"; 
    } 
    try{ 
        json response_json = json::parse(readBuffer); 
        std::string base64_audio = response_json["candidates"][0]["content"]["parts"][0]["inlineData"]["data"]; 
        std::string audio_data = base64_decode(base64_audio); 
        if (audio_data.empty()) {
            std::cerr << "[DEBUG] Decoded audio data is empty!" << std::endl; 
            return "Error: Decoded audio data is empty!"; 
        } 
        WavHeader header; 
        header.chunkSize = 36 + audio_data.size(); 
        header.byteRate = header.sampleRate * header.numChannels * (header.bitsPerSample / 8); 
        header.blockAlign = header.numChannels * (header.bitsPerSample / 8); 
        header.subchunk2Size = audio_data.size(); 
        // Ensure output directory exists 
        if (!fs::exists(audioOutputDir)) { 
            try { fs::create_directory(audioOutputDir); 
                std::cout << "[DEBUG] Created audio output directory: " << audioOutputDir << std::endl; 
            } catch (const std::exception& e) { 
                std::cerr << "[DEBUG] Failed to create audio output directory: " << e.what() << std::endl; 
                return "Error: Could not create audio output directory!"; 
            } 
        } 
        std::string temp_filename = generateAudioFilename(); 
        std::ofstream out_file(temp_filename, std::ios::binary); 
        if(!out_file){ 
            std::cerr << "[DEBUG] Could not open file for writing: " << temp_filename << std::endl; 
            return "Error: Could not create audio file!"; 
        } 
        out_file.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader)); 
        out_file.write(audio_data.data(), audio_data.size()); 
        out_file.close(); 
        return temp_filename; 
    }catch(const std::exception& e){ 
        std::cerr << "[DEBUG] Exception: " << e.what() << std::endl; 
        return "Error processing TTS response: " + std::string(e.what()); 
    } 
}
    

void ResponseHandler::playAudio(std::string filename){
     // Check file size after writing
        std::ifstream check_file(filename, std::ios::binary | std::ios::ate);
        auto file_size = check_file.tellg();
        check_file.close();
        if (file_size <= 0) {
            std::cerr << "[DEBUG] Written audio file is empty: " << filename << std::endl;
            return;
        }

        // PlaySound only supports WAV files. Warn if not .wav
        if (filename.find(".wav") == std::string::npos) {
            std::cerr << "[DEBUG] PlaySound only supports WAV files. File: " << filename << std::endl;
            std::cout << "Warning: PlaySound only supports WAV files. File saved as: " << filename << std::endl;
            return;
        }

        if (!PlaySound(filename.c_str(), NULL, SND_FILENAME | SND_ASYNC)) {
            std::cerr << "[DEBUG] PlaySound failed for file: " << filename << std::endl;
            return;
        }
}


std::string ResponseHandler::base64_decode(const std::string& in){
    std::string out;
    std::vector<int> T(256, -1);
    for(int i = 0; i < 64; i++){
        T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    }

    int val = 0, valb = -8;
    for(unsigned char c : in){
        if(T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if(valb >= 0){
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }

    return out;
}
