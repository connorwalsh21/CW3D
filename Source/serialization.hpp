// serialization.hpp
#pragma once
#include <nlohmann/json.hpp>
#include "gameobject.hpp"

// Convert GameObject to JSON
inline void to_json(nlohmann::json& j, const GameObject& obj) {
    j = nlohmann::json{
        {"type", obj.getType()},
        {"position", {obj.getPosition().x, obj.getPosition().y, obj.getPosition().z}},
        {"size", {obj.getSize().x, obj.getSize().y, obj.getSize().z}},
        {"radius", obj.getRadius()},
        {"texture", obj.getTexture()},
        {"color", {obj.getColor().x, obj.getColor().y, obj.getColor().z}},
        {"scale", {obj.getScale().x, obj.getScale().y, obj.getScale().z}}
    };
}

// Convert JSON to GameObject
inline void from_json(const nlohmann::json& j, GameObject& obj) {
    obj.setType(j.at("type").get<std::string>());
    obj.setPosition(glm::vec3(j.at("position")[0], j.at("position")[1], j.at("position")[2]));
    obj.setSize(glm::vec3(j.at("size")[0], j.at("size")[1], j.at("size")[2]));
    obj.setRadius(j.at("radius").get<float>());
    obj.setTexture(j.at("texture").get<std::string>());
    obj.setColor(glm::vec3(j.at("color")[0], j.at("color")[1], j.at("color")[2]));
    obj.setScale(glm::vec3(j.at("scale")[0], j.at("scale")[1], j.at("scale")[2]));
}
