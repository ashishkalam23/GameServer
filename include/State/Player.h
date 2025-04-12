#pragma once

#include <string>
#include "../Network/Server.h"  // Assumes Connection type is defined here
#include <iostream>
#include <map>

using std::string, std::map, std::cout, std::endl, networking::Connection;

class Player {
public:
    // Constructors
    Player() : con{0}, playerName("dummy"), score(0) {}  // Default constructor
    Player(Connection c) : con(c), score(0) {}  // Constructor with Connection
    Player(Connection c, const string& name) : con(c), playerName(name), score(0) {}  // Constructor with Connection and name

    // Setters
    void setName(string n);
    void setVariable(const string& key, const string& value);

    // Getters
    string getName() const;
    string getVariable(const string& key) const;
    string getState() const;
    Connection getConnection() const;

    // Connection management
    void connect();  // Changes the player's state to "connected"
    void disconnect();  // Changes the player's state to "disconnected"

    // Score handling methods
    void addScore(int points);
    int getScore() const;

    // Messaging functions
    void sendMessage(string msg);  // Sends a message to the server (simulated)
    void getMessage();  // Receives a message from the server (simulated)

    // Audience management
    static void joinAudience(const string& audienceName);

private:
    string playerName = "tester";
    string state = "disconnected";  // Connection state: "connected" or "disconnected"
    Connection con;
    int score;  // Player's score

    // Variables storage
    map<string, string> variables;
};