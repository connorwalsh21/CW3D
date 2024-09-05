// gameobject.hpp
#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp> // glm::vec3 for 3D positions

class GameObject {
private:
    std::string type;
    glm::vec3 position; // Using glm::vec3 for positions
    glm::vec3 size;
    float radius;
    std::string texture;
    glm::vec3 color;
    glm::vec3 scale;

public:
    // Constructors, getters, and setters
    GameObject() = default;

    // Setters
    void setType(const std::string& type) { this->type = type; }
    void setPosition(const glm::vec3& pos) { this->position = pos; }
    void setSize(const glm::vec3& size) { this->size = size; }
    void setRadius(float radius) { this->radius = radius; }
    void setTexture(const std::string& texture) { this->texture = texture; }
    void setColor(const glm::vec3& color) { this->color = color; }
    void setScale(const glm::vec3& scale) { this->scale = scale; }

    // Getters
    std::string getType() const { return type; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getSize() const { return size; }
    float getRadius() const { return radius; }
    std::string getTexture() const { return texture; }
    glm::vec3 getColor() const { return color; }
    glm::vec3 getScale() const { return scale; }
};
