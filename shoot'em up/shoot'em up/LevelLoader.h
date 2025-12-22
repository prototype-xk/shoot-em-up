#pragma once
#include <string>
#include <vector>

struct ScriptCommand {
    float time;
    std::string command;
    std::vector<std::string> params;
};

class LevelLoader {
public:
    static bool loadLevelsOrder(const std::string& path, std::vector<std::string>& outLevels);
    static bool loadLevelScript(const std::string& path, std::vector<ScriptCommand>& outScript);
};