#include "State/Game.h"
#include "State/Player.h"
#include "State/Audience.h"
#include <algorithm>  // For std::find_if
#include <iostream>   // For logging
#include <random> // For generating room id

#define GAMEID_RANGE_MAX 10000000

// Constructor initializes the game with display name, total rounds, and the host
Game::Game(const std::string& displayName, int totalRounds, Connection creatorConn, const std::string& creatorName)
    : name(displayName), rounds(totalRounds), curRound(0), gameStarted(false), isHostPlayer(true), host(Player(creatorConn, creatorName)) {
    // The initial host is set as a Player by default
    std::random_device rd;   // Seed generator
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<> distr(1, GAMEID_RANGE_MAX);
    gameID = distr(gen);

}

// Start the game by setting the current round and updating the flag
void Game::startGame() {
    if (!gameStarted) {
        gameStarted = true;
        curRound = 1;
        std::cout << "Game started: " << name << std::endl;
        // Connect all players when the game starts
        for (auto& player : plyrs) {
            player.connect();
        }
    }
    else {
        std::cerr << "Game has already started!" << std::endl;
    }
}

// Advance to the next round only if the game is started
bool Game::advanceRound() {
    if (!gameStarted) {
        std::cerr << "Cannot advance round before starting the game!" << std::endl;
        return false;
    }

    if (canAdvanceRound()) {
        curRound++;
        std::cout << "Advanced to round " << curRound << "." << std::endl;

        // Example of interacting with players each round
        broadcastMessage("Round " + std::to_string(curRound) + " has started!");


        return true;
    } else {
        std::cerr << "Cannot advance beyond the total number of rounds." << std::endl;
        return false;
    }
}

// End the game by resetting the current round and setting the flag
void Game::endGame() {
    if (gameStarted) {
        gameStarted = false;
        curRound = 0;
        std::cout << "Game ended: " << name << std::endl;

          for (auto& player : plyrs) {
            player.disconnect();
        }

    } else {
        std::cerr << "Game is not in progress!" << std::endl;
    }
}

// Set host as either a Player or Audience
void Game::setHost(Connection conn, const std::string& name, bool isPlayer) {
    if (isPlayer) {
        host = Player(conn, name);
        isHostPlayer = true;
    } else {
        host = Audience(conn, name);
        isHostPlayer = false;
    }
}

// Get the host's name
std::string Game::getHostName() const {
    return std::visit([](const auto& h) { return h.getName(); }, host);
}

// Check if a connection is the host
bool Game::isHost(const Connection& conn) const {
    return std::visit([&](const auto& h) { return h.getConnection() == conn; }, host);
}

// Check if the game is over based on rounds
bool Game::isGameOver() const {
    return curRound >= rounds;
}

// Add a player by creating a Player object and adding to the vector
void Game::addPlayer(Connection conn, const std::string& name) {
    Player newPlayer(conn, name);
    plyrs.push_back(newPlayer);
    std::cout << "Added player: " << name << std::endl;
}

// Add an audience member by creating an Audience object and adding to the vector
void Game::addAudience(Connection conn, const std::string& name) {
    Audience newAudience(conn, name);
    auds.push_back(newAudience);
}

// Remove a player by finding and erasing from the vector
bool Game::removePlayer(Connection conn) {
    auto it = std::find_if(plyrs.begin(), plyrs.end(), [&](const Player& p) {
        return p.getConnection() == conn;
    });
    if (it != plyrs.end()) {
        plyrs.erase(it);
        return true;
    }
    return false;
}

// Remove an audience member by finding and erasing from the vector
bool Game::removeAudience(Connection conn) {
    auto it = std::find_if(auds.begin(), auds.end(), [&](const Audience& a) {
        return a.getConnection() == conn;
    });
    if (it != auds.end()) {
        auds.erase(it);
        return true;
    }
    return false;
}

// Update the score of a player
void Game::updatePlayerScore(Connection conn, int score) {
    for (auto& player : plyrs) {
        if (player.getConnection() == conn) {
            player.addScore(score);
             std::cout << "Updated score for player " << player.getName() << ": " << player.getScore() << std::endl;
            break;
        }
    }
}

// Get the score of a player
int Game::getPlayerScore(Connection conn) const {
    for (const auto& player : plyrs) {
        if (player.getConnection() == conn) {
            return player.getScore();
        }
    }
    return -1;
}

// Helper to check if advancing the round is possible
bool Game::canAdvanceRound() const {
    return curRound < rounds;
}

// Get the display name of the game
std::string Game::getDisplayName() const {
    return name;
}

void Game::broadcastMessage(const std::string& message) {
    for (auto& player : plyrs) {
        player.sendMessage(message);
    }
}
void Game::displayLeaderboard() const {
    // Copy and sort players by score in descending order
    std::vector<Player> sortedPlayers = plyrs;
    std::sort(sortedPlayers.begin(), sortedPlayers.end(), [](const Player& a, const Player& b) {
        return a.getScore() > b.getScore();
    });

    std::cout << "Leaderboard:\n";
    for (const auto& player : sortedPlayers) {
        std::cout << player.getName() << ": " << player.getScore() << " points\n";
    }
}

Player* Game::findPlayerByName(const std::string& name) {
    for (auto& player : plyrs) {
        if (player.getName() == name) {
            return &player;
        }
    }
    return nullptr;
}

bool Game::checkForWinner(int winningScore) {
    for (const auto& player : plyrs) {
        if (player.getScore() >= winningScore) {
            std::cout << player.getName() << " has won the game with " << player.getScore() << " points!\n";
            endGame();
            return true;
        }
    }
    return false;
}