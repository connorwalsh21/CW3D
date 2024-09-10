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

    void generateFile() {
        const int size = 512; // This is the constant to change (Must be a perfect cube root)
        glm::vec3 cubePositions[size];
        std::vector<GameObject> gameObjects;
        int count = 0;

        // Generate positions
        for (int i = 0; i < cbrt(size); i++) {
            for (int j = -1; j > -1 - cbrt(size); j--) {
                for (int k = 0; k < cbrt(size); k++) {
                    if (i == 0 || i == cbrt(size) - 1 || j == -1 || j == -cbrt(size) || k == 0 || k == cbrt(size)) {
                        glm::vec3 position = glm::vec3(i, j, k);
                        cubePositions[count] = position;

                        // Create a GameObject instance
                        GameObject cube;

                        // Use setters to assign values
                        cube.setType("cube");
                        cube.setPosition(position);
                        cube.setSize(glm::vec3(1.0f, 1.0f, 1.0f)); // Default size (can be modified)
                        cube.setRadius(0.5f);                      // Example value, not used for cubes
                        cube.setTexture("container2.png");    // Default texture
                        cube.setColor(glm::vec3(1.0f, 1.0f, 1.0f)); // Default color (white)
                        cube.setScale(glm::vec3(1.0f, 1.0f, 1.0f)); // Default scale

                        // Add to the gameObjects vector
                        gameObjects.push_back(cube);
                    }
                    count++;
                }
            }
        }

        nlohmann::json jsonObjects;

        // Convert each GameObject to JSON and add to the JSON array
        for (const auto& gameObject : gameObjects) {
            jsonObjects["gameObjects"].push_back(gameObject);
        }

        // Write the JSON data to a file
        
        std::ofstream file("level2.json");
        if (file.is_open()) {
            file << jsonObjects.dump(4); // Pretty-print with indentation of 4 spaces
            file.close();
            std::cout << "Game objects saved to " << "level2.json" << " successfully.\n";
        }
        else {
            std::cerr << "Error: Could not open file " << "level2.json" << " for writing.\n";
        }


    }
};
