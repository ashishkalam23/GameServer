#include "GameState.h"
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

std::optional<Value*>GameState::getGameVariable
                                    (const std::string& key) {
    auto it = gameVariables.find(key);
    return it != gameVariables.end() ? std::optional<Value*>(&(it->second))
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
    for (auto& p: std::get<List>(playersState.val))
    {
        Map pMap = std::get<Map>(p.val);
        if (std::get<std::string>(std::get<Literal>((pMap["name"]).val))==playerName)
        {
            throw std::runtime_error("Player already exist.");
        }
    }
    // TODO: Initialize the player's state with the blueprint
    Map m;
    m["name"] = Value{playerName};
    std::get<List>(playersState.val).push_back(Value{m});
}

void GameState::removePlayer(const std::string& playerName) {
    auto list = std::get<List>(playersState.val);
    for (auto i = list.begin();i!=list.end();i++)
    {
        Map pMap = std::get<Map>(i->val);
        if (std::get<std::string>(std::get<Literal>((pMap["name"]).val))==playerName)
        {
            list.erase(i);
        }
    }
}

Value* GameState::getPlayerVariablesList() {
    return &playersState;   
}