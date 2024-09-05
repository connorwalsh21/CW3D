// levelmanager.hpp
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "gameobject.hpp"
#include "serialization.hpp"

class LevelManager {
private:
    std::vector<GameObject> gameObjects;

public:
    // Load GameObjects from a JSON file
    void loadFromFile(const std::string& filename) {
        std::filesystem::path filePath = std::filesystem::current_path() / "resources" / "levels" / filename;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        nlohmann::json j;
        file >> j;
        file.close();

        gameObjects.clear();
        if (j.contains("gameObjects") && j["gameObjects"].is_array()) {
            for (const auto& item : j["gameObjects"]) {
                GameObject obj = item.get<GameObject>();
                gameObjects.push_back(obj);
            }
        }
        else {
            throw std::runtime_error("Invalid JSON format in file: " + filename);
        }
    }

    // Save GameObjects to a JSON file
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        nlohmann::json j;
        j["gameObjects"] = gameObjects;  // Directly serialize the vector of GameObjects
        file << j.dump(4);  // Pretty print with an indentation of 4 spaces
        file.close();
    }

    // Display the loaded game objects
    void displayGameObjects() const {
        for (const auto& obj : gameObjects) {
            std::cout << "Type: " << obj.getType() << "\n"
                << "Position: " << obj.getPosition().x << " " << obj.getPosition().y << " " << obj.getPosition().z << "\n"
                << "Size: " << obj.getSize().x << " " << obj.getSize().y << " " << obj.getSize().z << "\n"
                << "Radius: " << obj.getRadius() << "\n"
                << "Texture: " << obj.getTexture() << "\n"
                << "Color: " << obj.getColor().x << " " << obj.getColor().y << " " << obj.getColor().z << "\n"
                << "Scale: " << obj.getScale().x << " " << obj.getScale().y << " " << obj.getScale().z << "\n\n";
        }
    }

    // Get the list of loaded game objects
    const std::vector<GameObject>& getGameObjects() const {
        return gameObjects;
    }
};
