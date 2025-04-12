#include "State/Player.h"
#include "State/Audience.h"
#include <iostream>

// Setters
void Player::setName(string n) {
    playerName = n;
}

void Player::setVariable(const string& key, const string& value) {
    variables[key] = value;
}

// Connection management
void Player::connect() {
    state = "connected";
    cout << "Player " << getName() << " is now connected." << endl;
}

void Player::disconnect() {
    state = "disconnected";
    cout << "Player " << getName() << " is now disconnected." << endl;
}

// Messaging functions
void Player::sendMessage(string msg) {
    cout << "Player " << getName() << " sent message to server: " << msg << endl;
}

void Player::getMessage() {
    cout << "Player " << getName() << " received message from server" << endl;
}

string Player::getVariable(const string& key) const {
    auto it = variables.find(key);
    if (it != variables.end()) {
        return it->second;
    }
    return "";
}

string Player::getName() const {
    return playerName;  // Return playerName if set
}

string Player::getState() const {
    return state;
}
Connection Player::getConnection() const {
    return con;
}

void Player::addScore(int points) {
    score += points;
}

int Player::getScore() const {
    return score;
}

// Audience management
void Player::joinAudience(const string& audienceName) {
    //Audience newAudience(audienceName);
    // Implement logic to use or store the newAudience as needed
}