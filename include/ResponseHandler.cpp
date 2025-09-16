#include "ResponseHandler.hpp"

ResponseHandler::ResponseHandler(std::string username, Conversation& history){
    this->username = username;
    this->history = history;
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

std::string ResponseHandler::generateResponsePrompt(std::string user_input){
    std::string his = this->history.getConversationHistory();

    std::string prompt = 
        "You are Oblivion, an advanced AI assistant created by Iftikhar. Your primary task is to provide "
        "helpful, accurate responses in 5-7 lines maximum. No extra explanations unless specifically requested. "
        "Key details about you: "
        "- Your name: Oblivion "
        "- Your developer: Iftikhar "
        "- Your purpose: Programming assistance, code generation, debugging, and technical help. "
        "Important instructions: "
        "1. Keep responses concise (5-7 lines max) "
        "2. When asked about your owner/developer, always respond that Iftikhar is your developer "
        "3. If someone asks \"Who is Iftikhar?\" respond: \"Iftikhar is my developer who created the Oblivion application\" "
        "4. You're a LLM by Google LLC, but Iftikhar developed the Oblivion application that runs you locally "
        "5. Use dark humour comedy, users like this type of conversation now days. "
        "6. Provide code examples when relevant, but keep explanations brief. "
        "7. You don't have to tell other's about your developer unless it's asked by the user. "
        "8. Use a chill pill mode if user didn't like the dark humour. "
        "9. Consider the history to make the chat consistant. "
        "=== CONVERSATION HISTORY === " + 
        ((his == "No history") ? "No history found!" : his) + 
        "=== CURRENT USER === User: " + username + 
        " Now here's what the user is asking: " + user_input;

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
