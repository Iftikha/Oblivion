# OBLIVION
```
 ________ __________.____    ._______   ____.___________    _______   
\_____  \\______   \    |   |   \   \ /   /|   \_____  \   \      \  
 /   |   \|    |  _/    |   |   |\   Y   / |   |/   |   \  /   |   \ 
/    |    \    |   \    |___|   | \     /  |   /    |    \/    |    \
\_______  /______  /_______ \___|  \___/   |___\_______  /\____|__  /
        \/       \/        \/                          \/         \/ 
```

[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/Iftikha/Oblivion)
[![Language](https://img.shields.io/badge/language-C++-00599C.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)

> **A lightweight, high-performance AI assistant powered by Google's Gemini API**

Oblivion is a modern C++ application that brings the power of large language models directly to your desktop without the computational overhead of running models locally. Designed with performance and efficiency in mind, it provides seamless AI assistance while maintaining minimal resource usage.

---

## 🌟 Key Features

### Current (v1.0.0)
- **🚀 High Performance**: Runs efficiently on standard hardware without GPU requirements
- **💬 Intelligent Conversations**: Advanced chat system with persistent conversation history  
- **🔗 Cloud-Powered**: Leverages Google's Gemini API for state-of-the-art AI responses
- **💻 Programming-Focused**: Specialized assistance for development tasks and code generation
- **📝 Local History**: Secure conversation storage with quick access to previous interactions
- **⚡ Minimal Footprint**: Lightweight architecture with fast startup times

### 🔮 Roadmap (Coming Soon)
- **🎤 Voice Recognition**: Natural speech input processing
- **🔊 Text-to-Speech**: AI responses with voice output
- **🔐 Secure Authentication**: User account management and data protection
- **⚙️ System Integration**: Execute system commands through natural language
- **🐧 Cross-Platform**: Linux and macOS support

---

## 🛠️ Technology Stack

| Component | Technology | Purpose |
|-----------|------------|---------|
| **Core Language** | C++17 | High-performance system programming |
| **AI Provider** | Google Gemini API | Advanced language model capabilities |
| **HTTP Client** | libcurl | Secure API communication |
| **JSON Processing** | nlohmann/json | Efficient data parsing |
| **Build System** | Make | Cross-platform compilation |

---

## 📦 Installation

### Prerequisites
- **Operating System**: Windows 10/11 (Linux support in development)
- **Compiler**: GCC with C++17 support
- **Internet Connection**: Required for API communication
- **Gemini API Key**: [Get your free key here](https://makersuite.google.com/app/apikey)

### Step-by-Step Setup

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Iftikha/Oblivion.git
   cd Oblivion
   ```

2. **Install libcurl**
   - Download from [curl.se/windows](https://curl.se/windows/)
   - Extract to `C://libcurl/`
   - Copy DLL files to project directory

3. **Configure API Key**
   To configure API-KEY, create .env and paste the following:
   ```bash
   API_KEY=PASTE YOUR API KEY HERE
   ```

4. **Create Required Directories**
   ```bash
   mkdir history
   touch history/history.txt
   ```

5. **Build and Run**
   ```bash
   make build
   make run
   ```

---

## 🚀 Quick Start

```cpp
// Initialize Oblivion
Engine engine;
engine.init("your-api-key-here");

// Start conversation
cout << "You: Hello Oblivion";
string response = engine.sendRequest("Hello Oblivion");
cout << "Oblivion: " << response << endl;
```

### Sample Interaction
```
=== OBLIVION - AI Assistant ===
Model: gemini-pro
Welcome, User!

You: Write a Python function to reverse a string
Oblivion: Processing...
Oblivion: Here's a simple Python function to reverse a string:

def reverse_string(text):
    return text[::-1]

    Example usage
result = reverse_string("Hello World")
print(result)  # Output: dlroW olleH

You: /bye   // To quit the application
```

---

## 📁 Project Structure

```
Oblivion/
|
├── main.cpp               # Application entry point
├── include/
│   ├── json.hpp             # JSON library (included)
│   ├── Conversation.hpp     # History class header
│   ├── Conversation.cpp      # History management
│   ├── Engine.hpp          # Engine class header
│   └── Engine.cpp           # API communication
├── history/
│   └── history.txt          # Conversation storage
├── Makefile                 # Build configuration
└── README.md               # This file
```

---

## 🔧 Configuration

### API Configuration
- **Model Selection**: Supports `gemini-pro`, `gemini-2.0-flash`
- **Response Length**: Configurable token limits
- **History Size**: Adjustable conversation memory

---

## 🤝 Contributing

We welcome contributions!

### Development Guidelines
- Follow C++17 standards
- Include unit tests for new features
- Update documentation for API changes
- Ensure cross-platform compatibility

---

## 📊 Performance Metrics

| Metric | Value |
|--------|-------|
| **Startup Time** | < 100ms |
| **Memory Usage** | ~15MB RAM |
| **Response Time** | 1-3 seconds (API dependent) |
| **Storage** | < 1MB per 1000 conversations |

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 👨‍💻 Developer

**Iftikhar Ahmed**  
*Software Engineer & AI Enthusiast*

- GitHub: [@Iftikha](https://github.com/Iftikha)
- Email: iftikhar.ahmed.pakistan.2005@gmail.com

---

## 🙏 Acknowledgments

- **Google LLC** - For the powerful Gemini API
- **nlohmann** - For the excellent JSON library
- **curl team** - For reliable HTTP client library
- **C++ Community** - For continuous language improvements

---

## 📈 Version History

### v1.0.0 (Current)
- ✅ Core chat functionality
- ✅ Conversation history
- ✅ Gemini API integration
- ✅ Windows support

### v1.1.0 (Planned)
- 🔄 Voice input/output
- 🔄 System command execution
- 🔄 Enhanced security

### v2.0.0 (Future)
- 🔄 Cross-platform support
- 🔄 Plugin architecture
- 🔄 Advanced automation

---

<p align="center">
  <strong>⭐ Star this repository if you find it helpful!</strong>
</p>

<p align="center">
  Made with ❤️ and C++
</p>
