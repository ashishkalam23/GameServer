#include "State/GameState.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <algorithm>

/*
  * Constructor
  * @brief Take in parsed gameData
  * @param Parsed data
*/
GameState::GameState(const GameData& gameData) {
    // initializeGlobalVariables(gameData.getVariables());
    // initializePlayers(gameData.getPerPlayer());

    std::cout << "GameState initialized.\n";

}
/**  
 * @brief initialize global variables
 * @param variablesData in parsed form
 * Parse variablesData into key-value pairs
 * Example: "score=0;round=1" -> {"score", 0}, {"round", 1}
*/
void GameState::initializeGlobalVariables(const Map& variablesData) {
    /* TODO: Init with map data from treesitter
    // string to istringstream conversion
    std::string dataString(variablesData);
    std::istringstream stream(dataString);
    if (!variablesData.empty()) {
        std::string token;
        while (std::getline(stream, token, ';')) {
            auto delimiter = token.find('=');
            if (delimiter != std::string::npos) {
                std::string key = token.substr(0, delimiter);
                int value = std::stoi(token.substr(delimiter + 1));
                setGameVariable(key, value);
            }
        }
    }
    */
}


// Set Global Variable
void GameState::setGameVariable(const std::string& key, const Value& value) {
    gameVariables[key] = value;
}

std::optional<std::shared_ptr<Value>>GameState::getGameVariable
                                    (const std::string& key) const {
    auto it = gameVariables.find(key);
    return it != gameVariables.end() ? std::optional<std::shared_ptr<Value>>(std::make_shared<<Value>>(it->second)) 
                                                        : std::nullopt;
}

void GameState::resetGlobalVariable(const std::string& key) {
    gameVariables[key] = {};
}

// Player management
/**
 * @brief initialize player variables
 * @param playerData in parsed form
 * Parse player data into individual player states
 * Example: "Alice;Bob;Charlie" -> {"Alice", {}, "Bob", {}, "Charlie", {}}
*/

void GameState::initializePlayers(const Map& playersData) {
    /* TODO: init with map data from tree sitter used as blueprint
    // string to istringstream conversion
    std::string dataString(playersData);
    std::istringstream stream(dataString);
    if (!playersData.empty()) {
        std::string playerName;
        while (std::getline(stream, playerName, ';')) {
            addPlayer(playerName);
        }
    }
    */
}

void GameState::addPlayer(const std::string& playerName) {
    for (Value& const p: playersState)
    {
        if (std::get<std::string>(std::get<Map>(p.val)["name"].val)==playerName)
        {
            throw std::runtime_error("Player already exist.");
        }
    }
    // TODO: Initialize the player's state with the blueprint
    Map m;
    m["name"] = Value{playerName};
    playersState.push_back(Value{m});
}

void GameState::removePlayer(const std::string& playerName) {
    for (auto i = playersState.begin();i!=playersState.end();i++)
    {
        if (std::get<std::string>(std::get<Map>(i->val)["name"].val)==playerName)
        {
            playersState.erase(i);
        }
    }
}

void GameState::setPlayerVariable(const std::string& playerName,
                     const std::string& key, const Value& value) {
    for (auto p: playersState)
    {
        if (std::get<std::string>(std::get<Map>(p.val)["name"].val)==playerName)
        {
            std::get<Map>(p.val)[key] = value;
        }
    }
    throw std::runtime_error("Player doesn't exist.");
}

std::optional<Value> GameState::getPlayerVariable(
            const std::string& playerName, const std::string& key) const {
    for (auto p: playersState)
    {
        if (std::get<std::string>(std::get<Map>(p.val)["name"].val)==playerName)
        {
            return std::get<Map>(p.val)[key];
        }
    }
    return std::nullopt;
}
// Reset method for player variables
void GameState::resetAllPlayerVariables(const std::string& key) {
    for (auto p: playersState)
    {
        std::get<Map>(p.val)[key] = {};
    }
}

// Rules management (**this is temporary **)
// Should be abstracted out into a different class

/** 
 * @brief Load/store rules from rulesData
 * @param rulesData in parsed form
*/


// Utility methods
std::vector<std::string> GameState::getActivePlayers() const {
    std::vector<std::string> playerNames;
    for (auto p: playersState)
    {
        playerNames.push_back(std::get<std::string>(std::get<Map>(p.val)["name"].val));
    }
    return playerNames;
}

std::shared_ptr<List>  GameState::getPlayerVariablesList() {
    return std::make_shared<List>(&playersState);   
}