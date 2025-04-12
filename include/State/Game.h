#pragma once

#include "Audience.h"
#include "../Network/Server.h"
#include "Player.h"

#include <vector>
#include <string>
#include <variant>

using std::vector, std::variant;
using networking::Connection;

class Game {
public:
    Game(const std::string& displayName, int totalRounds, Connection creatorConn, const std::string& creatorName);

    // Game management
    void startGame();
    bool advanceRound();
    void endGame();
    bool isGameOver() const;

    // Player and audience management
    void addPlayer(Connection conn, const std::string& name);
    void addAudience(Connection conn, const std::string& name);
    bool removePlayer(Connection conn);
    bool removeAudience(Connection conn);

    // Host management
    void setHost(Connection conn, const std::string& name, bool isPlayer = true);
    std::string getHostName() const;
    bool isHost(const Connection& conn) const;

    // Accessors
    std::string getDisplayName() const;
    int getGameID() const { return gameID; }

    // Score management
    void updatePlayerScore(Connection conn, int score);
    int getPlayerScore(Connection conn) const;

    void broadcastMessage(const std::string& message);
    void displayLeaderboard() const;
    Player* findPlayerByName(const std::string& name);
    bool checkForWinner(int winningScore);

private:
    // Internal helpers
    bool canAdvanceRound() const;

    // Members
    vector<Audience> auds;
    vector<Player> plyrs;
    std::string name;
    int rounds;
    int curRound;
    int gameID;  // Unique identifier for the game
    bool gameStarted;

    // Host management
    variant<Player, Audience> host;  // Host can be either a Player or Audience
    bool isHostPlayer;  // True if the host is a player, false if it's an audience member
};