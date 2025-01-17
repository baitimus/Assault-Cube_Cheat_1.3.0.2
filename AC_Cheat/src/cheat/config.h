#pragma once
#include "pch.h"


struct Config {
    static Config& Instance();
    bool espEnabled = true;    
    bool aimbotEnabled = true; 
    bool cheatRunning = true;
	bool fovEnabled = true;
    int fovAimbotSize = 25;

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
