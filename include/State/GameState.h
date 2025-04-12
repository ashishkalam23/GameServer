#pragma once

#include "../Parser/GameData.h"
#include "../Parser/PrimitiveTypes.h"
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <functional>

using std::shared_ptr;

// TODO: Work on initialize methods to take in Map

class GameState
{
public:
    // Constructor
    GameState() {}
    GameState(const GameData &gameData);

    // Global variables methods
    void initializeGlobalVariables(const Map &variablesData);
    void initializeGlobalConstants(const Map &constantsData);
    void setGameVariable(const std::string &key, const Value &value); // Remove it for production
    std::optional<Value*> getGameVariable(const std::string &key);
    void resetGlobalVariable(const std::string &key);

    // Player-specific variables
    void initializePlayers(const Map &playersData);
    void addPlayer(const std::string &playerName);
    void removePlayer(const std::string &playerName);
    // void setPlayerVariable(const std::string &playerName,
    //  const std::string &key, const Value &value);
    // std::optional<Value> getPlayerVariable(const std::string &playerName,
    //  const std::string &key) const;
    Value* getPlayerVariablesList();

    // Utility methods
    // std::vector<std::string> getActivePlayers() const;
    // void resetAllPlayerVariables(const std::string &key);

private:
    // Map for game variables
    Map gameVariables;

    Map gameConstants;

    Map playerBluePrintState; // Used a blueprint to initialize each player's
                              // state.

    // list of players
    Value playersState;
};