#pragma once
#include "pch.h"
#include "../imgui/imgui.h"

struct Config {
    static Config& Instance();
    bool espEnabled = true;    
    bool aimbotEnabled = true; 
    bool cheatRunning = true;
	bool fovEnabled = true;
    int fovAimbotSize = 25;
	bool animationEnabled = true;
	float animationSpeed = 1.0f;
    ImVec4 menuColor = ImVec4(0.2f, 0.4f, 0.8f, 1.0f);       // Default menu color
    ImVec4 animationColor = ImVec4(0.8f, 0.8f, 1.0f, 0.7f);  // Default animation color
};

class ConfigManager {
public:
    static Config& Instance() {
        static Config instance; 
        return instance;
    }

private:
    ConfigManager() = default; // Private constructor to prevent instantiation
    ConfigManager(const ConfigManager&) = delete; // Delete copy constructor
    ConfigManager& operator=(const ConfigManager&) = delete; // Delete assignment operator
};
