
#include "kcommon.h"

std::optional<std::string> loadTextFile(const std::string fileName)
{
    // SPDLOG_INFO("LoadTextFile {}", fileName); 
    std::ifstream fin(fileName); 
    if(!fin.is_open()){
        SPDLOG_INFO("failed to open file {}", fileName); 
        return {}; 
    }
    std::stringstream text; 
    text << fin.rdbuf(); 
    return text.str(); 
}