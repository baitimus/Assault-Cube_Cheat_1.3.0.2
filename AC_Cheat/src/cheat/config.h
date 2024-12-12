#pragma once
#include "pch.h"


struct Config {
    static Config& Instance();
    bool espEnabled = true;    // Default value can be true or false
    bool aimbotEnabled = true; // Default value can be true or false

    // You can add other settings here as needed
};

class ConfigManager {
public:
    static Config& Instance() {
        static Config instance; // Create a static instance
        return instance;
    }

private:
    ConfigManager() = default; // Private constructor to prevent instantiation
    ConfigManager(const ConfigManager&) = delete; // Delete copy constructor
    ConfigManager& operator=(const ConfigManager&) = delete; // Delete assignment operator
};
